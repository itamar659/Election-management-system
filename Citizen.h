#pragma once
#include "BaseObject.h"
#include "BaseDistrict.h"

class Citizen : public BaseObject
{
public:
	// Getters
	const string& GetName() const { return _name; }
	const string& GetID() const { return _id; }
	int GetBirthYear() const { return _birth_year; }
	const BaseDistrict* GetDistrict() const;
	bool GetIsRepresentative() const { return _is_representative; }

	// Setters
	void SetRepresentativeStatus(bool status) { _is_representative = status; }
	void SetDistrict(BaseDistrict* d) { _district = d; }

	// Save load methods
	virtual void Save(ostream&) const;
	virtual void Load(istream&);

	// Operators
	Citizen operator=(const Citizen&) = delete;
	bool operator==(const Citizen& other) const { return _id.compare(other._id); }
	bool operator!=(const Citizen& other) const { return !(*this == other); }
	friend ostream& operator<<(ostream& os, const Citizen& citizen) { return citizen.PrintSelf(os); }

	// Constructors
	Citizen() : _id("000000000"), _name(), _is_representative(false), _birth_year(0), _district(nullptr) {}
	Citizen(string name, string id, int birth_year, BaseDistrict* d);
	virtual ~Citizen() {}
private:
	string _name;
	string _id;
	int _birth_year;
	bool _is_representative;
	BaseDistrict* _district;

	virtual ostream& PrintSelf(ostream&) const;
	Citizen(const Citizen&);

	friend class Population;
};