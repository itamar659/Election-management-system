#pragma once
#include "BaseObject.h"

namespace date
{
	enum class Month
	{
		January = 1,
		February,
		March,
		April,
		May,
		June,
		July,
		August,
		September,
		October,
		November,
		December
	};

	class Date
	{
	public:
		static const int days_in_month[12];

		int GetDay() { return _day; }
		Month GetMonth() { return _month; }
		int GetYear() { return _year; }

		void SetDate(int day, Month month, int year);
		void SetDay(int day);
		void SetMonth(Month month);
		void SetYear(int year);

		Date operator=(const Date&);
		bool operator==(const Date& date) const;
		bool operator!=(const Date& date) const { return !(*this == date); }
		bool operator>(const Date& date) const;
		bool operator>=(const Date& date) const { return ((*this > date) || (*this == date)); }
		bool operator<(const Date& date) const { return !(*this >= date); }
		bool operator<=(const Date& date) const { return !(*this < date); }
		friend std::ostream& operator<<(std::ostream&, const Date&);
		friend std::istream& operator>>(std::istream&, Date&);

		void Save(ostream& out) const;
		void Load(istream& in);

		Date(const Date&);
		Date();
		Date(int day, Month month, int year);

	private:
		static const int _years_range[2];
		int _day, _year;
		Month _month;
	};
}