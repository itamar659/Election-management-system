#pragma once
#include "DynamicArray.h"
#include "District.h"

class RepList : public BaseObject
{
public:
	// Getters
	int GetLength() const { return _citizens.GetLength(); }
	District& GetDistrict() const { return *_district; }
	int CountActualReps() const { return _count_won_reps; }

	// Setters
	void SetActualReps(int reps);

	// Public Methods
	void AddCitizen(Citizen& civ) { _citizens.Add(&civ); }

	// Overload Operators
	RepList operator=(const RepList&) = delete;
	Citizen& operator[](int idx) const;
	friend std::ostream& operator<<(std::ostream& os, const RepList& rep) { return rep.PrintSelf(os); }

	// save load methods
	virtual void Save(ostream& out) const;
	virtual void Load(istream& in) {}
	void Load(istream& in, DynamicArray<Citizen*>& citizens, const DynamicArray<District*>& districts);

	// Constructors
	RepList(const RepList&) = delete;
	RepList(District& district, Citizen& civ);
	RepList() : _district(nullptr), _count_won_reps(), _citizens() {}

private:
	District* _district;
	DynamicArray<Citizen*> _citizens;
	int _count_won_reps;

	virtual ostream& PrintSelf(ostream&) const;
};