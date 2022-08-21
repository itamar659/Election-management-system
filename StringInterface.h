#pragma once
#include <iostream>
#include <string>
using namespace std;

#define rcastcc reinterpret_cast<const char*>
#define rcastc reinterpret_cast<char*>

constexpr const char* FILE_OPEN_ERROR = "Can not open the file.";
constexpr const char* FILE_WRITE_ERROR = "Can't write into the file.";
constexpr const char* FILE_READ_ERROR = "Can't read from the file.";

class StringInterface
{
protected:
	void SaveString(ostream& out, const string& str) const
	{
		int s = str.size();
		out.write(rcastcc(&s), sizeof(s));
		out.write(str.c_str(), str.size());

		if (!out.good())
			throw ios_base::failure(FILE_WRITE_ERROR);
	}
	void LoadString(istream& in, string& str)
	{
		int s;
		in.read(rcastc(&s), sizeof(s));
		if (!in.good())
			throw ios_base::failure(FILE_READ_ERROR);

		str.resize(s);
		in.read(&str[0], str.size());
		if (!in.good())
			throw ios_base::failure(FILE_READ_ERROR);
	}

	StringInterface() {}
	virtual ~StringInterface() {}
};