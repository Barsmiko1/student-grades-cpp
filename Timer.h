// This file declares the Timer class used for the speed analysis.
#ifndef TIMER_H
#define TIMER_H

#include <chrono>

// This class measures how much time has passed since it was created
// (or since reset() was last called).
class Timer {
public:
    Timer() : start_(std::chrono::steady_clock::now()) {}

    // This method starts the measurement again from zero.
    void reset() { start_ = std::chrono::steady_clock::now(); }

    // This method returns the elapsed time in seconds.
    double elapsed() const {
        return std::chrono::duration<double>(std::chrono::steady_clock::now() - start_).count();
    }

private:
    std::chrono::steady_clock::time_point start_;
};

#endif
