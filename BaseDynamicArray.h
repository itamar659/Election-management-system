#pragma once
#include <iostream>
using namespace std;

template<class T>
class BaseDynamicArray
{
public:
	explicit BaseDynamicArray(int size = 2) : _len(0), _size(size), _arr(new T[size]) {}
	BaseDynamicArray(const BaseDynamicArray& other) : _arr(nullptr) { *this = other; }
	virtual ~BaseDynamicArray() { delete[] _arr; }

	const BaseDynamicArray& operator=(const BaseDynamicArray& other)
	{
		if (this != &other)
		{
			_len = other._len;
			_size = other._size;
			delete[] _arr;
			_arr = new T[_size];
			for (int i = 0; i < _len; i++)
				_arr[i] = other._arr[i];
		}

		return *this;
	}
	bool operator==(const BaseDynamicArray& other)
	{
		if (_len != other._len)
			return false;

		for (int i = 0; i < _len; i++)
			if (_arr[i] != other._arr[i])
				return false;

		return true;
	}
	const T& operator[](int i) const { return _arr[i]; }
	T& operator[](int i) { return _arr[i]; }

	void push_back(const T& value)
	{
		if (_len == _size)
			resize();
		_arr[_len++] = value;
	}

	const T& front()	const { return _arr[0]; }
	int      size()		const { return _len; }
	int      capacity()	const { return _size; }
	bool     empty()	const { return _len == 0; }
	void     clear()		  { _len = 0; }

	class iterator
	{
	public:
		using iterator_category = std::bidirectional_iterator_tag;
		using different_type = std::ptrdiff_t;
		using value_type = T;
		using pointer = T*;
		using reference = T&;

		friend class const_iterator;

		iterator(BaseDynamicArray& arr, int i) : _da(&arr), _i(i) {}
		iterator(const iterator& other) : _da(other._da), _i(other._i) {}

		const iterator& operator=(const iterator& other)
		{
			_da = other._da;
			_i = other._i;
			return *this;
		}
		bool operator==(const iterator& other) const { return (_da == other._da) && (_i == other._i); }
		bool operator!=(const iterator& other) const { return !(*this == other); }
		T& operator*() { return _da->_arr[_i]; }
		T* operator->() { return &_da->_arr[_i]; }
		iterator& operator++()
		{
			++_i;
			return *this;
		}
		iterator operator++(int)
		{
			iterator temp(*this);
			++_i;
			return temp;
		}
		iterator& operator--()
		{
			--_i;
			return *this;
		}
		iterator operator--(int)
		{
			iterator temp(*this);
			--_i;
			return temp;
		}

	private:
		BaseDynamicArray* _da;
		int _i;
	};

	class const_iterator
	{
	public:
		using iterator_category = std::bidirectional_iterator_tag;
		using different_type = std::ptrdiff_t;
		using value_type = T;
		using pointer = T*;
		using reference = T&;

		const_iterator(const BaseDynamicArray& arr, int i) : _da(&arr), _i(i) {}
		const_iterator(const iterator& other) : _da(other._da), _i(other._i) {}

		const const_iterator& operator=(const iterator& other)
		{
			_da = other._da;
			_i = other._i;
			return *this;
		}
		bool operator==(const const_iterator& other) const { return (_da == other._da) && (_i == other._i); }
		bool operator!=(const const_iterator& other) const { return !(*this == other); }
		const T& operator*() { return _da->_arr[_i]; }
		const T* operator->() { return &_da->_arr[_i]; }
		const_iterator& operator++()
		{
			++_i;
			return *this;
		}
		const_iterator operator++(int)
		{
			const_iterator temp(*this);
			++_i;
			return temp;
		}
		const_iterator& operator--()
		{
			--_i;
			return *this;
		}
		const_iterator operator--(int)
		{
			const_iterator temp(*this);
			--_i;
			return temp;
		}

	private:
		const BaseDynamicArray* _da;
		int _i;
	};

	class reverse_iterator
	{
	public:
		using iterator_category = std::bidirectional_iterator_tag;
		using different_type = std::ptrdiff_t;
		using value_type = T;
		using pointer = T*;
		using reference = T&;

		reverse_iterator(BaseDynamicArray& arr, int i) : _da(arr), _i(i) {}
		reverse_iterator(const reverse_iterator& other) : _da(other._da), _i(other._i) {}

		const reverse_iterator& operator=(const reverse_iterator& other)
		{
			_da = other._da;
			_i = other._i;
			return *this;
		}
		bool operator==(const reverse_iterator& other) const { return (_da == other._da) && (_i == other._i); }
		bool operator!=(const reverse_iterator& other) const { return !(*this == other); }
		T& operator*() { return _da->_arr[_i]; }
		T* operator->() { return &_da->_arr[_i]; }
		reverse_iterator& operator++()
		{
			--_i;
			return *this;
		}
		reverse_iterator operator++(int)
		{
			reverse_iterator temp(*this);
			--_i;
			return temp;
		}
		reverse_iterator& operator--()
		{
			++_i;
			return *this;
		}
		reverse_iterator operator--(int)
		{
			reverse_iterator temp(*this);
			++_i;
			return temp;
		}

	private:
		BaseDynamicArray* _da;
		int _i;
	};

	const iterator& erase(const iterator& itr)
	{
		iterator itr_end = end();
		iterator itr_current = itr;
		iterator itr_next = itr;
		++itr_next;

		while (itr_next != itr_end)
		{
			*itr_current = *itr_next;
			itr_current = itr_next++;
		}

		--_len;
		return itr;
	}
	const iterator& erase(const iterator& first, const iterator& last)
	{
		int diff = 0;
		iterator itr_end = end();
		iterator itr_current = first;
		iterator itr_next = last;
		const iterator& itr_rtn = first;

		while (itr_next != itr_end)
		{
			*itr_current = *itr_next;
			++itr_current;
			++itr_next;
		}

		while (itr_current != itr_next)
		{
			++diff;
			++itr_current;
		}

		_len -= diff;
		return itr_rtn;
	}

	iterator begin() { return iterator(*this, 0); }
	iterator end() { return iterator(*this, _len); }
	const iterator cbegin() const { return iterator(*this, 0); }
	const iterator cend() const { return iterator(*this, _len); }

	const_iterator begin() const { return const_iterator(*this, 0); }
	const_iterator end() const { return const_iterator(*this, _len); }

	reverse_iterator rbegin() { return reverse_iterator(*this, 0); }
	reverse_iterator rend() { return reverse_iterator(*this, _len); }

	void insert(const iterator& pos, const T& val)
	{
		if (_len == _size)
			resize();

		iterator itr_end = end();
		iterator itr_current = itr_end;
		iterator itr_prev = --itr_end;

		while (itr_current != pos)
		{
			*itr_current = *itr_prev;
			itr_current = itr_prev--;
		}

		iterator p = pos;
		*p = val;
		++_len;
	}

protected:
	T* _arr;
	int _len;
	int _size;

	void resize() {
		_size *= 2;
		T* temp = new T[_size];
		for (int i = 0; i < _len; i++)
			temp[i] = _arr[i];

		delete[] _arr;
		_arr = temp;
	}
};