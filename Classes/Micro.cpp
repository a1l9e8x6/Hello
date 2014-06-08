#include "Micro.h"
#include "MZDataManager.h"
#include "PopupLayer.h"
#include "LevelView.h"
#include "Micro_Key.h"
#include "CsvUtil.h"
#include "CSVParse.h"

#include "curl/curl.h"

static int iclock = -1;

#define microfont "mirco/font.fnt"
#define microtextureplist "mirco/texture.plist"

#define LEFT_DANGBAN_AMOUT_TAG 1000
#define RIGHT_DANGBAN_AMOUT_TAG 1001

//
//static Micro*  _sharedMicroManager = NULL;
//
//Micro* Micro::shareMicroManager()
//{
//	if(!_sharedMicroManager)
//        _sharedMicroManager = new Micro();
//	return _sharedMicroManager;
//}


Micro::Micro(){
	init_mapx = 0;
    init_mapy = 0;

	distance =0.0f;
	 mscale = 1.0f;
    deltax = 0.0f;
    deltay = 0.0f;

	map_total_w =0;
	map_total_h = 0;

	show_xuanze = false;
	xuanze_count = 0;
    touch_oldPos = CCPointZero;
    touch_newPos = CCPointZero;
     iclock = 0;
	 level = -1;

	 bg_left_sprite = NULL;
	 xuanze_sprite = NULL;

	 testRect = CCRectMake(0,0,0,0);
    gameState = GAMESTATE_RUN;
    
 
}

Micro::~Micro(){

    
        curl_global_cleanup();
    
	//CCNotificationCenter::sharedNotificationCenter()->purgeNotificationCenter();
	CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, LEVEL_NOTIFICATION); 

	if(cnodeball){
	cnodeball->removeAllChildrenWithCleanup(true);
	cnodeball->removeFromParentAndCleanup(true);
	}
	//CC_SAFE_RELEASE(cnodeball);
	//CC_SAFE_RELEASE(ccnodetool);
//	CC_SAFE_RELEASE(ccnode_left);
	//cnodeball = NULL;
	//ccnodetool =NULL;
	//ccnode_left = NULL;
}

CCScene* Micro::scene(){
	// 'scene' is an autorelease object
	CCScene *scene = CCScene::create();
    
	// 'layer' is an autorelease object
	Micro *layer = Micro::create();
    
	// add		 layer as a child to scene
	scene->addChild(layer);
    
	// return the scene
	return scene;
    
}

static size_t writeData(void* buffer,size_t size,size_t nmemb,void* userp){
    CCLog("writeData%s",(char*)buffer);
    return  size;
    
}

bool Micro::init(){
	if(!CCLayer::init())
		return false;
    
    
    curl_global_init(CURL_GLOBAL_ALL);
   
    CURL* esay_handle = curl_easy_init();
    curl_easy_setopt(esay_handle, CURLOPT_URL,"www.baidu.com");
    curl_easy_setopt(esay_handle, CURLOPT_WRITEFUNCTION, writeData);
    CURLcode success = curl_easy_perform(esay_handle);
    CCLog("success ==%d",success);
//    if(success == CURLE_OK )
//    {
//        CCLog("curl...ok....");
//    }
    

    
	setTouchEnabled(true);

	MZDataManager::sharedDataManager()->initLanguageString("mirco/language_str.json");
	// 地图滚动有间隙 cocos2d 默认不是2D正交视图，貌似是这原因，所以要设置一下。
	//CCDirector::sharedDirector()->setProjection(kCCDirectorProjection2D);

        //第一个参数是监听消息的对象，第二个参数是消息发来的时候调用的函数，第三个参数消息的名字 //第四个参数消息体
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this,
                                                                  callfuncO_selector(Micro::getLevelNotification),
                                                                  LEVEL_NOTIFICATION,NULL);
    
	MyCCLog::set_showinfo(false);
 

	CCLog("akkkkkkkkkkkkkkkkkkk");

	/*CCSprite* sp = CCSprite::create("mirco/spott.png");

	sp->setPosition(CCPointMake(100,100));
	this->addChild(sp,100);
*/


	return true;
}





void Micro::initGame(){

	mscale = 1.0f;
	mapisdragorscale = false;
	
	cnodeball = CCNode::create();
	ccnodetool = CCNode::create();
	ccnode_left = CCNode::create();
	ccnode_stonewall = CCNode::create();
	ccnodekey = CCNode::create();
	ccnodelock = CCNode::create();

	left_view(level);
    TipMove();
	loadMap(level);
	

	 //选择框
	xuanze_sprite = CCSprite::create("mirco/xuanze.png");
	xuanze_sprite->setVisible(false);
	_tileMap->addChild(xuanze_sprite,_tileMap->getChildrenCount());
	
    //add ball
    cnodeball->setAnchorPoint(ccp(0.5,0.5));
	_tileMap ->addChild(cnodeball,_tileMap->getChildrenCount());

	//add tool
    
	ccnodetool->setAnchorPoint(ccp(0.5,0.5));
	_tileMap->addChild(ccnodetool);

	//add stonewall
	ccnode_stonewall->setAnchorPoint(ccp(0.5,0.5));
	_tileMap->addChild(ccnode_stonewall);

	//add key
	ccnodekey->setAnchorPoint(ccp(0.5,0.5));
	_tileMap->addChild(ccnodekey);
	
	//add lock
	ccnodelock->setAnchorPoint(ccp(0.5,0.5));
	_tileMap->addChild(ccnodelock);

	this->addChild(_tileMap,-1);
    
  
    //我把2个按钮添加到ccnode上了。
//    BTN_NODE = CCNode::create();
//    ButtonSprite* my_button = ButtonSprite::createButtonSprite("mirco/button.png");
//    my_button->setPosition(CCPoint(200, 200));
//    my_button->setTag(10001);
//    my_button->setOnClickListener(this,click_selector(Micro::btncall));
//    BTN_NODE->addChild(my_button);
//    
//    ButtonSprite* my_button2 = ButtonSprite::createButtonSprite("mirco/button.png");
//    my_button2->setPosition(CCPoint(400, 400));
//    my_button2->setTag(10002);
//    my_button2->setOnClickListener(this,click_selector(Micro::btncall));
//    BTN_NODE->addChild(my_button2);
//    
//    addChild(BTN_NODE);
    
   
}

//button 相应函数不执行 yinggai 进的 没进来啊
void Micro::btncall(){
    
    CCLog("btncall-----");
    
    if (BTN_NODE == NULL) {
        CCLog("isisisisisisiis");
    }
    else{
       // CCLog("sokwiwwsoss====%d",BTN_NODE->getChildrenCount());
        CCLog("btn_node is not null ");
        
        CCObject *temp;
        //BTN_NODE 报错，为null
        CCArray *btnArray = BTN_NODE->getChildren();
        if(NULL == btnArray || btnArray->count() == 0)
            return;
        
        //CCLog("node-aray count ==%d",nodeArray->count());
        
        CCARRAY_FOREACH_REVERSE(btnArray, temp)
        {
            ButtonSprite* btn =  dynamic_cast<ButtonSprite*>(temp);
            if(btn->getTag() == 10001)
            {
                CCLog("*********");
                break;
            }
            else if(btn->getTag() == 10002){
                
                CCLog("****%%%%%%%%%%****");
                break;
                
                
            }
        }
    }
    

}


//void Micro::registerWithTouchDispatcher(){
//    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
//}


void Micro::getLevelNotification(CCObject* obj){//关卡选择时捕获消息
	CCString* str1 = (CCString*) obj;
	level = str1->intValue();
	CCLog("getLevelNotification() = %d",level);

	initGame();

	 this->scheduleUpdate();

}

void Micro::readdangbaninfo(int level_){
   
    
   // langFileŒ™“™Ω‚ŒˆµƒŒƒº˛√˚
	#if ( (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) ||  (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) )
	
	//LOAD BY CSV
    CSVParse* csvFile = new CSVParse("mirco/mapdata.csv");
	if( NULL  == csvFile )
	{
		CCLog("csvFile is NULL");
	}
	else
	{
		CCLog("csvFile is not  NULL");
	}
    //µ√µΩCSVParse∂‘œÛ∫Ûø…“‘µ˜”√
	const char* left = csvFile->getData(level-1,1);
    level_leftpiece = atoi(left);
    
    const char* right = csvFile->getData(level-1,2);
    level_rightpiece = atoi(right);

	#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

	string sPath ="mirco/mapdata.csv";
	CsvUtil::sharedCsvUtil()->loadFile(sPath.c_str());
	const char* left = CsvUtil::sharedCsvUtil()->get(level,2,sPath.c_str());
	level_leftpiece = atoi(left);

	const char* right = CsvUtil::sharedCsvUtil()->get(level,3,sPath.c_str());
	level_rightpiece = atoi(right);
	
	#endif

    
	CCLog("leftpiece ==============%d rightpiece======================%d",level_leftpiece,level_rightpiece);

    //LOAD BY JSON

 //   char dangbanjson [20] = {0};
	//sprintf(dangbanjson,"map%d",level_);
	//CCLog("level_ ===%d",level_);
 //   
 //   
	//CCString* ccstr = CCString::createWithFormat("%s", MZDataManager::sharedDataManager()->getLanguageString(dangbanjson).c_str());  
	//string piece = ccstr->m_sString;
	//vect_str =  MZDataManager::sharedDataManager()->split(piece,";");

	////取vector元素的2种写法
	////写法一
	////	int leftpiece = atoi( vect_str.front().c_str() );
	////	int rigthpiece = atoi( vect_str.at(vect_str.size()-1).c_str() );
	////写法2
	//level_leftpiece = atoi(   (*(vect_str.begin()+1)).c_str()  );
	//level_rightpiece = atoi( (*(vect_str.end()-1)).c_str()  );
	//CCLog("leftpiece %d rightpiece=%d",level_leftpiece,level_rightpiece);
	////用迭代器删除元素的写法
	////for( vector<string>::iterator it = vect_str.begin(); it != vect_str.end(); ){
	////	if( (*it)  == "3")
	////	{
	////		it = vect_str.erase(it);
	////	}
	////	else
	////	{
	////		it++;
	////	}
	////}
 //   
    
}

