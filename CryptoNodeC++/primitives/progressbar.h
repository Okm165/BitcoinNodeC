#ifndef PROGRESSBAR_H
#define PROGRESSBAR_H

#include <mutex>
#include <thread>
#include <chrono>
#include <string>
#include <math.h>
#include <cassert>
#include <iostream>
#include <stdint.h>
#include <condition_variable>

class ProgressBar
{
    private:

    std::condition_variable cv;
    uint64_t prevprogress = 0;
    
    int width = 0;
    float sm_avg = 0;
    float smoothing = 0;
    float sm_nominator = 0;
    float sm_denominator = 0;
    
    std::chrono::time_point<std::chrono::high_resolution_clock> start;
    std::chrono::time_point<std::chrono::high_resolution_clock> loop_tp1;
    std::chrono::time_point<std::chrono::high_resolution_clock> loop_tp2;
    std::thread th;

    public:

    std::string title;
    uint64_t length = 0;
    uint64_t progress = 0;

    int percentage = 0;
    int refreshRate = 0;
    float it_per_sec = 0;

    ProgressBar(std::string& title, uint64_t& length, int width, float smoothing, int refreshRate);
    void loop();
    void display(std::string &prefix, std::string &bar, std::string &suffix, std::string &timepred);
    void calculateAvg();
    void update(int step);
    void close();
    std::string timeConv(int time);
    static void threadFunc(ProgressBar* obj);
};

#endif