#include "BaseDistrict.h"

int BaseDistrict::_current_id = 0;

void BaseDistrict::Save(ostream& out) const
{
	SaveString(out, _name);
	out.write(rcastcc(&_id), sizeof(_id)); 
	out.write(rcastcc(&_num_of_reps), sizeof(_num_of_reps));
	if (!out.good())
		throw ios_base::failure(FILE_WRITE_ERROR);

}
void BaseDistrict::Load(istream& in)
{
	LoadString(in, _name);
	in.read(rcastc(&_id), sizeof(_id));
	in.read(rcastc(&_num_of_reps), sizeof(_num_of_reps));
	if (!in.good())
		throw ios_base::failure(FILE_READ_ERROR);
}

void BaseDistrict::SaveStatic(ostream& out)
{
	out.write(rcastcc(&_current_id), sizeof(_current_id));
	if (!out.good())
		throw ios_base::failure(FILE_WRITE_ERROR);
}
void BaseDistrict::LoadStatic(istream& in)
{
	in.read(rcastc(&_current_id), sizeof(_current_id));
	if (!in.good())
		throw ios_base::failure(FILE_READ_ERROR);
}

ostream& BaseDistrict::PrintSelf(ostream& os) const
{
	os << "District ID: " << _id << endl;
	os << "District name: " << _name << endl;
	os << "Number of representatives: " << _num_of_reps << endl;

	return os;
}