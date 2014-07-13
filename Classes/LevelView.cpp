//
//  LevelView.cpp
//  Map
//
//  Created by leafsoar on 7/27/13.
//
//

#include "LevelView.h"
#include "Micro.h"

LevelView::LevelView():
m_nCurPage(0),
m_nPageCount(0),
m_pScrollView(0)
{
    
}

LevelView::~LevelView(){
    
}

CCScene* LevelView::scene(){
    CCScene* scene = CCScene::create();
    CCLayer* layer = LevelView::create();
    scene->addChild(layer);
    return scene;
}

bool LevelView::init(){
    bool bRef = false;
    do{
        CC_BREAK_IF(!CCLayer::init());
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        CCPoint pointCenter = ccp(winSize.width / 2, winSize.height / 2);
        
        // 显示背景图片
       CCScale9Sprite* background = CCScale9Sprite::create("mirco/level/level_background.png");
       background->setPosition(SCR_CENTER_POINT);
       this->addChild(background);
        
        float scr_view_width = 640; //CCDirector::sharedDirector()->getWinSize().width;//640
        float scr_view_height = 480;//CCDirector::sharedDirector()->getWinSize().height;//480
	   CCLog("scr_view_width ====%f scr_view_height ===== %f",scr_view_width,scr_view_height);
        // 添加一个半透明的灰显层
        CCLayerColor* backLayerColor = CCLayerColor::create(ccc4(100, 255, 25, 125));
        backLayerColor->setContentSize(CCSizeMake(scr_view_width   ,scr_view_height));
        backLayerColor->setPosition(ccp(winSize.width/2-scr_view_width/2,winSize.height/2-scr_view_height/2));
        this->addChild(backLayerColor);
        
        // 创建一个 CCScrollView, 内容大小和当前的界面一样
       CCScrollView* scrollView = CCScrollView::create();
        scrollView->setViewSize((CCSizeMake(scr_view_width, scr_view_height)));
        CCLog("getContentSize width===%f,heigth ===%f",this->getContentSize().width,this->getContentSize().height);
        scrollView->setPosition(backLayerColor->getPosition());//
        scrollView->setContainer(getContainLayer(scrollView->getViewSize()));
        scrollView->setTouchEnabled(false);
        setScrollView(scrollView);
        this->addChild(scrollView);

        setTouchEnabled(true);
        
        setCurPageBall();
        
        
        CCSprite* sprite_newgame_normal = CCSprite::create( MENU_NEWGAME_FILEPATH, CCRectMake(0, 0, 126, 33));
        CCSprite* sprite_newgame_press = CCSprite::create(MENU_NEWGAME_FILEPATH, CCRectMake(0, 0, 126, 33));
	
    
        CCMenuItemSprite* item_newgame_sprite = CCMenuItemSprite::create(sprite_newgame_normal, sprite_newgame_press,NULL,this,menu_selector(LevelView::newgame_click) );
 
        
        CCMenu* mainmenu = CCMenu::create(item_newgame_sprite,NULL);
        mainmenu->setAnchorPoint(CCPointZero);
        mainmenu->alignItemsHorizontallyWithPadding(20.f);
        //保证了按照项目数 以图片的位置的中心x位置对齐显示
        mainmenu->setPosition(item_newgame_sprite->getContentSize().width,SCR_SIZE.height-item_newgame_sprite->getContentSize().height);
        this->addChild(mainmenu,10);

        
        bRef = true;
    }while (0);
    return bRef;
}

void LevelView::newgame_click(CCObject* pSender){

//    CCMessageBox("aaa", "bbb");
    
    CCScene *pScene = Micro_Menu::scene();
    CCDirector::sharedDirector()->replaceScene(CCTransitionFadeTR::create(1,pScene));
}


