#pragma once

#define TEST_MODE 1

enum class ItemType : int
{
	NONE = 0,
	BRUSH,
	COIN,
	GEM,
	HAMMER,
	HEART,
	END
};

class ItemData
{
public:
	ItemType type;
	long value;
	// from 0 to 1000
	long rate;
};