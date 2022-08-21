#pragma once
#include "District.h"
#include "DynamicArray.h"

class UnionDistrict : public District
{
public:
	// Getters
	const Citizen& GetPresident() const { return *_president; }

	// Setters
	void SetPresident(Citizen& citizen) { _president = &citizen; }

	// Methods
	void AddRepresentative(Citizen& citizen) { _representatives.Add(&citizen); }
	void ResetRepsList() { return _representatives.Clear(); }

	// Static Methods
	static string GetType() { return typeid(UnionDistrict).name(); }

	// save / load
	virtual void Save(ostream& out) const;
	virtual void Load(istream& in);

	// overload operators
	UnionDistrict operator=(const UnionDistrict&) = delete;
	friend ostream& operator<<(ostream& os, const UnionDistrict& district) { return district.PrintSelf(os); }

	// Constructor
	UnionDistrict(const UnionDistrict&) = delete;
	UnionDistrict() : _president(nullptr), _representatives() {}
	UnionDistrict(string name, int num_of_represents);
	virtual ~UnionDistrict() {}

private:
	Citizen* _president;
	DynamicArray<Citizen*> _representatives;

	virtual ostream& PrintSelf(ostream&) const;
};