//左侧区域
void Micro::left_view(int level_){

	CCLog("left_view =====%d",level_);

	ccnode_left->setAnchorPoint(ccp(0.5,0.5));
	readdangbaninfo(level_);
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(microtextureplist);
	
	bg_left_sprite = CCSprite::createWithSpriteFrameName("bg_left.png");
	bg_left_sprite->setPosition(ccp(bg_left_sprite->getContentSize().width/2,bg_left_sprite->getContentSize().height/2));
	ccnode_left->addChild(bg_left_sprite);

	CCLabelBMFont* cclm_b1 = CCLabelBMFont::create(MZDataManager::sharedDataManager()->getLanguageString("str1").c_str(),microfont);
	cclm_b1->setPosition(ccp(SCR_SIZE.width/6,SCR_SIZE.height-cclm_b1->getContentSize().height-20));
	testPoint = cclm_b1->getPosition();
	bg_left_sprite->addChild(cclm_b1);

	CCLabelBMFont* cclm_level = CCLabelBMFont::create("LEVEL :",microfont);
	cclm_level->setPosition(ccp(SCR_SIZE.width/8,SCR_SIZE.height-cclm_b1->getContentSize().height*3-10));
	cclm_level->setColor(ccc3(255,255,35));
	bg_left_sprite->addChild(cclm_level);


	char levelnumber[3];
	sprintf(levelnumber,"%d",level_);
	CCLog("levelnumber =%s",levelnumber);
	CCLabelBMFont* cclm_level_number = CCLabelBMFont::create("0","mirco/font.fnt");
	cclm_level_number->setPosition( ccpAdd( cclm_level->getPosition(),  ccp(cclm_level->getContentSize().width-10,0)) );
	cclm_level_number->setColor(ccc3(200,255,255));
	cclm_level_number->setCString(levelnumber);
	bg_left_sprite->addChild(cclm_level_number);

	CCLabelBMFont* cclm_tool = CCLabelBMFont::create(MZDataManager::sharedDataManager()->getLanguageString("str4").c_str(),microfont);
	cclm_tool->setPosition(ccpSub( cclm_level->getPosition(),  ccp(-cclm_level->getContentSize().width/3, cclm_level->getContentSize().height*2)));
	cclm_tool->setColor(ccc3(32,230,35));
	bg_left_sprite->addChild(cclm_tool);

	//挡板 LEFT
	CCSprite* leftdangban = CCSprite::createWithSpriteFrameName("dangban_leftbg.png");

	CCMenuItemSprite* leftdangban_spriteitem = CCMenuItemSprite::create(leftdangban,leftdangban,NULL,this,menu_selector(Micro::menuLeftDangBanCallback));
		leftdangban_spriteitem->setAnchorPoint(ccp(0.5,0.5));
		leftdangban_spriteitem->setPosition(bg_left_sprite->getContentSize().width/3,cclm_tool->getPositionY()-cclm_tool->getContentSize().height*2);;

	CCMenu* leftdangban_menu = CCMenu::create(leftdangban_spriteitem,NULL);
		leftdangban_menu->setPosition(CCPointZero);
		bg_left_sprite->addChild(leftdangban_menu);

		//显示挡板数量 X 1
		char left_dangban_char[10];
		sprintf(left_dangban_char,"%d",level_leftpiece);
		
		CCString* ccstr_leftpiece = CCString::createWithFormat("%s",left_dangban_char);
		string leftx = "X  ";
		const char* left = (leftx.append(ccstr_leftpiece->m_sString)).c_str();

		CCLabelBMFont* cclm_left = CCLabelBMFont::create(left,microfont);
		//只能是由直接的父节点通过getTag获得；不可通过父节点的父节点等等获得
		cclm_left->setTag(LEFT_DANGBAN_AMOUT_TAG);
		cclm_left->setPosition(   ccp ( bg_left_sprite->getContentSize().width/2+30,leftdangban_spriteitem->getPositionY() ) );
	bg_left_sprite->addChild(cclm_left);


		//挡板RIGHT
		CCSprite* leftdangban_right= CCSprite::createWithSpriteFrameName("dangban_rightbg.png");

	CCMenuItemSprite* rightdangban_spriteitem = CCMenuItemSprite::create(leftdangban_right,leftdangban_right,NULL,this,menu_selector(Micro::menuRightDangBanCallback));
		rightdangban_spriteitem->setAnchorPoint(ccp(0.5,0.5));
		rightdangban_spriteitem->setPosition(bg_left_sprite->getContentSize().width/3,leftdangban_spriteitem->getPositionY()-leftdangban_spriteitem->getContentSize().height*2);;

	CCMenu* rightdangban_menu = CCMenu::create(rightdangban_spriteitem,NULL);
		rightdangban_menu->setPosition(CCPointZero);
		bg_left_sprite->addChild(rightdangban_menu);


		//显示挡板数量 X 1 有两种写法
		char right_dangban_char[10];
		sprintf(right_dangban_char,"%d",level_rightpiece);

		//写法1
		char rightchar[10] = "X   ";
	    char* right = strcat(rightchar ,right_dangban_char);

		//写法2
	/*	CCString* ccstr_rightpiece = CCString::createWithFormat("%s",right_dangban_char);
		string rightx = "X   ";
		const char* right = (rightx.append(ccstr_leftpiece->m_sString)).c_str();*/
		

		CCLabelBMFont* cclm_right = CCLabelBMFont::create(right,microfont);
		cclm_right->setTag(RIGHT_DANGBAN_AMOUT_TAG);
		cclm_right->setPosition(   ccp ( bg_left_sprite->getContentSize().width/2+30,rightdangban_spriteitem->getPositionY() ) );
	bg_left_sprite->addChild(cclm_right);


	//-------------------BUTTON--------------第1个ITEM
	//按钮
		CCSprite* btn_sprite_noraml = CCSprite::createWithSpriteFrameName("btn_click.png");
		CCSprite*  btn_sprite_press   = CCSprite::createWithSpriteFrameName("btn_click2.png");

		CCMenuItemSprite* restItem_reset = CCMenuItemSprite::create(btn_sprite_noraml,btn_sprite_press,NULL,this,menu_selector(Micro::menuRetGameCallback));
		restItem_reset->setAnchorPoint(ccp(0.5,0.5));
		restItem_reset->setPosition(bg_left_sprite->getContentSize().width/2,restItem_reset->getContentSize().height*2);;

		  // 添加文字说明并设置位置
			CCLabelBMFont* cclm_reset = CCLabelBMFont::create(MZDataManager::sharedDataManager()->getLanguageString("str2").c_str(),microfont);
			cclm_reset->setPosition(restItem_reset->getContentSize().width/2,restItem_reset->getContentSize().height/2);
			cclm_reset->setColor(ccc3(20,52,162));
			restItem_reset->addChild(cclm_reset);


	//---------------------------------第2个ITEM
		//还不能用一个sprite TMD
		CCSprite*  btn_sprite_noraml_pause = CCSprite::createWithSpriteFrameName("btn_click.png");
		CCSprite*   btn_sprite_press_pause = CCSprite::createWithSpriteFrameName("btn_click2.png");

		CCMenuItemSprite* restItem_pause = CCMenuItemSprite::create(btn_sprite_noraml_pause,btn_sprite_press_pause,NULL,this,menu_selector(Micro::menuPauseGameCallback));
		restItem_pause->setAnchorPoint(ccp(0.5,0.5));
		restItem_pause->setPosition(bg_left_sprite->getContentSize().width/2,restItem_pause->getContentSize().height/2);;
	
			  // 添加文字说明并设置位置
		CCLabelBMFont* cclm_pause = CCLabelBMFont::create(MZDataManager::sharedDataManager()->getLanguageString("str3").c_str(),microfont);
		cclm_pause->setPosition(restItem_pause->getContentSize().width/2,restItem_pause->getContentSize().height/2);
		cclm_pause->setColor(ccc3(20,52,162));
		restItem_pause->addChild(cclm_pause);

		CCMenu* menu = CCMenu::create(restItem_reset,restItem_pause,NULL);
		menu->setPosition(CCPointZero);
		bg_left_sprite->addChild(menu);

		ccnode_left->setPosition(ccp(bg_left_sprite->getContentSize().width/2*(-1),0));

		CCActionInterval * moveto = CCMoveTo ::create(.5f,ccp(0,0));
		CCActionInterval * scaleto2 = CCEaseOut::create(moveto,.5f);
		CCFiniteTimeAction * seq = CCSequence::create(scaleto2,NULL);	
		ccnode_left->runAction(seq);
	this->addChild(ccnode_left);

}

void Micro::TipMove(){
    
    CCLabelTTF* tip_font = CCLabelTTF::create(MZDataManager::sharedDataManager()->getLanguageString("str6").c_str(), "HiraKakuProN-W6", 36);
    //tip_font->setRotation(45);
    ccColor3B randcolor = ccc3(MZDataManager::sharedDataManager()->myRand(1, 255), MZDataManager::sharedDataManager()->myRand(1, 255), MZDataManager::sharedDataManager()->myRand(1, 255));
    tip_font->setColor(randcolor);
     tip_font->setTag(K_TAG_TIPS);
    float tip_y = (MZDataManager::sharedDataManager()->myRand(1, 9)/10.0f)*SCR_SIZE.height;
    CCLog("tip_y ===%.2f",tip_y);
	tip_font->setPosition(ccp(SCR_SIZE.width+tip_font->getContentSize().width,tip_y));
    
    float speed = 100.0f;
   CCActionInterval* move = CCMoveTo::create(SCR_SIZE.width/speed, ccp(-tip_font->getContentSize().width/2, tip_y));
      CCActionInterval* delay = CCDelayTime::create(1.0f);
    
    CCCallFuncN* func = CCCallFuncN::create(this, callfuncN_selector(Micro::tipMoveCallback));
    CCSequence* seq = CCSequence::create(move,delay,func,NULL);
    
    tip_font->runAction(seq);
    this->addChild(tip_font,10);
}

void Micro::tipMoveCallback(CCNode* pSender){
    CCSprite* psprite = dynamic_cast<CCSprite*>(pSender);
    CCLog("backback");
    
    float speed = 100.0f;
  
    
    ccColor3B randcolor = ccc3(MZDataManager::sharedDataManager()->myRand(1, 255), MZDataManager::sharedDataManager()->myRand(1, 255), MZDataManager::sharedDataManager()->myRand(1, 255));
    psprite->setColor(randcolor);

      float tip_newy = (MZDataManager::sharedDataManager()->myRand(1, 9)/10.0f)*SCR_SIZE.height;
    psprite->setPosition(ccp(SCR_SIZE.width+psprite->getContentSize().width,tip_newy));
    CCActionInterval* move = CCMoveTo::create(SCR_SIZE.width/speed, ccp(-psprite->getContentSize().width/2, tip_newy));
    CCActionInterval* delay = CCDelayTime::create(1.0f);
    
    CCCallFuncN* func = CCCallFuncN::create(this, callfuncN_selector(Micro::tipMoveCallback));
    CCSequence* seq = CCSequence::create(move,delay,func,NULL);
    
    psprite->runAction(seq);

}

