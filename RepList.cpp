#include "RepList.h"
#include "Compare.h"

RepList::RepList(District& district, Citizen& citizen)
	: _district(&district), _count_won_reps(0), _citizens()
{
	_citizens.Add(&citizen);
}

Citizen& RepList::operator[](int idx) const
{
	if (!(0 <= idx && idx < _citizens.GetLength()))
		throw out_of_range("Index is out of range.");

	return *_citizens[idx];
}

ostream& RepList::PrintSelf(ostream& os) const
{
	os << "In district: " << _district->GetName() << endl;
	for (int i = 0; i < _citizens.GetLength(); i++)
	{
		os << *_citizens[i];
	}

	return os;
}

void RepList::SetActualReps(int reps) {
	if (reps < 0)
		throw invalid_argument("actual representatives cannot be negative");

	_count_won_reps = reps;
}

void RepList::Save(ostream& out) const
{
	out.write(rcastcc(&_count_won_reps), sizeof(_count_won_reps));

	// Save District
	int d_id = _district->GetID();
	out.write(rcastcc(&d_id), sizeof(d_id));

	// Save Citizens
	int len = _citizens.GetLength();
	out.write(rcastcc(&len), sizeof(len));
	for (auto item : _citizens)
	{
		const string& c_id = item->GetID();
		SaveString(out, c_id);
	}

	if (!out.good())
		throw ios_base::failure(FILE_WRITE_ERROR);
}

void RepList::Load(istream& in, DynamicArray<Citizen*>& citizens, const DynamicArray<District*>& districts)
{
	in.read(rcastc(&_count_won_reps), sizeof(_count_won_reps));

	// Load District
	int d_id;
	in.read(rcastc(&d_id), sizeof(d_id));

	if (!districts.Contain(d_id, Compare::DistrictIDCmp))
		throw ios_base::failure(FILE_READ_ERROR);
	_district = districts.Find(d_id, Compare::DistrictIDCmp);

	// Load Citizens
	int len;
	in.read(rcastc(&len), sizeof(len));
	if (!in.good())
		throw ios_base::failure(FILE_READ_ERROR);

	for (int i = 0; i < len; i++)
	{
		string c_id;
		LoadString(in, c_id);

		if (!citizens.Contain(c_id, Compare::CitizenIDCmp))
			throw ios_base::failure(FILE_READ_ERROR);
		_citizens.Add(citizens.Find(c_id, Compare::CitizenIDCmp));
	}

	if (!in.good())
		throw ios_base::failure(FILE_READ_ERROR);
}