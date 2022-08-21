#include "SplitDistrict.h"
#include "Compare.h"

SplitDistrict::SplitDistrict(string name, int num_of_reps) :
	District(name, num_of_reps), _num_of_presidents(0), _representatives()
{
}

ostream& SplitDistrict::PrintSelf(ostream& os) const
{
	District::PrintSelf(os);
	os << "Type: " << "Split District" << endl;
	return os;
}

DynamicArray<Citizen*>& SplitDistrict::GetRepList(const Citizen& president) const
{
	int idx = 0;
	while (_presidents[idx]->GetID() != president.GetID())
		++idx;

	if (idx == _presidents.GetLength())
		throw invalid_argument("The president not exists.");

	return const_cast<DynamicArray<Citizen*>&>(_representatives[idx]); // Allow add or remove from the representative list.
}

void SplitDistrict::AddRepresentative(Citizen& citizen, Citizen& president)
{
	if (!_presidents.Contain(president.GetID(), Compare::CitizenIDCmp))
	{
		_presidents.Add(&president);
		DynamicArray<Citizen*> new_list;
		new_list.Add(&citizen);
		_representatives.Add(new_list);
	}
	else
		GetRepList(president).Add(&citizen);
}

void SplitDistrict::Save(ostream& out) const
{
	District::Save(out);
	out.write(rcastcc(&_num_of_presidents), sizeof(_num_of_presidents));
	if (!out.good())
		throw ios_base::failure(FILE_WRITE_ERROR);
}
void SplitDistrict::Load(istream& in)
{
	District::Load(in);
	in.read(rcastc(&_num_of_presidents), sizeof(_num_of_presidents));
	if (!in.good())
		throw ios_base::failure(FILE_READ_ERROR);
}