void LevelView::touchEventAction(LsTouch *touch, int type){

    // type 事件类型，0：touchbegin 1：touchend 触发 2：touchend 未触发
    if (touch)
        CCLog("touch event action id: %d  type: %d", touch->getEventId(), type);
    const int selectTag = 10001;
    if (type == 0 && touch){
        getScrollView()->getContainer()->removeChildByTag(selectTag);
        // 添加选撞状态的精灵，背景
        CCSprite* sprite = CCSprite::create("mirco/level/sel.png");
        sprite->setScaleX(2.5f);
        sprite->setScaleY(2.5f);
        sprite->setPosition(touch->getPosition());
        getScrollView()->getContainer()->addChild(sprite, 1, selectTag);
        
    } else {
        getScrollView()->getContainer()->removeChildByTag(selectTag);
    }
    if (type == 1 && touch){
        // 收到 type 为 1 表示触发关卡选择
        CCString* str = CCString::createWithFormat("%d", touch->getEventId() + 1);
        CCLog("stage：%s",str->getCString());

		CCScene* pScene = Micro::scene();
		 CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(2.0f,pScene));
		CCNotificationCenter::sharedNotificationCenter()->postNotification(LEVEL_NOTIFICATION, str);

      // CCMessageBox("关卡", str->getCString());
    }
}

void LevelView::registerWithTouchDispatcher(){
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
}

bool LevelView::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    CCLog("touch begin.");
    m_touchPoint = pTouch->getLocation();
    m_touchOffset = getScrollView()->getContentOffset();
    
    // 发送触摸消息，并会在 touEventAction 自动相应， 如果触摸到元素
    sendTouchMessage(pTouch, 0);
    
    return true;
}

void LevelView::ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    CCPoint movePoint = pTouch->getLocation();
    float distance = movePoint.x - m_touchPoint.x;
    // 设定当前偏移位置
    CCPoint adjustPoint = ccp(m_touchOffset.x + distance, 0);
    getScrollView()->setContentOffset(adjustPoint, false);
}

void LevelView::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    CCPoint endPoint = pTouch->getLocation();
    float distance = endPoint.x - m_touchPoint.x;
    float distanceY = endPoint.y - m_touchPoint.y;
    if (fabs(distance) < 3 && fabs(distanceY) < 3){
        // 小于三，不做拖动操作，也排除了（抖动误操作）,第二个参数，事件类型 1： touch end， 由 touchEventAction 自动相应
        sendTouchMessage(pTouch, 1);

    }else if (fabs(distance) > 140){
        // 大于 140，执行拖动效果
        adjustScrollView(distance);
        setCurPageBall();
    }else{
        // 回退为拖动之前的位置
        adjustScrollView(0);
    }
    
    sendTouchMessage(pTouch, 2);
}

void LevelView::ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    adjustScrollView(0);
}

void LevelView::adjustScrollView(float offset){
  //  CCSize winSize = CCDirector::sharedDirector()->getWinSize;
    CCSize srsi = getScrollView()->getViewSize();
    
    // 我们根据 offset 的实际情况来判断移动效果
    //这里要解释一下：由于开始的时候是显示的第一个视图，那么要接着滑动到第二个视图的话，操作就是要从右向左滑，那么这里的offset肯定就是负数了，所以这个时候m_nCurPage ++；反之，当从左向右滑动的时候，offset是正数，那么这里时候m_nCurPage --。
    if (offset < 0)
        m_nCurPage ++;
    else if (offset > 0)
        m_nCurPage --;

    if (m_nCurPage < 0)
        m_nCurPage = 0;
    else if (m_nCurPage > m_nPageCount - 1)
        m_nCurPage = m_nPageCount - 1;
    
    CCPoint adjustPoint = ccp(-srsi.width * m_nCurPage , 0);
    getScrollView()->setContentOffsetInDuration(adjustPoint, 0.2f);
    
    CCLog("current page index: %d", m_nCurPage);
}

