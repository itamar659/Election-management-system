#pragma once
#include "Election.h"
#include "Checks.h"
#include <vector>
#include <map>
#include <algorithm>

class ElectionCalculator
{
public:
	void CalculateResults(Election& elect);

	static Party& CalcWinnerInDistrictUnion(Election& elect, District& district);
	static inline float CalculatePrecentage(Results& res, District& dis)
	{
		return (float(res.GetTotalVotesInDistrict(dis)) / dis.CountCitizens() * 100);
	}

	vector<Citizen*> GetWinnersInOrder(Election& elect);

private:
	void CalcRepsInDistrict(Election& elect, District& cur_district);

	// Calculate how much representatives you need to contribute from each party to the current district.
	// Return an array of integers, which at position 'i' you'll find the required number of representatives for the party in index 'i'.
	vector<int> CalcPartyReps(Election& elect, District& cur_district);

	// Make sure every party will have enough representatives to give to the current district.
	void FixPartyReps(Election& elect, District& cur_district, vector<int>& party_reps);
};