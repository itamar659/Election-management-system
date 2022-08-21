#include "ElectionCalculator.h"
#include <algorithm>

Party& ElectionCalculator::CalcWinnerInDistrictUnion(Election& elect, District& district)
{
	int district_id = district.GetID();
	int total_votes = elect._results.GetTotalVotesInDistrict(district);
	int max_idx = 0;
	int max_percentage = 0;
	int size = elect._parties.GetLength();

	for (int j = 0; j < size; j++)
	{
		Party& cur_party = *elect._parties[j];

		int cur_party_votes = elect._results.GetPartyResultsInDistrict(cur_party, district);
		int percentage = static_cast<int>((static_cast<double>(cur_party_votes) / total_votes) * 100);

		if (max_percentage < percentage)
		{
			max_percentage = percentage;
			max_idx = j;
		}
		else if (max_percentage == percentage)
		{
			if (elect._parties[max_idx]->GetID() > cur_party.GetID())
				max_idx = cur_party.GetID();
		}
	}

	elect._parties[max_idx]->AddDistrictWon();
	if (district.GetType() == UnionDistrict::GetType())
		dynamic_cast<UnionDistrict&>(district).SetPresident(elect._parties[max_idx]->GetPresident());
	return *elect._parties[max_idx];
}

vector<Citizen*> ElectionCalculator::GetWinnersInOrder(Election& elect)
{
	vector<pair<int, Citizen*>> tree;
	for (auto party : elect._parties)
	{
		pair<int, Citizen*> p1;
		p1.first = party->CountActualReps();
		p1.second = &party->GetPresident();
		tree.push_back(p1);
	}

	auto cmp = [](pair<int, Citizen*>& a, pair<int, Citizen*>& b) {return a.first > b.first; };
	sort(tree.begin(), tree.end(), cmp);

	vector<Citizen*> presidents;
	for (auto item : tree)
		presidents.push_back(item.second);
	return presidents;
}

void ElectionCalculator::CalculateResults(Election& elect)
{
	for (int i = 0; i < elect._districts.GetLength(); i++)
	{
		elect._districts[i]->ResetRepsList();

		if (elect._results.GetTotalVotesInDistrict(*elect._districts[i]) == 0)
			throw logic_error("Need to vote in every district.");

		CalcRepsInDistrict(elect, *elect._districts[i]);
	}
}

void ElectionCalculator::CalcRepsInDistrict(Election& elect, District& cur_district)
{
	bool is_split_district = (cur_district.GetType() == SplitDistrict::GetType());
	int size = elect._parties.GetLength();

	vector<int> party_reps = CalcPartyReps(elect, cur_district);
	FixPartyReps(elect, cur_district, party_reps);

	// Set representatives in district
	for (int i = 0; i < size; i++)
	{
		if (!elect._parties[i]->HasRepresentationInDistrict(cur_district))
			continue;

		auto& list = elect._parties[i]->GetRepListForDistrict(cur_district);
		list.SetActualReps(party_reps[i]);

		for (int k = 0; k < party_reps[i]; k++)
		{
			Citizen& cur = list[k];

			if (is_split_district)
				dynamic_cast<SplitDistrict&>(cur_district).AddRepresentative(cur, elect._parties[i]->GetPresident());
			else
				dynamic_cast<UnionDistrict&>(cur_district).AddRepresentative(cur);
		}
	}
}

vector<int> ElectionCalculator::CalcPartyReps(Election& elect, District& cur_district)
{
	int size = elect._parties.GetLength();
	vector<int> party_reps(size, 0);
	vector<int> party_remainders(size, 0);

	int total_reps = cur_district.GetNumOfReps();
	int total_votes = elect._results.GetTotalVotesInDistrict(cur_district);


	int percentage, party_votes, max_idx;
	double ratio;

	for (int i = 0; i < size; i++)
	{
		party_votes = elect._results.GetPartyResultsInDistrict(*elect._parties[i], cur_district);
		ratio = (static_cast<double>(party_votes) / total_votes);
		percentage = static_cast<int>(total_reps * ratio * 100);

		party_reps[i] = percentage / 100;
		party_remainders[i] = percentage % 100;
		total_reps -= percentage / 100;
	}

	for (int i = 0; i < total_reps; i++)
	{
		auto max_ele = max_element(party_remainders.begin(), party_remainders.end());
		max_idx = distance(party_remainders.begin(), max_ele);
		party_remainders[max_idx] = 0;
		party_reps[max_idx] += 1;
	}

	return party_reps;
}

void ElectionCalculator::FixPartyReps(Election& elect, District& cur_district, vector<int>& party_reps)
{
	int size = elect._parties.GetLength();

	// Check if the party has enough reps to give, if not reserve it to other party in winning row.
	int reserved = 0;
	for (int i = 0; i < size; i++)
	{
		if (!elect._parties[i]->HasRepresentationInDistrict(cur_district))
			continue;

		auto& list = elect._parties[i]->GetRepListForDistrict(cur_district);
		int is_enough_reps = party_reps[i] - list.GetLength();

		if (is_enough_reps > 0)
		{
			reserved += is_enough_reps;
			party_reps[i] -= is_enough_reps;
		}
	}
	// add the reserved representative by order if there is extra representatives
	for (int i = 0; i < size && reserved != 0; i++)
	{
		if (!elect._parties[i]->HasRepresentationInDistrict(cur_district))
			continue;

		auto& list = elect._parties[i]->GetRepListForDistrict(cur_district);
		int cur_reps = party_reps[i] - list.GetLength();

		if (cur_reps < 0)
		{
			int n = min(reserved, -cur_reps);
			party_reps[i] += n;
			reserved -= n;
		}
	}
}