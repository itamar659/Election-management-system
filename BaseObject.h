#pragma once
#include "StringInterface.h"
#include <typeinfo>


class BaseObject : public StringInterface
{
private:
	virtual ostream& PrintSelf(ostream&) const = 0;

public:
	virtual string GetType() const { return typeid(*this).name(); } // Cant return ref, end of scope delete the typeid of this object.

	virtual void Save(ostream&) const = 0;
	virtual void Load(istream&) = 0;

	friend ostream& operator<<(ostream& os, const BaseObject& obj) { return obj.PrintSelf(os); }
	
	virtual ~BaseObject() {}
};