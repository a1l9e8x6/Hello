#include "Micro_Menu.h"
#include "Micro.h"
#include "LevelView.h"
#include "CUNinePatch.h"

Micro_Menu::Micro_Menu(){

    menu_bg = NULL;
}

Micro_Menu::~Micro_Menu(){
    menu_bg = NULL;

}

CCScene* Micro_Menu::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    Micro_Menu *layer = Micro_Menu::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool Micro_Menu::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    
	CCLog("visibleSize width ==%.2f height ==%.2f",visibleSize.width,visibleSize.height);

	//cocos2d::CCSprite * tempA =	CUNinePatch::New( "mirco/ssd.9.png", CCSizeMake(400,100) ); //CCSizeMake(800,600) 
	//addChild(tempA,10);
	//tempA->setPosition( ccp( getContentSize().width*0.5f,getContentSize().height*0.5f  ) );

	//menu_bg = CCScale9Sprite::create("mirco/ssd.9.png");
	//menu_bg->setContentSize(CCSizeMake(400,100));//CCSizeMake(400,100)
	//menu_bg->setPosition( ccp(getContentSize().width*0.5f,getContentSize().height*0.8f  ) );// getContentSize().width*0.5f,getContentSize().height*0.8f 
	//	addChild(menu_bg,12);


   menu_bg = CCScale9Sprite::create(MENU_BG_FILEPATH);
   menu_bg->setAnchorPoint(CCPointZero);//CCScale9Sprite
   menu_bg->setPosition(CCPointZero);//SCR_CENTER_POINT

   // float scalex = CCEGLView::sharedOpenGLView()->getScaleX();
    //menu_bg->setScale(1.0/scale);
   // CCLog("scalex === %f",scalex);
    
    CCSprite* sprite_newgame_normal = CCSprite::create( MENU_NEWGAME_FILEPATH, CCRectMake(0, 0, 126, 33));
    CCSprite* sprite_newgame_press = CCSprite::create(MENU_NEWGAME_FILEPATH, CCRectMake(0, 0, 126, 33));
    
    
    CCSprite* sprite_option_normal = CCSprite::create( MENU_NEWGAME_FILEPATH, CCRectMake(126, 0, 126, 33));
    CCSprite* sprite_option_press = CCSprite::create(MENU_NEWGAME_FILEPATH, CCRectMake(126, 33, 126, 33));

    CCSprite* sprite_about_normal = CCSprite::create( MENU_NEWGAME_FILEPATH, CCRectMake(126*2, 0, 126, 33));
    CCSprite* sprite_about_press = CCSprite::create(MENU_NEWGAME_FILEPATH, CCRectMake(126*2, 33*2, 126, 33));

    
   // CCSprite* sprite_playagain_normal = CCSprite::create( MENU_NEWGAME_FILEPATH, CCRectMake(126*3, 0, 126, 33));
    //CCSprite* sprite_playagain_press = CCSprite::create(MENU_NEWGAME_FILEPATH, CCRectMake(126*3, 33*3, 126, 33));

//    CCSprite* sprite_playagain_normal1 = CCSprite::create( MENU_NEWGAME_FILEPATH, CCRectMake(126*3, 0, 126, 33));
//    CCSprite* sprite_playagain_press1 = CCSprite::create(MENU_NEWGAME_FILEPATH, CCRectMake(126*3, 33*3, 126, 33));

    
    CCMenuItemSprite* item_newgame_sprite = CCMenuItemSprite::create(sprite_newgame_normal, sprite_newgame_press,NULL,this,menu_selector( Micro_Menu::menu_newgame_click) );
    CCMenuItemSprite* item_option_sprite = CCMenuItemSprite::create(sprite_option_normal, sprite_option_press,NULL,this,menu_selector( Micro_Menu::menu_option_click) );
    
     CCMenuItemSprite* item_about_sprite = CCMenuItemSprite::create(sprite_about_normal, sprite_about_press,NULL,this,menu_selector( Micro_Menu::menu_about_click) );
    
