//
// Created by zhouj on 2023/8/30.
//

#ifndef PERIPHERAL_RTC_HPP_
#define PERIPHERAL_RTC_HPP_

#include "peripheral.hpp"
#include "temporal.hpp"

namespace wibot::peripheral {

using namespace wibot::temporal;

class Rtc : public Initializable {
   public:
    Rtc(RTC_CTOR_ARG);
    ~Rtc();

    Result read(DateTime& datetime);
    Result write(DateTime datetime);

   protected:
    Result _init() override;
    void   _deinit() override;

   private:
    RTC_FIELD_DECL
};

}  // namespace wibot::peripheral

#endif  //PERIPHERAL_RTC_HPP_
