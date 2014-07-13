//
//  Gundong.cpp
//  phytest
//
//  Created by bobo on 14-7-12.
//
//

#include "Gundong.h"


Gundong::Gundong(){
       CCLog("Gundong");

}

Gundong::~Gundong(){
    
    
}

// returns a Scene that contains the HelloWorld as the only child
CCScene* Gundong::scene(){
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // add layer as a child to scene
    
    CCLayer* layer = Gundong::create();
    scene->addChild(layer);
    layer->release();
    
    return scene;
    
}


bool Gundong::init(){
    
    if(!CCLayer::init())
        return false;
    
    
    setTouchEnabled(true);
    
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    CCLog("size width = %f,height = %f",size.width,size.height);
    
    CCSprite* sprite = CCSprite::create("mirco/wbg01.png");
    sprite->setPosition(CCPointMake(size.width/2,size.height/2));
    this->addChild(sprite);
    
    
//    CCScale9Sprite* sca = CCScale9Sprite::create("mirco/wbg07.png");
//    sca->setContentSize(CCSizeMake(600, size.height-200));
//    sca->setPosition(ccp(size.width/2,size.height/2));
//    this->addChild(sca);
    
    CCMenuItemImage* itemImage = CCMenuItemImage::create("mirco/wbut02_1.png", "mirco/wbut02.png",this,menu_selector(Gundong::menuCloseCallback));
    
    itemImage->setPosition(CCPointMake(size.width/2, itemImage->getContentSize().height));
    CCMenu* menu = CCMenu::create(itemImage,NULL);
    menu->setPosition(CCPointZero);
    this->addChild(menu);
    
    
    CCSprite* title = CCSprite::create("mirco/tz01.png");
    if (NULL == title) {
        CCLog("title is null");
    }
    title->setPosition(CCPointMake(size.width/2, size.height-title->getContentSize().height/2));
    this->addChild(title);
   
    scrollbg = CCScale9Sprite::create("mirco/wbg07.png");
	scrollbg->setContentSize(CCSizeMake(600,size.height-200));
	scrollbg->setPosition(ccp(size.width/2,size.height/2));
	this->addChild(scrollbg);
    
    initScrollView();
    
    CCLog("sssddddd");
    
    
    return  true;
}

void Gundong::initScrollView(){
    
    
    CCLayer* layer = new CCLayer();
    addChild(layer,1);
    //增加一个容器
    containlayer = CCLayerColor::create();
       // containlayer->initWithColor(ccc4(244, 111, 221, 255));
    
    int num = 8;
    
    int spw = 0;//列间距
    int sph = 16;//行间距
    
    int ofx = 30; //调整条的X坐标
    int bw = 560;//每一条de宽度
    int bh = 240;//每一条的高度
    
    //int ofy = num*(bh+sph)-bh;
    
    for (int i = num; i > 0; i--) {
        CCScale9Sprite *groupBg = CCScale9Sprite::create("mirco/wbg03.png");
        groupBg->setAnchorPoint(CCPointZero);
        groupBg->setContentSize(CCSizeMake(bw,bh));
        groupBg->setPosition(ccp( (scrollbg->getContentSize().width-bw)/2,i*(bh+sph)));
        containlayer->addChild(groupBg,2);
        
        char number[16];
        sprintf(number, "%d",num-i);
        CCLog("number",number);


        CCLabelTTF* label = CCLabelTTF::create(number, microfont, 48.0f);
        label->setPosition(ccp(0,groupBg->getContentSize().height/2));
        groupBg->addChild(label);
    }
    
    
    containlayer->setContentSize(CCSizeMake(bw,num*(bh+sph))); //’‚ «»›∆˜µƒ¥Û–°£¨“™∏˘æ› ˝æ›µƒ∏ˆ ˝¿¥°£
	CCScrollView * sc = CCScrollView::create();
    
    
	sc->setDirection(kCCScrollViewDirectionVertical);
	sc->setBounceable(true);//…Ë÷√CCScrollView”–µØ–‘–ßπ˚
	
	sc->setTag(100);
	sc->setViewSize(CCSizeMake(scrollbg->getContentSize().width,scrollbg->getContentSize().height));
    sc->setAnchorPoint(CCPointZero);
	sc->setPosition(ccp(scrollbg->getPosition().x - (scrollbg->getContentSize().width/2),scrollbg->getPositionY() - (scrollbg->getContentSize().height/2)));
    //sc->setPosition(CCPointZero);
    
    
    //    …Ë÷√CCScrollView≥ﬂ¥Á
	containlayer->setAnchorPoint(ccp(0,0));
	containlayer->setPosition(ccp(0,-containlayer->getContentSize().height+scrollbg->getContentSize().height));//scrollbg->getPosition().x - (scrollbg->getContentSize().width/2)

    
	sc->setContainer(containlayer);//…Ë÷√»›∆˜
    CCLog("scrollbg x =%f y=%f scrollbgx =%f scrollbgy = %f",sc->getPositionX(),sc->getPositionY(),scrollbg->getPositionX(),scrollbg->getContentSize().width/2);

	    
	//    ŒÒ±ÿ‘⁄AppDelegate.cpp ÷–ø™∆Ùios∂‡µ„¥•ƒ™µƒ÷ß≥÷
	sc->setTouchEnabled(true);//ø™∆Ùº‡Ã˝∂‡¥•µ„
	sc->setDelegate(this);
    sc->updateInset();
	layer->addChild(sc,2);
    
//    CCScrollView* sc = CCScrollView::create();//锚点默认在 0，0 处
//    sc->setContentOffset(CCPointZero);
//
//    sc->setDirection(kCCScrollViewDirectionVertical);
//    sc->setBounceable(true);//设置CCScrollView有弹性效果
//    sc->setTag(100);
//
//    //    设置CCScrollView尺寸
//    //CCLog("scrollbg height = %f , sc posx =%f sc posy = %f",scrollbg->getContentSize().height,sc->getPositionX(),sc->getPositionY());;
//    sc->setViewSize(CCSizeMake(bw, scrollbg->getContentSize().height));
//    sc->setPosition(CCPointMake(scrollbg->getPositionX()-scrollbg->getContentSize().width/2, scrollbg->getPositionY()-scrollbg->getContentSize().height/2));
//    CCLog("scrollbg height = %f , sc posx =%f sc posy = %f",scrollbg->getContentSize().height,sc->getPositionX(),sc->getPositionY());;
//
//    
//    sc->setContainer(containlayer);//设置容器
//    containlayer->setContentSize(CCSizeMake(bw, num*(sph+bh)));
//
//    containlayer->setAnchorPoint(CCPointZero);
//    //containlayer->setPosition(ccp(0, -containlayer->getContentSize().height+scrollbg->getContentSize().height));
//    
//    sc->setTouchEnabled(true);
//    sc->setDelegate(this);
//    
//    layer->addChild(sc,2);
    
    
    
    
    
}

