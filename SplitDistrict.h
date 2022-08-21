#pragma once
#include "DynamicArray.h"
#include "District.h"

class SplitDistrict : public District
{
public:
	//Getters
	DynamicArray<Citizen*>& GetRepList(const Citizen& president) const;
	const Citizen& GetPresident(int idx) const { return *_presidents[idx]; }
	int CountPresidents() const { return _presidents.GetLength(); }

	//Methods
	void AddRepresentative(Citizen& citizen, Citizen& president);
	//void AddPresident(Citizen& citizen) { return _presidents.Add(&citizen); }
	void ResetRepsList() { _representatives.Clear(); _presidents.Clear(); }

	// Static Methods
	static string GetType() { return typeid(SplitDistrict).name(); }

	// save / load
	virtual void Save(ostream& out) const;
	virtual void Load(istream& in);

	// Operators
	SplitDistrict operator=(const SplitDistrict&) = delete;
	friend ostream& operator<<(ostream& os, const SplitDistrict& district) { return district.PrintSelf(os); }

	//Constructor
	SplitDistrict(const SplitDistrict&) = delete;
	SplitDistrict() : _representatives(), _num_of_presidents(0) {}
	SplitDistrict(string name, int num_of_represents);
	virtual ~SplitDistrict() {}

private:
	DynamicArray<Citizen*> _presidents;
	DynamicArray<DynamicArray<Citizen*>> _representatives;
	int _num_of_presidents;

	virtual ostream& PrintSelf(ostream&) const;
};