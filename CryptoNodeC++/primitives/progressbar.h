#ifndef PROGRESSBAR_H
#define PROGRESSBAR_H

#include <mutex>
#include <thread>
#include <chrono>
#include <string>
#include <atomic>
#include <math.h>
#include <cassert>
#include <iostream>
#include <stdint.h>
#include <condition_variable>

class ProgressBar
{
    protected:

    std::condition_variable cv;
    uint64_t prevprogress = 0;
    
    int width = 0;
    float sm_avg = 0;
    float smoothing = 0;
    float sm_nominator = 0;
    float sm_denominator = 0;
    
    int percentage = 0;
    int refreshRate = 0;
    
    float stat = 0;
    int fields = 0;
    int partial = 0;
    float it_per_sec = 0;
    int time_left = 0;
    
    std::chrono::time_point<std::chrono::high_resolution_clock> start;
    std::chrono::time_point<std::chrono::high_resolution_clock> loop_tp1;
    std::chrono::time_point<std::chrono::high_resolution_clock> loop_tp2;
    std::thread th;

    #if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)

    static const char charset_length = 5;
    char progressCharSet[charset_length] = {(char)32, (char)176, (char)177, (char)178, (char)219};

    #elif defined(__unix__)

    static const char charset_length = 9;
    const char* progressCharSet[charset_length] {" ", "▏", "▎", "▍", "▌", "▋", "▊", "▉", "█"};

    #endif
    
    std::string title;
    uint64_t length = 0;
    uint64_t progress = 0;
    std::atomic<bool> threadFlag;

    public:

    ProgressBar(const char* title, uint64_t length, int width, float smoothing, int refreshRate);
    void loop();
    void display(std::string &prefix, std::string &bar, std::string &suffix, std::string &timepred);
    void calculateAvg();
    void update(uint64_t step = 1);
    void close();
    std::string timeConv(int time);
    static void threadFunc(ProgressBar* obj, std::atomic<bool>& threadFlag);
};

#endif