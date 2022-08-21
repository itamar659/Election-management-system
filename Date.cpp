#include "Date.h"
#include <string>
#include <iostream>

namespace date
{
	const int Date::days_in_month[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	const int Date::_years_range[2] = { 0, 9999 };

#pragma region Default Constructors

	Date::Date(int day, Month month, int year)
	{
		SetDate(day, month, year);
	}

	Date::Date()
		: Date(1, Month::January, _years_range[0])
	{
	}

	Date::Date(const Date& other)
	{
		_year = other._year;
		_month = other._month;
		_day = other._day;
	}

#pragma endregion

#pragma region Overload Operators

	Date Date::operator=(const Date& other)
	{
		if (this != &other)
		{
			_year = other._year;
			_month = other._month;
			_day = other._day;
		}
		return *this;
	}

	bool Date::operator==(const Date& date) const
	{
		return ((_day == date._day) && (_month == date._month) && (_year == date._year));
	}

	bool Date::operator>(const Date& date) const
	{
		if (_year > date._year)
			return true;
		else if (_year < date._year)
			return false;

		if (_month > date._month)
			return true;
		else if (_month < date._month)
			return false;

		if (_day > date._day)
			return true;
		return false;
	}

	std::ostream& operator<<(std::ostream& os, const Date& date)
	{
		os << date._day << "/" << static_cast<int>(date._month) << "/" << date._year;
		return os;
	}

	std::istream& operator>>(std::istream& ins, Date& date)
	{
		Date* new_date = new Date;
		int d, m, y;
		ins >> d >> m >> y;

		new_date->SetDate(d, static_cast<Month>(m), y);
		date = *new_date;

		delete new_date;

		return ins;
	}

	#pragma endregion

	void Date::SetDate(int day, Month month, int year)
	{
		SetYear(year);
		SetMonth(month);
		SetDay(day);
	}

	void Date::SetDay(int day)
	{
		if (!(1 <= static_cast<int>(_month) && static_cast<int>(_month) <= 12))
			throw invalid_argument("Month has to be between 1 to 12 (January to December).");

		if (!(1 <= day && day <= days_in_month[static_cast<int>(_month)]))
			throw invalid_argument("Day has to be between 1 to " + to_string(days_in_month[static_cast<int>(_month)]) + ".");

		_day = day;
	}

	void Date::SetMonth(Month month)
	{
		if (!(1 <= static_cast<int>(month) && static_cast<int>(month) <= 12))
			throw invalid_argument("Month has to be between 1 to 12 (January to December).");

		if (!(1 <= _day && _day <= days_in_month[static_cast<int>(month)]))
			_day = 1;

		_month = month;
	}

	void Date::SetYear(int year)
	{
		if (!(_years_range[0] <= year && year <= _years_range[1]))
			throw invalid_argument("Year has to be between " + to_string(_years_range[0]) + " to " + to_string(_years_range[1]));

		_year = year;
	}

	void Date::Save(ostream& out) const
	{
		out.write(rcastcc(&_day), sizeof(_day));
		out.write(rcastcc(&_year), sizeof(_year));
		out.write(rcastcc(&_month), sizeof(_month));
		if (!out.good())
			throw ios_base::failure(FILE_WRITE_ERROR);
	}

	void Date::Load(istream& in)
	{
		in.read(rcastc(&_day), sizeof(_day));
		in.read(rcastc(&_year), sizeof(_year));
		in.read(rcastc(&_month), sizeof(_month));
		if (!in.good())
			throw ios_base::failure(FILE_READ_ERROR);
	}
}
