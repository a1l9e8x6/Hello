#ifndef _Micro_Loding_
#define _Micro_Loding_

#include "cocos2d.h"
#include "cocos-ext.h"

#include "SimpleAudioEngine.h"
#include <limits>

USING_NS_CC;
USING_NS_CC_EXT;

using namespace std;

class Micro_Loading : public CCLayerColor
{
public:
    ~Micro_Loading();
    
    virtual bool init();
    
    virtual void  registerWithTouchDispatcher();
    
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    
    virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);
    
    virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);
    
    CREATE_FUNC(Micro_Loading);
    
public:
    
    CCAnimation *animation;
    
    CCArray *ar;
    
    CCActionInterval *go;
    
};
#endif /* defined(__client1__LodingLayer__) */
