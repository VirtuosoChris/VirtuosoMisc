#ifndef V_FSM_STATE
#define V_FSM_STATE

#include <functional>

///null function for state machine default values.  Note that this function won't actually be inlined since it needs to be fptr'd.
inline void doNothing(){}


///FSM state - we have 3 function pointers : one each to enter and exit the state, as well as an execute function for states which have logic that will be executed
///repeatedly so long as the state is active
struct FsmState{

	std::function<void (void)> enterState;
	std::function<void (void)> exitState;
	std::function<void (void)> executeState;

	FsmState()
	:
	enterState(&doNothing),
	exitState(&doNothing),
	executeState(&doNothing)
	{
	}

};

#endif
