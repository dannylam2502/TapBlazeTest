#ifndef __TEST_SCENE_H__
#define __TEST_SCENE_H__

#include "cocos2d.h"
#include "cocos/ui/CocosGUI.h"
#include "GameDefine.h"

class Wheel;
class TestScene : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	// a selector callback
	void menuCloseCallback(cocos2d::Ref* pSender);

	// implement the "static create()" method manually
	CREATE_FUNC(TestScene);

	void onClickBtnPlay();
	void onClickBtnClaim();

	void onTouchBtnPlay(cocos2d::Ref*, cocos2d::ui::Widget::TouchEventType type);
	void onTouchBtnClaim(cocos2d::Ref*, cocos2d::ui::Widget::TouchEventType type);
private:
	Wheel* _wheel;
	cocos2d::ui::Button* _btnPlay;
	cocos2d::ui::Button* _btnClaim;

	std::vector<ItemData> _dataList;
	// cache sum of weight, cause it rarely changes
	int _sumOfWeight;

	// Track Last reward item to show
	int _lastRewardItemIndex;
private:
	int getRandomItemReward();
	void onEndWheelSpinAnimation();
#if TEST_MODE
	void testResult();
	const char* getItemDescription(int index);
#endif // TEST_MODE
};

#endif // __TEST_SCENE_H__
