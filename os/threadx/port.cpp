#include "os/os.hpp"
#include "port.hpp"

namespace ww::os
{
void Utils::delay(uint32_t ms)
{
    tx_thread_sleep(ms);
};

uint32_t Utils::tick_get()
{
    return tx_time_get();
};

void Thread::sleep(uint32_t ms)
{
    tx_thread_sleep(ms);
};

Mutex::Mutex(const char *name)
{
    tx_mutex_create(&(this->instance), name, 0);
};

Mutex::~Mutex(){
    // TODO:
};

bool Mutex::lock(uint32_t timeout)
{
    return tx_mutex_get(&(this->instance), timeout) == TX_SUCCESS;
};

void Mutex::unlock()
{
    return tx_mutex_put(&(this->instance)) == TX_SUCCESS;
};

EventGroup::EventGroup(const char *name)
{
    tx_event_flags_create(&(this->_instance), name);
};

EventGroup::~EventGroup(){
    // TODO:
};

Result EventGroup::set(uint32_t flags)
{
    return (tx_event_flags_set(&(this->_instance), flags, TX_OR) == TX_SUCCESS)
               ? Result::OK
               : Result::GeneralError;
};

Result EventGroup::reset(uint32_t flags)
{
    return (tx_event_flags_set(&(this->_instance), flags, TX_AND) == TX_SUCCESS)
               ? Result::OK
               : Result::GeneralError;
};

Result EventGroup::wait(uint32_t flags, uint32_t &actualFlags, EventOptions options,
                        uint32_t timeout)
{
    // TODO: handler TX_OPTION
    return (tx_event_flags_get(&(this->_instance), flags, option, &actualFlags, timeout) ==
            TX_SUCCESS)
               ? Result::OK
               : Result::GeneralError;
};