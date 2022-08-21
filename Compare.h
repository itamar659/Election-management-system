#pragma once
#include "Citizen.h"
#include "District.h"
#include "Party.h"

class Compare
{
public:
	static bool CitizenIDCmp(const Citizen* citizen, const string& id) { return (citizen->GetID().compare(id)) == 0; }
	static bool DistrictIDCmp(const District* district, int id) { return district->GetID() == id; }
	static bool PartyIDCmp(const Party* party, int id) { return party->GetID() == id; }

private:
	Compare() {}
};