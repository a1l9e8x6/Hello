//
//  LevelView.h
//  Map
//
//  Created by leafsoar on 7/27/13.
//
//

#ifndef Map_LevelView_h
#define Map_LevelView_h

#include "cocos2d.h"
#include "cocos-ext.h"

#include "LsTouch.h"
#include "Micro_Menu.h"

USING_NS_CC;
USING_NS_CC_EXT;
#define LEVEL_NOTIFICATION "LEVEL_NOTIFICATION"
class LevelView: public CCLayer, LsTouchEvent{
public:
    
    LevelView();
    ~LevelView();
    
    static CCScene* scene();
    virtual bool init();
    CREATE_FUNC(LevelView);
    void newgame_click(CCObject* pSender);

    
    virtual void touchEventAction(LsTouch* touch, int type);
    virtual void registerWithTouchDispatcher(void);
    
	virtual bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent);
	virtual void ccTouchMoved(CCTouch* pTouch, CCEvent* pEvent);
	virtual void ccTouchEnded(CCTouch* pTouch, CCEvent* pEvent);
	virtual void ccTouchCancelled(CCTouch* pTouch, CCEvent* pEvent);
    
    void adjustScrollView(float offset);
    CCLayer* getContainLayer(const CCSize& srsi);
    void setCurPageBall();
    

private:
    CCPoint m_touchPoint;
    CCPoint m_touchOffset;
    
    int m_nCurPage;
    int m_nPageCount;
    

    CC_SYNTHESIZE_RETAIN(CCScrollView*, m_pScrollView, ScrollView);
};

#endif
