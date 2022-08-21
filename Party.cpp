#include "Party.h"
#include "Compare.h"

int Party::_current_id = 0;

Party::Party(string name, Citizen& citizen)
	: _name(name), _president(&citizen), _id(++_current_id), _reps(), _own_districts(0)
{
}

ostream& Party::PrintSelf(ostream& os) const
{
	// Print party details
	os << "Party " << _id;
	os << ": " << _name << endl;

	// print president details
	os << "Party president: " << _president->GetName() << endl;

	// print all the representatives in each district
	if (_reps.GetLength() == 0)
		os << "List of representatives is empty.";

	for (int i = 0; i < _reps.GetLength(); i++)
		os << endl << *_reps[i];

	return os;
}

void Party::AddRepresentative(Citizen& citizen, District& district)
{
	auto cmp = [](const RepList* list, const District& d1) {return d1 == list->GetDistrict(); };
	if (_reps.Contain<const District&>(district, cmp))
	{
		RepList* list = _reps.Find<const District&>(district, cmp);
		list->AddCitizen(citizen);
	}
	else
	{
		RepList* list = new RepList(district, citizen);
		_reps.Add(list);
	}
}

int Party::CountActualReps() const
{
	int total = 0;
	for (int i = 0; i < _reps.GetLength(); i++)
		total += _reps[i]->CountActualReps();
	return total;
}

RepList& Party::GetRepListForDistrict(District& district) const
{
	auto cmp = [](const RepList* list, const District& d1) {return d1 == list->GetDistrict(); };
	return *_reps.Find<const District&>(district, cmp);
}

bool Party::HasRepresentationInDistrict(District& district) const
{
	auto cmp = [](const RepList* list, const District& d1) {return d1 == list->GetDistrict(); };
	return _reps.Contain<const District&>(district, cmp);
}

void Party::SaveStatic(ostream& out)
{
	out.write(rcastcc(&_current_id), sizeof(_current_id));
	if (!out.good())
		throw ios_base::failure(FILE_WRITE_ERROR);
}

void Party::LoadStatic(istream& in)
{
	in.read(rcastc(&_current_id), sizeof(_current_id));
	if (!in.good())
		throw ios_base::failure(FILE_READ_ERROR);
}

void Party::Save(ostream& out) const
{
	SaveString(out, _name);
	out.write(rcastcc(&_id), sizeof(_id));
	out.write(rcastcc(&_own_districts), sizeof(_own_districts));

	const string& p_id = _president->GetID();
	SaveString(out, p_id);

	// save president id
	// save all representatives
	int len = _reps.GetLength();
	out.write(rcastcc(&len), sizeof(len));
	for (auto rep : _reps)
		rep->Save(out);

	if (!out.good())
		throw ios_base::failure(FILE_WRITE_ERROR);
}

void Party::Load(istream& in, DynamicArray<Citizen*>& citizens, const DynamicArray<District*>& districts)
{
	LoadString(in, _name);
	in.read(rcastc(&_id), sizeof(_id));
	in.read(rcastc(&_own_districts), sizeof(_own_districts));

	// load president id
	string p_id;
	LoadString(in, p_id);

	if (!citizens.Contain(p_id, Compare::CitizenIDCmp))
		throw ios_base::failure(FILE_READ_ERROR);
	_president = citizens.Find(p_id, Compare::CitizenIDCmp);

	// load all representatives
	int len;
	in.read(rcastc(&len), sizeof(len));
	for (int i = 0; i < len; i++)
	{
		RepList* list = new RepList();
		list->Load(in, citizens, districts);
		_reps.Add(list);
	}

	if (!in.good())
		throw ios_base::failure(FILE_READ_ERROR);
}