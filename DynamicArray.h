#pragma once
#include "BaseDynamicArray.h"

template<class T>
class DynamicArray : public BaseDynamicArray<T>
{
public:

	int GetLength() const { return this->size(); }
	void Add(const T& value) { this->push_back(value); }
	void Clear() { this->clear(); }

	template<class Ty>
	DynamicArray<Ty> DyCast()
	{
		DynamicArray<Ty> n_arr;

		for (auto val : *this)
		{
			Ty n_val = dynamic_cast<Ty>(val);
			n_arr.push_back(n_val);
		}
		return n_arr;
	}

	template<class V, class Func>
	bool Contain(const V value, const Func& compare) const
	{
		for (auto v : *this)
			if (compare(v, value))
				return true;

		return false;
	}

	template<class V, class Func>
	T& Find(const V& value, const Func& compare) const
	{
		for (int i = 0; i < this->_len; i++)
			if (compare(this->_arr[i], value))
				return this->_arr[i];

		throw invalid_argument("The item isn't in the array");
	}
};