#pragma once
#include "Election.h"
#include "Compare.h"

class bad_input : public exception
{
public:
	inline const char* what() const throw () {
		return msg ? msg : "some input invalid - unkown error"; 
	}

	bad_input() : msg(nullptr) {}
	bad_input(const char* message) : msg(message) {}

private:
	const char* msg;
};

class Checks
{
public:
	static void Check_AddDistrict(const Election& elect);
	static void Check_AddCitizen(const Election& elect, const string& citizen_id, int district_id, int age);
	static void Check_AddParty(const Election& elect, const string& citizen_id);
	static void Check_AddRepresentative(const Election& elect, const string& citizen_id, int district_id, int party_id);

	static void Check_PrintDistricts(const Election& elect);
	static void Check_Vote(const Election& elect, const string& citizen_id, int party_id);
	static void Check_ElectionResults(const Election& elect);
	static bool Check_IsNumber(const string& str);

private:
	Checks() {}
};