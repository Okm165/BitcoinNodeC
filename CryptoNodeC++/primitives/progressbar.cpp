#include "progressbar.h"

ProgressBar::ProgressBar(std::string& title, uint64_t& length, int width, float smoothing, int refreshRate)
{
    this->title = title;
    this->length = length;
    this->width = width;
    this->smoothing = smoothing;
    this->refreshRate = refreshRate;
    start = std::chrono::high_resolution_clock::now();
    std::cout << title << std::endl;
    assert(!system("setterm -cursor off"));
    th = std::thread(threadFunc, this);   
}

void ProgressBar::loop()
{   
    loop_tp2 = std::chrono::high_resolution_clock::now(); 
    float stat = (progress+0.0)/length;
    std::string prefix = std::to_string(int(stat*100)) + "%";

    std::string bar = "";
    int fields = stat*width;
    for(int it = 0; it < fields; it++){bar += "█";}
    int partial = stat*width*7 - fields*7;
    switch (partial)
    {
        case 1:
            bar += "▏";
            break;
        case 2:
            bar += "▎";
            break;
        case 3:
            bar += "▍";
            break;
        case 4:
            bar += "▌";
            break;
        case 5:
            bar += "▋";
            break;
        case 6:
            bar += "▊";
            break;
        case 7:
            bar += "▉";
            break;
        
        default:
            break;
    }
    bool partialSign = partial;
    for(int it = 0; it < width-fields-partialSign; it++){bar += " ";}

    std::string suffix;
    std::string timepred;
    suffix += " ";
    suffix += std::to_string(progress);
    suffix += "/";
    suffix += std::to_string(length);
    suffix += " ";

    if(progress == prevprogress || loop_tp1.time_since_epoch().count() == 0)
    {
        loop_tp1 = loop_tp2;
        prevprogress = progress;
        display(prefix, bar, suffix, timepred);
        return;
    }
    
    std::chrono::duration<double, std::ratio<1,1>> loop_time (loop_tp2-loop_tp1);
    loop_tp1 = loop_tp2;
    it_per_sec = (progress-prevprogress)/loop_time.count();
    prevprogress = progress;
    calculateAvg();
    
    std::chrono::duration<float, std::ratio<1,1>> total_time (loop_tp1-start);
    int time_left = (length-progress)/sm_avg;
    
    timepred += std::to_string(int(std::round(sm_avg))) + " it/s";
    timepred += " ";
    timepred += timeConv(int(total_time.count()));
    timepred += ">";
    timepred += timeConv(time_left);
    display(prefix, bar, suffix, timepred);
    return;
}

void ProgressBar::display(std::string &prefix, std::string &bar, std::string &suffix, std::string &timepred)
{   
    std::cout << "\r" << prefix << " |" << bar << "| " << suffix << timepred << "  " << std::flush;
}

void ProgressBar::calculateAvg()
{
    sm_nominator *= 1-smoothing;
    sm_denominator *= 1-smoothing;
    sm_nominator += it_per_sec;
    sm_denominator += 1;
    sm_avg = sm_nominator/sm_denominator;
}

std::string ProgressBar::timeConv(int time)
{ 
    short seconds = time % 60;
    int minutes = time/60;
    std::string string;
    if(minutes < 10){string += "0";}
    string += std::to_string(minutes);
    string += ":";
    if(seconds < 10){string += "0";}
    string += std::to_string(seconds);
    return string;
}

void ProgressBar::update(int step)
{
    progress += step;
}

void ProgressBar::close()
{
    cv.notify_all();
    th.join();
    std::cout << std::endl;
    //show cursor
    assert(!system("setterm -cursor on"));
}

void ProgressBar::threadFunc(ProgressBar* obj)
{
    while (true)
    {
        std::mutex mtx;
        std::unique_lock<std::mutex> lck(mtx);
        if(std::cv_status::timeout == obj->cv.wait_for(lck, std::chrono::milliseconds(obj->refreshRate))){obj->loop();}
        else
        {
            obj->loop();
            return;
        }
    }
}