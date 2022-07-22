#ifndef __WWBASE_TEMPORAL_H__
#define __WWBASE_TEMPORAL_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "stdint.h"
#include "stdbool.h"

    typedef struct Date
    {
        uint8_t year;
        uint8_t month;
        uint8_t day;
    } Date;

    typedef struct Time
    {
        uint8_t hour;
        uint8_t minute;
        uint8_t second;
    } Time;

    typedef struct DateTime
    {
        uint8_t year;
        uint8_t month;
        uint8_t day;
        uint8_t hour;
        uint8_t minute;
        uint8_t second;
    } DateTime;

    typedef enum
    {
        //    TEMPORAL_UNIT_TYPE_YEAR,
        //    TEMPORAL_UNIT_TYPE_MONTH,
        TEMPORAL_UNIT_TYPE_DAY,
        //    TEMPORAL_UNIT_TYPE_HOUR,
        TEMPORAL_UNIT_TYPE_MINUTE,
        //    TEMPORAL_UNIT_TYPE_SECOND,
        //    TEMPORAL_UNIT_TYPE_WEEK,
    } TEMPORAL_UNIT_TYPE;

    int32_t date_span_get(const Date start, const Date end, TEMPORAL_UNIT_TYPE unitType);
    int32_t time_span_get(const Time start, const Time end, TEMPORAL_UNIT_TYPE unitType);
    int32_t datetime_span_get(const DateTime start, const DateTime end, TEMPORAL_UNIT_TYPE unitType);

    uint32_t datetime_to(const DateTime datetime, TEMPORAL_UNIT_TYPE unitType);
    uint32_t time_to(const Time time, TEMPORAL_UNIT_TYPE unitType);
    uint32_t date_to(const Date date, TEMPORAL_UNIT_TYPE unitType);

    bool date_is_same(const Date date1, const Date date2, TEMPORAL_UNIT_TYPE unitType);
    bool time_is_same(const Time time1, const Time time2, TEMPORAL_UNIT_TYPE unitType);
    bool datetime_is_same(const DateTime datetime1, const DateTime datetime2, TEMPORAL_UNIT_TYPE unitType);
#ifdef __cplusplus
}
#endif

#endif // __WWBASE_TEMPORAL_H__