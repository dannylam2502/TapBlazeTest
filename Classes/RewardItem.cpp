#include "RewardItem.h"
#include "GameDefine.h"
#include "cocos2d/cocos/ui/CocosGUI.h"
#include "WheelItem.h"

USING_NS_CC;
using namespace cocos2d::ui;

RewardItem::RewardItem()
{
}

RewardItem::~RewardItem()
{
}

RewardItem * RewardItem::createWith(ItemType type, long value)
{
	RewardItem* instance = new (std::nothrow) RewardItem();
	if (instance && instance->initWith(type, value))
	{
		instance->autorelease();
		return instance;
	}
	CC_SAFE_DELETE(instance);
	return nullptr;
}

bool RewardItem::initWith(ItemType type, long value)
{
	if (!Layout::init())
	{
		return false;
	}

	this->setContentSize(Size(200, 500));

	// Layout
	this->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	this->setPosition(Vec2(getContentSize().width * 0.5f, getContentSize().height));

	// The sprite of item
	std::string sprfmName = WheelItem::getSpriteFrameNameByItemType(type);
	Sprite* sprite = Sprite::createWithSpriteFrameName(sprfmName);
	this->addChild(sprite);
	sprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	sprite->setPosition(Vec2(this->getContentSize().width * 0.5f, this->getContentSize().height * 0.7f));

	// convert long to string
	std::ostringstream stm;
	stm << value;
	//

	// The quantity of item
	Label* lblValue = Label::createWithTTF("Hello World", "fonts/arial.ttf", 60);
	this->addChild(lblValue);
	lblValue->setTextColor(Color4B::WHITE);
	lblValue->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	lblValue->setAlignment(TextHAlignment::CENTER);
	lblValue->setString("X" + stm.str());
	lblValue->setPosition(Vec2(this->getContentSize().width * 0.5f, sprite->getPositionY() - sprite->getContentSize().height * 0.5f));
	lblValue->enableOutline(Color4B(123, 66, 25, 255), 3);
	lblValue->enableShadow(Color4B(123, 66, 25, 255));

	// The heart is the exception with time
	if (type == ItemType::HEART)
	{
		lblValue->setString(stm.str() + " min");
	}

	// light background effect
	Sprite* sprLightCircle = Sprite::createWithSpriteFrameName("light_circle.png");
	this->addChild(sprLightCircle, -1);
	sprLightCircle->setPosition(sprite->getPosition());
	sprLightCircle->setScale(2.0f);
	sprLightCircle->setAnchorPoint(Vec2::ANCHOR_MIDDLE);

	RotateBy* rotateBy = RotateBy::create(0.2f, 30);
	sprLightCircle->runAction(RepeatForever::create(rotateBy));
	sprLightCircle->setBlendFunc(cocos2d::BlendFunc::ADDITIVE);

	return true;
}