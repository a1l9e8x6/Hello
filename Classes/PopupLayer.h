#ifndef _Popup_Layer_H
#define _Popup_Layer_H

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

class PopupLayer:public CCLayer{
public:
	 PopupLayer();
    ~PopupLayer();
    
    virtual bool init();
    CREATE_FUNC(PopupLayer);
    
    virtual void registerWithTouchDispatcher(void);
    bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    static PopupLayer* create(const char* backgroundImage);
	static PopupLayer* create(CCSprite* spritebg,CCScale9Sprite* sprite9);
	void setTitle(const char*title,int fontsize = 20,ccColor3B color = ccc3(255, 255, 255));


    void setContentText(const char* text, int fontsize = 20, int padding = 50, int paddintTop = 100);

    void setCallbackFunc(CCObject* target, SEL_CallFuncN callfun);
    
	bool addButton(CCSprite*normalsprite, CCSprite *selectedsprite, const char *title, int tag = 0);
    bool addButton(const char* normalImage, const char* selectedImage, const char* title, int tag = 0);    
    virtual void onEnter();
    virtual void onExit();

private:

	 void buttonCallback(CCObject* pSender);

    // 文字内容两边的空白区
    int m_contentPadding;
    int m_contentPaddingTop;
    
    CCObject* m_callbackListener;
    SEL_CallFuncN m_callback;

    CC_SYNTHESIZE_RETAIN(CCMenu*, m__pMenu, MenuButton);
    CC_SYNTHESIZE_RETAIN(CCSprite*, m__sfBackGround, SpriteBackGround);
    CC_SYNTHESIZE_RETAIN(CCScale9Sprite*, m__s9BackGround, Sprite9BackGround);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m__ltTitle, LabelTitle);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m__ltContentText, LabelContentText);

};
#endif
