#include "Results.h"


void Results::UseRight(const Citizen& citizen)
{
	auto right = _citizens.find(citizen.GetID());
	if (right == _citizens.end())
		throw invalid_argument("Can not find this citizen.");

	right->second = false;
}

bool Results::HasRight(const Citizen& citizen) const
{
	auto right = _citizens.find(citizen.GetID());
	if (right == _citizens.end())
		throw invalid_argument("Can not find this citizen.");

	return right->second;
}

void Results::Clear()
{
	_citizens.clear();
	for (pair<PartyID, map<DistrictID, Votes>> p : _parties_votes)
		p.second.clear();
	_parties_votes.clear();
}

void Results::AddRepresentation(const Party& party, const BaseDistrict& district)
{
	map<DistrictID, Votes>& party_status = _parties_votes.at(party.GetID());
	//party_status.insert(pair<DistrictID, Votes>(district.GetID(), 0));
	party_status[district.GetID()] = 0;
}

bool Results::HasRepresentation(const Party& party, const BaseDistrict& district) const
{
	map<DistrictID, Votes> party_status = _parties_votes.at(party.GetID());
	auto itr = party_status.find(district.GetID());
	return itr != party_status.end();
}

void Results::VoteFor(const Party& party, const BaseDistrict& district)
{
	map<DistrictID, Votes>& party_status = _parties_votes.at(party.GetID());

	auto d = _districts.find(district.GetID());
	auto p_d = party_status.find(district.GetID());
	if (p_d == party_status.end())
		throw invalid_argument("Can not find this district.");

	d->second += 1;
	p_d->second += 1;
}

int Results::GetPartyResultsInDistrict(const Party& party, const District& district)
{
	map<DistrictID, Votes>& party_status = _parties_votes.at(party.GetID());
	auto itr = party_status.find(district.GetID());
	if (itr == party_status.end())
		return 0;
	return itr->second;
}

int Results::GetTotalVotesInParty(const Party& party) const
{
	TotalVotes total = 0;
	auto& party_status = _parties_votes.at(party.GetID());
	for (auto p : party_status)
		total += p.second;
	return total;
}

void Results::Save(ostream& out) const
{
	SaveCitizens(out);
	SaveParties(out);
	SaveDistricts(out);
}

void Results::Load(istream& in)
{
	Clear();

	LoadCitizens(in);
	LoadParties(in);
	LoadDistricts(in);
}

void Results::SaveCitizens(ostream& out) const
{
	int len = _citizens.size();
	out.write(rcastcc(&len), sizeof(len));
	for (pair<CitizenID, Right> p : _citizens)
	{
		SaveString(out, p.first);
		out.write(rcastcc(&p.second), sizeof(p.second));
	}
	if (!out.good())
		throw ios_base::failure(FILE_WRITE_ERROR);
}

void Results::LoadCitizens(istream& in)
{
	int len;
	in.read(rcastc(&len), sizeof(len));
	if (!in.good())
		throw ios_base::failure(FILE_READ_ERROR);

	for (int i = 0; i < len; i++)
	{
		CitizenID citizen_id;
		Right right;
		LoadString(in, citizen_id);
		in.read(rcastc(&right), sizeof(right));

		_citizens.insert(pair<CitizenID, Right>(citizen_id, right));
	}
	if (!in.good())
		throw ios_base::failure(FILE_READ_ERROR);
}

void Results::SaveParties(ostream& out) const
{
	int len = _parties_votes.size();
	out.write(rcastcc(&len), sizeof(len));

	for (pair<PartyID, map<DistrictID, Votes>> p : _parties_votes)
	{
		int size = p.second.size();
		out.write(rcastcc(&p.first), sizeof(p.first));
		out.write(rcastcc(&size), sizeof(size));

		for (pair<DistrictID, Votes> p2 : p.second)
		{
			out.write(rcastcc(&p2.first), sizeof(p2.first));
			out.write(rcastcc(&p2.second), sizeof(p2.second));
		}

		if (!out.good())
			throw ios_base::failure(FILE_WRITE_ERROR);
	}

	if (!out.good())
		throw ios_base::failure(FILE_WRITE_ERROR);
}

void Results::LoadParties(istream& in)
{
	int len;
	in.read(rcastc(&len), sizeof(len));
	if (!in.good())
		throw ios_base::failure(FILE_READ_ERROR);

	for (int i = 0; i < len; i++)
	{
		PartyID party_id;
		int size;
		map<DistrictID, Votes> party_status;

		in.read(rcastc(&party_id), sizeof(party_id));
		in.read(rcastc(&size), sizeof(size));
		if (!in.good())
			throw ios_base::failure(FILE_READ_ERROR);

		for (int j = 0; j < size; j++)
		{
			DistrictID district_id;
			Votes votes;
			in.read(rcastc(&district_id), sizeof(district_id));
			in.read(rcastc(&votes), sizeof(votes));
			party_status[district_id] = votes;
		}

		if (!in.good())
			throw ios_base::failure(FILE_READ_ERROR);
		_parties_votes[party_id] = party_status;

	}

	if (!in.good())
		throw ios_base::failure(FILE_READ_ERROR);
}

void Results::SaveDistricts(ostream& out) const
{
	int len = _districts.size();
	out.write(rcastcc(&len), sizeof(len));
	for (pair<DistrictID, TotalVotes> p : _districts)
	{
		out.write(rcastcc(&p.first), sizeof(p.first));
		out.write(rcastcc(&p.second), sizeof(p.second));
	}
	if (!out.good())
		throw ios_base::failure(FILE_WRITE_ERROR);
}

void Results::LoadDistricts(istream& in)
{
	int len;
	in.read(rcastc(&len), sizeof(len));
	if (!in.good())
		throw ios_base::failure(FILE_READ_ERROR);

	for (int i = 0; i < len; i++)
	{
		DistrictID district_id;
		TotalVotes TVotes;
		in.read(rcastc(&district_id), sizeof(district_id));
		in.read(rcastc(&TVotes), sizeof(TVotes));

		_districts.insert(pair<DistrictID, TotalVotes>(district_id, TVotes));
	}
	if (!in.good())
		throw ios_base::failure(FILE_READ_ERROR);
}