CCLayer* LevelView::getContainLayer(const CCSize& srsi){
    // 48 个关卡
    int levelCount = 19;
    int widthCount =4;//一行4个
    int heightCount = 4; //一列4个
    m_nPageCount = (levelCount ) / (widthCount * heightCount) ;
    if(levelCount%(widthCount*heightCount)!=0)
        m_nPageCount++;
    
    CCLog("关卡数：%d, 页数：%d", levelCount, m_nPageCount);
    
   // CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCPoint pointCenter = ccp(srsi.width / 2, srsi.height / 2);
    
    CCLayer* layer = CCLayer::create();
    layer->setPosition(CCPointZero);
    
    // 添加 frame
    for (int i = 0; i < m_nPageCount; i++) {
        CCSprite* frame = CCSprite::create("mirco/level/frame.png");
       // frame->setScale(1.25f);
        frame->setPosition(ccpAdd(pointCenter, ccp(srsi.width * i, 0)));
        layer->addChild(frame, 0, 0);
    }

    // 添加关卡项
	int spw = 120;
	int sph = 120;
	float offx_init = srsi.width/3;
	float offy_init = 150;

	//CCSprite* ssd = CCSprite::create("mirco/testRect.png");
	//ssd->setPosition(ccp(offx_init,offy_init));
	//addChild(ssd,100);

    for (int i = 0; i < levelCount; i ++) {
        CCSprite* sprite = CCSprite::create("mirco/level/level.png");

        // 添加关卡标示
        CCString* str = CCString::createWithFormat("%d", i + 1);
        CCLabelBMFont* label = CCLabelBMFont::create(str->getCString(),"mirco/font.fnt");
        label->setPosition(ccp(40, 45));
        sprite->addChild(label);
        // 这里可以再为 关卡精灵做些装饰，就像 lable 一样
        // sprite->setScale(0.70);
        
        // 触摸处理
        LsTouch* touch = LsTouch::create();
        touch->setDisplay(sprite);
        this->addLsTouch(touch, i);

        // 设置位置
        int curPageIndex = i / (widthCount * heightCount);
		float xOffset =offx_init+ (i % widthCount)* spw ;
		float yOffset = srsi.height -  (    (i % (widthCount * heightCount)) / widthCount * sph + offy_init  );
        
        touch->setPosition(ccp(xOffset + srsi.width * curPageIndex, yOffset));
        layer->addChild(touch ,2, i);
    }
    
    layer->setContentSize(CCSizeMake(srsi.width * m_nPageCount, srsi.height));

    return layer;
}


void LevelView::setCurPageBall()
{
    //标签小球
	CCSize size = CCDirector::sharedDirector()->getVisibleSize();
    const int tagNodeBall = 1002;
	float fs = 0.2f;
	float scale = 0.7f;
	// 根据分屏数量显示小圆球标示
	CCNode* layer = this->getChildByTag(tagNodeBall);
	if (layer)
	{
		CCLayer* lyBall = dynamic_cast<CCLayer*>(layer);
		for (int i = 0; i < m_nPageCount; i++)
		{
			CCNode* n = lyBall->getChildByTag(i);
			if (n)
			{
				CCSprite* sprite = dynamic_cast<CCSprite*>(n);
				if (m_nCurPage == i)
				{
					sprite->runAction(
                                      CCScaleTo::create(fs,
                                                        1.0f));
				}
				else
				{
					sprite->runAction(
                                      CCScaleTo::create(0.5,
                                                        scale));
				}
			}
		}
	}
	else
	{
        CCLayer* lyBall = CCLayer::create();
		this->addChild(lyBall, 10, tagNodeBall);
		int ball_width = 30;			// 圆球间距
		for (int i = 0; i < m_nPageCount; i++)
		{
			CCSprite* sprite = CCSprite::create("mirco/level/ball.png");
			sprite->setPosition(ccp(ball_width*i+size.width / 2 - m_nPageCount/2*ball_width, ball_width));
			sprite->setScale(scale);
			lyBall->addChild(sprite, 0, i);
			if (i == m_nCurPage)
			{
				sprite->runAction(CCScaleTo::create(fs, 1.0f));
			}
		}

	}
}