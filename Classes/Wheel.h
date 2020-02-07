#pragma once

#include "cocos2d.h"
#include "cocos/ui/CocosGUI.h"

enum class ItemType : int;
class ItemData;

class Wheel : public cocos2d::ui::Layout
{
public:
	Wheel();
	~Wheel();

	CREATE_FUNC(Wheel);
	
	void setItemData(const std::vector<ItemData>& data);

	// We will spin around circle and use index to determine the result, this function only needs index of the item in list to work
	void playSpinAnimation(int index, const std::function<void()>& onEndSpinAnimation);

	void resetToStartPosition();

protected:
	virtual bool init();

private:
	cocos2d::Sprite* _wheelSection;
	std::vector<ItemData> _listItem;
	std::function<void()> _callbackEndSpinAnimation;

	float _animationDuration;
	float _speed;
};