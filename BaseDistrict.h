#pragma once
#include "BaseObject.h"

class BaseDistrict : public BaseObject
{
public:
	// Getters
	virtual int CountCitizens() const = 0;
	string GetName() const { return _name; }
	int GetID() const { return _id; }
	int GetNumOfReps() const { return _num_of_reps; }

	// Load save methods
	virtual void Save(ostream& out) const;
	virtual void Load(istream& in);
	static void SaveStatic(ostream& out);
	static void LoadStatic(istream& in);

	// Operators
	bool operator==(const BaseDistrict& other) const { return _id == other._id; }
	friend ostream& operator<<(ostream& os, const BaseDistrict& district) { return district.PrintSelf(os); }

	// Constuctors
	BaseDistrict() : _name(), _id(0), _num_of_reps(0) {}
	BaseDistrict(string name, int num_of_reps) : _name(name), _id(++_current_id), _num_of_reps(num_of_reps)
	{
		if (num_of_reps < 0)
			throw invalid_argument("Number of presidents can not be negative.");
	}
	virtual ~BaseDistrict() {}

protected:
	string _name;
	int _id;
	int _num_of_reps;

	virtual ostream& PrintSelf(ostream&) const;

private:
	static int _current_id;
};