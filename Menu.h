#pragma once
#include "MenuOption.h"
#include "DynamicArray.h"
#include "Action.h"

class Menu
{
public:
	// Methods
	void Add(MenuOption* item) { _options.Add(item); }
	void Display(ostream& os) const
	{
		for (int i = 0; i < _options.GetLength(); i++)
			os << _options[i]->GetActiveCode() << ". " << _options[i]->GetLabel() << endl;
	}

	bool Trigger(const string& key)
	{
		if (!_options.Contain<const string&>(key, compareKeys))
			throw invalid_argument("There is no such an action.");

		MenuOption& choosen = *_options.Find<string>(key, compareKeys);
		return choosen.GetAction().Execute();
	}

	~Menu() {
		for (int i = 0; i < _options.GetLength(); i++)
			delete _options[i];
	}
private:
	DynamicArray<MenuOption*> _options;

	static bool compareKeys(const MenuOption* b, const string& a) { return a == b->GetActiveCode(); }
};