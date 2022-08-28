#ifndef __WWBASE_EVENT_HPP__
#define __WWBASE_EVENT_HPP__

#include "os.hpp"

namespace ww
{

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
class WaitHandler
{
  public:
    void set_value(void *value);
    void *get_value();
    void *get_sender();

    virtual Result reset() = 0;
    virtual bool is_busy() = 0;
    Result wait();
    virtual Result wait(uint32_t timeout) = 0;
    virtual void done_set(void *sender) = 0;
    virtual void error_set(void *sender) = 0;

  protected:
    void *_sender;

    void *_value;
};

typedef void (*Callback)(void *sender, void *event, void *receiver);

/**
 * @brief
 *
 */
class CallbackWaitHandler : public WaitHandler
{
  public:
    CallbackWaitHandler(void *receiver) : _receiver(receiver){};

    CallbackWaitHandler(void *receiver, Callback onDone, Callback onError)
        : _receiver(receiver), _onDone(onDone), _onError(onError){};

    void done_callback_set(Callback onDone);
    void error_callback_set(Callback onError);
    virtual Result reset();
    virtual bool is_busy();

    virtual Result wait(uint32_t timeout);

    virtual void done_set(void *sender);
    virtual void error_set(void *sender);

  private:
    void *_receiver;
    Callback _onDone;
    Callback _onError;
    union _status {
        struct
        {
            bool is_done : 1;
            bool is_error : 1;
        };
        uint32_t value;
    } _status;
};

class PollingWaitHandler : public WaitHandler
{
  public:
    PollingWaitHandler(){};

    virtual Result reset();
    virtual bool is_busy();

    virtual Result wait(uint32_t timeout);

    virtual void done_set(void *sender);
    virtual void error_set(void *sender);

  private:
    union _status {
        struct
        {
            bool is_done : 1;
            bool is_error : 1;
        };
        uint32_t value;
    } _status;
};

using namespace ww::os;

class EventGroupWaitHandler : public WaitHandler
{
  public:
    EventGroupWaitHandler(EventGroup &eventGroup, uint32_t doneFlag,
                          uint32_t errorFlag)
        : _eventGroup(eventGroup), _doneFlag(doneFlag), _errorFlag(errorFlag){};

    Result init();
    void deinit();
    virtual Result reset();
    virtual bool is_busy();
    virtual Result wait(uint32_t timeout);
    virtual void done_set(void *sender);
    virtual void error_set(void *sender);

  private:
    EventGroup &_eventGroup;
    uint32_t _doneFlag;
    uint32_t _errorFlag;
};

} // namespace ww

#endif // __WWBASE_EVENT_HPP__