#include <chrono>

class Timer {
    double seconds = 0;
    long last_recorded_time;
public:
    void start() {
        seconds = 0;
        
        last_recorded_time = std::chrono::steady_clock::now().time_since_epoch().count();
    }

    void update() {
        auto new_time = std::chrono::steady_clock::now().time_since_epoch().count();
        
        auto span = new_time - last_recorded_time;

        seconds += span;
        last_recorded_time = new_time;
    }

    void sync(double newTime) {
        seconds = newTime;
    }

    double getTime(){
        update();
        return seconds;
    }
};