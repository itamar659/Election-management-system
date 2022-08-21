#pragma once
#include "District.h"
#include "Citizen.h"
#include "Party.h"
#include <map>

// Make sure to add a citizen/district/party every time you add them into election
// Holds the information about:
// Citizen has the right to vote or not.
// How many votes each party have.
// How many people voted in each district.
class Results : private StringInterface
{
private:
	typedef string	CitizenID;
	typedef int		PartyID;
	typedef int		DistrictID;
	typedef int		Votes;
	typedef int		TotalVotes;
	typedef int		presentativesOwn;
	typedef int		DistrictOwn;
	typedef bool	Right;

public:
	// Methods for citizens
	void AddCitizen(const Citizen& citizen, Right right = true) { _citizens[citizen.GetID()] = right; }
	void UseRight(const Citizen&);
	bool HasRight(const Citizen&) const;

	// Methods for votes
	void AddParty(const Party& party) { _parties_votes[party.GetID()] = map<DistrictID, Votes>(); }
	void AddRepresentation(const Party&, const BaseDistrict&);
	bool HasRepresentation(const Party&, const BaseDistrict&) const;
	TotalVotes GetTotalVotesInParty(const Party& party) const;
	Votes GetPartyResultsInDistrict(const Party& party, const District& district);

	// Methods for districts
	void AddDistrict(const District& district) { _districts[district.GetID()] = 0; }
	TotalVotes GetTotalVotesInDistrict(const BaseDistrict& district) const { return _districts.at(district.GetID()); }
	void VoteFor(const Party&, const BaseDistrict&);

	void Clear();

	Results operator=(const Results&) = delete;

	// Save load methods
	void Save(ostream& out) const;
	void Load(istream& in);

	Results(const Results&) = delete;
	Results() : _citizens(), _parties_votes() {}
	~Results() {}
private:
	map<CitizenID, Right> _citizens;
	map<PartyID, map<DistrictID, Votes>> _parties_votes;
	map<DistrictID, TotalVotes> _districts;

	void SaveCitizens(ostream& out) const;
	void LoadCitizens(istream& in);
	void SaveParties(ostream& out) const;
	void LoadParties(istream& in);
	void SaveDistricts(ostream& out) const;
	void LoadDistricts(istream& in);
};