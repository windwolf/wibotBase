#ifndef __WWBASE_EVENT_HPP__
#define __WWBASE_EVENT_HPP__

#include "os.hpp"

namespace ww
{

using namespace ww::os;

typedef void (*Callback)(void *sender, void *event, void *receiver);
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
class WaitHandler : public Initializable
{
  public:
    WaitHandler(EventGroup &eventGroup, uint32_t doneFlag, uint32_t errorFlag);
    void set_value(void *value);
    void *get_value();
    void *get_sender();

    Result reset();
    bool is_busy();
    // Result wait();
    // Result wait(uint32_t level);
    Result wait(uint32_t level, uint32_t timeout);
    void done_set(void *sender);
    void error_set(void *sender);
    void done_callback_set(void *receiver, Callback onDone);
    void error_callback_set(void *receiver, Callback onError);
    uint32_t scope_begin();
    void scope_end();
    uint32_t scope_get();

  protected:
    void *_sender;
    void *_value;
    EventGroup &_eventGroup;
    uint32_t _doneFlag;
    uint32_t _errorFlag;
    void *_receiver;
    Callback _onDone;
    Callback _onError;
    uint32_t _level;
};

} // namespace ww

#endif // __WWBASE_EVENT_HPP__