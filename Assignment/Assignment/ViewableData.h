#pragma once
#include "Data.h"
class ViewableData : public Data
{
public:
	virtual bool view() {
		return true;
	}
};

