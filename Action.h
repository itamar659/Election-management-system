#pragma once
#include "BaseAction.h"

class Application;

template<class TargetClass, class Func>
class Action : public BaseAction
{
public:
	bool Execute() override {
		if (p_class)
			return function(*p_class);
		return function(**pp_class);
	}
	virtual BaseAction* Clone() const { 
		if (p_class)
			return new Action<TargetClass, Func>(p_class);
		return new Action<TargetClass, Func>(pp_class);
	}

	Action(TargetClass** t_class) : function(), pp_class(t_class), p_class(nullptr) {}
	Action(TargetClass* t_class) : function(), p_class(t_class), pp_class(nullptr) {}
private:
	Func function;
	TargetClass** pp_class;		// For pointers to allocated items
	TargetClass* p_class;		// For allocated items
};

// If the class approve pointers only
template<class TargetClass, class Func>
class Action<TargetClass*, Func> : public BaseAction
{
public:
	bool Execute() override {
		return function(_class);
	}
	virtual BaseAction* Clone() const { return new Action<TargetClass*, Func>(_class); }

	Action(TargetClass** t_class) : function(), _class(t_class) {}
private:
	Func function;
	TargetClass** _class;
};