void Micro::loadMap(int _LEVEL)
{
	do
	{
		char mapname [20] = {0};
		sprintf(mapname,"mirco/map_%d.tmx",_LEVEL);
		CCLog("mapname ===============%s",mapname);

		_tileMap = CCTMXTiledMap::create(mapname);
		CC_BREAK_IF(NULL == _tileMap);
		//默认为0
		//_tileMap->setAnchorPoint(ccp(0,0));
		init_mapx = bg_left_sprite->getPositionX()+ bg_left_sprite->getContentSize().width/2+
			(SCR_SIZE.width- bg_left_sprite->getContentSize().width ) /2  - _tileMap->getContentSize().width/2;
		init_mapy = SCR_SIZE.height/2 - _tileMap->getContentSize().height/2;
		CCLog("init_mapxinit_mapx ==%d",init_mapx);
		_tileMap->setPosition(init_mapx,init_mapy);

		MyCCLog::verbose("mapx =%f mapy=%f",_tileMap->getPositionX(),_tileMap->getPositionY());

	} while (0);

	CCSize mapSize = _tileMap->getContentSize();
	CCLog(" Content size: width = %.2f, height = %.2f.", mapSize.width, mapSize.height );
	// width and heigth with one tile(eg.map1-> 16,16)
	CCLog("onetile width =%.2f onetiled height =%.2f",_tileMap->getTileSize().width,_tileMap->getTileSize().height);
	//how much tild in width and height (eg.map1-->  60 ,80)
	CCLog("tiledwidth =%.2f tiledheight =%.2f",_tileMap->getMapSize().width,_tileMap->getMapSize().height);

	map_total_w = mapSize.width;
	map_total_h = mapSize.height;

	loadStoneWall(_tileMap);
	loadkey(_tileMap);
	loadLock(_tileMap);


	//testPoint = ccp(SCR_SIZE.width/2,SCR_SIZE.height/2);
	//门的粒子效果
	CCParticleSystemQuad *particle_men = CCParticleSystemQuad::create("mirco/lizi/men.plist");
	particle_men->setTag(MEN_TAG);
	menPos = getParitclePos(_tileMap,"door","men");
	if(!menPos.equals(CCPointZero)){
	particle_men->setPosition(ccpAdd(menPos,ccp(tile_one_w/2,tile_one_h)));
	CCLog("paritcle----men----x=%.2f,y=%.2f",particle_men->getPositionX(),particle_men->getPositionY());
	CCLog("particle-----men----contentsize w =%.2f,h=%.2f",particle_men->getContentSize().width,particle_men->getContentSize().height);
	//particle_men->setPosition(ccpAdd(particle_men->getPosition(),ccp(_tileMap->getPositionX(), _tileMap->getPositionY() )));
	_tileMap->addChild(particle_men,_tileMap->getChildrenCount());
	}
    

	
}


CCPoint Micro::getParitclePos(CCTMXTiledMap* _tileMap,const char* groupName,const char* objectName){
	//对象层
	CCTMXObjectGroup *objects=_tileMap->objectGroupNamed(groupName);
	if(objects){
		CCPoint particlePoint;
		CCDictionary  *spawnPoint=objects->objectNamed(objectName);
		particlePoint.x=spawnPoint->valueForKey("x")->floatValue();
		particlePoint.y=spawnPoint->valueForKey("y")->floatValue();

		//以tilemap里地图的左下为zero的，固定坐标
		//heroPoint 最终是添加到 tilemap上所以 永远是对于map的相对坐标 ，不是屏幕的实际坐标
		//CCLog("-------particlePoint-----%.2f,%.2f",particlePoint.x,particlePoint.y);

		return particlePoint;
	}
	return CCPointZero;
}

void Micro::herocreate(CCTMXTiledMap* _tileMap,const char* filename,const char* groupName,const char* objectName){
	do{
		CC_BREAK_IF(NULL == _tileMap);
        CCLog("SSSSSSS----herocreate----SSSSSS");
        
        //Micro_ball* mball =  Micro_ball::create(_tileMap,"mirco/hero_0.png","hero","ball");
        Micro_ball* mball =  Micro_ball::create(_tileMap,filename,groupName,objectName);
            // vector_ball.push_back(mball);
        mball->ball_move();
        CCLog(" herocreate()-----init------ m_ball pos x =%.2f posy =%.2f",mball->getPositionX(),mball->getPositionY());
        //CCLog("vector size ==%d",vector_ball.size());
        
        
        //CCLog("getchilde count = %d",_tileMap->getChildren()->count());//图层 AMOUNT
        // _tileMap->addChild(mball,_tileMap->getChildren()->count());
        
        //cnode 是父亲节点，设置为 跟随tilemap 移动，然后 mball是根据tmx 读取出来的子节点，他的坐标是相对于 tmx的
		//cnodeball->setPosition(ccp(_tileMap->getPositionX(), _tileMap->getPositionY() ));
        cnodeball->addChild(mball);
     
       // Micro* micro = (Micro*)(cnodeball->getParent());


        
	}while(0);
    
}


void Micro::collision_ball(float t)
{
	 
    CCObject *temp;
    CCArray *nodeArray =cnodeball ->getChildren();
    if(NULL == nodeArray || nodeArray->count() == 0)
        return;

	 //CCLog("node-aray count ==%d",nodeArray->count());
	
	CCARRAY_FOREACH_REVERSE(nodeArray, temp)
	{
		Micro_ball* mball =  dynamic_cast<Micro_ball*>(temp);

	//	CCLog("update()----mball xx == %.2f yy =%.2f",mball->getPositionX(),mball->getPositionY());
		
		  if( checkTool(_tileMap,mball) ){
			 CCLog("collision_ball---wwwwwwww");
		  	mball->set_check_tool(true);
			mball->changeballdir();
		  }


		  if(checkKey(_tileMap,mball)){
			 	  if( NULL != mball->getParent() && NULL!= mball){
					  mball->removeFromParentAndCleanup(true);
					  CCLog("CHECK---LOCK------");
					  break;
				  }
		  }

		  if(checkLock(_tileMap,mball)){
			 	  if( NULL != mball->getParent() && NULL!= mball){
					  mball->removeFromParentAndCleanup(true);
					  CCLog("CHECK---LOCK------");
					  break;
				  }
		  }


	  //removeFromParentAndCleanup mball还存在。不得不break
	  if(NULL == mball){
		  CCLog("mball---NULLLLLLLLLLLLLLLLLLLLLLLLLL----after----removeFromParentAndCleanup---");
	  }

	
	  if( checkCollStoneWall(_tileMap,mball) ){
			 CCLog("ttttttttt");
		 	if( NULL != mball->getParent() && NULL!= mball){
				mball->removeFromParentAndCleanup(true);
					break;
			}
	  }

		//ball collision with wall
		if(Collision(_tileMap,mball,true)){
			CCLog("rrrrrrrrrrrrrrrrrrr");
			//显示墙壁碰撞效果
			CCSprite* sprite_left_collsion = getTouchSpot(_tileMap, "build_left",
                                                          ccp(_tileMap->getPositionX()+mball->getPositionX()+mball->getPositionX()*(mscale-1.0f),
                                                              _tileMap->getPositionY()+mball->getPositionY()+mball->getPositionY()*(mscale-1.0f)));
			CCSprite* sprite_up_collsion = getTouchSpot(_tileMap, "build_up", ccp(_tileMap->getPositionX()+mball->getPositionX()+mball->getPositionX()*(mscale-1.0f),
                                                                                  _tileMap->getPositionY()+mball->getPositionY()+mball->getPositionY()*(mscale-1.0f)));
            
			CCSprite* sprite_down_collsion = getTouchSpot(_tileMap, "build_down", ccp(_tileMap->getPositionX()+mball->getPositionX()+mball->getPositionX()*(mscale-1.0f), _tileMap->getPositionY()+mball->getPositionY()+mball->getPositionY()*(mscale-1.0f)));
			CCSprite* sprite_right_collsion = getTouchSpot(_tileMap, "build_right", ccp(_tileMap->getPositionX()+mball->getPositionX()+mball->getPositionX()*(mscale-1.0f), _tileMap->getPositionY()+mball->getPositionY()+mball->getPositionY()*(mscale-1.0f)));
			if(NULL == sprite_left_collsion){
				//CCLog("sprite left is nulllll");
			}
			else{
				CCActionInterval * scaleto =  CCScaleTo ::create(0.1f, 1.5f);
				CCActionInterval * scaleto1 = CCScaleTo ::create(0.1f,1.2f);
				CCActionInterval * scaleto2 = CCScaleTo ::create(0.1f,1.0f);
				CCFiniteTimeAction * seq2 = CCSequence::create(scaleto,scaleto1,scaleto2,NULL);
				sprite_left_collsion->runAction(seq2);
			}

			if(NULL == sprite_up_collsion){
				//CCLog("sprite up is nulllll");
			}
			else{
				CCActionInterval * scaleto =  CCScaleTo ::create(0.1f, 1.5f);
				CCActionInterval * scaleto1 = CCScaleTo ::create(0.1f,1.2f);
				CCActionInterval * scaleto2 = CCScaleTo ::create(0.1f,1.0f);
				CCFiniteTimeAction * seq2 = CCSequence::create(scaleto,scaleto1,scaleto2,NULL);
				sprite_up_collsion->runAction(seq2);
			}


			if(NULL == sprite_down_collsion){
				//CCLog("sprite down is nulllll");
			}
			else{
				CCActionInterval * scaleto =  CCScaleTo ::create(0.1f, 1.5f);
				CCActionInterval * scaleto1 = CCScaleTo ::create(0.1f,1.2f);
				CCActionInterval * scaleto2 = CCScaleTo ::create(0.1f,1.0f);
				CCFiniteTimeAction * seq2 = CCSequence::create(scaleto,scaleto1,scaleto2,NULL);
				sprite_down_collsion->runAction(seq2);
			}


			if(NULL == sprite_right_collsion){
				//CCLog("sprite right is nulllll");
			}
			else{
				CCActionInterval * scaleto =  CCScaleTo ::create(0.1f, 1.5f);
				CCActionInterval * scaleto1 = CCScaleTo ::create(0.1f,1.2f);
				CCActionInterval * scaleto2 = CCScaleTo ::create(0.1f,1.0f);
				CCFiniteTimeAction * seq2 = CCSequence::create(scaleto,scaleto1,scaleto2,NULL);
				sprite_right_collsion->runAction(seq2);
			}

			if( NULL != mball->getParent() && NULL!= mball){
				mball->removeFromParentAndCleanup(true);
					break;
			}
		}

	//	float distance = ccpDistance( mball->getPosition(), menPos ) ;
	//		CCLog("update()----mball xx == %.2f yy =%.2f",mball->getPositionX(),mball->getPositionY());
	//		CCLog("update()----menPos xx == %.2f yy =%.2f",menPos.x,menPos.y);
	//	CCLog("distacen =%.2f",distance);
		CCRect menRect = CCRectMake(menPos.x,menPos.y,tile_one_w,tile_one_h);
		if(menRect.containsPoint(mball->getPosition()) )
		{
			gameOver();
			if( NULL != mball->getParent() && NULL!= mball){
			mball->removeFromParentAndCleanup(true);
			break;
			}
		}
		
	}

    // CCLog("node-aray count ==========================%d",nodeArray->count());
    
    
    
    //
    //	if(!vector_ball.empty()){
    //        // CCLog(" Collision vect size =%d",vector_ball.size());
    //
    //		for(Micro_ball* m_ball : vector_ball)
    //		{
    //            //CCLog(" Collision m_ball pos x =%.2f posy =%.2f",m_ball->getPositionX(),m_ball->getPositionY());
    //			if(Collision(_tileMap,m_ball,true)){
    //                vector_ball.find_if(vector_ball.begin(), vector_ball.end(), m_ball);
    //                vector_ball.erase(vector_ball.begin());
    //                m_ball->removeFromParentAndCleanup(true);
    //			}
    //		}
    //	}
    
}






