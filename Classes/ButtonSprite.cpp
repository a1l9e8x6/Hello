//
//  ButtonSprite.cpp
//  Hello
//
//  Created by bobo on 14-5-27.
//
//

#include "ButtonSprite.h"

ButtonSprite::ButtonSprite(){
    m_touchPriority = 0;
    m_BtnClick_Func = 0;
}
ButtonSprite::~ButtonSprite(){}

ButtonSprite* ButtonSprite::createButtonSprite(const char* filename,int tPriority){
     ButtonSprite* buttonsprite = new ButtonSprite();
    buttonsprite->m_touchPriority = tPriority;
    if(buttonsprite && buttonsprite->initWithFile(filename)){
        buttonsprite ->autorelease();
        return  buttonsprite;
    }
    else{
    CC_SAFE_DELETE(buttonsprite);
    return buttonsprite;
    }
}

ButtonSprite* ButtonSprite::createButtonSpriteWithSpriteFrame(CCSpriteFrame* frame,int tPriority){

    ButtonSprite* popSprite = new ButtonSprite();
    popSprite->m_touchPriority = tPriority;
    if (popSprite && popSprite->initWithSpriteFrame(frame)) {
        popSprite->autorelease();
        return popSprite;
    }
    else{
        CC_SAFE_DELETE(popSprite);
        return popSprite;
    }
}


void ButtonSprite::onEnter(){
    
    CCSprite::onEnter();
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, m_touchPriority, true);
    
}
void ButtonSprite::onExit(){
    CCSprite::onExit();
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    
}
bool ButtonSprite::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    
    CCPoint touchPoint = getParent()->convertTouchToNodeSpace(pTouch);
    
    if(boundingBox().containsPoint(touchPoint)){
        m_isDown = true;
        return true;
    
    }
    return  false;
    
    
}
// optional

void ButtonSprite::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
    CCPoint touchpoint = getParent()->convertTouchToNodeSpace(pTouch);
    float deltax = pTouch->getDelta().x;
    deltax = deltax > 0 ? deltax:-deltax;
    if (!boundingBox().containsPoint(touchpoint)  || !m_isDown || deltax > 15) {
        m_isDown = false;
        return;
    }
    
}
void ButtonSprite::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    
    CCPoint touchpoint = getParent()->convertTouchToNodeSpace(pTouch);
    if(boundingBox().containsPoint(touchpoint)&&m_isDown){
        if(m_BtnClick_Func){
            //点击效果
            CCActionInterval* action1 = CCScaleTo::create(0.1, 1.2);
            CCActionInterval* action2 = CCScaleTo::create(0.1, 1.0);
            
          //  m_BtnClick_Func = (ButtonClick_Func)(&ButtonSprite::clicked);
            
            CCCallFunc* func = CCCallFunc::create(target, callfunc_selector(ButtonSprite::clicked));
            this->runAction(CCSequence::create(action1,action2,func,NULL));
        }
        m_isDown = false;
    }
    
}

void ButtonSprite::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent){
    
    m_isDown = false;
    
    
}

void ButtonSprite::clicked(){
    if(target && m_BtnClick_Func)
        (target->*m_BtnClick_Func)();
    
}
void ButtonSprite::setOnClickListener(CCObject* pTarget, ButtonClick_Func tBtnClickFunc){
    target = pTarget;
    m_BtnClick_Func = tBtnClickFunc;
}