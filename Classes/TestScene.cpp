#include "TestScene.h"
#include "SimpleAudioEngine.h"
#include "cocos2d/cocos/ui/CocosGUI.h"
#include "Wheel.h"
#include "RewardItem.h"
#include "GameDefine.h"

#if TEST_MODE 
#include <fstream>
#endif

USING_NS_CC;
using namespace cocos2d::ui;

#define TAG_REWARD_ITEM 1

Scene* TestScene::createScene()
{
	return TestScene::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in TestScene.cpp\n");
}

// on "init" you need to initialize your instance
bool TestScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Scene::init())
	{
		return false;
	}

	// Load SpriteFrame
	SpriteFrameCache* instance = SpriteFrameCache::getInstance();
	instance->addSpriteFramesWithFile("Images/game_ui.plist");

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// Button Play
	_btnPlay = Button::create("spin_button.png", "", "", Widget::TextureResType::PLIST);
	this->addChild(_btnPlay);
	_btnPlay->setScale9Enabled(false);
	_btnPlay->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_btnPlay->setPosition(Vec2(visibleSize.width * 0.5f, visibleSize.height * 0.1f));
	_btnPlay->addTouchEventListener(CC_CALLBACK_2(TestScene::onTouchBtnPlay, this));
	_btnPlay->setPressedActionEnabled(false);

	//Text Play On
	Text* txtPlay = Text::create("Play On", "fonts/arial.ttf", 75);
	_btnPlay->addChild(txtPlay);
	txtPlay->setColor(Color3B::WHITE);
	txtPlay->enableOutline(Color4B::BLACK, 2);
	txtPlay->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
	txtPlay->setPosition(_btnPlay->getContentSize() * 0.5f);

	// Button Claim
	_btnClaim = Button::create("spin_button.png", "", "", Widget::TextureResType::PLIST);
	this->addChild(_btnClaim);
	_btnClaim->setVisible(false);
	_btnClaim->setScale9Enabled(false);
	_btnClaim->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_btnClaim->setPosition(Vec2(visibleSize.width * 0.5f, visibleSize.height * 0.1f));
	_btnClaim->addTouchEventListener(CC_CALLBACK_2(TestScene::onTouchBtnClaim, this));
	_btnClaim->setPressedActionEnabled(false);

	//Text Claim
	Text* txtClaim = Text::create("Claim", "fonts/arial.ttf", 75);
	_btnClaim->addChild(txtClaim);
	txtClaim->setColor(Color3B::WHITE);
	txtClaim->enableOutline(Color4B::BLACK, 2);
	txtClaim->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
	txtClaim->setPosition(_btnClaim->getContentSize() * 0.5f);

	// Spin wheel
	_wheel = Wheel::create();
	this->addChild(_wheel);
	_wheel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_wheel->setPosition(visibleSize * 0.5f);

	// fake item data
	ItemData data;
	// HEART X30min
	data.type = ItemType::HEART;
	data.value = 30L;
	data.rate = 20;
	_dataList.push_back(data);

	// BRUSH X3
	data.type = ItemType::BRUSH;
	data.value = 3L;
	data.rate = 10;
	_dataList.push_back(data);

	// GEM X35
	data.type = ItemType::GEM;
	data.value = 35L;
	data.rate = 10;
	_dataList.push_back(data);

	// HAMMER X3
	data.type = ItemType::HAMMER;
	data.value = 3L;
	data.rate = 10;
	_dataList.push_back(data);

	// COIN X750
	data.type = ItemType::COIN;
	data.value = 750;
	data.rate = 5;
	_dataList.push_back(data);

	// BRUSH X1
	data.type = ItemType::BRUSH;
	data.value = 1L;
	data.rate = 20;
	_dataList.push_back(data);

	// GEM X75
	data.type = ItemType::GEM;
	data.value = 75L;
	data.rate = 5;
	_dataList.push_back(data);

	// HAMMER X1
	data.type = ItemType::HAMMER;
	data.value = 1L;
	data.rate = 20;
	_dataList.push_back(data);
	// End Fake ItemData

	// Set Data to display
	_wheel->setItemData(_dataList);

	// calculate sum of weight
	_sumOfWeight = 0;
	for (unsigned int i = 0; i < _dataList.size(); i++)
	{
		_sumOfWeight += _dataList.at(i).rate;
	}


	// Add listener keyboard event to test
	EventListenerKeyboard* listener = EventListenerKeyboard::create();
	listener->onKeyPressed = [=](cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* e)
	{
		if (keycode == EventKeyboard::KeyCode::KEY_T)
		{
#if TEST_MODE
			this->testResult();
#endif
		}
	};
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
	return true;
}


