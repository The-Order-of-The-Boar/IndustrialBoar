#pragma once

#include <chrono>
#include <string>


class TimeMeasurer
{
public:

    enum TimeUnit
    {
        MICROSECOND,
        MILLISECOND,
        SECOND
    };

private:

    std::string message;
    TimeUnit time_unit;
    std::chrono::system_clock::time_point start_time;
    std::chrono::system_clock::time_point stopped_time;
    bool stopped = false;

public:

    TimeMeasurer(std::string&& message = "", TimeUnit const time_unit = TimeUnit::MICROSECOND);

public:

    [[nodiscard]] auto get_time() const;
    void restart_time();
    void stop_time();

    [[nodiscard]] auto get_decimal_time() const;

    void print_time() const;
};
