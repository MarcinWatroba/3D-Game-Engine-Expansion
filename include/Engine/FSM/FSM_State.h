#pragma once

namespace fsm
{
	enum FSM_Command
	{
		Continue,//keep running current state
		Finish,//end current state and switch to previous state
		Switch//switch to a different state (as indicated by FSM_State::nextState)
	};
}

template <typename T>
class FSM_State
{
private:
	//state ID for lookup
	const int stateID;
	//internal run status
	int runStatus;

protected:
	//state machine data
	T* data;
	//next state to switch to
	unsigned int nextStateID;
	//called when state is entered 
	virtual void OnEnter() = 0;
	//called every update
	virtual fsm::FSM_Command OnRun() = 0;
	//called when state is exited
	virtual void OnExit() = 0;

public:
	//get state ID to switch to
	const unsigned int NextDesiredStateID();
	//get state ID
	const int StateID();
	//run the state
	fsm::FSM_Command Run(T* data_in);
	//end and exit the state
	void Finish();
	//create state with given ID
	FSM_State(const int setStateID);
};




template <typename T>
FSM_State<T>::FSM_State(const int setStateID) :
	stateID(setStateID),
	runStatus(0)
{}

template <typename T>
const unsigned int FSM_State<T>::NextDesiredStateID()
{
	return nextStateID;
}

template <typename T>
const int FSM_State<T>::StateID()
{
	return stateID;
}

template <typename T>
fsm::FSM_Command FSM_State<T>::Run(T* data_in)
{
	data = data_in;
	if (runStatus == 0)
	{
		OnEnter();
		runStatus = 1;
	}
	return OnRun();
}

template <typename T>
void FSM_State<T>::Finish()
{
	runStatus = 0;
	OnExit();
}