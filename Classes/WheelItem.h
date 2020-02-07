#pragma once

#include "cocos2d.h"
#include "cocos2d/cocos/ui/CocosGUI.h"

enum class ItemType : int;

class WheelItem : public cocos2d::ui::Layout
{
public:
	WheelItem();
	~WheelItem();
	
	static WheelItem* createWith(ItemType type, long value);
	static const std::string getSpriteFrameNameByItemType(ItemType type);
protected:
	bool initWith(ItemType type, long value);
};