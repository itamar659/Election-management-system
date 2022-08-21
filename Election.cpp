#include "Election.h"


Election::~Election()
{
	for (int i = 0; i < _districts.GetLength(); i++)
		delete _districts[i];
	for (int i = 0; i < _parties.GetLength(); i++)
		delete _parties[i];
	_results.Clear();
}

void Election::SaveArrays(ostream& out) const
{
	int len = _districts.GetLength();
	out.write(rcastcc(&len), sizeof(len));
	for (auto item : _districts)
	{
		const string& type = item->GetType();
		SaveString(out, type);

		item->Save(out);
	}

	_citizens.Save(out);

	len = _parties.GetLength();
	out.write(rcastcc(&len), sizeof(len));
	for (auto item : _parties)
		item->Save(out);
}

void Election::LoadArrays(istream& in)
{
	int len;
	in.read(rcastc(&len), sizeof(len));
	for (int i = 0; i < len; i++)
	{
		District* d;
		string type;
		LoadString(in, type);

		if (type == UnionDistrict::GetType())
			d = new UnionDistrict();
		else
			d = new SplitDistrict();

		d->Load(in);
		_districts.Add(d);
	}

	_citizens.Load(in);

	in.read(rcastc(&len), sizeof(len));
	for (int i = 0; i < len; i++)
	{
		Party* p = new Party();
		p->Load(in, _citizens, _districts);
		_parties.Add(p);
	}
}

void Election::Save(ostream& out) const
{
	District::SaveStatic(out);
	Party::SaveStatic(out);

	_date.Save(out);
	_results.Save(out);

	SaveArrays(out);

	// Save connections

	// save for each citizen his district
	for (auto citizen : _citizens)
	{
		int cur_dis_id = citizen->GetDistrict()->GetID();
		out.write(rcastcc(&cur_dis_id), sizeof(cur_dis_id));

		if (!out.good())
			throw ios::failure(FILE_WRITE_ERROR);
	}

	// save for each district all his citizens
	for (int i = 0; i < _districts.GetLength(); i++)
	{
		const DynamicArray<const Citizen*>& cur_citizens = _districts[i]->GetCitizens();
		int len = cur_citizens.GetLength();
		out.write(rcastcc(&len), sizeof(len));
		for (int j = 0; j < cur_citizens.GetLength(); j++)
		{
			const string& cur_cit_id = cur_citizens[j]->GetID();
			SaveString(out, cur_cit_id);
		}
	}
}

void Election::Load(istream& in)
{
	District::LoadStatic(in);
	Party::LoadStatic(in);

	_date.Load(in);
	_results.Load(in);

	LoadArrays(in);

	// Load connections

	// load for each citizen his district
	int cur_dis_id;
	for (int i = 0; i < _citizens.GetLength(); i++)
	{
		in.read(rcastc(&cur_dis_id), sizeof(cur_dis_id));
		if (!in.good())
			throw ios_base::failure(FILE_READ_ERROR);

		for (int j = 0; j < _districts.GetLength(); j++)
			if (_districts[j]->GetID() == cur_dis_id)
				_citizens[i]->SetDistrict(_districts[j]);
	}

	// load for each district all his citizens
	for (int i = 0; i < _districts.GetLength(); i++)
	{
		int len;
		in.read(rcastc(&len), sizeof(len));
		if (!in.good())
			throw ios_base::failure(FILE_READ_ERROR);

		for (int j = 0; j < len; j++)
		{
			string cur_cit_id;
			LoadString(in, cur_cit_id);

			for (int j = 0; j < _citizens.GetLength(); j++)
				if (_citizens[j]->GetID() == cur_cit_id)
					_districts[i]->AddCitizen(*_citizens[j]);
		}
	}
}