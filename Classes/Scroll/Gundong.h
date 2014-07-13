//
//  Gundong.h
//  phytest
//
//  Created by bobo on 14-7-12.
//
//

#ifndef phytest_Gundong_h
#define phytest_Gundong_h

#include "cocos2d.h"
#include "cocos-ext.h"
#include "Micro.h"
USING_NS_CC;
USING_NS_CC_EXT;

class Gundong :public CCLayer,public CCScrollViewDelegate {
public:
    Gundong();
    ~Gundong();
    virtual bool init();
    // returns a Scene that contains the HelloWorld as the only child
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);
	void chooseCallback( CCObject * sender , CCControlEvent controlEvent );
    // implement the "static node()" method manually
    
    bool ccTouchBegan( cocos2d::CCTouch* touch, cocos2d::CCEvent* event );
    
    void scrollViewDidScroll(cocos2d::extension::CCScrollView * view);
	void scrollViewDidZoom(cocos2d::extension::CCScrollView * view);
	void initScrollView();
	void inputNameCallback(CCNode* pSender );
	CCSprite* slider;
	CCSprite* sliderbg;
	CCLayerColor * containlayer;
	CCScale9Sprite *scrollbg;

    void draw();
    
    CREATE_FUNC(Gundong);
    
    
};

class mButton :public CCControlButton
{
public:
	virtual void registerWithTouchDispatcher();
	bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
	static mButton* createWithid( int id_, bool block_);
	
	
    bool block;
    
	CCSprite* icon;
	CCSprite* name;
	CCSprite* bLockSprite;
private:
	int id;
    bool moved_;
    CCRect  visibleRect_;


	
};

#endif