void Micro::ccTouchesBegan(CCSet* pTouches,CCEvent* event){
	MyCCLog::verbose("ccTouchesBegan");
    
	if(!pTouches||!pTouches->count()){
		return ;
	}
	cocos2d::CCTouch* pTouch = (cocos2d::CCTouch*)pTouches->anyObject();
    
	

	if(pTouches->count()==1)
	{
	CCLog("111111111111111111111111111");

		

	}

	 if(pTouches->count()>=2)  //如果触摸点不少于两个
    {

	 
		CCLog("222222222222222222222222222222222222");
        CCSetIterator iter=pTouches->begin();
		CCPoint mPoint1=((CCTouch *)(*iter))->getStartLocationInView();
        mPoint1 = CCDirector::sharedDirector()->convertToGL(mPoint1);
        iter++;
        CCPoint mPoint2=((CCTouch *)(*iter))->getStartLocationInView();
        mPoint2 = CCDirector::sharedDirector()->convertToGL(mPoint2);
        
		distance=ccpDistance(mPoint1,mPoint2);//计算两个触摸点距离
		deltax = (mPoint1.x + mPoint2.x)/2 - _tileMap->getPositionX();     //得到两个触摸点中点和精灵锚点的差值
        deltay = (mPoint1.y + mPoint2.y)/2 - _tileMap->getPositionY();
		CCLog("ccTouchesMOVE  ... begindistance === %f",distance);

    }

	//¬™√í¬ª¬∞¬µ¬±¬´‚àû‚Äù‚àö¬™√ü¬•‚Ä¢‚àÜ¬°¬µ‚Äû‚óä¬Ø¬±√ç‚â§¬¢Œ©¬?d‚óä¬Ø¬±√ç‚Äù‚â•‚Ä¶‚Ä?d‚óä¬Ø¬±√ç≈ì¬?
	//CCPoint touchPoint = touch->getLocationInView();
	//touchPoint = CCDirector::sharedDirector()->convertToGL( touchPoint );
	//oldx = touchPoint.x;
	//oldy = touchPoint.y;
	//MyCCLog::verbose("ccTouchBegan || oldx %f oldy %f ",oldx,oldy);
}

void Micro::ccTouchesMoved(CCSet* pTouches,CCEvent* event){
    //MyCCLog::verbose("ccTouchesMoved");
    
	CCTouch *touch = (CCTouch*)pTouches->anyObject();
    cocos2d::CCTouch* pTouch = (cocos2d::CCTouch*)pTouches->anyObject();
    
	int move_start_x = pTouch->getStartLocation().x;
	int move_start_y = pTouch->getStartLocation().y;


	if(move_start_x < bg_left_sprite->getPositionX()+ bg_left_sprite->getContentSize().width/2)
		return;

	mapisdragorscale = true;
	 if(pTouches->count()>=2)  //如果移动时触摸点的个数不少于两个
    {
		
        
        CCSetIterator iter = pTouches->begin();
        CCPoint mPoint1 = ((CCTouch*)(*iter))->getLocationInView();
        mPoint1 = CCDirector::sharedDirector()->convertToGL(mPoint1);
        iter++;
        CCPoint mPoint2 = ((CCTouch*)(*iter))->getLocationInView();
        mPoint2 = CCDirector::sharedDirector()->convertToGL(mPoint2);        //获得新触摸点两点之间的距离
        double mdistance = sqrt((mPoint1.x-mPoint2.x)*(mPoint1.x-mPoint2.x)+(mPoint1.y-mPoint2.y)*(mPoint1.y-mPoint2.y));
        mscale = mdistance/distance * mscale;                      //   新的距离 / 老的距离  * 原来的缩放比例，即为新的缩放比例
        distance = mdistance;
        _tileMap->setScale(mscale);
        
        double x = (mPoint2.x+mPoint1.x)/2 - deltax;      //计算两触点中点与精灵锚点的差值
        double y = (mPoint2.y+mPoint1.y)/2 - deltay;
        _tileMap->setPosition(ccp(x,y));                        //保持两触点中点与精灵锚点的差值不变
        deltax = (mPoint1.x+ mPoint2.x)/2 - _tileMap->getPositionX();       //计算新的偏移量
        deltay = (mPoint2.y + mPoint1.y)/2 - _tileMap->getPositionY();
        CCLog("ccTouchMoved  ....");
        
        
	
//        CCSetIterator niter = pTouches->begin();
//        CCPoint nmPoint1 = ((CCTouch*)(*niter))->getLocationInView();
//        nmPoint1 = CCDirector::sharedDirector()->convertToGL(nmPoint1);
//        niter++;
//        CCPoint nmPoint2 = ((CCTouch*)(*niter))->getLocationInView();
//        nmPoint2 = CCDirector::sharedDirector()->convertToGL(nmPoint2);        //获得新触摸点两点之间的距离
//		double mdistance = ccpDistance(nmPoint1,nmPoint2);
//
//
//                            //   新的距离 / 老的距离  * 原来的缩放比例，即为新的缩放比例
//
//		CCLog("ccTouchesMOVE  ... Mdistance === %f   .... mscale ====%f",mdistance,mdistance/distance);
//        if(mscale <1.0f)
//		mscale = MAX( mdistance/distance,min_mscale);
//		else
//		mscale = MIN( mdistance/distance,max_mscale);
//	   
//		_tileMap->setScale( mscale);
//		
//		   distance = mdistance;
//        
//        double x = (mPoint2.x+mPoint1.x)/2 - deltax;      //计算两触点中点与精灵锚点的差值
//        double y = (mPoint2.y+mPoint1.y)/2 - deltay;
//        _tileMap->setPosition(ccp(x,y));                        //保持两触点中点与精灵锚点的差值不变
//        deltax = (mPoint2.x+ mPoint1.x)/2 - _tileMap->getPositionX();       //计算新的偏移量
//        deltay = (mPoint2.y + mPoint1.y)/2 - _tileMap->getPositionY();
    }
    if(pTouches->count()==1)                          //如果触摸点为一个
    {
        //CCSetIterator iter =  pTouches->begin();
        //CCPoint mPoint=((CCTouch*)(*iter))->getLocationInView();
        //mPoint=CCDirector::sharedDirector()->convertToGL(mPoint);    //坐标转换
        //bg->setPosition(mPoint);                    //直接移动精灵
   


	//CCLog("xx =%.2f,yy=%.2f,xxww = %.2f,yyhh = %.2f",xx,yy,xxww,yyhh);

	CCPoint diff = touch->getDelta();

		CCString* ccstr = CCString::createWithFormat("%f%s%f",diff.x,"|",diff.y);
		CCNotificationCenter::sharedNotificationCenter()->postNotification(MAP_NOTIFICATION, ccstr);

		if( _tileMap )
		{
			if(diff.x<=(  bg_left_sprite->getContentSize().width- _tileMap->getPositionX() ))
				diff.x = bg_left_sprite->getContentSize().width- _tileMap->getPositionX();


			if(diff.x >=( SCR_SIZE.width-(_tileMap->getPositionX()+_tileMap->getContentSize().width) ))
				diff.x = SCR_SIZE.width-(_tileMap->getPositionX()+_tileMap->getContentSize().width);


			/*if(diff.y<=-_tileMap->getPositionY())
				diff.y = _tileMap->getPositionY();*/

			//if(diff.y>= (SCR_SIZE.height-(_tileMap->getPositionY()+_tileMap->getContentSize().height )))
				//diff.y = SCR_SIZE.height-(_tileMap->getPositionY()+_tileMap->getContentSize().height);


			/*  {
			if(tilePos.x<=bg_left_sprite->getContentSize().width)
			tilePos.x =bg_left_sprite->getContentSize().width;
			}

			{
			if(tilePos.x+_tileMap->getContentSize().width>=SCR_SIZE.width)
			tilePos.x = SCR_SIZE.width - _tileMap->getContentSize().width;
			}

			{
			if(tilePos.y+_tileMap->getContentSize().height>=SCR_SIZE.height)
			tilePos.y = SCR_SIZE.height - _tileMap->getContentSize().height;

			}
			{
			if(tilePos.y<=0)
			tilePos.y =0;
			}
			//CCLog("tilePosMOVING xxxx =%.2f,yyyy=%.2f",tilePos.x,tilePos.y);
			*/

			CCLog("diff movex===%.2f diff movey =====%.2f",diff.x,diff.y);
			_tileMap->setPosition(   ccpAdd(_tileMap->getPosition(), diff )  );
		
		}

	}
    
}

	
			//选择框移动
			//xuanze_sprite->setPosition(ccpAdd( xuanze_sprite->getPosition(),diff));

			//移动挡板
			/*if(ccnodetool->getChildrenCount()>0)
			{
				CCObject *temp;
				CCArray *nodeArray =ccnodetool ->getChildren();
				CCLog("node-aray count ==%d",nodeArray->count());

				CCARRAY_FOREACH(nodeArray, temp)
				{
					Micro_Tool* mtool =  dynamic_cast<Micro_Tool*>(temp);
					mtool->setPosition(ccp(mtool->getPositionX()+diff.x, mtool->getPositionY()+diff.y));
				}
			}*/

				//传送门移动
		/*	CCParticleSystemQuad*  ccsq = (CCParticleSystemQuad*)this->getChildByTag(MEN_TAG);
			if( NULL != ccsq)
				ccsq->setPosition(ccpAdd( ccsq->getPosition(),diff));*/

			//移动stonewall
			/*if(ccnode_stonewall->getChildrenCount()>0){
				CCObject *stonewall_temp;
				CCArray *nodestonewallArray =ccnode_stonewall ->getChildren();
				CCLog("nodestonewallArray count ==%d",nodestonewallArray->count());
				CCARRAY_FOREACH(nodestonewallArray, stonewall_temp)
				{
					Micro_StoneWall* msw =  dynamic_cast<Micro_StoneWall*>(stonewall_temp);
					msw->setPosition(ccp(msw->getPositionX()+diff.x, msw->getPositionY()+diff.y));
				}
			}*/




