#include "fsm.hpp"
#define LOG_MODULE "fsm"
#include "log.h"

namespace wibot::fsm
{
	Result FSM_State::_init(FSM& fsm)
	{
		if (config.parentStateNo != 0)
		{
			this->_parent = fsm._find_state_by_no(config.parentStateNo);
			if (this->_parent == nullptr)
			{
				LOG_E("%s %s's parent not exsits.", (fsm._name == NULL) ? "" : fsm._name,
					(config.name == NULL) ? "" : config.name);
				return Result::GeneralError;
			}
		}
		else
		{
			this->_parent = nullptr;
		}
		return Result::OK;
	};

	void FSM_State::poll(FSM& fsm)
	{
		FSM_State* sta = this;
		while (sta != NULL)
		{
			sta->do_poll(fsm);
			sta = sta->_parent;
		}
	};

	void FSM_State::do_poll(FSM& fsm)
	{
		if ((config.pollAction != NULL) &&
			((fsm.currentTick - lastPollingTick) >= config.pollingInterval))
		{
			config.pollAction(fsm, this);
			lastPollingTick = fsm.currentTick;
		}
	};

	bool FSM_State::is_parent_of(FSM_State* state)
	{
		while (state != NULL)
		{
			if (state->config.stateNo == config.stateNo)
			{
				return true;
			}
			state = state->_parent;
		}
		return false;
	}

	void FSM_State::enter(FSM& fsm, FSM_State* fromState)
	{
		fsm._currentState = this;
		FSM_State* curSta = this;
		while (curSta != NULL && !curSta->is_parent_of(fromState))
		{
			curSta->do_enter(fsm);
			curSta = curSta->_parent;
		}

		LOG_I("%lu %s.%s: entry", fsm.currentTick, (fsm._name == NULL) ? "" : fsm._name,
			(config.name == NULL) ? "" : config.name);
	};

	void FSM_State::do_enter(FSM& fsm)
	{
		enterTick = fsm.currentTick;
		if (config.entryAction != NULL)
		{
			config.entryAction(fsm, this);
		}
	};

	void FSM_State::exit(FSM& fsm, FSM_State* toState)
	{
		FSM_State* curSta = this;
		while (curSta != NULL && !curSta->is_parent_of(toState))
		{
			curSta->do_exit(fsm);
			curSta = curSta->_parent;
		}

		LOG_I("%lu %s.%s: exit", fsm.currentTick, (fsm._name == NULL) ? "" : fsm._name,
			(config.name == NULL) ? "" : config.name);
	};

	void FSM_State::do_exit(FSM& fsm)
	{
		if (config.exitAction != NULL)
		{
			config.exitAction(fsm, this);
		}
	};

	Result FSM_Transition::_init(FSM& fsm)
	{
		FSM_State* fromState = fsm._find_state_by_no(config.from);
		FSM_State* toState = fsm._find_state_by_no(config.to);
		if (fromState == NULL)
		{
			LOG_E("%s transition's from %d not exsits.", (fsm._name == NULL) ? "" : fsm._name,
				config.from);
			return Result::GeneralError;
		}
		if (toState == NULL)
		{
			LOG_E("%s transition's to %d not exsits.", (fsm._name == NULL) ? "" : fsm._name,
				config.to);
			return Result::GeneralError;
		}

		for (uint8_t i = 0; i < fsm._stateCount; i++)
		{
			if (fsm._states[i].config.parentStateNo == config.to)
			{
				LOG_E("%s transition 's to state[%d] must be leaf node.",
					(fsm._name == NULL) ? "" : fsm._name, config.to);
				return Result::GeneralError;
			}
		}
#ifdef FSM_TRANSITION_PREFILTER
		fromState->_transitions[fromState->_transitionCount] = this;
		fromState->_transitionCount++;
#endif
		_to = toState;
		return Result::OK;
	};

	bool FSM_Transition::_do_event_check(FSM& fsm, FSM_State* fromState)
	{
		// skip transit to self.
		if (config.to == fromState->config.stateNo)
		{
			return false;
		}

		if (config.mode == FsmTransitionMode::Event && fsm._events.check(config.events) &&
			(config.guard == NULL || config.guard(fsm, fromState)))
		{
			fromState->exit(fsm, _to);

			if (config.action != NULL)
			{
				config.action(fsm, fromState);
			}
			_to->enter(fsm, fsm._currentState);
			return true;
		}
		return false;
	};

