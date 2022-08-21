#pragma once
#include <fstream>
#include "UnionDistrict.h"
#include "SplitDistrict.h"
#include "Citizen.h"
#include "Party.h"
#include "Date.h"
#include "Results.h"
#include "Population.h"

class Election : public BaseObject
{
public:
	// Methods
	inline string GetAction()
	{
		string input;
		cin >> input;
		return input;
	}

	// Save load methods
	void SaveArrays(ostream& out) const;
	void LoadArrays(istream& in);
	virtual void Save(ostream& out) const;
	virtual void Load(istream& in);

	// Operators
	Election operator=(const Election& other) = delete;

	// Constructors
	Election(const Election&) = delete;
	Election() : _citizens(), _districts(), _parties() {}
	virtual ~Election();

protected:
	date::Date _date;
	Results _results;
	Population _citizens;
	DynamicArray<District*> _districts;
	DynamicArray<Party*> _parties;

	friend class ElectionManager;
	friend class ElectionCalculator;
	friend class Checks;
private:
	virtual ostream& PrintSelf(ostream& os) const { return os; }
};


class SimpleElection : public Election
{
public:
	static string GetType() { return typeid(SimpleElection).name(); }
};

class RegularElection : public Election
{
public:
	static string GetType() { return typeid(RegularElection).name(); }
};