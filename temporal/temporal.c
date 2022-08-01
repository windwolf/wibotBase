#include "temporal.h"

#define LOG_MODULE "temporal"
#include "log.h"

static inline uint32_t _date_to_days(uint8_t year, uint8_t month, uint8_t day)
{
    uint8_t a = (14 - month) / 12;
    uint8_t y = year - a;
    uint32_t m = month + 12 * a - 3;

    return day + (153 * m + 2) / 5 + 365 * y + y / 4 - y / 100 + y / 400 - 30;
};

int32_t date_span_get(const Date start, const Date end, TEMPORAL_UNIT_TYPE unitType)
{
    int32_t rst = _date_to_days(end.year, end.month, end.day) - _date_to_days(start.year, start.month, start.day);
    if (unitType == TEMPORAL_UNIT_TYPE_DAY)
    {
        return rst;
    }
    rst = rst * 24;
    if (unitType == TEMPORAL_UNIT_TYPE_HOUR)
    {
        return rst;
    }
    rst = rst * 60;
    if (unitType == TEMPORAL_UNIT_TYPE_MINUTE)
    {
        return rst;
    }
    rst = rst * 60;
    if (unitType == TEMPORAL_UNIT_TYPE_SECOND)
    {
        return rst;
    }

    LOG_E("unsupported unit type: %d", unitType);
    return 0 - 1;
};

int32_t time_span_get(const Time start, const Time end, TEMPORAL_UNIT_TYPE unitType)
{
    int32_t rst = 0;
    if (unitType == TEMPORAL_UNIT_TYPE_DAY)
    {
        return 0;
    }
    rst = end.hour - start.hour;
    if (unitType == TEMPORAL_UNIT_TYPE_HOUR)
    {
        return rst;
    }
    rst = rst * 60 + end.minute - start.minute;
    if (unitType == TEMPORAL_UNIT_TYPE_MINUTE)
    {
        return rst;
    }
    rst = rst * 60 + end.second - start.second;
    if (unitType == TEMPORAL_UNIT_TYPE_SECOND)
    {
        return rst;
    }

    LOG_E("unsupported unit type: %d", unitType);
    return 0 - 1;
};

int32_t datetime_span_get(const DateTime start, const DateTime end, TEMPORAL_UNIT_TYPE unitType)
{
    int32_t rst = _date_to_days(end.year, end.month, end.day) - _date_to_days(start.year, start.month, start.day);
    if (unitType == TEMPORAL_UNIT_TYPE_DAY)
    {
        return rst;
    }
    rst = rst * 24 + end.hour - start.hour;
    if (unitType == TEMPORAL_UNIT_TYPE_HOUR)
    {
        return rst;
    }
    rst = rst * 60 + end.minute - start.minute;
    if (unitType == TEMPORAL_UNIT_TYPE_MINUTE)
    {
        return rst;
    }
    rst = rst * 60 + end.second - start.second;
    if (unitType == TEMPORAL_UNIT_TYPE_SECOND)
    {
        return rst;
    }

    LOG_E("unsupported unit type: %d", unitType);
    return 0 - 1;
};

uint32_t datetime_to(const DateTime datetime, TEMPORAL_UNIT_TYPE unitType)
{
    uint32_t rst = _date_to_days(datetime.year, datetime.month, datetime.day);
    if (unitType == TEMPORAL_UNIT_TYPE_DAY)
    {
        return rst;
    }
    rst = rst * 24 + datetime.hour;
    if (unitType == TEMPORAL_UNIT_TYPE_HOUR)
    {
        return rst;
    }
    rst = rst * 60 + datetime.minute;
    if (unitType == TEMPORAL_UNIT_TYPE_MINUTE)
    {
        return rst;
    }
    rst = rst * 60 + datetime.second;
    if (unitType == TEMPORAL_UNIT_TYPE_SECOND)
    {
        return rst;
    }

    LOG_E("unsupported unit type: %d", unitType);
    return 0 - 1;
};

uint32_t time_to(const Time time, TEMPORAL_UNIT_TYPE unitType)
{
    uint32_t rst = 0;
    if (unitType == TEMPORAL_UNIT_TYPE_DAY)
    {
        return rst;
    }
    rst = time.hour;
    if (unitType == TEMPORAL_UNIT_TYPE_HOUR)
    {
        return rst;
    }
    rst = rst * 60 + time.minute;
    if (unitType == TEMPORAL_UNIT_TYPE_MINUTE)
    {
        return rst;
    }
    rst = rst * 60 + time.second;
    if (unitType == TEMPORAL_UNIT_TYPE_SECOND)
    {
        return rst;
    }
    LOG_E("unsupported unit type: %d", unitType);
    return 0 - 1;
};
uint32_t date_to(const Date date, TEMPORAL_UNIT_TYPE unitType)
{
    uint32_t rst = _date_to_days(date.year, date.month, date.day);
    if (unitType == TEMPORAL_UNIT_TYPE_DAY)
    {
        return rst;
    }
    rst = rst * 24;
    if (unitType == TEMPORAL_UNIT_TYPE_HOUR)
    {
        return rst;
    }
    rst = rst * 60;
    if (unitType == TEMPORAL_UNIT_TYPE_MINUTE)
    {
        return rst;
    }
    rst = rst * 60;
    if (unitType == TEMPORAL_UNIT_TYPE_SECOND)
    {
        return rst;
    }

    LOG_E("unsupported unit type: %d", unitType);
    return 0 - 1;
};

bool date_is_same(const Date date1, const Date date2, TEMPORAL_UNIT_TYPE unitType)
{
    return date1.year == date2.year && date1.month == date2.month && date1.day == date2.day;
};
bool time_is_same(const Time time1, const Time time2, TEMPORAL_UNIT_TYPE unitType)
{
    bool rst = time1.hour == time2.hour;
    if (unitType == TEMPORAL_UNIT_TYPE_HOUR)
    {
        return rst;
    }
    rst = rst && (time1.minute == time2.minute);
    if (unitType == TEMPORAL_UNIT_TYPE_MINUTE)
    {
        return rst;
    }
    rst = rst && (time1.second == time2.second);
    if (unitType == TEMPORAL_UNIT_TYPE_SECOND)
    {
        return rst;
    }

    LOG_E("unsupported unit type: %d", unitType);
    return false;
};

bool datetime_is_same(const DateTime datetime1, const DateTime datetime2, TEMPORAL_UNIT_TYPE unitType)
{
    bool rst = datetime1.year == datetime2.year && datetime1.month == datetime2.month && datetime1.day == datetime2.day;
    if (unitType == TEMPORAL_UNIT_TYPE_DAY)
    {
        return rst;
    }
    rst = rst && (datetime1.hour == datetime2.hour);
    if (unitType == TEMPORAL_UNIT_TYPE_HOUR)
    {
        return rst;
    }
    rst = rst && (datetime1.minute == datetime2.minute);
    if (unitType == TEMPORAL_UNIT_TYPE_MINUTE)
    {
        return rst;
    }
    rst = rst && (datetime1.second == datetime2.second);
    if (unitType == TEMPORAL_UNIT_TYPE_SECOND)
    {
        return rst;
    }

    LOG_E("unsupported unit type: %d", unitType);
    return false;
};