	bool FSM_Transition::_do_timeout_check(FSM& fsm, FSM_State* fromState, uint32_t duration)
	{
		// skip transit to self.
		if (config.to == fromState->config.stateNo)
		{
			return false;
		}
		if (config.mode == FsmTransitionMode::Timeout && config.timeout <= duration &&
			(config.guard == NULL || config.guard(fsm, fromState)))
		{
			fromState->exit(fsm, _to);
			if (config.action != NULL)
			{
				config.action(fsm, fromState);
			}
			_to->enter(fsm, fsm._currentState);
			return true;
		}
		return false;
	};

	FSM::FSM(const char* name, uint32_t eventClearMask, FSM_State (& states)[], uint32_t stateCount,
		FSM_Transition (& transitions)[], uint32_t transitionCount)
		: _name(name), _events(eventClearMask), _states(states), _stateCount(stateCount),
		  _transitions(transitions), _transitionCount(transitionCount)
	{
	};

	Result FSM::_init()
	{
		Result rst = Result::OK;
#ifdef FSM_TRANSITION_PREFILTER
		for (uint8_t i = 0; i < _stateCount; i++)
		{
			_states[i]._transitionCount = 0;
		}
#endif

		for (uint8_t i = 0; i < _stateCount; i++)
		{

			rst = _states[i]._init(*this);
			if (rst != Result::OK)
			{
				LOG_E("FSM %s build error.", (_name == NULL) ? "" : _name);
				return Result::GeneralError;
			}
		}

		for (uint32_t i = 0; i < _transitionCount; i++)
		{
			rst = _transitions[i]._init(*this);
			if (rst != Result::OK)
			{
				LOG_E("FSM %s build error.", (_name == NULL) ? "" : _name);
				return Result::GeneralError;
			}
		}
		return Result::OK;
	};
	void FSM::_deinit()
	{
	};

	Result FSM::start(uint32_t stateNo, void* userData, uint32_t initialTick)
	{
		init();
		currentTick = initialTick;
		FSM_State* state = _find_state_by_no(stateNo);
		if (state == nullptr)
		{
			LOG_E("start state: %s.%lu not found.", (_name == NULL) ? "" : _name, stateNo);
			return Result::InvalidParameter;
		}
		this->userData = userData;
		state->enter(*this, nullptr);
		lastUpdateTick = initialTick;
		LOG_I("FSM %s started.", (_name == NULL) ? "" : _name);
		return Result::OK;
	};

//#undef FSM_TRANSITION_PREFILTER
	void FSM::_transition_check(FSM_State& state)
	{
		uint32_t duration = (currentTick - state.enterTick);
		FSM_State* curSta = &state;
		while (curSta != NULL)
		{
#ifdef FSM_TRANSITION_PREFILTER
			for (uint8_t i = 0; i < curSta->_transitionCount; i++)
			{
				FSM_Transition *transition = curSta->_transitions[i];
#else
			for (uint8_t i = 0; i < _transitionCount; i++)
			{
				FSM_Transition* transition = &(_transitions[i]);
				if (transition->config.from != curSta->config.stateNo)
				{
					continue;
				}
#endif
				if (transition->_do_event_check(*this, curSta))
				{
					return;
				}
			}
			curSta = curSta->_parent;
		}

		curSta = &state;
		while (curSta != NULL)
		{
#ifdef FSM_TRANSITION_PREFILTER
			for (uint8_t i = 0; i < curSta->_transitionCount; i++)
			{
				FSM_Transition *transition = curSta->_transitions[i];
#else
			for (uint8_t i = 0; i < _transitionCount; i++)
			{
				FSM_Transition* transition = &(_transitions[i]);
				if (transition->config.from != curSta->config.stateNo)
				{
					continue;
				}
#endif
				if (transition->_do_timeout_check(*this, curSta, duration))
				{
					return;
				}
			}
			curSta = curSta->_parent;
		}
	};

	void FSM::event_set(uint32_t events)
	{
		this->_events.set(events);
	};

	void FSM::event_reset(uint32_t events)
	{
		this->_events.reset(events);
	};

	void FSM::update(uint32_t tick)
	{
		currentTick = tick;
		FSM_State* state = _currentState;
		if (state == nullptr)
		{
			return;
		}
		_events.update_begin();
		state->poll(*this);
		_transition_check(*state);
		_events.update_end();

		lastUpdateTick = tick;
	};

	void FSM::update_inc(uint32_t tickInc)
	{
		update(lastUpdateTick + tickInc);
	};

	FSM_State* FSM::_find_state_by_no(uint8_t stateNo)
	{
		for (int32_t i = 0; i < this->_stateCount; i++)
		{
			if (this->_states[i].config.stateNo == stateNo)
			{
				return &this->_states[i];
			}
		}
		return nullptr;
	};
} // namespace wibot::fsm