//      CCMenuItemSprite* item_playagain_sprite = CCMenuItemSprite::create(sprite_playagain_normal, sprite_playagain_press,NULL,this,menu_selector( Micro_Menu::menu_about_click) );
     
    
    CCMenu* mainmenu = CCMenu::create(item_newgame_sprite,item_option_sprite, item_about_sprite,NULL);
    mainmenu->setAnchorPoint(CCPointZero);
    mainmenu->alignItemsHorizontallyWithPadding(20.f);
    //保证了按照项目数 以图片的位置的中心x位置对齐显示
    mainmenu->setPosition(SCR_SIZE.width/2,menu_bg->getContentSize().height/4);
    menu_bg->addChild(mainmenu,10);
    
    
    
    MZDataManager::sharedDataManager()->resetRandSeed();
    
    for (int i  = 0; i < 10 ; i++) {
        CCSprite* sprite_ball = CCSprite::create(MENU_BALL_FILEPATH);
        sprite_balls.push_back(sprite_ball);
        this->addChild(sprite_ball,10);
    }


    //用迭代器
	for(  iter_ball = sprite_balls.begin(); iter_ball != sprite_balls.end(); ){
        CCSprite*  csp = (*iter_ball);
        runShipAnim(csp);
        
        iter_ball++;
	}
    
    
    CCLabelTTF* ttf = CCLabelTTF::create("欧巴欧巴", "HiraKakuProN-W6", 40);
    ttf->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width/2,CCDirector::sharedDirector()->getWinSize().height*0.75));
    
    CCLabelTTF* ttf2 = CCLabelTTF::create("－－－来自星星的你", "HiraKakuProN-W6", 30);
    ttf2->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width*0.75,CCDirector::sharedDirector()->getWinSize().height*0.6));
    addChild(ttf,10);
    addChild(ttf2,10);
    
    
    
    
    this->addChild(menu_bg);
    
  
//    MZDataManager::sharedDataManager()->resetRandSeed();
//    int idd =MZDataManager::sharedDataManager()->myRand(1, 10);
//    CCLog("idd===%d",idd);
//    if(idd<=5)
//    {
//        CCLog("go to jiading");
//    }
//    else
//        CCLog("go to rugao");
//    CCLayer* server_layer = ServerPrizeList::create();
//    this->addChild(server_layer);
    
    
    return true;
}



void Micro_Menu::runShipAnim(CCSprite *psprite)
{

    psprite->setPosition(ccp( MZDataManager::sharedDataManager()->myRand(0, SCR_SIZE.width), -SCR_CENTER_POINT.y  ));
    ccBlendFunc ccb = {GL_SRC_ALPHA, GL_ONE};
  //  ccBlendFunc ccb1 = {GL_DST_COLOR, GL_ZERO};
  //  ccBlendFunc ccb2 = {GL_DST_COLOR, GL_SRC_COLOR};
    psprite->setBlendFunc(ccb);
    
     // 从底部移动到顶部，执行回调继续执行移动动仿
    CCActionInterval* moveto = CCMoveTo::create(3.0f, ccp(CCRANDOM_0_1() * SCR_SIZE.width, SCR_SIZE.height + psprite->getContentSize().height / 2));
    CCActionInterval* delay = CCDelayTime::create(1.0f);
    CCCallFuncN* func = CCCallFuncN::create(this, callfuncN_selector(Micro_Menu::spriteAnimCallback));
    CCSequence* seq = CCSequence::create(moveto,delay,func,NULL);
    psprite->runAction(seq);
   

}

void Micro_Menu::spriteAnimCallback(cocos2d::CCNode *pSender)
{
    
    runShipAnim((CCSprite*)pSender);
}




void Micro_Menu::menu_newgame_click(CCObject* pSender){

    CCLog("ssssss");
    MZDataManager::sharedDataManager()->showLoading(menu_bg);
    

    scheduleOnce(schedule_selector(Micro_Menu::updatescene), 2.0f);
}

void Micro_Menu::updatescene(float t)
{
    
        MZDataManager::sharedDataManager()->delLoading();
        CCScene *pScene = LevelView::scene();
        CCDirector::sharedDirector()->replaceScene(CCTransitionFadeTR::create(1,pScene));

}


void Micro_Menu::menu_option_click(CCObject* pSender){
    
   // CCMessageBox("庆哥真是好女丿, "title");
}

void Micro_Menu::menu_about_click(CCObject* pSender){
    
//    CCMessageBox("庆哥真是好好女人", "title");
}




void Micro_Menu::menuCloseCallback(CCObject* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
#else
    CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
#endif
}



void Micro_Menu::editBoxEditingDidBegin( cocos2d::extension::CCEditBox* editBox )
{

}

void Micro_Menu::editBoxEditingDidEnd( cocos2d::extension::CCEditBox* editBox )
{

}

void Micro_Menu::editBoxTextChanged( cocos2d::extension::CCEditBox* editBox, const std::string& text )
{

}

void Micro_Menu::editBoxReturn( cocos2d::extension::CCEditBox* editBox )
{

}

void Micro_Menu::draw(){
    
  //  ccDrawColor4B(255,0,0,130);
	//glLineWidth(3);

        
	//ccDrawLine(CCPointZero,(SCR_CENTER_POINT));
    
   // ccDrawRect(ccp(0,0),ccp(SCR_SIZE.width,SCR_SIZE.height));
    
}

