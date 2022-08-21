#include "Checks.h"

void Checks::Check_AddDistrict(const Election& elect)
{
	if (elect.GetType() == SimpleElection::GetType())
		throw logic_error("Can't make this action in simple election");
}

void Checks::Check_AddCitizen(const Election& elect, const string& citizen_id, int district_id, int age)
{
	if (!elect._districts.Contain(district_id, Compare::DistrictIDCmp))
		throw bad_input("No district was found by this district id");

	if (elect._citizens.Contain(citizen_id, Compare::CitizenIDCmp))
		throw bad_input("Citizen exists already.");

	if (age < 18)
		throw bad_input("Citizen has to be at least 18 years old.");
}

void Checks::Check_AddParty(const Election& elect, const string& citizen_id)
{
	if (!elect._citizens.Contain(citizen_id, Compare::CitizenIDCmp))
		throw bad_input("No such citizen exists.");

	const Citizen& citizen = *elect._citizens.Find(citizen_id, Compare::CitizenIDCmp);
	if (citizen.GetIsRepresentative())
		throw bad_input("Citizen already a representative.");
}

void Checks::Check_AddRepresentative(const Election& elect, const string& citizen_id, int district_id, int party_id)
{
	if (!elect._citizens.Contain(citizen_id, Compare::CitizenIDCmp))
		throw bad_input("No such citizen exists.");

	if (!elect._parties.Contain(party_id, Compare::PartyIDCmp))
		throw bad_input("No such party exists.");

	if (!elect._districts.Contain(district_id, Compare::DistrictIDCmp))
		throw bad_input("No such district exists.");

	if (elect._citizens.Find(citizen_id, Compare::CitizenIDCmp)->GetIsRepresentative())
		throw bad_input("This citizen already a representative.");
}

void Checks::Check_PrintDistricts(const Election& elect)
{
	Check_AddDistrict(elect);
}

void Checks::Check_Vote(const Election& elect, const string& citizen_id, int party_id)
{
	if (!elect._citizens.Contain(citizen_id, Compare::CitizenIDCmp))
		throw bad_input("No such citizen exists.");

	if (!elect._parties.Contain(party_id, Compare::PartyIDCmp))
		throw bad_input("No such party exists.");

	const Citizen& citizen = *elect._citizens.Find(citizen_id, Compare::CitizenIDCmp);
	if (!elect._results.HasRight(citizen))
		throw bad_input("This citizen has no voting right.");

	const Party& party = *elect._parties.Find(party_id, Compare::PartyIDCmp);
	if (!elect._results.HasRepresentation(party, *citizen.GetDistrict()))
		throw bad_input("No representatives to this citizen's district.");
}

void Checks::Check_ElectionResults(const Election& elect)
{
	if (elect._districts.GetLength() == 0)
		throw logic_error("You dont even have districts.");

	for (int i = 0; i < elect._districts.GetLength(); i++)
	{
		int total = 0;
		District& district = *elect._districts[i];
		int party_size = elect._parties.GetLength();

		for (int j = 0; j < party_size; j++)
		{
			if (elect._parties[j]->HasRepresentationInDistrict(district))
			{
				const RepList& list = elect._parties[j]->GetRepListForDistrict(district);
				total += list.GetLength();
			}
		}

		if (elect._districts[i]->GetNumOfReps() > total)
			throw logic_error("Need to fill all the sits in each district.");
	}
}

bool Checks::Check_IsNumber(const string& str)
{
	int n = str.size();
	for (int i = 0; i < n; i++)
		if (str[i] < '0' || str[i] > '9')
			return false;
	return true;
}