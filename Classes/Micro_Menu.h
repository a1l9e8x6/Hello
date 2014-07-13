#pragma once
#ifndef  __Micro_Menu_H__
#define  __Micro_Menu_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "MZDataManager.h"
#include "ServerPrizeList.h"
#include "AudioManager.h"

#define MENU_NEWGAME_FILEPATH "mirco/menu/menu_text.png"
#define MENU_BG_FILEPATH "mirco/menu/menu_bg.png"
#define MENU_BALL_FILEPATH "mirco/menu/menu_ball.png"


USING_NS_CC_EXT;
USING_NS_CC;
using namespace std;



class Micro_Menu : public cocos2d::CCLayer,CCEditBoxDelegate
{
public:
    
    Micro_Menu();
    ~Micro_Menu();
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);
    
    // implement the "static node()" method manually
    CREATE_FUNC(Micro_Menu);
    
    CCScale9Sprite* menu_bg;
   // CCSprite* menu_bg;
    void menu_newgame_click(CCObject* pSender);
    void menu_option_click(CCObject* pSender);
    void menu_about_click(CCObject* pSender);
    
    void runShipAnim(CCSprite *psprite);
    void spriteAnimCallback(cocos2d::CCNode *pSender);
    
    void updatescene(float t);

    std::vector<CCSprite*> sprite_balls;
    vector<CCSprite*>::iterator iter_ball;


	
	UILayer* ul; 
	UILayer* ul_zc; 

	//开始进入编辑
	virtual void editBoxEditingDidBegin(cocos2d::extension::CCEditBox* editBox);
	//结束编辑
	virtual void editBoxEditingDidEnd(cocos2d::extension::CCEditBox* editBox);
	//编辑框文本改变
	virtual void editBoxTextChanged(cocos2d::extension::CCEditBox* editBox, const std::string& text);
	//当触发return后的回调函数
	virtual void editBoxReturn(cocos2d::extension::CCEditBox* editBox);
	CCEditBox * boxName_login;
	CCEditBox * boxPassWord_login;
	CCEditBox * boxName_regist;
	CCEditBox * boxPassWord1_regist;
	CCEditBox * boxPassWord2_regist;
    
    void draw();
    
private:
        void sliderMove(CCObject* pSender);
};

#endif 
