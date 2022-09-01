#ifndef __WWBASE_TEMPORAL_HPP__
#define __WWBASE_TEMPORAL_HPP__

#include "base.hpp"
namespace ww::temporal {
enum TEMPORAL_UNIT_TYPE {
    //    TEMPORAL_UNIT_TYPE_YEAR,
    //    TEMPORAL_UNIT_TYPE_MONTH,
    TEMPORAL_UNIT_TYPE_DAY,
    TEMPORAL_UNIT_TYPE_HOUR,
    TEMPORAL_UNIT_TYPE_MINUTE,
    TEMPORAL_UNIT_TYPE_SECOND,
    //    TEMPORAL_UNIT_TYPE_WEEK,
};
struct Date {
    uint8_t year;
    uint8_t month;
    uint8_t day;

    Date() : year(0), month(1), day(1) {}
    Date(uint8_t year, uint8_t month, uint8_t day)
        : year(year), month(month), day(day) {}
    int32_t span_get(const Date &end, TEMPORAL_UNIT_TYPE unitType) const;

    uint32_t to_number(TEMPORAL_UNIT_TYPE unitType) const;
    bool equals(const Date &other, TEMPORAL_UNIT_TYPE unitType) const;
};

struct Time {
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
    Time() : hour(0), minute(0), second(0){};
    Time(uint8_t hour, uint8_t minute, uint8_t second)
        : hour(hour), minute(minute), second(second){};
    int32_t span_get(const Time &end, TEMPORAL_UNIT_TYPE unitType) const;
    uint32_t to_number(TEMPORAL_UNIT_TYPE unitType) const;
    bool equals(const Time &other, TEMPORAL_UNIT_TYPE unitType) const;
};

struct DateTime {
    uint8_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
    DateTime() : year(0), month(1), day(1), hour(0), minute(0), second(0) {}
    DateTime(uint8_t year, uint8_t month, uint8_t day, uint8_t hour,
             uint8_t minute, uint8_t second)
        : year(year), month(month), day(day), hour(hour), minute(minute),
          second(second) {}
    int32_t span_get(const DateTime &end, TEMPORAL_UNIT_TYPE unitType) const;
    uint32_t to_number(TEMPORAL_UNIT_TYPE unitType) const;
    bool equals(const DateTime &other, TEMPORAL_UNIT_TYPE unitType) const;
};
} // namespace ww::temporal

#endif // __WWBASE_TEMPORAL_HPP__