void TestScene::menuCloseCallback(Ref* pSender)
{
	//Close the cocos2d-x game scene and quit the application
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

void TestScene::onClickBtnPlay()
{
	// prevent user from clicking too fast
	_btnPlay->setTouchEnabled(false);
	if (_wheel)
	{
		_lastRewardItemIndex = getRandomItemReward();
		_wheel->playSpinAnimation(_lastRewardItemIndex, CC_CALLBACK_0(TestScene::onEndWheelSpinAnimation, this));
	}
}

void TestScene::onTouchBtnPlay(cocos2d::Ref *, cocos2d::ui::Widget::TouchEventType type)
{
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		_btnPlay->setScale(1.05f);
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
	{
		_btnPlay->setScale(1.0f);
		// Run Action Scale
		ScaleTo* scaleTo = ScaleTo::create(0.3f, 0.0f);
		_btnPlay->runAction(EaseIn::create(scaleTo, 0.7f));

		onClickBtnPlay();
	}
	break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		_btnPlay->setScale(1.0f);
		break;
	}
}

void TestScene::onTouchBtnClaim(cocos2d::Ref *, cocos2d::ui::Widget::TouchEventType type)
{
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		_btnClaim->setScale(1.05f);
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
	{
		_btnClaim->setScale(1.0f);
		// Run Action Scale
		ScaleTo* scaleTo = ScaleTo::create(0.3f, 0.0f);
		_btnClaim->runAction(EaseIn::create(scaleTo, 0.7f));

		onClickBtnClaim();
	}
	break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		_btnClaim->setScale(1.0f);
		break;
	}
}

void TestScene::onClickBtnClaim()
{
	_wheel->runAction(EaseBackOut::create(ScaleTo::create(0.25f, 1.0f)));
	_wheel->resetToStartPosition();
	_btnPlay->runAction(EaseBackOut::create(ScaleTo::create(0.25f, 1.0f)));
	_btnPlay->setTouchEnabled(true);
	_btnClaim->setTouchEnabled(false);
	// remove reward item
	this->getChildByTag(TAG_REWARD_ITEM)->removeFromParent();
}

int TestScene::getRandomItemReward()
{
	int rnd = cocos2d::RandomHelper::random_int(1, _sumOfWeight);
	for (unsigned int i = 0; i < _dataList.size(); i++) {
		if (rnd < _dataList.at(i).rate)
			return i;
		rnd -= _dataList.at(i).rate;
	}
	return 0;
}

void TestScene::onEndWheelSpinAnimation()
{
	float scaleSmallDuration = 0.2f;
	_wheel->runAction(ScaleTo::create(scaleSmallDuration, 0.0f));
	if (_lastRewardItemIndex < _dataList.size())
	{
		ItemData data = _dataList.at(_lastRewardItemIndex);
		RewardItem* item = RewardItem::createWith(data.type, data.value);
		this->addChild(item);
		item->setTag(TAG_REWARD_ITEM);
		Size winSize = Director::getInstance()->getWinSize();
		item->setPosition(Vec2(winSize.width * 0.5f, winSize.height));
		float effectDuration = 0.7f;
		MoveTo* moveTo = MoveTo::create(effectDuration, winSize * 0.5f);
		item->setScale(0.2f);
		ScaleTo* scaleTo = ScaleTo::create(effectDuration, 1.3f);
		item->runAction(Sequence::create(DelayTime::create(scaleSmallDuration), Spawn::create(EaseBackOut::create(moveTo), EaseBackOut::create(scaleTo), nullptr), nullptr)); // wait end animation to do this animation
	}
	// show btn claim
	_btnClaim->setVisible(true);
	_btnClaim->setTouchEnabled(true);
	// som effect
	_btnClaim->setScale(0.0f);
	_btnClaim->runAction(EaseBackOut::create(ScaleTo::create(0.25f, 1.0f)));
}

#if TEST_MODE
void TestScene::testResult()
{
	std::string result;
	std::vector<int> listIndex;
	std::vector<int> countVector;

	for (unsigned int i = 0; i < _dataList.size(); i++)
	{
		countVector.push_back(i);
	}

	for (unsigned int i = 0; i < 1000; i++)
	{
		int rnd = getRandomItemReward();
		listIndex.push_back(rnd);
	}

	for (unsigned int i = 0; i < listIndex.size(); i++)
	{
		result += cocos2d::StringUtils::format("Turn %d, Prize = %s\n", i + 1, getItemDescription(listIndex.at(i)));
		countVector.at(listIndex.at(i))++;
	}
	
	for (unsigned int i = 0; i < countVector.size(); i++)
	{
		result += cocos2d::StringUtils::format("Prize = %s, Appear = %d\n", getItemDescription(i), countVector.at(i));
	}

	std::ofstream ofs("D:/log.txt", std::ofstream::binary);
	if (ofs.fail() == false) 
	{
		ofs.write(result.c_str(), result.length());
		ofs.close();
	}
	log(result.c_str());
}
const char * TestScene::getItemDescription(int index)
{
	switch (index)
	{
	case 0: return "Life 30 min";
	case 1: return "Brush 3X";
	case 2: return "Gem 35";
	case 3: return "Hammer 3X";
	case 4: return "Coin 750";
	case 5: return "Brush 1X";
	case 6: return "Gem 75";
	case 7: return "Hammer 1X";
	default:
		break;
	}
	return "";
}
#endif