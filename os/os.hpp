#ifndef ___OS_PORT_HPP__
#define ___OS_PORT_HPP__

#include "base.hpp"
#include "port.hpp"

namespace wibot::os
{

	class Utils
	{
	 public:
		static void delay(uint32_t ms);
		static uint32_t tick_get();
		static uint32_t tick_diff(uint32_t tick);
	};

    //template<uint32_t stack_size>
	class Thread
	{
	 public:
        //Thread(const char* name, void (*func)(void*), void* arg, uint32_t priority);
        //void start();
		static void sleep(uint32_t ms);

     private :
        //uint8_t _stack[stack_size];
        //THREAD_TYPEDEF _instance;
	};

	class Mutex
	{
	 public:
		Mutex(const char* name);
		~Mutex();
		bool lock(uint32_t timeout);
		void unlock();

	 private:
		MUTEX_TYPEDEF _instance;
	};

	typedef uint8_t EventOptions;
#define EventOptions_Wait_Flag 0x02
#define EventOptions_WaitForAny 0x00
#define EventOptions_WaitForAll EventOptions_Wait_Flag
#define EventOptions_Clear_Flag 0x01
#define EventOptions_NoClear 0x00
#define EventOptions_Clear EventOptions_Clear_Flag

    /**
     *
     */
	class EventGroup
	{
	 public:
		EventGroup(const char* name);
		~EventGroup();
		Result set(uint32_t flags);
		Result reset(uint32_t flags);
		Result wait(uint32_t flags, uint32_t& actualFlags, EventOptions options, uint32_t timeout);
        uint32_t fetch_empty_flag();
	 private:
		EVENTGROUP_TYPEDEF _instance;
        uint8_t used_flags_;
	};


    class MessageQueue
    {
     public:
        /**
         *
         * @param name
         * @param msg_addr message queue buffer address
         * @param msg_size message size in WORD
         * @param queue_size queue size in message
         */
        MessageQueue(const char* name, void* msg_addr, uint32_t msg_size, uint32_t queue_size);
        ~MessageQueue();
        Result send(const void* msg, uint32_t timeout);
        Result receive(void* msg, uint32_t timeout);
        /**
         * @brief Clear the message queue
         * @return
         */
        Result flush();
     private:
        MESSAGEQUEUE_TYPEDEF _instance;
    };
} // namespace wibot::os

#endif // ___OS_PORT_HPP__
