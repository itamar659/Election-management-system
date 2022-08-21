#include "Citizen.h"
#include "Checks.h"

Citizen::Citizen(string name, string id, int birth_year, BaseDistrict* d)
	: _id(id), _name(name), _is_representative(false), _birth_year(birth_year), _district(d)
{
	if (id.size() != 9)
		throw invalid_argument("The ID of the citizen has to have 9 digits.");

	if (!Checks::Check_IsNumber(id))
		throw invalid_argument("The ID need to have only digits.");

}

Citizen::Citizen(const Citizen& other)
{
	_name = other._name;
	_id = other._id;
	_birth_year = other._birth_year;
	_district = other._district;
	_is_representative = other._is_representative;
}

ostream& Citizen::PrintSelf(ostream& os) const
{
	os << "Citizen name:       " << _name << endl;
	os << "Citizen ID:         " << _id << endl;
	os << "Citizen birth year: " << _birth_year << endl;
	os << "District name:      " << _district->GetName() << endl;
	return os;
}
const BaseDistrict* Citizen::GetDistrict() const
{
	if (!_district)
		throw logic_error("District is nullptr.");
	return _district;
}

void Citizen::Save(ostream& out) const
{
	SaveString(out, _name);
	SaveString(out, _id);
	out.write(rcastcc(&_birth_year), sizeof(_birth_year));
	out.write(rcastcc(&_is_representative), sizeof(_is_representative));

	if (!out.good())
		throw ios_base::failure(FILE_WRITE_ERROR);
}
void Citizen::Load(istream& in)
{
	LoadString(in, _name);
	LoadString(in, _id);
	in.read(rcastc(&_birth_year), sizeof(_birth_year));
	in.read(rcastc(&_is_representative), sizeof(_is_representative));

	if (!in.good())
		throw ios_base::failure(FILE_READ_ERROR);
}