// a selector callback
void Gundong::menuCloseCallback(CCObject* pSender){
    
    
}
void Gundong::chooseCallback( CCObject * sender , CCControlEvent controlEvent ){
    
    
}
// implement the "static node()" method manually


void Gundong::scrollViewDidScroll(cocos2d::extension::CCScrollView * view){
    
//    float scale = (scrollbg->getContentSize().height-60)/sliderbg->getContentSize().height;
//	float sl =	-view->getContentOffset().y/1248;
//	//CCLOG("%f",view->getContentOffset().y);
//	if(sl>1)
//	{
//		sl = 1;
//	}else if(sl<0)
//	{
//		sl = 0;
//	}
    
}

void Gundong::scrollViewDidZoom(cocos2d::extension::CCScrollView * view){
    
    
}

bool Gundong::ccTouchBegan( cocos2d::CCTouch* touch, cocos2d::CCEvent* event )
{
	return true;
}

void Gundong::inputNameCallback(CCNode* pSender ){
    
    
    
}



void mButton::registerWithTouchDispatcher()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 1, true);
}

bool mButton::ccTouchBegan( CCTouch *pTouch, CCEvent *pEvent )
{
	
	if(visibleRect_.size.width&&visibleRect_.size.height){
		if(!visibleRect_.containsPoint(pTouch->getLocation()))
			return false;
	}
	
	return CCControlButton::ccTouchBegan(pTouch,pEvent);
}

void mButton::ccTouchMoved( CCTouch *pTouch, CCEvent *pEvent )
{
	moved_=true;
	CCControlButton::ccTouchMoved(pTouch,pEvent);
}

void mButton::ccTouchEnded( CCTouch *pTouch, CCEvent *pEvent )
{
	if(ccpDistance(pTouch->getStartLocation(),pTouch->getLocation())>20)
	{
		moved_ = true;
	}
	else
		moved_ = false;
    
	if(!moved_)
		CCControlButton::ccTouchEnded(pTouch,pEvent);
	else
	{
		m_isPushed = false;
		setHighlighted(false);
	}
}

mButton* mButton::createWithid( int id_ ,bool block_)
{
	mButton* pbutton = new mButton();
	if(pbutton&&pbutton->initWithBackgroundSprite(CCScale9Sprite::create("micro/wbut01.png")))
	{
		pbutton->setPreferredSize(CCSizeMake(224,96));
		pbutton->autorelease();
		pbutton->id = id_;
		pbutton->block = block_;
		char iconres [32];
		char iname [32];
		if(id_>9)
		{
			sprintf(iconres,"country/c%d.png",id_);
			sprintf(iname,"country/cz%d.png",id_);
		}else
		{
			sprintf(iconres,"country/c0%d.png",id_);
			sprintf(iname,"country/cz0%d.png",id_);
		}
        
		pbutton->icon = CCSprite::create(iconres);
		pbutton->icon->setPosition(ccp(5+pbutton->icon->getContentSize().width/2,pbutton->getContentSize().height/2));
		pbutton->addChild(pbutton->icon);
        
		pbutton->name =  CCSprite::create(iname);
		pbutton->name->setPosition(ccp(pbutton->icon->getPositionX()+pbutton->icon->getContentSize().width/2 +pbutton->name->getContentSize().width/2,pbutton->getContentSize().height/2));
		pbutton->addChild(pbutton->name);
        
		if(pbutton->block){
            pbutton->bLockSprite = CCSprite::create("wbut00.png");
            
            pbutton->bLockSprite->setPosition(ccp(pbutton->getContentSize().width/2,pbutton->getContentSize().height/2));
            pbutton->addChild(pbutton->bLockSprite);
		}
		return pbutton;
	}
	
	else
	{
		CC_SAFE_DELETE(pbutton);
		return pbutton;
	}
	
}

void Gundong::draw()
{
    //
    // IMPORTANT:
    // This is only for debug purposes
    // It is recommend to disable it
    //
    CCLayer::draw();
    
    ccGLEnableVertexAttribs( kCCVertexAttribFlag_Position );
    
    kmGLPushMatrix();
    
    //world->DrawDebugData();
    
    kmGLPopMatrix();
}

