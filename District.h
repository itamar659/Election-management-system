#pragma once
#include "Citizen.h"
#include "BaseDistrict.h"
#include "DynamicArray.h"

class District : public BaseDistrict
{
public:
	int CountCitizens() const { return _citizens.GetLength(); }
	int CountRepresentatives() const { return _num_of_reps; }
	const DynamicArray<const Citizen*>& GetCitizens() const { return _citizens; }

	// Methods
	void AddCitizen(const Citizen& c) { _citizens.Add(&c); }
	virtual void ResetRepsList() = 0;

	// Operators
	District operator=(const District&) = delete;
	bool operator==(const District& other) const { return _id == other._id; }
	friend ostream& operator<<(ostream& os, const District& district) { return district.PrintSelf(os); }

	// Constructors
	District(const District&) = delete;
	District() : BaseDistrict("", 0), _citizens() {}
	District(string name, int num_of_reps) : BaseDistrict(name, num_of_reps), _citizens() {}
	virtual ~District() {}

protected:
	DynamicArray<const Citizen*> _citizens;
};