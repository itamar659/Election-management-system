#pragma once
#include "Citizen.h"
#include "DynamicArray.h"
#include "BaseObject.h"
#include "RepList.h"

class Party : public BaseObject
{
public:
	// Getters
	const string& GetName() const { return _name; }
	int GetID() const { return _id; }
	Citizen& GetPresident() const { return *_president; }
	RepList& GetRepListForDistrict(District& district) const;
	int CountOwnDistricts() const { return _own_districts; }

	// Methods
	bool HasRepresentationInDistrict(District& district) const;
	void AddRepresentative(Citizen& citizen, District& district);
	void AddDistrictWon() { ++_own_districts; }
	int CountActualReps() const;

	// Save load methods
	virtual void Save(ostream&) const;
	virtual void Load(istream&) {};
	void Load(istream&, DynamicArray<Citizen*>& citizens, const DynamicArray<District*>& districts);
	static void SaveStatic(ostream&);
	static void LoadStatic(istream& in);

	// Overload Operators:
	Party& operator=(const Party&) = delete;
	bool operator==(const Party& other) { return _id == other._id; }
	friend ostream& operator<<(ostream& os, const Party& party) { return party.PrintSelf(os); }

	// Constructors
	Party(const Party&) = delete;
	Party() : _id(0), _president(nullptr), _own_districts(0) {}
	Party(string, Citizen&);
	virtual ~Party() { 
		for (int i = 0; i < _reps.GetLength(); i++)
			delete _reps[i];
	}

private:
	string _name;
	int _id;
	Citizen* _president;
	DynamicArray<RepList*> _reps;
	int _own_districts;

	static int _current_id;

	virtual ostream& PrintSelf(ostream& os) const;
};