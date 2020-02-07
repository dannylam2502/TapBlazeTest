#include "WheelItem.h"
#include "GameDefine.h"
#include "cocos2d/cocos/ui/CocosGUI.h"
USING_NS_CC;
using namespace cocos2d::ui;

WheelItem::WheelItem()
{
}

WheelItem::~WheelItem()
{
}

WheelItem * WheelItem::createWith(ItemType type, long value)
{
	WheelItem* instance = new (std::nothrow) WheelItem();
	if (instance && instance->initWith(type, value))
	{
		instance->autorelease();
		return instance;
	}
	CC_SAFE_DELETE(instance);
	return nullptr;
}

const std::string WheelItem::getSpriteFrameNameByItemType(ItemType type)
{
	switch (type)
	{
	case ItemType::BRUSH:
		return "brush.png";
		break;
	case ItemType::COIN:
		return "coin.png";
		break;
	case ItemType::GEM:
		return "gem.png";
		break;
	case ItemType::HAMMER:
		return "hammer.png";
		break;
	case ItemType::HEART:
		return "heart.png";
		break;
	default:
		return "heart.png";
		break;
	}
}

bool WheelItem::initWith(ItemType type, long value)
{
	if (!Layout::init())
	{
		return false;
	}

	this->setContentSize(Size(200, 500));

	// Layout
	this->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	this->setPosition(Vec2(getContentSize().width * 0.5f, getContentSize().height));
	//this->setBackGroundColorType(cocos2d::ui::LayoutBackGroundColorType::SOLID);
	//this->setBackGroundColor(Color3B::GREEN);
	//this->setOpacity(150);

	// The sprite of item
	std::string sprfmName = getSpriteFrameNameByItemType(type);
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
	lblValue->setPosition(Vec2(this->getContentSize().width * 0.5f, sprite->getPositionY() - sprite->getContentSize().height * 0.5f - 30.0f));
	lblValue->enableOutline(Color4B(123, 66, 25, 255), 3);
	lblValue->enableShadow(Color4B(123, 66, 25, 255));

	// The heart is the exception with time
	if (type == ItemType::HEART)
	{
		lblValue->setString(stm.str());

		Label* lblMin = Label::createWithTTF("Hello World", "fonts/arial.ttf", 30);
		this->addChild(lblMin);
		lblMin->setTextColor(Color4B::WHITE);
		lblMin->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
		lblMin->setAlignment(TextHAlignment::CENTER);
		lblMin->setString("min");
		lblMin->setPosition(Vec2(lblValue->getPositionX() + lblValue->getContentSize().width * 0.5f - 30.0f, lblValue->getPositionY() - 30.0f));
		lblMin->enableOutline(Color4B(123, 66, 25, 255), 3);
		lblMin->enableShadow(Color4B(123, 66, 25, 255));
	}

	return true;
}