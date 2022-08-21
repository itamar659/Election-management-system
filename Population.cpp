#include "Population.h"

const Citizen* Population::last() const 
{
	if (_len == 0)
		return nullptr;
	return _arr[_len - 1];
}

void Population::Add(const Citizen& value)
{
	push_back(value);
}

void Population::push_back(const Citizen& value)
{
	if (_len == _size)
		resize();

	Citizen* c = new Citizen(value);
	_arr[_len++] = c;
}

void Population::Save(ostream& out) const
{
	out.write(rcastcc(&_len), sizeof(_len));
	for (auto item : *this)
		item->Save(out);
}

void Population::Load(istream& in)
{
	int len;
	in.read(rcastc(&len), sizeof(len));
	for (int i = 0; i < len; i++)
	{
		if (_len == _size)
			resize();

		Citizen* c = new Citizen();
		c->Load(in);
		_arr[_len++] = c;
	}
}

Population::~Population()
{
	for (auto item : *this)
		delete item;
}