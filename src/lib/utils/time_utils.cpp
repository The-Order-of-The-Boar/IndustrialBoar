#include "time_utils.hpp"

// builtin
#include <iostream>
#include <unordered_map>

std::unordered_map<TimeMeasurer::TimeUnit, std::string> const unit_strings{
    {TimeMeasurer::MICROSECOND, "us"},
    {TimeMeasurer::MILLISECOND, "ms"},
    {TimeMeasurer::SECOND, "s"},
};

TimeMeasurer::TimeMeasurer(std::string&& message, TimeUnit const time_unit):
    message{message}, time_unit{time_unit}, start_time{std::chrono::high_resolution_clock::now()}
{
}

[[nodiscard]] auto TimeMeasurer::get_time() const
{
    auto const finished_time =
        this->stopped ? this->stopped_time : std::chrono::high_resolution_clock::now();
    auto const elapsed_time = finished_time - this->start_time;
    switch (this->time_unit)
    {
        case MICROSECOND:
            return std::chrono::duration_cast<std::chrono::microseconds>(elapsed_time).count();
        case MILLISECOND:
            return std::chrono::duration_cast<std::chrono::milliseconds>(elapsed_time).count();
        case SECOND:
            return std::chrono::duration_cast<std::chrono::seconds>(elapsed_time).count();
        default:
            return std::chrono::duration_cast<std::chrono::milliseconds>(elapsed_time).count();
    }
}

void TimeMeasurer::restart_time()
{
    this->start_time = std::chrono::high_resolution_clock::now();
}

void TimeMeasurer::stop_time()
{
    this->stopped_time = std::chrono::high_resolution_clock::now();
    this->stopped      = true;
}

[[nodiscard]] auto TimeMeasurer::get_decimal_time() const
{
    auto const finished_time =
        this->stopped ? this->stopped_time : std::chrono::high_resolution_clock::now();
    auto const elapsed_time = finished_time - this->start_time;
    return std::chrono::duration_cast<std::chrono::microseconds>(elapsed_time).count();
    switch (this->time_unit)
    {
        case MICROSECOND:
            return std::chrono::duration_cast<std::chrono::microseconds>(elapsed_time).count();
        case MILLISECOND:
            return std::chrono::duration_cast<std::chrono::milliseconds>(elapsed_time).count();
        case SECOND:
            return std::chrono::duration_cast<std::chrono::seconds>(elapsed_time).count();
        default:
            return std::chrono::duration_cast<std::chrono::milliseconds>(elapsed_time).count();
    }
}

void TimeMeasurer::print_time() const
{
    auto const elapsed_time = this->get_time();
    std::cout << message << " in " << elapsed_time << unit_strings.at(this->time_unit) << std::endl;
}
