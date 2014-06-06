//
//  ButtonSprite.h
//  Hello
//
//  Created by bobo on 14-5-27.
//
//

#ifndef __Hello__ButtonSprite__
#define __Hello__ButtonSprite__

#include <iostream>
#include "cocos2d.h"
USING_NS_CC;

typedef  void  (CCObject::*ButtonClick_Func)();
typedef  void  (CCObject::*ButtonClick_FuncN)(CCObject* obj);
typedef  void  (CCObject::*ButtonClick_FuncND)(CCObject* obj,void* param);


#define click_selector(_SELECTOR) (ButtonClick_Func)(&_SELECTOR)
#define click_selectorN(_SELECTOR) (ButtonClick_FuncN)(&_SELECTOR)
#define click_selectorND(_SELECTOR) (ButtonClick_FuncND)(&_SELECTOR)

class ButtonSprite : public CCSprite,public CCTouchDelegate{

public:
    
       
    int m_touchPriority;
    
    bool m_isDown;
    
    ButtonClick_Func m_BtnClick_Func;
    CCObject* target;
    
    ButtonSprite();
    virtual ~ButtonSprite();
    
    static ButtonSprite* createButtonSprite(const char* filename,int tPriority = kCCMenuHandlerPriority);
    
    static ButtonSprite* createButtonSpriteWithSpriteFrame(CCSpriteFrame* frame,int tPriority = kCCMenuHandlerPriority);
    
    virtual void onEnter();
    virtual void onExit();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    // optional
    
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    
public:
    void clicked();
    void setOnClickListener(CCObject* pTarget,ButtonClick_Func tBtnClickFunc);




};

#endif /* defined(__Hello__ButtonSprite__) */
