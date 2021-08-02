#include "progressbar.h"

ProgressBar::ProgressBar(const char* title, uint64_t length, int width, float smoothing, int refreshRate)
{
    this->title = title;
    this->length = length;
    this->width = width;
    this->smoothing = smoothing;
    this->refreshRate = refreshRate;
    start = std::chrono::high_resolution_clock::now();
    loop_tp1 = std::chrono::high_resolution_clock::now();
    std::cout << title << std::endl;
    #if defined(__unix__)
    assert(!system("setterm -cursor off"));
    #endif
    threadFlag = true;
    th = std::thread(threadFunc, this, std::ref(threadFlag));   
}

void ProgressBar::loop()
{   
    loop_tp2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float, std::ratio<1,1>> total_time (loop_tp2-start);
    if(progress != prevprogress)
    {
        stat = (progress+0.0)/length;
        fields = stat*width;
        partial = stat*width*charset_length - fields*charset_length;
        std::chrono::duration<double, std::ratio<1,1>> loop_time (loop_tp2-loop_tp1);
        loop_tp1 = loop_tp2;
        it_per_sec = (progress-prevprogress)/loop_time.count();
        prevprogress = progress;
        calculateAvg();
        time_left = (length-progress)/sm_avg;
    }

    std::string prefix = std::to_string(int(stat*100)) + "%";

    std::string bar = "";
    for(int it = 0; it < fields; it++){bar += progressCharSet[charset_length-1];}
    bar += progressCharSet[partial];
    for(int it = 0; it < width-fields; it++){bar += " ";}

    std::string suffix;
    suffix += " ";
    suffix += std::to_string(progress);
    suffix += "/";
    suffix += std::to_string(length);
    suffix += " ";
    
    std::string timepred;
    if(sm_avg >= 1){timepred += std::to_string(int(std::round(sm_avg))) + " it/s";}
    else{timepred += std::to_string(sm_avg) + " it/s";}
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

void ProgressBar::update(uint64_t step)
{
    progress += step;
}

void ProgressBar::close()
{
    threadFlag = false;
    cv.notify_all();
    th.join();
    std::cout << std::endl;
    //show cursor
    #if defined(__unix__)
    assert(!system("setterm -cursor on"));
    #endif
}

void ProgressBar::threadFunc(ProgressBar* obj, std::atomic<bool>& threadFlag)
{
    std::mutex progmtx;
    std::unique_lock<std::mutex> proglck(progmtx);
    while(threadFlag)
    {
        if(std::cv_status::timeout == obj->cv.wait_for(proglck, std::chrono::milliseconds(obj->refreshRate))){obj->loop();}
        else
        {
            obj->loop();
            return;
        }
    }
}