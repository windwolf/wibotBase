#ifndef __WWBASE_EVENT_HPP__
#define __WWBASE_EVENT_HPP__

#include "os.hpp"

namespace wibot {

using namespace wibot::os;

struct WaitHandlerConfig {
    bool disableAutoReset : 1;
};

/**
 * @brief Implement the sender receiver async pattern.
 * Sender maintain the handler's state, then receiver wait for sender's done or
 * error signal.
 * For normally operation,
 * 1. receiver require sender for some async function, and pass the WaitHandler
 * to sender;
 * 2. sender will return immediatly
 *
 */
class WaitHandler : public Configurable<WaitHandlerConfig> {
   public:
    WaitHandler(EventGroup& eventGroup);
    WaitHandler(EventGroup& eventGroup, uint32_t doneFlag, uint32_t errorFlag);
    ~WaitHandler();
    WaitHandler folk();

    WaitHandler merge(const WaitHandler& other);

    void set_value(void* value);
    void* get_value();
    void* get_sender();
    Result reset();
    /**
     * @brief Check if the handler is triggered by done or error.
     * @param handler
     * @return Result::OK if triggered by done, Result::GeneralError if triggered,
     * Result::NoResource if not triggered.
     */
    Result triggeredFor(WaitHandler& handler) {
        if (&_eventGroup != &handler._eventGroup) {
            return Result::NoResource;
        }
        if (_currentFlag & handler._doneFlag) {
            this->_value = handler._value;
            this->_sender = handler._sender;
            return Result::OK;
        }
        if (_currentFlag & handler._errorFlag) {
            this->_value = handler._value;
            this->_sender = handler._sender;
            return Result::GeneralError;
        }
        return Result::NoResource;
    }

    Result wait(uint32_t timeout);
    void done_set(void* sender);
    void error_set(void* sender);

   protected:
    void* _sender;
    void* _value;
    EventGroup& _eventGroup;
    uint32_t _currentFlag;
    uint32_t _doneFlag;
    uint32_t _errorFlag;
    bool isMerge_;

   private:
    bool is_busy();
};

}  // namespace wibot

#endif  // __WWBASE_EVENT_HPP__
