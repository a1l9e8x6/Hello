#include "Micro_Loading.h"


bool Micro_Loading::init(){
    if(!CCLayerColor::initWithColor(ccc4(0, 0, 0, 100))){
        return false;
    }
    
    setTouchEnabled(true);
    
    
    
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("mirco/loading.plist");
    CCLog("loaidng.....");
    CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
    CCSize size=CCDirector::sharedDirector()->getWinSize();
    ar=CCArray::create();
    char str[64] = {0};
    for(int i = 1; i <=12; i++)
    {
        sprintf(str,"load%d.png", i);
        CCSpriteFrame* frame = cache->spriteFrameByName(str);
        ar->addObject(frame);
    }
    animation = CCAnimation::createWithSpriteFrames(ar, 0.1f);
    CCSprite *sp=CCSprite::createWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("load1.png"));
    sp->setPosition(ccp(size.width/2, size.height/2));
    this->addChild(sp);
    CCActionInterval* action=CCAnimate::create(animation);
    go=CCRepeatForever::create(action);
    sp->runAction(go);
    go->retain();
    
    return true;
}
void Micro_Loading::registerWithTouchDispatcher()
{
	//在使用 numeric_limits<int>::max() 之前 #undef max，或者用 INT_MAX 来替换
	int a = INT_MIN;

    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,(std::numeric_limits<int>::min()), true);
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, a, true);

	CCLayer::registerWithTouchDispatcher();
}

bool Micro_Loading::ccTouchBegan(CCTouch *touch, CCEvent *event)
{
    return true;
}
void Micro_Loading::ccTouchMoved(CCTouch *touch, CCEvent *event)
{
    
}
void Micro_Loading::ccTouchEnded(CCTouch *touch, CCEvent *event)
{
    
}
Micro_Loading::~Micro_Loading()
{   
    go->release();
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("mirco/loading.plist");
    CCTextureCache::sharedTextureCache()->removeTextureForKey("loading.png");

}
