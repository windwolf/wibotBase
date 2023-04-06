#include "os.hpp"
#include "ringbuffer.hpp"
#include "soc.hpp"

namespace wibot::os {
void Utils::delay(uint32_t ms) {
    wibot::peripheral::Misc::ms_delay(ms);
};

uint32_t Utils::tick_get() {
    return wibot::peripheral::Misc::get_tick_ms();
};

void Thread::sleep(uint32_t ms) {
    volatile uint32_t start = Utils::tick_get();
    if (Utils::tick_get() - start < ms) {
    }
    return;
};

Mutex::Mutex(const char* name) : name_(name) {
    _instance = 0;
};
Mutex::~Mutex() {
    _instance = 0;
};

bool Mutex::lock(uint32_t timeout) {
    if (timeout == TIMEOUT_NOWAIT) {
        if (this->_instance) {
            return false;
        } else {
            this->_instance = 1;
            return true;
        }
    } else {
        uint32_t start = Utils::tick_get();
        while (this->_instance) {
            if (Utils::tick_get() - start > timeout) {
                return false;
            }
        };
        this->_instance = 1;
        return true;
    }
};

void Mutex::unlock() {
    this->_instance = 0;
};

EventGroup::EventGroup(const char* name) : name_(name) {
    _instance = 0;
};
EventGroup::~EventGroup() {
    _instance = 0;
};

Result EventGroup::set(uint32_t flags) {
    this->_instance |= flags;
    return Result::OK;
};

Result EventGroup::reset(uint32_t flags) {
    this->_instance &= ~flags;
    return Result::OK;
};

Result EventGroup::wait(uint32_t flags, uint32_t& actualFlags, EventOptions options,
                        uint32_t timeout) {
    Result rst = Result::OK;

    if (timeout == TIMEOUT_NOWAIT) {
        if ((options & EventOptions_Wait_Flag) == EventOptions_WaitForAll) {
            rst = ((this->_instance & flags) == flags) ? Result::OK : Result::NoResource;
        } else {
            rst = ((this->_instance & flags) != 0) ? Result::OK : Result::NoResource;
        }
    } else {
        uint32_t start = Utils::tick_get();
        if ((options & EventOptions_Wait_Flag) == EventOptions_WaitForAll) {
            while ((this->_instance & flags) != flags) {
                if (Utils::tick_diff(start) > timeout) {
                    rst = Result::Timeout;
                    break;
                }
            };
        } else {
            while ((this->_instance & flags) == 0) {
                if (Utils::tick_diff(start) > timeout) {
                    rst = Result::Timeout;
                    break;
                }
            };
        }
    }
    if ((rst == Result::OK) && ((options & EventOptions_Clear_Flag) == EventOptions_Clear_Flag)) {
        this->_instance &= ~flags;
    }
    actualFlags = this->_instance;
    return Result::OK;
};

MessageQueue::MessageQueue(const char* name, void* msg_addr, uint32_t msg_size, uint32_t queue_size)
    : _instance(msg_addr, msg_size * sizeof(uint32_t), queue_size) {
}

MessageQueue::~MessageQueue() {
}

Result MessageQueue::send(const void* msg, uint32_t timeout) {
    Result   rst = Result::OK;
    uint32_t actLength;
    if (!_instance.buffer.is_full()) {
        return _instance.buffer.write(const_cast<void*>(msg), 1, false, actLength);
    } else {
        if (timeout == TIMEOUT_FOREVER) {
            while (_instance.buffer.is_full()) {
            }
            _instance.buffer.write(const_cast<void*>(msg), 1, false, actLength);
        } else if (timeout == TIMEOUT_NOWAIT) {
            return Result::NoResource;
        } else {
            uint32_t start = Utils::tick_get();
            while (_instance.buffer.is_full()) {
                if (Utils::tick_diff(start) > timeout) {
                    rst = Result::Timeout;
                    break;
                }
            };
            if (rst == Result::OK) {
                _instance.buffer.write(const_cast<void*>(msg), 1, false, actLength);
            }
        }
    }
    return rst;
}

Result MessageQueue::receive(void* msg, uint32_t timeout) {
    Result   rst = Result::OK;
    uint32_t actLength;
    if (!_instance.buffer.is_empty()) {
        return _instance.buffer.read(msg, 1, actLength);
    } else {
        if (timeout == TIMEOUT_FOREVER) {
            while (_instance.buffer.is_empty()) {
            }
            _instance.buffer.read(msg, 1, actLength);
        } else if (timeout == TIMEOUT_NOWAIT) {
            return Result::NoResource;
        } else {
            uint32_t start = Utils::tick_get();
            while (_instance.buffer.is_empty()) {
                if (Utils::tick_diff(start) > timeout) {
                    rst = Result::Timeout;
                    break;
                }
            };
            if (rst == Result::OK) {
                _instance.buffer.read(msg, 1, actLength);
            }
        }
    }
    return rst;
}
Result MessageQueue::flush() {
    _instance.buffer.clear();
    return Result::OK;
}

Result EventGroup::_init() {
    return Result::OK;
}
void EventGroup::_deinit() {
}

Result MessageQueue::_init() {
    return Result::OK;
}
void MessageQueue::_deinit() {
}

Result Mutex::_init() {
    return Result::OK;
}
void Mutex::_deinit() {
}

}  // namespace wibot::os
