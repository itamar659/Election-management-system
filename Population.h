#pragma once
#include "Citizen.h"
#include "DynamicArray.h"

class Population : public DynamicArray<Citizen*>
{
public:
	const Citizen* last() const;
	void Add(const Citizen& value);
	void push_back(const Citizen& value);

	void Save(ostream& out) const;
	void Load(istream& in);

	~Population();
};