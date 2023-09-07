#include "rtc.hpp"
#include "perip_port.hpp"
#include "misc.hpp"
#include "os.hpp"

#ifdef HAL_RTC_MODULE_ENABLED

namespace wibot::peripheral {

Rtc::Rtc(RTC_HandleTypeDef& handle) : _handle(handle){};

Rtc::~Rtc(){};

Result Rtc::read(DateTime& datetime) {
    RTC_TimeTypeDef sTime = {0};
    RTC_DateTypeDef sDate = {0};
    HAL_RTC_GetTime(&_handle, &sTime, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(&_handle, &sDate, RTC_FORMAT_BIN);

    datetime.year   = sDate.Year;
    datetime.month  = sDate.Month;
    datetime.day    = sDate.Date;
    datetime.hour   = sTime.Hours;
    datetime.minute = sTime.Minutes;
    datetime.second = sTime.Seconds;
    return Result::OK;
};

Result Rtc::_init() {
    return Result::OK;
};
void Rtc::_deinit() {
}
Result Rtc::write(DateTime datetime) {
    RTC_TimeTypeDef sTime = {0};
    RTC_DateTypeDef sDate = {0};
    sTime.Hours           = datetime.hour;
    sTime.Minutes         = datetime.minute;
    sTime.Seconds         = datetime.second;
    sTime.DayLightSaving  = RTC_DAYLIGHTSAVING_NONE;
    sTime.StoreOperation  = RTC_STOREOPERATION_RESET;
    HAL_RTC_SetTime(&_handle, &sTime, RTC_FORMAT_BIN);

    sDate.WeekDay = RTC_WEEKDAY_MONDAY;
    sDate.Month   = datetime.month;
    sDate.Date    = datetime.day;
    sDate.Year    = datetime.year;

    HAL_RTC_SetDate(&_handle, &sDate, RTC_FORMAT_BIN);
    return Result::OK;
};

}  // namespace wibot::peripheral

#endif