void Micro::ccTouchesEnded(CCSet* pTouches,CCEvent* event){
    MyCCLog::verbose("ccTouchEnded");
    
	if(!pTouches||!pTouches->count()){
		return;
	}
    
	cocos2d::CCTouch* pTouch = (cocos2d::CCTouch*)pTouches->anyObject();
    

	//double oldx = pTouch->getStartLocation().x;
	//double oldy = pTouch->getStartLocation().y;
    
	double newx = pTouch->getLocation().x;
	double newy = pTouch->getLocation().y;
    
    
    MyCCLog::verbose("newx %f newy %f oldx %f oldy %f",newx,newy);
    
	if(_tileMap)
	{
        do{
            
			
			CC_BREAK_IF(NULL ==  getTouchSpot(_tileMap,"ground",ccp(newx,newy) ) );
			CCSprite* touch_collison_check_sprite =  getTouchSpot(_tileMap,"ground",ccp(newx,newy) );
			if(touch_collison_check_sprite)
			CC_BREAK_IF(Collision(_tileMap, touch_collison_check_sprite,false));
            
			CCPoint touch_collison_check_point = touch_collison_check_sprite->getPosition();

            
            if (touch_oldPos.equals(CCPointZero)) {
                  touch_oldPos = touch_collison_check_sprite->getPosition();
                CCLog("touch_oldPos x =%f ,y=%f ",  touch_oldPos.x,touch_oldPos.y);
                show_xuanze = true;
				xuanze_count++;
			}
			else{
				touch_newPos = touch_collison_check_sprite->getPosition();
				CCLog("touch_newPos x =%f ,y=%f ",  touch_newPos.x,touch_newPos.y);

				if(touch_oldPos.equals(touch_newPos)){

					if(xuanze_count==-1){
						show_xuanze = true;
						xuanze_count++;
					}
					else{
					show_xuanze = false;
					xuanze_count = -1;
					}
					CCLog("1111");
				}
				else{
					show_xuanze = true;
					xuanze_count++;
					CCLog("2222");
				}
				touch_oldPos = touch_newPos;

			}
		
			xuanze_sprite->setPosition(  	ccp(+tile_one_w/2+touch_collison_check_point.x,
			tile_one_h/2+touch_collison_check_point.y )    );
				CCLog("show_xuanze-before =%d",show_xuanze);
			CCLog("xuanze_sprite x =%.2f ,y=%.2f ",  xuanze_sprite->getPositionX(),xuanze_sprite->getPositionY());


				/*	if(mapisdragorscale&&show_xuanze){
						show_xuanze = false;
						mapisdragorscale = false;
					}*/

			xuanze_sprite->setVisible(show_xuanze);


			//if(show_xuanze)
			//{
			//	//判断该位置上有没有挡板
			//	CCObject *temp;
			//	CCArray *nodeArray =ccnodetool ->getChildren();
			//	if(NULL != nodeArray&& nodeArray->count() >= 0)
			//	{
			//		CCARRAY_FOREACH(nodeArray, temp)
			//		{
			//			Micro_Tool* mtool =  dynamic_cast<Micro_Tool*>(temp);
			//			CCLog("mtool x =%.2f ,y=%.2f ",  mtool->getPositionX(),mtool->getPositionY());

			//			CCRect toolRec = CCRectMake (
			//				_tileMap->getPositionX()+mtool->getPositionX()-tile_one_w/2,
			//				_tileMap->getPositionY()+mtool->getPositionY()-tile_one_h/2,
			//				tile_one_w,
			//				tile_one_h);

			//			CCLog("toolRec leftdown_x == %.2f leftdown_y =%.2f ----- rightupX =%.2f rightupY=%.2f",toolRec.getMinX(),toolRec.getMinY(),toolRec.getMaxX(),toolRec.getMaxY());

			//			CCPoint m2 =  xuanze_sprite->getPosition();
			//		//	testPoint = m2;
			//			CCLog("mtool222222 x =%.2f ,y=%.2f ",m2.x,m2.y  );
			//			if(toolRec.containsPoint(m2))
			//			{
			//					CCLog("show_xuanze-node-aray count ==%d",nodeArray->count());
			//				show_xuanze = false;
			//				break;
			//			}
			//		}

			//	}
			//}
			//		CCLog("show_xuanze-after =%d",show_xuanze);

			
		

        }while(0);
        
        
	}
    
    
    
    
    //if(_tileMap)
    //{
    //    CCTMXLayer*  layer = _tileMap->layerNamed("ground");
    //    do{
    //    CC_BREAK_IF(layer);
    //    }while (0);
    //
    //    //unsigned int m_gid = layer->tileGIDAt(CCPointMake(newx, newy));
    //   // MyCCLog::info("m_gid = %d",m_gid);
    //
    //     CCSprite* sprite = layer->tileAt(ccp(1,20));
    //  //  sprite->retain();
    //
    //   // CCSprite* sprite = layer ? layer->tileAt(tilePosFromLocation(CCPointMake(newx, newy), _tileMap)) : NULL;
    //    if(sprite == NULL)
    //    {
    //        MyCCLog::info("sprite is null");
    //    }
    //    else{
    //        sprite->setColor(ccc3(255,255,255));
    //        MyCCLog::info("sprite is not null");
    
    //    }
    //    do{
    //        CC_BREAK_IF(sprite);
    //    }while (0);
    //}
    
	
}



//返回的是地图的图块元素sprite
CCSprite* Micro::getTouchSpot(CCTMXTiledMap* _tileMap,const char* layername,const CCPoint& touchCCpoint){
	CCTMXLayer* mapLayer = _tileMap->layerNamed(layername);
    if(mapLayer)
    {
        CCSize layerSize = mapLayer->getLayerSize();
        MyCCLog::info("layersize-width =%f layersize-height =%f",layerSize.width,layerSize.height);
        
        float tile_w = 0.0f;
        float tile_h = 0.0f;
        // if( strcmp(layername,"ground") == 0 )
        tile_w =  _tileMap->getTileSize().width;
        tile_h  =  _tileMap->getTileSize().height;
        
        
        MyCCLog::verbose("tile_w=%.2f tile_h=%.2f",tile_w,tile_h);
        
        
		CCPoint touchP =  ccpSub( touchCCpoint,_tileMap->getPosition()); //CCPointMake(touchCCpoint.x,touchCCpoint.y);
        
        MyCCLog::verbose("touch end xx = %f yy = %f",touchP.x,touchP.y);
        
        for(int col=0;col<_tileMap->getMapSize().width;col++) //列
        {
            for(int row=0;row<_tileMap->getMapSize().height;row++) //行
            {
                //touchsprite -->相当于地图的坐标
                CCSprite* touchsprite = mapLayer->tileAt(ccp(col,row));

				//CCLog("getTouchSpot () === mscale == %f",mscale);
			
					
                MyCCLog::verbose("COL =%d, ROW =%d ",col,row);
                if(touchsprite != NULL){
                    MyCCLog::verbose("touchsprite is not null0-000000");
                    //	unsigned int GID	= mapLayer->tileGIDAt(touchP);
                    //	MyCCLog::verbose("GID ------------------- %d",GID);
                    MyCCLog::verbose("col====%d,row===%d," ,col,row);
                    MyCCLog::verbose("one tile sprite--w =%.2f one tile sprite--h =%.2f ",touchsprite->getContentSize().width,touchsprite->getContentSize().height);
                    
                    CCPoint tilePos =  touchsprite->getPosition();

					tilePos.x  += tilePos.x*(mscale-1.0f);
					tilePos.y  += tilePos.y*(mscale-1.0f);

					//CCLog("col === %d ,row ====%d ,tilePos x===%f,tilePos y====%f touchsprite-contentw ==%f" ,col,row,tilePos.x,tilePos.y,touchsprite->getContentSize().width);
					CCRect tileRect = CCRectMake(tilePos.x,tilePos.y,tile_w*mscale,tile_h*mscale);
					//CCRect tileRect = CCRectMake(tilePos.x,tilePos.y,tile_w,tile_h);
					//CCLog("tileRect leftbottomx =%f ,leftbottomy=%f,rightupx = %f, rightupy =%f",tileRect.getMinX(),tileRect.getMinY(),tileRect.getMaxX(),tileRect.getMaxY());
                    if(tileRect.containsPoint(touchP)){
                        CCSprite* sprite = mapLayer->tileAt(ccp(col,row));//触点地图的行列
                        CCLog("COLLLIONS sprite x===%f,sprite y====%f" ,sprite->getPositionX(),sprite->getPositionY());
                        CCLog("COLLLIONS CCSprite col====%d,CCSprite row===%d," ,col,row);
                        return sprite;
                    }
                }
                
            }
        }
    }
    return NULL;
}



bool Micro::checkLock(CCTMXTiledMap* _tileMap,Micro_ball* pball){

		//480 40
    if(NULL == pball){
		return false;
    }

	  CCObject *temp;
	  CCArray *lockArray = ccnodelock->getChildren();
	//  CCLog("ccnodetool  toolArray =%d",toolArray->count());
	 if(NULL == lockArray||lockArray->count() ==0){
        return false;
	 }

	CCPoint ballpoint = pball->getPosition();
	//CCLog("checkLock----ballpoint xx == %.2f yy =%.2f",ballpoint.x,ballpoint.y);
	CCARRAY_FOREACH(lockArray, temp)
	{
		Micro_Lock* lock =  dynamic_cast<Micro_Lock*>(temp);
       // CCLog("lockstate ======%d",lock->get_lockstate());
		if(lock->get_lockstate())
			continue;
		CCArmature* ccarm_lock = lock->get_ccarlock();
		CCRect lockRect =  CCRectMake( ccarm_lock->getPositionX(),ccarm_lock->getPositionY()-tile_one_h/2, tile_one_w/2,tile_one_h);
            
		//CCLog("lockRect leftdown_x == %.2f leftdown_y =%.2f ----- rightupX =%.2f rightupY=%.2f",lockRect.getMinX(),lockRect.getMinY(),
	//		lockRect.getMaxX(),lockRect.getMaxY());
			//coll_rect 实际或者相对坐标
		if( lockRect.containsPoint(ballpoint))
            {
			CCLog("locklocklock------------------------------------");
			//播放
			lock->playParticleEffect();			
				return true; //
            }
	}

	return false;


}

bool Micro::checkKey(CCTMXTiledMap* _tileMap,Micro_ball* pball){
	//480 40
    if(NULL == pball){
		return false;
    }

	  CCObject *temp;
	  CCArray *keyArray = ccnodekey->getChildren();
	//  CCLog("ccnodetool  toolArray =%d",toolArray->count());
	 if(NULL == keyArray||keyArray->count() ==0){
		 return false;
	 }

	CCPoint ballpoint = pball->getPosition();
//	CCLog("checkKey----ballpoint xx == %.2f yy =%.2f",ballpoint.x,ballpoint.y);
	CCARRAY_FOREACH(keyArray, temp)
	{
		Micro_Key* key =  dynamic_cast<Micro_Key*>(temp);
		CCArmature* ccarm_key = key->get_ccar();
		CCRect keyRect =  CCRectMake( ccarm_key->getPositionX()-tile_one_w/2,ccarm_key->getPositionY()-tile_one_h/2, 
			tile_one_w,tile_one_h);
            
	//	CCLog("keyRect leftdown_x == %.2f leftdown_y =%.2f ----- rightupX =%.2f rightupY=%.2f",keyRect.getMinX(),keyRect.getMinY(),keyRect.getMaxX(),keyRect.getMaxY());

			//coll_rect 实际或者相对坐标
		if( keyRect.containsPoint(ballpoint))
          {
			//	sprite->unschedule(schedule_selector(Micro_ball::updateballpos));
			CCLog("keykeykey------------------------------------");
              
            //CCActionInterval* waves = CCWaves3D::create(18,80,ccg(15,10),10);
              
			
			key->setIsHit(true);
			key->playArmKey();

		CCNotificationCenter::sharedNotificationCenter()->postNotification(MICRO_KEY_NOTIFICATION, key);

				
				return true; //
            }
	}

	return false;

}

