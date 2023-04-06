#ifndef __WWBASE_TEMPORAL_HPP__
#define __WWBASE_TEMPORAL_HPP__

#include "base.hpp"
namespace wibot::temporal {
enum class TemporalUnitType {
    //    TEMPORAL_UNIT_TYPE_YEAR,
    //    TEMPORAL_UNIT_TYPE_MONTH,
    Day,
    Hour,
    Minute,
    Second,
    //    TEMPORAL_UNIT_TYPE_WEEK,
};
struct Date {
    uint8_t year;
    uint8_t month;
    uint8_t day;

    Date() : year(0), month(1), day(1) {
    }
    Date(uint8_t year, uint8_t month, uint8_t day) : year(year), month(month), day(day) {
    }
    int32_t span_get(const Date &end, TemporalUnitType unitType) const;

    uint32_t to_number(TemporalUnitType unitType) const;
    bool     equals(const Date &other, TemporalUnitType unitType) const;
};

struct Time {
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
    Time() : hour(0), minute(0), second(0){};
    Time(uint8_t hour, uint8_t minute, uint8_t second)
        : hour(hour), minute(minute), second(second){};
    int32_t  span_get(const Time &end, TemporalUnitType unitType) const;
    uint32_t to_number(TemporalUnitType unitType) const;
    bool     equals(const Time &other, TemporalUnitType unitType) const;
};

struct DateTime {
    uint8_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
    DateTime() : year(0), month(1), day(1), hour(0), minute(0), second(0) {
    }
    DateTime(uint8_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second)
        : year(year), month(month), day(day), hour(hour), minute(minute), second(second) {
    }
    int32_t  span_get(const DateTime &end, TemporalUnitType unitType) const;
    uint32_t to_number(TemporalUnitType unitType) const;
    bool     equals(const DateTime &other, TemporalUnitType unitType) const;
};
}  // namespace wibot::temporal

#endif  // __WWBASE_TEMPORAL_HPP__
