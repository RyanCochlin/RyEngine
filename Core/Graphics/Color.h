#pragma once

#include "Windows.h"

union Color
{
	struct c
	{
		FLOAT r, g, b, a;
	} c;
	FLOAT rbga[4];
};