bool Micro::checkTool(CCTMXTiledMap* _tileMap,Micro_ball* sprite){
	//480 40
    if(NULL == sprite){
        CCLog("checktool()---m_tool  IS NULL-------");
		return false;
    }

	  CCObject *temp;
	  CCArray *toolArray =ccnodetool ->getChildren();
	//  CCLog("ccnodetool  toolArray =%d",toolArray->count());
	 if(NULL == toolArray||toolArray->count() ==0){
		// CCLog("checkTool ()----toolArray ---null");
        return false;
	 }

	 CCPoint ballpoint = sprite->getPosition();
	CCARRAY_FOREACH(toolArray, temp)
	{
		Micro_Tool* tool =  dynamic_cast<Micro_Tool*>(temp);
		CCRect dangbanRect =  CCRectMake( tool->getPositionX()- ball_speed/2,tool->getPositionY()-ball_speed/2,ball_speed,ball_speed);
		

		//	CCLog("checktool()----spritePoint xx == %.2f yy =%.2f",ballpoint.x,ballpoint.y);
            
		//	CCLog("dangbanRect leftdown_x == %.2f leftdown_y =%.2f ----- rightupX =%.2f rightupY=%.2f",dangbanRect.getMinX(),dangbanRect.getMinY(),dangbanRect.getMaxX(),dangbanRect.getMaxY());
	/*	float distance = ccpDistance( tool->getPosition(), sprite->getPosition() );
		CCLog("distance = %.2f",distance);*/

	//	testPoint = ccpAdd(  ballpoint, ccp(_tileMap->getPositionX(),_tileMap->getPositionY() ) );
	//	testPoint = ccpAdd(  ccp(   tool->getPositionX(),tool->getPositionY() ), ccp(_tileMap->getPositionX(),_tileMap->getPositionY() ) );

			//coll_rect 实际或者相对坐标
		if( dangbanRect.containsPoint(ballpoint))
            {
			//	sprite->unschedule(schedule_selector(Micro_ball::updateballpos));

				//播放变色效果
				CCActionInterval * tinby = CCTintBy ::create(0.6f, 255, 255,255);
				  tool->runAction(tinby);

				int kind = 	tool->get_tool_kind();
				sprite->set_tool_dir(kind);
				CCLog("checkTool xiangjiao----- tool_kind =%d",kind);
				
				return true; //
            }
	}

	return false;
}

void Micro::loadLock(CCTMXTiledMap* _tileMap){
	
	if(NULL == _tileMap)
		return ;

    CCTMXObjectGroup* m_pGroup = _tileMap->objectGroupNamed("jiguanlock");
    if(m_pGroup){
        CCArray* m_pObjects = m_pGroup->getObjects();
        CCLog("loadlock--m_pObjects =%d",m_pObjects->count());
        CCDictionary* m_pDict = NULL;
        CCObject* m_pObj = NULL;
        CCARRAY_FOREACH(m_pObjects, m_pObj)
        {
            m_pDict = (CCDictionary*)m_pObj;
            int x = ((CCString*)m_pDict->objectForKey("x"))->intValue();
            int y = ((CCString*)m_pDict->objectForKey("y"))->intValue();
            int width = ((CCString*)m_pDict->objectForKey("width"))->intValue();
            int height = ((CCString*)m_pDict->objectForKey("height"))->intValue();
            
			int lock_dir = m_pDict->valueForKey("lockdir")->intValue();
            int	lock_color_kind = m_pDict->valueForKey("lockcolor")->intValue();
            int lock_id = m_pDict->valueForKey("lockid")->intValue();
			CCLog("loadlock--object tile ~ x:%d_y:%d_width:%d_height:%d;lock_color_kind ===%d lock-id ===%d lock_dir ===%d",x,y,width,height,lock_color_kind,lock_id,lock_dir);
           
		   CCPoint lockInitPos = CCPointMake(x+width/2,y+height/2);
		   Micro_Lock* lock = Micro_Lock::create(_tileMap,lock_id,lock_dir,lock_color_kind,lockInitPos);
		   ccnodelock->addChild(lock);

        }
    }
	
}


void Micro::loadkey(CCTMXTiledMap* _tileMap){
	
	if(NULL == _tileMap)
		return ;

    CCTMXObjectGroup* m_pGroup = _tileMap->objectGroupNamed("jiguankey");
    if(m_pGroup){
        CCArray* m_pObjects = m_pGroup->getObjects();
        CCLog("loadkey--m_pObjects =%d",m_pObjects->count());
        CCDictionary* m_pDict = NULL;
        CCObject* m_pObj = NULL;
        CCARRAY_FOREACH(m_pObjects, m_pObj)
        {
            m_pDict = (CCDictionary*)m_pObj;
            int x = ((CCString*)m_pDict->objectForKey("x"))->intValue();
            int y = ((CCString*)m_pDict->objectForKey("y"))->intValue();
            int width = ((CCString*)m_pDict->objectForKey("width"))->intValue();
            int height = ((CCString*)m_pDict->objectForKey("height"))->intValue();
            
			int key_dir = m_pDict->valueForKey("keydir")->intValue();
            int	key_color_kind = m_pDict->valueForKey("keycolor")->intValue();
            int key_id = m_pDict->valueForKey("keyid")->intValue();
            
			CCLog("loadkey--object tile ~ x:%d_y:%d_width:%d_height:%d;key_color_kind ===%d key_id ====%d key_dir ===%d",x,y,width,height,key_color_kind,key_id,key_dir);
            
		   CCPoint keyInitPos = CCPointMake(x+width/2,y+height/2);
		   Micro_Key* key = Micro_Key::create(_tileMap,key_id,key_dir,key_color_kind,keyInitPos);
            ccnodekey->addChild(key);

        }
    }
	
}

	//加载 阻挡石
void Micro::loadStoneWall(CCTMXTiledMap* _tileMap){
	
	if(NULL == _tileMap)
		return ;

    CCTMXObjectGroup* m_pGroup = _tileMap->objectGroupNamed("bigstone");
    if(m_pGroup){
        CCArray* m_pObjects = m_pGroup->getObjects();
        CCLog("loadStoneWall--m_pObjects =%d",m_pObjects->count());
        CCDictionary* m_pDict = NULL;
        CCObject* m_pObj = NULL;
        CCARRAY_FOREACH(m_pObjects, m_pObj)
        {
            m_pDict = (CCDictionary*)m_pObj;
            int x = ((CCString*)m_pDict->objectForKey("x"))->intValue();
            int y = ((CCString*)m_pDict->objectForKey("y"))->intValue();
            int width = ((CCString*)m_pDict->objectForKey("width"))->intValue();
            int height = ((CCString*)m_pDict->objectForKey("height"))->intValue();
            
            
           CCLog("loadStoneWall--object tile ~ x:%d_y:%d_width:%d_height:%d",x,y,width,height);
           
		   CCPoint stonewallPosInit = CCPointMake(x+width/2,y+height/2);
		   Micro_StoneWall* stonewall = Micro_StoneWall::create(_tileMap,WALL_KIND_ONE,stonewallPosInit);

		   //	ccnode_stonewall->setPosition(ccp(_tileMap->getPositionX(), _tileMap->getPositionY() ));
            ccnode_stonewall->addChild(stonewall);
		   //_tileMap->addChild(stonewall);
            

        }
		  int wallcoutn = ccnode_stonewall->getChildrenCount();
		  CCLog("wall_count =%d",wallcoutn);
    }
}



bool Micro::checkCollStoneWall(CCTMXTiledMap* _tileMap,CCSprite* sprite){

	if(NULL == sprite)
		return false;

	 CCObject *temp;
	 CCArray *stonewall_array =ccnode_stonewall->getChildren();
	//  CCLog("stonewall_array   =%d",stonewall_array->count());
	 if(NULL == stonewall_array||stonewall_array->count() ==0){
        return false;
	 }

		 CCPoint ballpoint = sprite->getPosition();
	 CCARRAY_FOREACH(stonewall_array, temp)
	 {
		 Micro_StoneWall* stonewall =  dynamic_cast<Micro_StoneWall*>(temp);
		if(stonewall->get_disappear()){
			CCLog(" stonewall->removeFromParentAndCleanup(true);");
			 stonewall->removeFromParentAndCleanup(true);
			  return false;
		 }
		 CCArmature* stonewall_ccar =  stonewall->get_ccar();
	
			 CCRect stonewall_rect =  CCRectMake( stonewall_ccar->getPositionX()- ball_speed/2,
				 stonewall_ccar->getPositionY()-ball_speed/2,ball_speed,ball_speed);
		

			// CCLog("stonewall()----ballpoint xx == %.2f yy =%.2f",ballpoint.x,ballpoint.y);
			// CCLog("stonewall leftdown_x == %.2f leftdown_y =%.2f ----- rightupX =%.2f rightupY=%.2f",stonewall_rect.getMinX(),stonewall_rect.getMinY(),
			//	 stonewall_rect.getMaxX(),stonewall_rect.getMaxY());

			 //coll_rect 实际或者相对坐标
			 if( stonewall_rect.containsPoint(ballpoint))
			 {
				 CCLog("ddddddddddddddddddddddddddd");
				 stonewall->changeArmIndex();
				 return true; //
			 }

		 
	 }
	return false;
}



bool Micro::Collision(CCTMXTiledMap* _tileMap,CCSprite* sprite,bool isBall){
	//480 40
    //  CCLog("Collision _tileMap->getPositionX() ＝ %.2f _tileMap->getPositionY() = %.2f",_tileMap->getPositionX(),_tileMap->getPositionY());
    if(NULL == sprite){
        CCLog("collison---SPRITE  IS NULL-------");
        return true;
    }

    CCTMXObjectGroup* m_pGroup = _tileMap->objectGroupNamed("collsion");
    if(m_pGroup){
        CCArray* m_pObjects = m_pGroup->getObjects();
        MyCCLog::verbose("m_pObjects =%d",m_pObjects->count());
        CCDictionary* m_pDict = NULL;
        CCObject* m_pObj = NULL;
        CCARRAY_FOREACH(m_pObjects, m_pObj)
        {
            m_pDict = (CCDictionary*)m_pObj;
            int x = ((CCString*)m_pDict->objectForKey("x"))->intValue();
            int y = ((CCString*)m_pDict->objectForKey("y"))->intValue();
            int width = ((CCString*)m_pDict->objectForKey("width"))->intValue();
            int height = ((CCString*)m_pDict->objectForKey("height"))->intValue();
            
            
            MyCCLog::verbose("object tile ~ x:%d_y:%d_width:%d_height:%d",x,y,width,height);
            
            //    glLineWidth(3);
            //   ccDrawLine( ccp((float)x, (float)y), ccp((float)(x+width), (float)y) );
            //   ccDrawLine( ccp((float)(x+width), (float)y), ccp((float)(x+width), (float)(y+height)) );
            //   ccDrawLine( ccp((float)(x+width), (float)(y+height)), ccp((float)x, (float)(y+height)) );
            //  ccDrawLine( ccp((float)x, (float)(y+height)), ccp((float)x, (float)y) );
            
            
			CCRect coll_rect = CCRectMake(0,0,0,0);
           // CCRect mapRect = CCRectMake(0,0,0,0);
			CCPoint spritePoint = CCPointZero;
			if(isBall){

                int spx = sprite->getPositionX()+sprite->getPositionX()*(mscale-1.0f);
                int spy = sprite->getPositionY()+sprite->getPositionY()*(mscale-1.0f);
               // spritePoint   = CCPointMake(spx,spy);
                spritePoint   = CCPointMake(sprite->getPositionX(),sprite->getPositionY());
                coll_rect = CCRectMake(x,y,width,height);
                
              //  mapRect = CCRectMake(0,0,_tileMap->getContentSize().width,_tileMap->getContentSize().height);
                
			}
			else
			{
				spritePoint   = CCPointMake(_tileMap->getPositionX()+sprite->getPositionX()+sprite->getContentSize().width/2,
                                            _tileMap->getPositionY()+sprite->getPositionY()+sprite->getContentSize().height/2);
                coll_rect = 	CCRectMake(_tileMap->getPositionX()+x,_tileMap->getPositionY()+y,width,height);
                
//                 mapRect = CCRectMake(_tileMap->getPositionX(),_tileMap->getPositionY(),_tileMap->getPositionX()+_tileMap->getContentSize().width,_tileMap->getPositionY()+_tileMap->getContentSize().height);
//                testxuanzePoint = CCPointMake(spritePoint.x,spritePoint.y);
			}
            
			//CCLog("Collision()----spritePoint xx == %.2f yy =%.2f",spritePoint.x,spritePoint.y);
            
			//CCLog("coll_rect leftdown_x == %.2f leftdown_y =%.2f ----- rightupX =%.2f rightupY=%.2f",coll_rect.getMinX(),coll_rect.getMinY(),coll_rect.getMaxX(),coll_rect.getMaxY());
            
            //CCLog("mapRect leftdown_x == %.2f leftdown_y =%.2f ----- rightupX =%.2f rightupY=%.2f",mapRect.getMinX(),mapRect.getMinY(),mapRect.getMaxX(),mapRect.getMaxY());
            
            
			//coll_rect 实际或者相对坐标
            if( coll_rect.containsPoint(spritePoint))
            {
				CCLog("Collison Rect xiangjiao-----");
                CCLog("coll_rect leftdown_x == %.2f leftdown_y =%.2f ----- rightupX =%.2f rightupY=%.2f",coll_rect.getMinX(),coll_rect.getMinY(),coll_rect.getMaxX(),coll_rect.getMaxY());
                return true;
            }
            
           /* if(false == mapRect.containsPoint(spritePoint)){
                
                CCLog("out of screen-----");
                return true;
            }*/
            
        }
    }
    

	return false;
}


