#include "UnionDistrict.h"

UnionDistrict::UnionDistrict(string name, int num_of_represents) :
	District(name, num_of_represents), _president(nullptr), _representatives()
{
}

ostream& UnionDistrict::PrintSelf(ostream& os) const
{
	District::PrintSelf(os);
	os << "Type: " << "Union District" << endl;
	return os;
}

void UnionDistrict::Save(ostream& out) const
{
	District::Save(out);
}
void UnionDistrict::Load(istream& in)
{
	District::Load(in);
}