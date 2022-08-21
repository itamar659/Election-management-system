#pragma once

class BaseAction
{
public:
	virtual bool Execute() = 0;
	virtual BaseAction* Clone() const = 0;

	BaseAction() {}
	virtual ~BaseAction() {}
};