void Micro::gameOver(){

    if(cnodeball)//重置时候不需要把该节点从父节点中移除
        cnodeball->removeAllChildrenWithCleanup(true);
    
    CCSprite* gameover_Bg = CCSprite::createWithSpriteFrameName("gameover_bg.png");
    CCScale9Sprite* gameover_Bg_s9 =  CCScale9Sprite::createWithSpriteFrameName("gameover_bg.png");
    
    
    CCSprite* btn_nomarl_restart = CCSprite::createWithSpriteFrameName("btn_click.png");
    CCSprite* btn_selected_restart = CCSprite::createWithSpriteFrameName("btn_click2.png");
    
    
    CCSprite* btn_nomarl_exit = CCSprite::createWithSpriteFrameName("btn_click.png");
    CCSprite* btn_selected_exit = CCSprite::createWithSpriteFrameName("btn_click2.png");
    
    
    PopupLayer* popLayer = PopupLayer::create(gameover_Bg,gameover_Bg_s9);
    popLayer->setContentSize(CCSizeMake(600,420));
    popLayer->setTitle("Game Pass",20,ccRED);
    popLayer->setCallbackFunc(this,callfuncN_selector(Micro::gameover_btnCallback));
    popLayer->addButton(btn_nomarl_restart,btn_selected_restart,"nextlevel",0);
    popLayer->addButton(btn_nomarl_exit,btn_selected_exit,"back",1);
        
    this->addChild(popLayer,100);

	gameState = GAMESTATE_OVER;
}

void Micro::gameover_btnCallback(CCNode* node){
    MyCCLog::verbose("node %d",node->getTag());
    if(node->getTag()==0){
		nextLevel();
    }
    else if(node->getTag()==1)
    {
        
      
		CCScene* pScene = LevelView::scene();
		 CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(2.0f,pScene));
    }
    
}

void Micro::nextLevel(){
    this->removeAllChildrenWithCleanup(false);
   
   

	 if(level<42)
        level++;
    else
        level = 42;
	 CCLog("nextLevelnextLevel ===%d",level);
    
    initGame();
	resetGame(level);
	

}


void Micro::update(float t){
   // CCLog("iclock ===%d gamestate === %d",iclock,gameState);
    if(iclock<=65536)
        iclock++;
    else
        iclock = 0;
    
    if(gameState == GAMESTATE_RUN)
    {
		if(iclock >= 50 && iclock % 60 ==  0 ){
           
           herocreate(_tileMap,"mirco/hero_0.png","hero","ball");
        }
        
        collision_ball(t);
    }
    else if(gameState == GAMESTATE_OVER)
    {
        
    }
    
}

void Micro::resetGame(int level_){
		readdangbaninfo(level_);
			//重置左挡板数字
		char left_dangban_char_new[10] ={0};
			sprintf(left_dangban_char_new,"%d",level_leftpiece);

			CCString* ccstr_leftpiece_new = CCString::createWithFormat("%s",left_dangban_char_new);
			string leftx_new = "X  ";
			const char* leftnew = (leftx_new.append(ccstr_leftpiece_new->m_sString)).c_str();
			CCLabelBMFont*  leftbmf = (CCLabelBMFont*)  bg_left_sprite->getChildByTag(LEFT_DANGBAN_AMOUT_TAG);
			leftbmf->setString(leftnew);

			//重置右挡板数字
			char right_dangban_char_new[10]  ={0};
	sprintf(right_dangban_char_new,"%d",level_rightpiece);

	CCString* ccstr_rightpiece_new = CCString::createWithFormat("%s",right_dangban_char_new);
	string rightx_new = "X  ";
	const char* rightnew = (rightx_new.append(ccstr_rightpiece_new->m_sString)).c_str();
	CCLabelBMFont*  rightbmf = (CCLabelBMFont*)  bg_left_sprite->getChildByTag(RIGHT_DANGBAN_AMOUT_TAG);
	rightbmf->setString(rightnew);

	if(ccnodetool)//重置时候不需要把该节点从父节点中移除
		ccnodetool->removeAllChildrenWithCleanup(true);
	
	if(cnodeball)//重置时候不需要把该节点从父节点中移除
	   cnodeball->removeAllChildrenWithCleanup(true);
    
    //if(ccnode_stonewall)
        //ccnode_stonewall->removeAllChildrenWithCleanup(true);
    
    if(ccnodekey)
        ccnodekey->removeAllChildrenWithCleanup(true);
    
    if(ccnodelock)
        ccnodelock->removeAllChildrenWithCleanup(true);
	
    
    
    
	mapisdragorscale = false;
	show_xuanze = false;
	xuanze_count = 0;
    touch_oldPos = CCPointZero;
    touch_newPos = CCPointZero;
     iclock = 0;
    gameState = GAMESTATE_RUN;
	 xuanze_sprite->setVisible(false);
    
    mscale = 1.0f;
	 _tileMap->setScale(1.0f);
    init_mapx = bg_left_sprite->getPositionX()+ bg_left_sprite->getContentSize().width/2+
    (SCR_SIZE.width- bg_left_sprite->getContentSize().width ) /2  - _tileMap->getContentSize().width/2;
    init_mapy = SCR_SIZE.height/2 - _tileMap->getContentSize().height/2;
    _tileMap->setPosition(ccp(init_mapx,init_mapy));
    
    loadkey(_tileMap);
    loadLock(_tileMap);
    
	CCLog("resetGame----");
}

void Micro::menuRetGameCallback( CCObject* psender )
{
	resetGame(level);
}

void Micro::menuPauseGameCallback( CCObject* psender )
{
	CCLog("fffffffffffffffff");
	if(mscale == 1.0){
        mscale = 2.0f;
	_tileMap->setScale(mscale);
	
	}
	else{
        mscale = 1.0f;
		_tileMap->setScale(mscale);
		
	}

}

void Micro::menuLeftDangBanCallback(CCObject* pSender){

		CCSprite* dangban =(CCSprite*)pSender;
		CCActionInterval * scaleto = CCScaleTo ::create(0.2, 1.3);
		CCActionInterval * scaleto2 = CCScaleTo ::create(0.2, 1.0);
		CCFiniteTimeAction * seq = CCSequence::create(scaleto,scaleto2,NULL);		;
		dangban->runAction(seq);


		if(show_xuanze) 
		{

			if(level_leftpiece > 0){
			
		
				//实际坐标减去地图坐标
				Micro_Tool* tool =  Micro_Tool::create(_tileMap,TOOL_DIR_LEFT, xuanze_sprite->getPosition());

				//	testPoint = chosse_spr->getPosition();

				if( NULL == tool)
				{CCLog("IS NULLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL");}
				else{
					//ccnodetool->setPosition(ccp(_tileMap->getPositionX(), _tileMap->getPositionY() ));
                    
                    if(check_xuanze(tool->getPosition()))
                    {
					ccnodetool->addChild(tool);
                    level_leftpiece --;
                    show_xuanze =false;
                    CCLog("put  ----------------mtool x =%.2f ,y=%.2f ",  tool->getPositionX(),tool->getPositionY());

					xuanze_sprite->setVisible(false);
                    }
				}
			}
			else
				level_leftpiece = 0;

			char left_dangban_char_new[10];
			sprintf(left_dangban_char_new,"%d",level_leftpiece);

			CCString* ccstr_leftpiece_new = CCString::createWithFormat("%s",left_dangban_char_new);
			string leftx_new = "X  ";
			const char* leftnew = (leftx_new.append(ccstr_leftpiece_new->m_sString)).c_str();
			CCLabelBMFont*  leftbmf = (CCLabelBMFont*)  bg_left_sprite->getChildByTag(LEFT_DANGBAN_AMOUT_TAG);
			leftbmf->setString(leftnew);

		}
}

