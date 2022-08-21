#pragma once
#include "Action.h"
#include "BaseObject.h"

// An option to add to the menu. Have a key to press and the label what it's doing.
class MenuOption
{
public:
	// Getters
	const string& GetLabel() const { return _label; }
	const string& GetActiveCode() const { return _active_code; }
	BaseAction& GetAction() const { return *_action; }

	// Setters
	void SetActiveCode(string key) { _active_code = key; }
	void SetLabel(string label) { _label = label; }
	void SetAction(BaseAction* action) { 
		delete _action;
		_action = action; 
	}

	MenuOption operator=(const MenuOption& other) {
		if (this != &other)
		{
			_label = other._label;
			_active_code = other._active_code;
			_action = other._action->Clone();
		}
		return *this;
	}

	// Constructors
	MenuOption(const MenuOption& other) { *this = other; }
	MenuOption() : _label(), _active_code(), _action(nullptr) {}
	MenuOption(string key, string label, BaseAction* action) : _label(label), _active_code(key), _action(action) {}
	~MenuOption() {
		delete _action;
	}

private:
	string _label;
	string _active_code;
	BaseAction* _action;
};