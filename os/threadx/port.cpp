#include "port.hpp"

#include "os/os.hpp"

namespace wibot::os {
void Utils::delay(uint32_t ms) {
    tx_thread_sleep(ms);
};

uint32_t Utils::tick_get() {
    return tx_time_get();
};

// template<uint32_t stack_size>
// void Thread<stack_size>::sleep(uint32_t ms)
//{
//     tx_thread_sleep(ms);
// }

// template<uint32_t stack_size>
// Thread<stack_size>::Thread(const char* name, void (* func)(void*), void* arg, uint32_t priority)
//{
//     tx_thread_create(&_instance, const_cast<CHAR*>(name), func, arg, _stack, stack_size,
//     priority, priority, TX_NO_TIME_SLICE, TX_AUTO_START);
//
// };

Mutex::Mutex(const char* name): name_(name) {
};

Mutex::~Mutex(){
    // TODO:
};

bool Mutex::lock(uint32_t timeout) {
    return tx_mutex_get(&(this->_instance), timeout) == TX_SUCCESS;
};

void Mutex::unlock() {
    tx_mutex_put(&(this->_instance));
}
Result Mutex::_init() {
    return (Result)tx_mutex_create(&(this->_instance), const_cast<CHAR*>(name_), 0);
}
void Mutex::_deinit() {
    tx_mutex_delete(&(this->_instance));
};

EventGroup::EventGroup(const char* name): name_(name) {
};

EventGroup::~EventGroup(){
    // TODO:
};

Result EventGroup::_init() {
    return (Result)tx_event_flags_create(&(this->_instance), const_cast<CHAR*>(name_));
}
void EventGroup::_deinit() {
    tx_event_flags_delete(&(this->_instance));
};

Result EventGroup::set(uint32_t flags) {
    return (tx_event_flags_set(&(this->_instance), flags, TX_OR) == TX_SUCCESS)
               ? Result::OK
               : Result::GeneralError;
};

Result EventGroup::reset(uint32_t flags) {
    return (tx_event_flags_set(&(this->_instance), flags, TX_AND) == TX_SUCCESS)
               ? Result::OK
               : Result::GeneralError;
};

Result EventGroup::wait(uint32_t flags, uint32_t& actualFlags, EventOptions options,
                        uint32_t timeout) {
    // TODO: handler TX_OPTION
    return (tx_event_flags_get(&(this->_instance), flags, options, &actualFlags, timeout) ==
            TX_SUCCESS)
               ? Result::OK
               : Result::GeneralError;
}

MessageQueue::MessageQueue(const char* name, void* msg_addr, uint32_t msg_size,
                           uint32_t queue_size) {
    name_       = name;
    msg_addr_   = msg_addr;
    msg_size_   = msg_size;
    queue_size_ = queue_size;
}

MessageQueue::~MessageQueue() {
    tx_queue_delete(&(this->_instance));
}
Result MessageQueue::_init() {
    return (Result)tx_queue_create(&(this->_instance), const_cast<CHAR*>(name_), msg_size_,
                                   msg_addr_, queue_size_);
}
void MessageQueue::_deinit() {
    tx_queue_delete(&(this->_instance));
};

Result MessageQueue::send(const void* msg, uint32_t timeout) {
    return (tx_queue_send(&(this->_instance), const_cast<void*>(msg), timeout) == TX_SUCCESS)
               ? Result::OK
               : Result::GeneralError;
}

Result MessageQueue::receive(void* msg, uint32_t timeout) {
    return (tx_queue_receive(&(this->_instance), const_cast<void*>(msg), timeout) == TX_SUCCESS)
               ? Result::OK
               : Result::GeneralError;
}
Result MessageQueue::flush() {
    return (tx_queue_flush(&(this->_instance)) == TX_SUCCESS) ? Result::OK : Result::GeneralError;
}

}  // namespace wibot::os