bool Micro::check_xuanze(const CCPoint& pPoint){
    
    //check_tool
    
    int toolindex = 0;
    
    CCObject *tooltemp;
    CCArray *toolArray =ccnodetool ->getChildren();
    if(NULL == toolArray||toolArray->count() ==0){
        toolindex = 1;
    }
    else
    
	CCARRAY_FOREACH(toolArray, tooltemp)
	{
		Micro_Tool* tool =  dynamic_cast<Micro_Tool*>(tooltemp);
		CCRect dangbanRect =  CCRectMake( tool->getPositionX()- tile_one_w/2,tool->getPositionY()-tile_one_h/2,tile_one_w,tile_one_h);
        if(dangbanRect.containsPoint(pPoint))
            toolindex = -1;
		
    }
    
    
    //check_stonewall
    
      int stonewall_index = 0;
    
    
    
        CCObject *temp;
        CCArray *stone_wall_array =ccnode_stonewall ->getChildren();
        
    if(NULL == stone_wall_array||stone_wall_array->count() ==0){
        // CCLog("checkTool ()----toolArray ---null");
        stonewall_index = 1;
        
    }
    else
        
        CCARRAY_FOREACH(stone_wall_array, temp)
    {
        Micro_StoneWall* stonewall =  dynamic_cast<Micro_StoneWall*>(temp);
		if(stonewall->get_disappear()){
            stonewall_index = 1;
        }
        CCArmature* stonewall_ccar =  stonewall->get_ccar();
        
        CCRect stonewall_rect =  CCRectMake( stonewall_ccar->getPositionX()- ball_speed/2,
                                            stonewall_ccar->getPositionY()-ball_speed/2,ball_speed,ball_speed);
        if(stonewall_rect.containsPoint(pPoint))
            stonewall_index = -1;
    }
    
    
    
    
    //check key
    
    int key_index = 0;
    
    CCObject *keytemp;
    CCArray * key_array =ccnodekey ->getChildren();
    
    if(NULL == key_array||key_array->count() ==0){
        // CCLog("checkTool ()----toolArray ---null");
        key_index = 1;
        
    }
    else
        
        CCARRAY_FOREACH(key_array, temp)
        {
		Micro_Key* key =  dynamic_cast<Micro_Key*>(temp);
		CCArmature* ccarm_key = key->get_ccar();
		CCRect keyRect =  CCRectMake( ccarm_key->getPositionX()-tile_one_w/2,ccarm_key->getPositionY()-tile_one_h/2,
                                     tile_one_w,tile_one_h);
        if(keyRect.containsPoint(pPoint))
            key_index = -1;
        }
    
   
    //check lock
    
    int lock_index = 0;
    
    CCObject *locktemp;
    CCArray *lockArray = ccnodelock->getChildren();
	//  CCLog("ccnodetool  toolArray =%d",toolArray->count());
    if(NULL == lockArray||lockArray->count() ==0){
        lock_index = 1;
    }
    else
	CCARRAY_FOREACH(lockArray, locktemp)
	{
		Micro_Lock* lock =  dynamic_cast<Micro_Lock*>(locktemp);
        if(lock->get_lockstate())
            continue;
		CCArmature* ccarm_lock = lock->get_ccarlock();
		CCRect lockRect =  CCRectMake( ccarm_lock->getPositionX(),ccarm_lock->getPositionY()-tile_one_h/2, tile_one_w/2,tile_one_h);
        if(lockRect.containsPoint(pPoint))
            lock_index = -1;
        
    }
    
    CCLog("stonewall_Index ==%d,key_Index ===%d,lock_index ===%d",stonewall_index,key_index,lock_index);
    
    if(toolindex == -1 ||stonewall_index == -1||key_index==-1||lock_index==-1)
        return false;
    
        return true;
    
    
}

void Micro::menuRightDangBanCallback(CCObject* pSender){

	CCSprite* dangban =(CCSprite*)pSender;
	CCActionInterval * scaleto = CCScaleTo ::create(0.2, 1.3);
	CCActionInterval * scaleto2 = CCScaleTo ::create(0.2, 1.0);
	CCFiniteTimeAction * seq = CCSequence::create(scaleto,scaleto2,NULL);		;
	dangban->runAction(seq);

	if(show_xuanze) 
	{

		if(level_rightpiece > 0){
		
		//实际坐标减去地图坐标
		Micro_Tool* tool =  Micro_Tool::create(_tileMap,TOOL_DIR_RIGHT, xuanze_sprite->getPosition());
		//	testPoint = chosse_spr->getPosition();

		if( NULL == tool)
		{CCLog("IS NULLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL");}
		else{
			//ccnodetool->setPosition(ccp(_tileMap->getPositionX(), _tileMap->getPositionY() ));
            
            if(check_xuanze(tool->getPosition()))
            {
			ccnodetool->addChild(tool);
				show_xuanze =false;
				level_rightpiece --;
			xuanze_sprite->setVisible(false);
            }
		}
	}
	else
		level_rightpiece = 0;

	char right_dangban_char_new[10];
	sprintf(right_dangban_char_new,"%d",level_rightpiece);

	CCString* ccstr_rightpiece_new = CCString::createWithFormat("%s",right_dangban_char_new);
	string rightx_new = "X  ";
	const char* rightnew = (rightx_new.append(ccstr_rightpiece_new->m_sString)).c_str();
	CCLabelBMFont*  rightbmf = (CCLabelBMFont*)  bg_left_sprite->getChildByTag(RIGHT_DANGBAN_AMOUT_TAG);
	rightbmf->setString(rightnew);
	}


}


void Micro::draw(){
	ccDrawColor4B(255,0,0,130);
	glLineWidth(3);
    
    //  MyCCLog::verbose("src.width = %f src.height = %f",SCR_SIZE.width,SCR_SIZE.height);
    // ccDrawRect(ccp(40,40),ccp(40+16,40+16));
    
	//ccDrawRect(ccp(SCR_SIZE.width/2,SCR_SIZE.height/2),ccp(SCR_SIZE.width/2+40,SCR_SIZE.height/2+40));
	//ccDrawLine(ccp(_tileMap->getPositionX(),_tileMap->getPositionY()),ccp(_tileMap->getPositionX()+testPoint.x,_tileMap->getPositionY()+testPoint.y));
    
    //ccDrawLine(ccp(_tileMap->getPositionX(),_tileMap->getPositionY()),ccp(testxuanzePoint.x,testxuanzePoint.y));

//	ccDrawLine(ccp(_tileMap->getPositionX(),_tileMap->getPositionY()),ccp(testPoint.x,testPoint.y));

	//ccDrawLine(ccp(_tileMap->getPositionX(),_tileMap->getPositionY()),ccp(testPoint.x,testPoint.y));

   // ccDrawColor4B(255,0,255,130);
    
    
    // ccDrawLine(ccp(_tileMap->getPositionX(),_tileMap->getPositionY()),ccp(testPoint.x+_tileMap->getPositionX(),testPoint.y+_tileMap->getPositionY()));
    
    //  ccDrawLine(ccp(_tileMap->getPositionX(),_tileMap->getPositionY()),ccp(testPoint.x,testPoint.y));

    //	ccDrawRect(ccp(testRect.getMinX(),testRect.getMinY),ccp(testRect.getMaxX(),testRect.getMaxY()));
}




void Micro::setViewpointCenter(CCPoint position)
{
    
    // 求出屏幕的范围包括宽和高
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    //显示屏幕中心点的坐标大于屏幕宽和高的一半
    int x = MAX(position.x, winSize.width / 2);
    int y = MAX(position.y, winSize.height / 2);
    
    //求出的是整个瓦片地图的宽
    //_tileMap->getMapSize().width瓦片地图横向有多少个瓦片
    //_tileMap->getTileSize().width每一个瓦片的宽度
    int mapWidth =_tileMap->getMapSize().width *_tileMap->getTileSize().width;
    
    
    //求出的是整个瓦片地图的高
    //_tileMap->getMapSize().height瓦片地图纵向有多少个瓦片
    //_tileMap->getTileSize().height每一个瓦片的高度
    
    int mapHeight =_tileMap->getMapSize().height *_tileMap->getTileSize().height;
    
    x = MIN(x, mapWidth- winSize.width / 2);
    y = MIN(y, mapHeight - winSize.height / 2);
    
    //目标点
    CCPoint actualPoint = ccp(x, y);
    
    //屏幕的中心点
    CCPoint viewCenterPoint = ccp(winSize.width / 2,winSize.height / 2);
    //计算出重置显示屏幕的中心点
    //ccpSub 返回的是viewCenterPoint.x - actualPoint.x和viewCenterPoint.y - actualPoint.y
    CCPoint viewPoint = ccpSub(viewCenterPoint, actualPoint);
    //重置显示屏幕的中心点
    this->setPosition(viewPoint);
    
}

void Micro::onExit(){

    curl_global_cleanup();
}


//#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
//	CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
//#else
//	CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFrameByName("explosion.plist");
//	CCDirector::sharedDirector()->end();
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//	exit(0);
//#endif

// 获取指定节点坐标的地块索引
//// 方法来自网络
//CCPoint Micro::tilePosFromLocation( const CCPoint& location, CCTMXTiledMap* tileMap )
//{
//	// Tilemap position must be subtracted, in case the tilemap position is scrolling
//	CCPoint pos = ccpSub(location, tileMap->getPosition() );
//	float halfMapWidth = tileMap->getMapSize().width * 0.5f;
//	float mapHeight = tileMap->getMapSize().height;
//	float tileWidth = tileMap->getTileSize().width;
//	float tileHeight = tileMap->getTileSize().height;
//	CCPoint tilePosDiv = CCPointMake(pos.x / tileWidth, pos.y / tileHeight);
//	float inverseTileY = mapHeight - tilePosDiv.y;
//	// Cast to int makes sure that result is in whole numbers
//	float posX = (int)(inverseTileY + tilePosDiv.x - halfMapWidth);
//	float posY = (int)(inverseTileY - tilePosDiv.x + halfMapWidth);
//	// make sure coordinates are within isomap bounds
//	posX = MAX(0, posX);
//	posX = MIN(tileMap->getMapSize().width - 1, posX);
//	posY = MAX(0, posY);
//	posY = MIN(tileMap->getMapSize().height - 1, posY);
//	return CCPointMake(posX, posY);
//}


//test
//   CCTMXObjectGroup* m_pGroup = _tileMap->objectGroupNamed("collsion");
//   if(m_pGroup){
//       CCArray* m_pObjects = m_pGroup->getObjects();
//       MyCCLog::verbose("m_pObjects =%d",m_pObjects->count());
//       CCDictionary* m_pDict = NULL;
//       CCObject* m_pObj = NULL;
//       CCARRAY_FOREACH(m_pObjects, m_pObj)
//       {
//           m_pDict = (CCDictionary*)m_pObj;
//           int x = ((CCString*)m_pDict->objectForKey("x"))->intValue();
//           int y = ((CCString*)m_pDict->objectForKey("y"))->intValue();
//           int width = ((CCString*)m_pDict->objectForKey("width"))->intValue();
//           int height = ((CCString*)m_pDict->objectForKey("height"))->intValue();
//
//
//		 MyCCLog::verbose("object tile ~ x:%d_y:%d_width:%d_height:%d",x,y,width,height);
//
//       //    glLineWidth(3);
//        //   ccDrawLine( ccp((float)x, (float)y), ccp((float)(x+width), (float)y) );
//        //   ccDrawLine( ccp((float)(x+width), (float)y), ccp((float)(x+width), (float)(y+height)) );
//        //   ccDrawLine( ccp((float)(x+width), (float)(y+height)), ccp((float)x, (float)(y+height)) );
//         //  ccDrawLine( ccp((float)x, (float)(y+height)), ccp((float)x, (float)y) );
//
//
//		CCRect coll_rect = CCRectMake(0,0,0,0);
//		CCPoint spritePoint = CCPointZero;
//		for(Micro_ball* m_ball : vector_ball)
//		{
//		//m_ball->ballmove(_tileMap);
//		spritePoint   = CCPointMake(m_ball->getPositionX(),m_ball->getPositionY());
//		coll_rect = 	CCRectMake(x,y,width,height);
//		}
//
//
//	testPoint = CCPointMake(spritePoint.x,spritePoint.y);
//
//		CCLog("spritePoint xx == %.2f yy =%.2f",spritePoint.x,spritePoint.y);

//		CCLog("coll_rect leftdown_x == %.2f leftdown_y =%.2f ----- rightupX =%.2f rightupY=%.2f",coll_rect.getMinX(),coll_rect.getMinY(),coll_rect.getMaxX(),coll_rect.getMaxY());
//
//		//testRect = coll_rect;
//	}
//
//}
