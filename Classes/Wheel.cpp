#include "Wheel.h"
#include "WheelItem.h"
#include "GameDefine.h"

USING_NS_CC;
using namespace cocos2d::ui;

// The res doesn't align at the center of items, add a little offset
#define WHEEL_SECTION_ROTATION_OFFSET -22.5f
#define ROTATION_PER_ITEM_SLOTS 45.0f // 360 / 8 item slots

Wheel::Wheel()
{
	_wheelSection = nullptr;
	_animationDuration = 0.0f;
	_speed = 360.0f;
}

Wheel::~Wheel()
{
}

bool Wheel::init()
{
	if (!Layout::init())
	{
		return false;
	}

	_wheelSection = Sprite::createWithSpriteFrameName("wheel_sections_8.png");
	this->addChild(_wheelSection);
	_wheelSection->setAnchorPoint(Vec2::ANCHOR_MIDDLE);

	Sprite* wheelBorder = Sprite::createWithSpriteFrameName("wheel_border.png");
	this->addChild(wheelBorder);

	Sprite* wheelArrow = Sprite::createWithSpriteFrameName("wheel_arrow.png");
	this->addChild(wheelArrow);
	wheelArrow->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	wheelArrow->setPosition(Vec2(0.0f, wheelBorder->getContentSize().height * 0.5f - 20.0f));

	EventListenerKeyboard* listener = EventListenerKeyboard::create();
	listener->onKeyPressed = [=](cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* e)
	{
		if (keycode == EventKeyboard::KeyCode::KEY_V)
		{
			playSpinAnimation(cocos2d::RandomHelper::random_int(0, 7), [=]() {});
		}
	};
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
	
	return true;
}

void Wheel::setItemData(const std::vector<ItemData>& data)
{
	_listItem = data;
	for (unsigned int i = 0; i < data.size(); i++)
	{
		ItemData itemData = data.at(i);
		WheelItem* item = WheelItem::createWith(itemData.type, itemData.value);
		if (item)
		{
			_wheelSection->addChild(item);
			item->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
			item->setPosition(Vec2(_wheelSection->getContentSize().width * 0.5f, _wheelSection->getContentSize().height * 0.5f));
			item->setRotation(-WHEEL_SECTION_ROTATION_OFFSET + ROTATION_PER_ITEM_SLOTS * i);
		}
	}
}

void Wheel::playSpinAnimation(int index, const std::function<void()>& onEndSpinAnimation)
{
	_callbackEndSpinAnimation = onEndSpinAnimation;
	// Split 3 cycles into 1.5 each action
	// Add a little random here to control the time
	RotateBy* action1 = RotateBy::create(RandomHelper::random_real(2.0f, 2.5f), 360.0f * 1.5f);
	// if you random slot 2 (index 1), you go 90 degree counter closewise, then need to add offset to show the item in center
	RotateBy* action2 = RotateBy::create(RandomHelper::random_real(3.0f, 3.5f), 360.0f * 1.5f - ROTATION_PER_ITEM_SLOTS * index + WHEEL_SECTION_ROTATION_OFFSET);
	EaseBackIn* easeBackIn = EaseBackIn::create(action1);
	EaseBackOut* easeBackOut = EaseBackOut::create(action2);
	_wheelSection->runAction(Sequence::create(easeBackIn, easeBackOut, CallFunc::create([=]()
	{
		if (this->_callbackEndSpinAnimation)
		{
			this->_callbackEndSpinAnimation();
		}
	}
	), nullptr));
}

void Wheel::resetToStartPosition()
{
	_wheelSection->setRotation(0);
}
