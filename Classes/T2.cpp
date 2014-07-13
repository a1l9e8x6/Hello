#include "T2.h"
#include "MyCCLog.h"
#include "MZDataManager.h"
#include "PopupLayer.h"


int arr_Num [4][4]= {
	{-1,-1,-1,-1},
	{-1,-1,-1,-1},
	{-1,-1,-1,-1},
	{-1,-1,-1,-1}
};//‚àÜ√Ç‚âà√É¬†Àù‚óä√?

T2::T2():move_state(MOVE_NONE),isMoving(0),game_state(GAME_RUN){
	dir_guester = DIR_NONE;

	isUpOver = false;
	isRightOver = false;
	isDownOver =false;
	isLeftOver = false;

	current_score = max_score = 0;
	cnode = CCNode::create();

}

T2::~T2(){
	CC_SAFE_RELEASE(node_array);
}

CCScene* T2::scene(){
	// 'scene' is an autorelease object
	CCScene *scene = CCScene::create();

	// 'layer' is an autorelease object
	T2 *layer = T2::create();

	// add		 layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;

}

bool T2::init(){
	if(!CCLayer::init())
		return false;


	setTouchEnabled(true);


	node_array = CCArray::create();
	node_array->retain();

	initChess(0,15);

	setupView();

	return true;
}



void T2::setupView(){
	CCLog("%s","setupView");

//	CCSprite* bgspr = CCSprite::create("HelloWorld.png");
//	bgspr->setPosition(ccp(SCR_SIZE.width/2,SCR_SIZE.height/2));
//	this->addChild(bgspr,-1);
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    
	CCLog("visibleSize width ==%.2f height ==%.2f",visibleSize.width,visibleSize.height);
    
    
    AudioManager::instance()->playGroundMusic(kAudio_jingyiwanzhuan);
    AudioManager::instance()->updateGroundMusicVolume(0.8f);
    
    MZDataManager::sharedDataManager()->initXMLString("mirco/strings.xml");
    CCString* currentScore = MZDataManager::sharedDataManager()->getStringfromXml("currentScore");
    const char* cus = currentScore->m_sString.c_str();

    
	CCLabelBMFont* cclm_b1 = CCLabelBMFont::create(cus,"2048/menu.fnt");
	cclm_b1->setPosition(ccp(SCR_SIZE.width/2-150,SCR_SIZE.height-cclm_b1->getContentSize().height-10));
	this->addChild(cclm_b1);

 
    

	char cur[16];
	sprintf(cur,"%d",current_score);
	cclbmf_score = CCLabelBMFont::create(cur,"2048/menu.fnt");
	cclbmf_score->setPosition(ccp(SCR_SIZE.width/2-50,SCR_SIZE.height-cclbmf_score->getContentSize().height-10));
	this->addChild(cclbmf_score);

    
    CCString* maxScore = MZDataManager::sharedDataManager()->getStringfromXml("maxScore");
    const char* mas = maxScore->m_sString.c_str();

	CCLabelBMFont* cclm_b2 = CCLabelBMFont::create(mas,"2048/menu.fnt");
	cclm_b2->setPosition(ccp(SCR_SIZE.width/2+80,SCR_SIZE.height-cclm_b2->getContentSize().height-10));
	this->addChild(cclm_b2);
    
//    CCLabelTTF* ttf = CCLabelTTF::create("沙沙快来膜拜一下~", "HelveticaNeue", 36.0f);
//    ttf->setPosition(ccpAdd(ccp(50, -150),  cclm_b1->getPosition()));
//    this->addChild(ttf);



	
	int max_score = 0;
	if(!CCUserDefault::sharedUserDefault()->getBoolForKey("ISEXIST")){
		CCUserDefault::sharedUserDefault()->setBoolForKey("ISEXIST",true);
		CCUserDefault::sharedUserDefault()->setIntegerForKey("MAXSCORE",0);
	MyCCLog::verbose("is not esixts ============= ");
	}
	else{
	max_score = CCUserDefault::sharedUserDefault()->getIntegerForKey("MAXSCORE");
	MyCCLog::verbose("is exitsxt  -----max_socre ============= %d",max_score);
	}
	
	char  ss[16];
	sprintf(ss,"%d",max_score);
		cclbmf_max_score = CCLabelBMFont::create(ss,"2048/menu.fnt");
	cclbmf_max_score->setPosition(ccp(SCR_SIZE.width/2+200,SCR_SIZE.height-cclbmf_score->getContentSize().height-10));
	this->addChild(cclbmf_max_score);



	this->scheduleUpdate();
}


void T2::checkMove(int dir_guester)
{
	if(dir_guester== DIR_RIGHT){
		//≈ì√ö‚Äù‚Ä?
		if (move_state == MOVE_YD1) //¬µ‚ÅÑ‚Äú¬™‚â§Œ©‚Äú‚àÜ‚àÇ√?
		{
			for (int i=0;i<MAX_ROW;i++)
			{
				for (int j = MAX_COL-2; j >=0 ; --j)
				{
					int index  = -1;
					int count = 0;

					if(arr_Num[i][j]>-1&&arr_Num[i][j+1]==-1)//¬ª√ÅœÄÀö‚Äù‚Äú¬±Ô¨Ç¬†¬´√∏‚Äô¬µ∆?
					{
						bemoved_yd1 = true;

						index = j+1;
						while (arr_Num[i][index]==-1)
						{
							count++;
							if(index<MAX_COL-1)
								index++;
							else
								break;
						}
						nodeMove(dir_guester,i,j,count);
						change2number(arr_Num[i][j],arr_Num[i][j+count]);

					}
				}

			}

			logcat_chessArr("move-yd1");
			if(bemoved_yd1){
				CCLog("can move yd1");
				move_state = MOVE_NONE;
			}else{
				CCLog("can not move yd1");
				move_state = MOVE_HETI;
			}
		}
		else if(move_state == MOVE_HETI)//¬µ‚Å?‚â§Œ©‚à´≈ì‚â•‚Ä?
		{
			CCLog("rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr");

			update_moveyd1();

			for (int i=0;i<MAX_ROW;i++)
			{
				for (int j=MAX_COL-1;j>0;j--)
				{
					if(arr_Num[i][j]>=0&&arr_Num[i][j]==arr_Num[i][j-1]&&arr_Num[i][j]==arr_Num[i][j-2]&&arr_Num[i][j]==arr_Num[i][j-3])
					{
						CCLog("ppppppppppppppp");
						becomposed = true;

					

						arr_Num[i][j]++;
						arr_Num[i][j-1]= -1;

						addScore(arr_Num[i][j]);

						change(dir_guester,i,j,arr_Num[i][j]);
						removeChessnode(dir_guester,i,j-1);

						MyCCLog::verbose("score---");

						break;
					}

					else if(arr_Num[i][j]>=0&&arr_Num[i][j]==arr_Num[i][j-1])
					{
						CCLog("qqqqqqqqqqqqqq");
						becomposed = true;
					

						arr_Num[i][j]++;
						arr_Num[i][j-1]= -1;

						addScore(arr_Num[i][j]);

						change(dir_guester,i,j,arr_Num[i][j]);
						removeChessnode(dir_guester,i,j-1);

					}
				}
			}
			CCLog("tttttttttttttttttttttttttttttttttttttttttttttttttttttttt");
			logcat_chessArr("right-move-heti1");

			if(bemoved_yd1&&!becomposed)
			{
				CCLog("can move and can not heti");
				move_state = MOVE_OVER;
				return;
			}

			move_state = MOVE_NONE;

		}
		else if(move_state == MOVE_YD2)
		{
			CCLog("MOVE_YD2MOVE_YD2");
			for (int i=0;i<MAX_ROW;i++)
			{
				for (int j = MAX_COL-2; j >=0 ; --j)
				{
					int index  = -1;
					int count = 0;
					if(arr_Num[i][j]>-1&&arr_Num[i][j+1]==-1)//¬ª√ÅœÄÀö‚Äù‚Äú¬±Ô¨Ç¬†¬´√∏‚Äô¬µ∆?
					{
						index = j+1;
						while (arr_Num[i][index]==-1)
						{
							count++;
							if(index<MAX_COL-1)
								index++;
							else
								break;
						}
						nodeMove(dir_guester,i,j,count);
						change2number(arr_Num[i][j],arr_Num[i][j+count]);

					}
				}
			}

			logcat_chessArr("move-yd2");
			move_state = MOVE_OVER;

		}

		else if(move_state == MOVE_OVER)
		{
			CCLog("right-----------move-over---over");
			addNewChess();
			move_state = MOVE_NONE;
			if(checkFull())
			{
				bool rightover = checkGameOver();
				CCLog("rightover = %d",rightover);
			}


		}

	}


	else if(dir_guester== DIR_LEFT){
		//≈ì√ö‚Äù‚Ä?

		if (move_state == MOVE_YD1) //¬µ‚ÅÑ‚Äú¬™‚â§Œ©‚Äú‚àÜ‚àÇ√?
		{
			for (int i=0;i<MAX_ROW;i++)
			{
				for (int j =1; j < MAX_COL ; j++)
				{
					int index  = -1;
					int count = 0;

					if(arr_Num[i][j]>-1&&arr_Num[i][j-1]==-1)//¬ª√ÅœÄÀö‚Äù‚Äú¬±Ô¨Ç¬†¬´√∏‚Äô¬µ∆?
					{
						bemoved_yd1 = true;

						index = j-1;
						while (arr_Num[i][index]==-1)
						{
							count++;
							if(index>0)
								index--;
							else
								break;
						}
						nodeMove(dir_guester,i,j,count);
						change2number(arr_Num[i][j],arr_Num[i][j-count]);

					}
				}

			}

			logcat_chessArr("left-move-yd1");
			if(bemoved_yd1){
				CCLog("can move left yd1");
				move_state = MOVE_NONE;
			}else{
				CCLog("can not move left yd1");
				move_state = MOVE_HETI;
			}
		}
		else if(move_state == MOVE_HETI)//¬µ‚Å?‚â§Œ©‚à´≈ì‚â•‚Ä?
		{

			update_moveyd1();

			for (int i=0;i<MAX_ROW;i++)
			{
				for (int j=0;j<MAX_COL-1;j++)
				{

					if(arr_Num[i][j]>=0&&arr_Num[i][j]==arr_Num[i][j+1]&&arr_Num[i][j]==arr_Num[i][j+2]&&arr_Num[i][j]==arr_Num[i][j+3])
					{
						CCLog("wdddddddsssssss");
						becomposed = true;
						arr_Num[i][j]++;
						arr_Num[i][j+1]= -1;
							addScore(arr_Num[i][j]);

						change(dir_guester,i,j,arr_Num[i][j]);
						removeChessnode(dir_guester,i,j+1);
						break;
					}
					if(arr_Num[i][j]>=0&&arr_Num[i][j]==arr_Num[i][j+1])
					{
						CCLog("ssssdwww");
						becomposed = true;
						arr_Num[i][j]++;
						arr_Num[i][j+1]= -1;

							addScore(arr_Num[i][j]);
						change(dir_guester,i,j,arr_Num[i][j]);
						removeChessnode(dir_guester,i,j+1);

					}
				}
			}
			CCLog("tttttttttttttttttttttttttttttttttttttttttttttttttttttttt");
			logcat_chessArr("left-move-heti1");

			if(bemoved_yd1&&!becomposed)
			{
				CCLog("can move and can not heti");
				move_state = MOVE_OVER;
				return;
			}
			move_state = MOVE_NONE;

		}
		else if(move_state == MOVE_YD2)
		{
			CCLog("MOVE_YD2MOVE_YD2");
			for (int i=0;i<MAX_ROW;i++)
			{
				for (int j =1; j < MAX_COL ; j++)
				{
					int index  = -1;
					int count = 0;

					if(arr_Num[i][j]>-1&&arr_Num[i][j-1]==-1)//¬ª√ÅœÄÀö‚Äù‚Äú¬±Ô¨Ç¬†¬´√∏‚Äô¬µ∆?
					{
						bemoved_yd1 = true;

						index = j-1;
						while (arr_Num[i][index]==-1)
						{
							count++;
							if(index>0)
								index--;
							else
								break;
						}
						nodeMove(dir_guester,i,j,count);
						change2number(arr_Num[i][j],arr_Num[i][j-count]);

					}
				}

			}
			logcat_chessArr("move-yd2");
			move_state = MOVE_OVER;

		}

		else if(move_state == MOVE_OVER)
		{
			CCLog("left-----------move-over---over");
			addNewChess();
			if(checkFull())
			{
				bool leftover = checkGameOver();
				CCLog("leftover = %d",leftover);
			}
			move_state = MOVE_NONE;
		}

	}

	else if(dir_guester == DIR_UP){	//≈ì√ö‚Ä¶≈?

		if (move_state == MOVE_YD1) //¬µ‚ÅÑ‚Äú¬™‚â§Œ©‚Äú‚àÜ‚àÇ√?
		{
			for (int j =0; j < MAX_COL ; j++)//lie
			{
				for (int i= MAX_ROW-2;i>=0;i--)//hang
				{
					int index  = -1;
					int count = 0;
					if(arr_Num[i][j]>-1&&arr_Num[i+1][j]==-1)//¬ª√ÅœÄÀö‚Äù‚Äú¬±Ô¨Ç¬†¬´√∏‚Äô¬µ∆?
					{
						bemoved_yd1 = true;

						index = i+1;
						while (arr_Num[index][j]==-1)
						{
							count++;
							if(index<MAX_ROW-1)
								index++;
							else
								break;
						}
						CCLog("j = %d i= %d COUNT = %d",j,i,count);
						nodeMove(dir_guester,j,i,count);
						change2number(arr_Num[i][j],arr_Num[i+count][j]);

					}
				}

			}

			logcat_chessArr("up-move-yd1");
			if(bemoved_yd1){
				CCLog("up -can move yd1");
				move_state = MOVE_NONE;
			}else{
				CCLog("up-can not move yd1");
				move_state = MOVE_HETI;
			}
		}
		else if(move_state == MOVE_HETI)//¬µ‚Å?‚â§Œ©‚à´≈ì‚â•‚Ä?
		{
			CCLog("mmmmmmmmmmmmmmmmmm");

			update_moveyd1();
			for (int j=0;j<MAX_COL;j++)
			{
				for (int i=MAX_ROW-1;i>0;i--)
				{
					if(arr_Num[i][j]>=0&&arr_Num[i][j]==arr_Num[i-1][j]&&arr_Num[i][j]==arr_Num[i-2][j]&&arr_Num[i][j]==arr_Num[i-3][j])
					{
						CCLog("ppppppppppppppp");
						becomposed = true;
						arr_Num[i][j]++;
						arr_Num[i-1][j]= -1;
							addScore(arr_Num[i][j]);
						change(dir_guester,j,i,arr_Num[i][j]);
						removeChessnode(dir_guester,j,i-1);

						break;
					}

					else if(arr_Num[i][j]>=0&&arr_Num[i][j]==arr_Num[i-1][j])
					{
						CCLog("qqqqqqqqqqqqqq");
						becomposed = true;
						arr_Num[i][j]++;
						arr_Num[i-1][j]= -1;
							addScore(arr_Num[i][j]);
						change(dir_guester,j,i,arr_Num[i][j]);
						removeChessnode(dir_guester,j,i-1);



					}
				}
			}
			CCLog("vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv");
			logcat_chessArr("up------move-heti1");

			if(bemoved_yd1&&!becomposed)
			{
				CCLog("can move and can not heti");
				move_state = MOVE_OVER;
				return;
			}

			move_state = MOVE_NONE;

		}
		else if(move_state == MOVE_YD2)
		{
			for (int j =0; j < MAX_COL ; j++)//lie
			{
				for (int i= MAX_ROW-2;i>=0;i--)//hang
				{
					int index  = -1;
					int count = 0;
					if(arr_Num[i][j]>-1&&arr_Num[i+1][j]==-1)//¬ª√ÅœÄÀö‚Äù‚Äú¬±Ô¨Ç¬†¬´√∏‚Äô¬µ∆?
					{
						bemoved_yd1 = true;

						index = i+1;
						while (arr_Num[index][j]==-1)
						{
							count++;
							if(index<MAX_ROW-1)
								index++;
							else
								break;
						}
						CCLog("j = %d i= %d COUNT = %d",j,i,count);
						nodeMove(dir_guester,j,i,count);
						change2number(arr_Num[i][j],arr_Num[i+count][j]);

					}
				}

			}

			logcat_chessArr("move-yd2");
			move_state = MOVE_OVER;
		}

		else if(move_state == MOVE_OVER)
		{
			CCLog("up-----------move-over---over");
			addNewChess();
			if(checkFull())
			{
					bool upover = checkGameOver();
				CCLog("upover = %d",upover);
			}
			move_state = MOVE_NONE;
		}


	}

	else if(dir_guester == DIR_DOWN){
		if (move_state == MOVE_YD1) //¬µ‚ÅÑ‚Äú¬™‚â§Œ©‚Äú‚àÜ‚àÇ√?
		{
			for (int j =0; j < MAX_COL ; j++)//lie
			{
				for (int i= 1;i< MAX_ROW;i++)//hang
				{
					int index  = -1;
					int count = 0;
					if(arr_Num[i][j]>-1&&arr_Num[i-1][j]==-1)//¬ª√ÅœÄÀö‚Äù‚Äú¬±Ô¨Ç¬†¬´√∏‚Äô¬µ∆?
					{
						bemoved_yd1 = true;

						index = i-1;
						while (arr_Num[index][j]==-1)
						{
							count++;
							if(index>0)
								index--;
							else
								break;
						}
						CCLog("j = %d i= %d COUNT = %d",j,i,count);
						nodeMove(dir_guester,j,i,count);
						change2number(arr_Num[i][j],arr_Num[i-count][j]);

					}
				}

			}

			logcat_chessArr("down-move-yd1");
			if(bemoved_yd1){
				CCLog("down -can move yd1");
				move_state = MOVE_NONE;
			}else{
				CCLog("down-can not move yd1");
				move_state = MOVE_HETI;
			}
		}
		else if(move_state == MOVE_HETI)//¬µ‚Å?‚â§Œ©‚à´≈ì‚â•‚Ä?
		{
			CCLog("mmmmmmmmmmmmmmmmmm");

			update_moveyd1();
			for (int j=0;j<MAX_COL;j++)
			{
				for (int i=0;i<MAX_ROW-1;i++)
				{

					if(arr_Num[i][j]>=0&&arr_Num[i][j]==arr_Num[i+1][j]&&arr_Num[i][j]==arr_Num[i+2][j]&&arr_Num[i][j]==arr_Num[i+3][j])
					{
						CCLog("ppppppppppppppp");
						becomposed = true;
						arr_Num[i][j]++;
						arr_Num[i+1][j]= -1;
							addScore(arr_Num[i][j]);
						change(dir_guester,j,i,arr_Num[i][j]);
						removeChessnode(dir_guester,j,i+1);

						break;
					}

					else if(arr_Num[i][j]>=0&&arr_Num[i][j]==arr_Num[i+1][j])
					{
						CCLog("qqqqqqqqqqqqqq");
						becomposed = true;
						arr_Num[i][j]++;
						arr_Num[i+1][j]= -1;
							addScore(arr_Num[i][j]);
						change(dir_guester,j,i,arr_Num[i][j]);
						removeChessnode(dir_guester,j,i+1);



					}
				}
			}
			CCLog("vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv");
			logcat_chessArr("down------move-heti1");

			if(bemoved_yd1&&!becomposed)
			{
				CCLog("can move and can not heti");
				move_state = MOVE_OVER;
				return;
			}

			move_state = MOVE_NONE;

		}
		else if(move_state == MOVE_YD2)
		{
			for (int j =0; j < MAX_COL ; j++)//lie
			{
				for (int i= 1;i< MAX_ROW;i++)//hang
				{
					int index  = -1;
					int count = 0;
					if(arr_Num[i][j]>-1&&arr_Num[i-1][j]==-1)//¬ª√ÅœÄÀö‚Äù‚Äú¬±Ô¨Ç¬†¬´√∏‚Äô¬µ∆?
					{
						bemoved_yd1 = true;

						index = i-1;
						while (arr_Num[index][j]==-1)
						{
							count++;
							if(index>0)
								index--;
							else
								break;
						}
						CCLog("j = %d i= %d COUNT = %d",j,i,count);
						nodeMove(dir_guester,j,i,count);
						change2number(arr_Num[i][j],arr_Num[i-count][j]);

					}
				}

			}
			logcat_chessArr("move-yd2");
			move_state = MOVE_OVER;
		}

		else if(move_state == MOVE_OVER)
		{
			CCLog("down-----------move-over---over");
			addNewChess();
			if(checkFull())
			{
				bool downover = checkGameOver();
				CCLog("downover = %d",downover);
			}
			move_state = MOVE_NONE;
		}

	}

}

bool T2::checkGameOver()
{

	isRightOver = isLeftOver  = false;
	 isUpOver = isDownOver  =false;
	 over_check_move_right = over_check_heti_right =false;
	 over_check_move_left = over_check_heti_left = false;
	 over_check_move_up = over_check_heti_up= false;
	 over_check_move_down = 	 over_check_heti_down = false;

	//check right
	for (int i=0;i<MAX_ROW;i++)
	{
		for (int j = MAX_COL-2; j >=0 ; --j)
		{
			if(arr_Num[i][j]>-1&&arr_Num[i][j+1]==-1)//¬ª√ÅœÄÀö‚Äù‚Äú¬±Ô¨Ç¬†¬´√∏‚Äô¬µ∆?
				over_check_move_right = true;
		}
	}

	for (int i=0;i<MAX_ROW;i++)
	{
		for (int j=MAX_COL-1;j>0;j--)
		{
			if(arr_Num[i][j]>=0&&arr_Num[i][j]==arr_Num[i][j-1])
				over_check_heti_right = true;

		}
	}
	if(!over_check_move_right&&!over_check_heti_right)
	{
		isRightOver = true;
	}



	//check -left
	for (int i=0;i<MAX_ROW;i++)
	{
		for (int j =1; j < MAX_COL ; j++)
		{
			if(arr_Num[i][j]>-1&&arr_Num[i][j-1]==-1)//¬ª√ÅœÄÀö‚Äù‚Äú¬±Ô¨Ç¬†¬´√∏‚Äô¬µ∆?
				over_check_move_left = true;
		}
	}

	for (int i=0;i<MAX_ROW;i++)
	{
		for (int j=0;j<MAX_COL-1;j++)
		{
			if(arr_Num[i][j]>=0&&arr_Num[i][j]==arr_Num[i][j+1])
				over_check_heti_left = true;
		}
	}

	if(!over_check_move_left&&!over_check_heti_left)
		isLeftOver = true;



	//check -up
	for (int j =0; j < MAX_COL ; j++)//lie
	{
		for (int i= MAX_ROW-2;i>=0;i--)//hang
		{
			if(arr_Num[i][j]>-1&&arr_Num[i+1][j]==-1)//¬ª√ÅœÄÀö‚Äù‚Äú¬±Ô¨Ç¬†¬´√∏‚Äô¬µ∆?
			{
				over_check_move_up = true;
			}
		}
	}


	for (int j=0;j<MAX_COL;j++)
	{
		for (int i=MAX_ROW-1;i>0;i--)
		{
			if(arr_Num[i][j]>=0&&arr_Num[i][j]==arr_Num[i-1][j])
			{
				over_check_heti_up = true;
			}
		}
	}

	if(!over_check_move_up&&!over_check_heti_up)
		isUpOver = true;

	//check-down
	for (int j =0; j < MAX_COL ; j++)//lie
	{
		for (int i= 1;i< MAX_ROW;i++)//hang
		{
			if(arr_Num[i][j]>-1&&arr_Num[i-1][j]==-1)//¬ª√ÅœÄÀö‚Äù‚Äú¬±Ô¨Ç¬†¬´√∏‚Äô¬µ∆?
			{
				over_check_move_down = true;
			}
		}

	}


	for (int j=0;j<MAX_COL;j++)
	{
		for (int i=0;i<MAX_ROW-1;i++)
		{
			if(arr_Num[i][j]>=0&&arr_Num[i][j]==arr_Num[i+1][j])
			{
				over_check_heti_down = true;
			}
		}
	}
	if(!over_check_move_down&&!over_check_heti_down)
		isDownOver = true;


	MyCCLog::verbose("isUpOver = %d isDownOver = %d isLeftOver = %d isRightOver= %d",isUpOver,isDownOver,isLeftOver,isRightOver);

	if(isUpOver&&isDownOver&&isLeftOver&&isRightOver){
		CCLog("game----over------over");
		game_state = GAME_OVER;
        
		PopupLayer* popLayer = PopupLayer::create("2048/gameover_bg.png");
        popLayer->setContentSize(CCSizeMake(400,400));
        popLayer->setTitle("Game Over",20);
        popLayer->setCallbackFunc(this,callfuncN_selector(T2::gameover_btnCallback));
        popLayer->addButton("2048/restart.png", "2048/restart.png", "",0);
        popLayer->addButton("2048/exit.png", "2048/exit.png", "",1);
        
        this->addChild(popLayer);
        
		return true;
	}

	return false;
}


bool T2::checkFull()
{
	bool full = true;
	for (int i = 0; i < MAX_ROW*MAX_COL; i++)
	{
		if(arr_Num[i/4][i%4] == -1)
		{
			full = false;
			return false;
		}
	}

	if(full)
		return true;

	return false;
}


void T2::change(int dir,int row,int col,int value){

	if(dir == DIR_RIGHT||dir==DIR_LEFT){
		CCObject *temp;  
		//¬™√í¬µ‚àörootnode‚àèÀò≈ì¬¨¬µ∆íŒ©‚ÅÑ¬µ‚Ä? 
		CCArray *nodeArray =cnode ->getChildren(); 
		CCARRAY_FOREACH(nodeArray, temp)  
		{  		
			ChessNode* cesnode =  dynamic_cast<ChessNode*>(temp);
			CCLog("cesnode : row = %d cesnode: col = %d cesnode : value = % d",cesnode->getRow(),cesnode->getCol(),cesnode->getValue());
			if( cesnode->getCol()==col&&cesnode->getRow() == row)
			{
				cesnode->removeFromParent();
				ChessNode* ces = ChessNode::create(row, col, value,false);

				ces->setScale(1.5f);
				CCActionInterval * scaleto = CCScaleTo ::create(0.2f, 1.0f);

				CCCallFuncN * funcall= CCCallFuncN::create(this, callfuncN_selector(T2::callback_hecheng));

				CCFiniteTimeAction * seq2 = CCSequence::create(scaleto,funcall,NULL);
				ces->runAction(seq2);
				cnode->addChild(ces);

			}
		}
	}

	else if(dir == DIR_UP||dir==DIR_DOWN){
		CCObject *temp;  
		//¬™√í¬µ‚àörootnode‚àèÀò≈ì¬¨¬µ∆íŒ©‚ÅÑ¬µ‚Ä? 
		CCArray *nodeArray =cnode ->getChildren(); 
		CCARRAY_FOREACH(nodeArray, temp)  
		{  		
			ChessNode* cesnode =  dynamic_cast<ChessNode*>(temp);
			CCLog("cesnode : row = %d cesnode: col = %d cesnode : value = % d",cesnode->getRow(),cesnode->getCol(),cesnode->getValue());
			if( cesnode->getCol()==row&&cesnode->getRow() == col)
			{
				cesnode->removeFromParent();
				ChessNode* ces = ChessNode::create(col, row, value,false);

				ces->setScale(1.5f);
				CCActionInterval * scaleto = CCScaleTo ::create(0.2f, 1.0f);

				CCCallFuncN * funcall= CCCallFuncN::create(this, callfuncN_selector(T2::callback_hecheng));

				CCFiniteTimeAction * seq2 = CCSequence::create(scaleto,funcall,NULL);
				ces->runAction(seq2);
				cnode->addChild(ces);

			}
		}
	}

}

void T2::removeChessnode(int dir,int row,int col){
	if(dir == DIR_RIGHT||dir == DIR_LEFT){
		CCObject *temp;  
		//¬™√í¬µ‚àörootnode‚àèÀò≈ì¬¨¬µ∆íŒ©‚ÅÑ¬µ‚Ä? 
		CCArray *nodeArray =cnode ->getChildren(); 

		CCLog("before-childcount = %d",cnode->getChildrenCount());

		CCARRAY_FOREACH(nodeArray, temp)  
		{  		
			ChessNode* cesnode =  dynamic_cast<ChessNode*>(temp);
			if( cesnode->getCol()==col&&cesnode->getRow() == row)
			{
				cesnode->removeFromParent();
				break;
			}

		}
		CCLog("after-childcount = %d",cnode->getChildrenCount());
	}

	else if(dir == DIR_UP||dir == DIR_DOWN)
	{
		CCObject *temp;  
		//¬™√í¬µ‚àörootnode‚àèÀò≈ì¬¨¬µ∆íŒ©‚ÅÑ¬µ‚Ä? 
		CCArray *nodeArray =cnode ->getChildren(); 

		CCLog("before-childcount = %d",cnode->getChildrenCount());

		CCARRAY_FOREACH(nodeArray, temp)  
		{  		
			ChessNode* cesnode =  dynamic_cast<ChessNode*>(temp);
			if( cesnode->getCol()==row&&cesnode->getRow() == col)
			{
				cesnode->removeFromParent();
				break;
			}

		}
		CCLog("after-childcount = %d",cnode->getChildrenCount());

	}

}

void T2::change2number(int& a,int& b)
{
	int temp = a;
	a = b;
	b = temp;
}

void T2::update(float t){
	//checkMove1(dir_guester);
	if(game_state == GAME_RUN)
		checkMove(dir_guester);
	else if(game_state == GAME_OVER)
	{
      //  MyCCLog::verbose("sssss");
        
              
        
        
        
		//setTouchEnabled(false);
//	GAME_OVER_LAYER = CCLayerColor::create();
//	GAME_OVER_LAYER->setColor(ccBLUE);
//	GAME_OVER_LAYER->setOpacity(180);
//	GAME_OVER_LAYER->setVisible(true);
//	int init_chess_x = (SCR_SIZE.width-110*4)/2;
//	int init_chess_y = (SCR_SIZE.height-110*4)/2;
//	GAME_OVER_LAYER->setPosition(ccp(init_chess_x,init_chess_y));
//	GAME_OVER_LAYER->setContentSize(CCSizeMake(440,440));
//
//	CCLabelBMFont* startLabel = CCLabelBMFont::create(MZDataManager::sharedDataManager()->getLanguageString("str49").c_str(),"2048/menu.fnt");
//CCLabelBMFont* startLabel1 = CCLabelBMFont::create(MZDataManager::sharedDataManager()->getLanguageString("str50").c_str(),"2048/menu.fnt");
//
//	startLabel->setTag(10);
//	startLabel->setPosition(ccp(SCR_SIZE.width/2,SCR_SIZE.height/2));
//
//	startLabel1->setTag(11);
//	startLabel1->setPosition(ccp(SCR_SIZE.width/2,SCR_SIZE.height/2-100));
//
//	CCMenuItemLabel* menulael = CCMenuItemLabel::create(startLabel,this,menu_selector(T2::menuGameOverCallback));
//	CCMenuItemLabel* menulael1 = CCMenuItemLabel::create(startLabel1,this,menu_selector(T2::menuGameOverCallback));
//	CCMenu* menu = CCMenu::create(menulael,menulael1,NULL);
//	menu->setPosition(CCPointZero);
//	menu->setTouchPriority(-128);
//	
//
//	this->addChild(GAME_OVER_LAYER,10);
//	this->addChild(menu,11);
	//return;
	}


	//	if(dir_guester!=DIR_NONE)
	//		dir_guester = DIR_NONE;
}

void T2::ccTouchesBegan(CCSet* touches,CCEvent* event){
	CCLog("%s","ccTouchBegan");

	if(!touches||!touches->count()){	
		return ;
	}
	cocos2d::CCTouch* pTouch = (cocos2d::CCTouch*)touches->anyObject();

	//¬™√í¬ª¬∞¬µ¬±¬´‚àû‚Äù‚àö¬™√ü¬•‚Ä¢‚àÜ¬°¬µ‚Äû‚óä¬Ø¬±√ç‚â§¬¢Œ©¬?d‚óä¬Ø¬±√ç‚Äù‚â•‚Ä¶‚Ä?d‚óä¬Ø¬±√ç≈ì¬?
	//CCPoint touchPoint = touch->getLocationInView();
	//touchPoint = CCDirector::sharedDirector()->convertToGL( touchPoint );
	//oldx = touchPoint.x;
	//oldy = touchPoint.y;
	//CCLog("ccTouchBegan || oldx %f oldy %f ",oldx,oldy);
}

void T2::ccTouchesMoved(CCSet* touch,CCEvent* event){
}


void T2::ccTouchesEnded(CCSet* touches,CCEvent* event){


	if(!touches||!touches->count()){	
		return;
	}

	cocos2d::CCTouch* pTouch = (cocos2d::CCTouch*)touches->anyObject();
	double oldx = pTouch->getStartLocation().x;
	double oldy = pTouch->getStartLocation().y;

	double newx = pTouch->getLocation().x;
	double newy = pTouch->getLocation().y;

	//CCPoint touchPoint = touches->getLocationInView();
	//touchPoint = CCDirector::sharedDirector()->convertToGL( touchPoint );
	//newx = touchPoint.x;
	//newy = touchPoint.y;

	double absX = fabs(oldx-newx);
	double absY = fabs(oldy-newy);
	CCLog("newx %f newy %f absX %f absY %f",newx,newy,absX,absY);

	if(newy-oldy>0&&absX<absY){
		if(isMoving)
			return;
		dir_guester = DIR_UP;//up
		move_state = MOVE_YD1;
		bemoved_yd1 = false;
		becomposed = false;
		CCLog("up-------------");
	}
	else if(oldy-newy>0&&absX<absY){
		if(isMoving)
			return;
		dir_guester = DIR_DOWN;//down
		move_state = MOVE_YD1;
		CCLog("down-------------");
		bemoved_yd1 = false;
		becomposed = false;
	}

	if(newx-oldx<0&&absX>absY){
		if(isMoving)
			return;
		dir_guester = DIR_LEFT;//left
		CCLog("left-------------");
		move_state = MOVE_YD1;
		bemoved_yd1 = false;
		becomposed = false;
	}
	else	if(newx-oldx>0&&absX>absY){
		if(isMoving)
			return;
		dir_guester = DIR_RIGHT;//right
		CCLog("right-------------");
		move_state = MOVE_YD1;

		bemoved_yd1 = false;
		becomposed = false;
	}

	CCLog("dir = %d",dir_guester);

	oldx = newx = 0;
	oldy = newy = 0;
}


//void T2::registerWithTouchDispatcher(){
	// ‚Äô‚Äö¬ø√î¬µ∆í¬•‚Ä¢‚àöÀõ‚Äù‚âà≈ì¬ª¬∫‚àÇ‚Ä¶√ã√∑‚àö≈í‚Ñ?-128 ‚Äô‚Äö¬±¬£√∑¬ß¬°√Ä¬£¬®‚àÜ¬°¬±≈í≈ì¬¨‚àëŒ©¬µ∆í¬•‚Ä¢‚àöÀõ
	//CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
//}


int T2::myRand(int start,int end){ //[start,end]
	float randnum  = (CCRANDOM_0_1()* (end-start+1)+start);
	return (int)randnum;
}

void T2::resetRandSeed(){
	cc_timeval psv;  
	CCTime::gettimeofdayCocos2d(&psv, NULL);  
	unsigned long int rand_seed = psv.tv_sec*1000 + psv.tv_usec/1000+rand();  
	srand(rand_seed); 

}


void T2::getDiffRandNumer(int start,int end){//µ√µΩ[STRART,END]  quality∏ˆ≤ª÷ÿ∏¥µƒÀÊª˙ ˝

	resetRandSeed();
   const int quality = 9;
   int output[quality]={};

   
	int sequence[16] = {};//16  = end+1;
	if(quality>(end-start))
		return;
	for (int i = 0; i <= (end-start); i++)
	{
		sequence[i] = i+start;
	}
	//‚â§Àô‚Ä¶À?‚àèÀÜ‚â§¬™≈ì‚Ä°√ï¬®√Ä√ä¬™Àô¬´‚Äú‚Äò‚ÅÑ[start.end]‚àë‚àÇ≈í√ü∆í‚ÅÑ¬µ∆í√Ä√ä¬™Àô¬†Àù
	for (int i = 0; i < quality; i++)
	{
		int number = myRand(start,end );
		CCLog("number == %d ",number);
		output[i]=sequence[number];
		sequence[number] = sequence[end];
		end --;
	}

	for(int i = 0;i<quality;i++)
	{
		CCLog("%d",output[i]);
	}


}

void T2::initChess(int start ,int end){//√Ä√ä¬™Àô√∑√∑‚óä‚Äù‚Äú‚Ñ¢‚àë‚âà‚Äò‚ÅÑ‚Äî‚â†¬™‚àë√ï‚Ä?
	//MyCCLog::set_showinfo(false);
	//MyCCLog::info("initChess");
	MyCCLog::verbose("%d",true);
	MyCCLog::verbose("%d",false);

	//√É√ì‚â•‚Ä∞¬±‚â•√¶‚à?
	/*int init_chess_x = (ccsize.width-110*4)/2;
	int init_chess_y = (ccsize.height-110*4)/2;
	for (int i = 0; i < MAX_ROW; i++)
	{
	for (int j = 0; j < MAX_COL; j++)
	{
	arr_Num [i][j] = -1;
	CCSprite*  sprite_bg = CCSprite::create("2048/but-1.png");
	sprite_bg->setPosition(ccp(init_chess_x+110*i,init_chess_y+110*j));
	sprite_bg->setAnchorPoint(CCPointZero);
	this->addChild(sprite_bg,0);
	}
	}
	*/

	//resetRandSeed();



	//for(int i = 0 ;i<4;i++){
	//	for(int j=0;j<4;j++){
	//		if(i==output[0]/4&&j==output[0]%4)
	//			arr_Num [i][j] = myRand(0,1);
	//		else if(i==output[1]/4&&j==output[1]%4)
	//			arr_Num [i][j] = myRand(0,1);
	//		else if(i==output[2]/4&&j==output[2]%4)
	//			arr_Num [i][j] = myRand(0,1);
	//		else if(i==output[3]/4&&j==output[3]%4)
	//			arr_Num [i][j] = myRand(0,1);
	//		else
	//			arr_Num [i][j] = -1;
	//	}
	//}

	//for(int i=0 ;i<4;i++){
	//	for(int j=0;j<4;j++){
	//		//CCLog("init---arr_Num [%d] = %d",j+4*i,arr_Num [i][j]);

	//		arr_Num[0][0]= 1;
	//		arr_Num[0][1]= 1;
	//		arr_Num[0][2]= 1;
	//		arr_Num[0][3]= -1;
	//		arr_Num[1][0]= -1;
	//		arr_Num[1][1]= -1;
	//		arr_Num[1][2]= -1;
	//		arr_Num[1][3]= -1;

	//		arr_Num[2][j]= -1;
	//		arr_Num[3][j] = -1;

	//		ChessNode* ces = ChessNode::create(i,j,arr_Num[i][j]);
	//		node_array->addObject(ces);
	//		this->addChild(ces);

	//		//	CCLog("ces row = %d",ces->getRow());
	//		//	CCLog("ces col =  %d",ces->getCol());
	//		//	CCLog("ces value =  %d",ces->getValue());
	//		ces->log_ces_info("init->");

	//	}			
	//}


     addNewChess();
	 addNewChess();

	 /*


	arr_Num[0][0]=0;
	arr_Num[0][1]=1;
	arr_Num[0][2]=2;
	arr_Num[0][3]=3;

	arr_Num[1][0]=3;
	arr_Num[1][1]=2;
	arr_Num[1][2]=1;
	arr_Num[1][3]=0;

	arr_Num[2][0]=4;
	arr_Num[2][1]=5;
	arr_Num[2][2]=6;
	arr_Num[2][3]=7;

	arr_Num[3][0]=-1;
	arr_Num[3][1]=-1;
	arr_Num[3][2]=1;
	arr_Num[3][3]=2;

	for(int i=0;i<MAX_COL*MAX_ROW;i++){
		ChessNode* ces =  ChessNode::create(i/4,i%4,arr_Num[i/4][i%4],false);
		cnode->addChild(ces);
	}*/

	
	this->addChild(cnode);


	/*CCLog("init-----node_array length===  %d",node_array->count());
	CCObject* obj = NULL;
	CCARRAY_FOREACH(node_array,obj){
	ChessNode* onenode = dynamic_cast<ChessNode*>(obj);
	onenode->log_ces_info("initinit-=======-----");
	}*/

	logcat_chessArr("init ---->arr_Num");



}
//‚àö√∏‚â§Œ©√∑√ã‚àÇ¬∫‚Äú‚Ñ¢‚àè¬∏‚Äì¬¨≈ì‚Äò¬†√¶‚àÜ√Ç‚âà‚àÜ¬†Àù‚óä√?
void T2::update_moveyd1()
{
	//CCLog("node_array length=====  %d",node_array->count());

	/*CCObject* obj = NULL;
	CCARRAY_FOREACH(node_array,obj){
	ChessNode* onenode = dynamic_cast<ChessNode*>(obj);
	onenode->log_ces_info("change-before-str");
	}*/


	this->removeChild(cnode,true);

	cnode = CCNode::create();

	for(int i =0;i<MAX_ROW;i++){
		for(int j=0;j<MAX_COL;j++){
			if(arr_Num[i][j]!=-1){
				ChessNode* ces = ChessNode::create(i,j,arr_Num[i][j],false);
				ces->log_ces_info("change-new-str");
				cnode->addChild(ces);
			}
		}
	}
	this->addChild(cnode);
	CCLog("cnode-children = %d",cnode->getChildrenCount());

}


void T2::resetGame()
{
	isRightOver = isLeftOver  = false;
	 isUpOver = isDownOver  =false;
	 over_check_move_right = over_check_heti_right =false;
	 over_check_move_left = over_check_heti_left = false;
	 over_check_move_up = over_check_heti_up= false;
	 over_check_move_down = 	 over_check_heti_down = false;

	for(int i=0;i<MAX_ROW;i++){
		for(int j = 0;j<MAX_COL;j++){
			arr_Num[i][j] = -1;
		}
	}
	if(cnode)
		cnode->removeAllChildren();

	saveData();
	game_state = GAME_RUN;
    addNewChess();
    addNewChess();
    
    cclbmf_score->setString("0");
}


void T2::nodeMove(int dir_guester,int desRow,int desCol,int count){
	CCLog("count================ %d",count);
	if(dir_guester == DIR_RIGHT){
		CCObject *temp;
		CCArray *nodeArray =cnode ->getChildren();

		CCARRAY_FOREACH(nodeArray, temp)
		{
			ChessNode* cesnode =  dynamic_cast<ChessNode*>(temp);
			if(((ChessNode*)temp)->getCol()==desCol&&((ChessNode*)temp)->getRow() == desRow)
			{
				CCLog("cesnode : row = %d cesnode: col = %d cesnode : value = % d",cesnode->getRow(),cesnode->getCol(),cesnode->getValue());
				CCMoveTo* moveto = CCMoveTo::create( 0.2f,ccp(cesnode->getPositionX()+count*110,cesnode->getPositionY()));
				CCSequence* seq = NULL;
				if(move_state == MOVE_YD1){
					isMoving = true;
					seq = CCSequence::create(
						moveto,CCCallFuncN::create(this,callfuncN_selector(T2::callback_EleMove)),
						NULL);
				}
				else if(move_state == MOVE_YD2){
					isMoving = true;
					seq = CCSequence::create(
						moveto,CCCallFuncN::create(this,callfuncN_selector(T2::callback_EleMove_YD2)),
						NULL);
				}
				cesnode->runAction(seq);

				break;
			}

		}

	}
	else if(dir_guester == DIR_LEFT)
	{
		CCObject *temp;
		CCArray *nodeArray =cnode ->getChildren();
		CCLog("cnode-length= %d",nodeArray->count());
		CCARRAY_FOREACH(nodeArray, temp)
		{
			ChessNode* cesnode =  dynamic_cast<ChessNode*>(temp);
			if(cesnode->getCol()==desCol&& cesnode->getRow() == desRow)
			{
				CCLog("cesnode : row = %d cesnode: col = %d cesnode : value = % d",cesnode->getRow(),cesnode->getCol(),cesnode->getValue());
				CCMoveTo* moveto = CCMoveTo::create( 0.2f,ccp(cesnode->getPositionX()-count*110,cesnode->getPositionY()));
				CCSequence* seq = NULL;


				if(move_state == MOVE_YD1){
					isMoving = true;
					seq = CCSequence::create(
						moveto,CCCallFuncN::create(this,callfuncN_selector(T2::callback_EleMove)),
						NULL);
				}
				else if(move_state == MOVE_YD2){
					isMoving = true;
					seq = CCSequence::create(
						moveto,CCCallFuncN::create(this,callfuncN_selector(T2::callback_EleMove_YD2)),
						NULL);
				}
				cesnode->runAction(seq);
				break;
			}

		}
	}
	else if(dir_guester == DIR_UP)
	{
		CCObject *temp;
		CCArray *nodeArray =cnode ->getChildren();
		CCLog("cnode-length= %d",nodeArray->count());
		CCARRAY_FOREACH(nodeArray, temp)
		{
			ChessNode* cesnode =  dynamic_cast<ChessNode*>(temp);
			if(((ChessNode*)temp)->getCol()==desRow&&((ChessNode*)temp)->getRow() == desCol)
			{
				CCLog("cesnode : row = %d cesnode: col = %d cesnode : value = % d",cesnode->getRow(),cesnode->getCol(),cesnode->getValue());
				CCMoveTo* moveto = CCMoveTo::create( 0.2f,ccp(cesnode->getPositionX(),cesnode->getPositionY()+count*110));
				CCSequence* seq = NULL;

				if(move_state == MOVE_YD1){
					isMoving = true;
					seq = CCSequence::create(
						moveto,CCCallFuncN::create(this,callfuncN_selector(T2::callback_EleMove)),
						NULL);
				}
				else if(move_state == MOVE_YD2){
					isMoving = true;
					seq = CCSequence::create(
						moveto,CCCallFuncN::create(this,callfuncN_selector(T2::callback_EleMove_YD2)),
						NULL);
				}
				cesnode->runAction(seq);
				break;
			}

		}
	}

	else if(dir_guester == DIR_DOWN)
	{
		CCObject *temp;
		CCArray *nodeArray =cnode ->getChildren();
		CCLog("cnode-length= %d",nodeArray->count());
		CCARRAY_FOREACH(nodeArray, temp)
		{
			ChessNode* cesnode =  dynamic_cast<ChessNode*>(temp);
			if(((ChessNode*)temp)->getCol()==desRow&&((ChessNode*)temp)->getRow() == desCol)
			{
				CCLog("cesnode : row = %d cesnode: col = %d cesnode : value = % d",cesnode->getRow(),cesnode->getCol(),cesnode->getValue());
				CCMoveTo* moveto = CCMoveTo::create( 0.2f,ccp(cesnode->getPositionX(),cesnode->getPositionY()-count*110));
				CCSequence* seq = NULL;

				if(move_state == MOVE_YD1){
					isMoving = true;
					seq = CCSequence::create(
						moveto,CCCallFuncN::create(this,callfuncN_selector(T2::callback_EleMove)),
						NULL);
				}
				else if(move_state == MOVE_YD2){
					isMoving = true;
					seq = CCSequence::create(
						moveto,CCCallFuncN::create(this,callfuncN_selector(T2::callback_EleMove_YD2)),
						NULL);
				}
				cesnode->runAction(seq);
				break;
			}

		}
	}

}

void T2::callback_EleMove(CCNode* node){


	ChessNode* ces = (ChessNode*)node;
	CCLog("callback_EleMove  row = %d col = %d value = %d",ces->getRow(),ces->getCol(),ces->getValue());
	CCLog("move_state = %d",move_state);

	move_state = MOVE_HETI;
	isMoving = false;
}

void T2::callback_EleMove_YD2(CCNode* node)
{
	CCLog("callback_EleMove_YD2");
	//addNewChess();
	isMoving = false;
}


void T2::callback_hecheng(CCNode* node ){
	ChessNode* ces = (ChessNode*)node;
	CCLog("callback_hecheng  row = %d col = %d value = %d",ces->getRow(),ces->getCol(),ces->getValue());


	move_state = MOVE_YD2;

}

bool T2::isChessFull()
{
	bool ret = true;

	for (int i=0;i<MAX_ROW;i++)
	{
		for (int j=0;j<MAX_COL;j++)
		{
			if(arr_Num[i][j]==-1)
				return false;
		}
	}
	return ret;
}

void T2::addNewChess()
{

	/*for (int i= 5;i>0;i--)
	{
	CCLog("eee i=%d",i);
	}*/

	/*if(isChessFull()){
	CCLog("chess is full------------------");
	return;
	}*/

	resetRandSeed();
	//¬ª√ÅœÄÀö‚â§Àô‚Ä¶Àô¬µ∆í≈í¬™√∑‚àö‚Ä¶≈ì¬µ∆í¬†Àù‚â§¬™¬†¬´-1¬£¬®√¶√ï‚Äú¬™√∑¬±‚â§Àô‚Ä¶Àô¬£¬?

	int index = myRand(0,15);
	CCLog("indx = %d",index);
	while(arr_Num[index/4][index%4]==-1	)
	{

		arr_Num[index/4][index%4]=myRand(0,1);
		ChessNode* ces =  ChessNode::create(index/4,index%4,arr_Num[index/4][index%4],true);
		cnode->addChild(ces);
		return;
	}
	addNewChess();
}

void T2::logcat_chessArr(char* movestate){
	for(int i=0 ;i<4;i++){
		for(int j=0;j<4;j++){
			CCLog("movestate %s current-arr_Num [%d] = %d",movestate,j+4*i,arr_Num [i][j]);
		}			
	}
}


void T2::draw(){
	ccDrawColor4B(255,0,0,130);
	glLineWidth(3);

	int init_chess_x = (SCR_SIZE.width-110*4)/2;
	int init_chess_y = (SCR_SIZE.height-110*4)/2;
	ccDrawRect(ccp(init_chess_x,init_chess_y),ccp(4*110+init_chess_x,4*110+init_chess_y));
}

void T2::menuGameOverCallback(CCObject* pSender)
{
	CCLog("menu-menuGameOverCallback");

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#else
	
#endif
}


void T2::menuGameExitCallback(CCObject* pSender)
{
	CCLog("menu-menuGameExitCallback");

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#else
	exit(0);
#endif
}


void T2::gameover_btnCallback(CCNode* node){
    
    
    MyCCLog::verbose("node %d",node->getTag());
    if(node->getTag()==0){
        resetGame();
     
    }
    else if(node->getTag()==1)
    {
        exit(0);
    }
    
}

void T2::addScore(int current_num){
    
    
	switch (current_num)
	{
	case 1://4
		current_score += 4;
		break;
	case 2://8
		current_score+= 8;
		break;
		case 3://16
		current_score+=16;
		break;
		case 4://32
		current_score+=32;
		break;
		case 5://64
		current_score+=64;
		break;
		case 6://128
		current_score+=128;
		break;
		case 7://256
		current_score+=256;
		case 8://512
		current_score+=512;
		case 9://1024
															current_score+=512;
		case 10://2048
		current_score+=2048;
			break;
		case 11://4096
		current_score+=4096;
			break;
		break;
	default:
		break;
	}
	MyCCLog::verbose("current_num ===========%d",current_score);
	cclbmf_score->setString(  (CCString::createWithFormat("%d",current_score))->getCString()   );

}

void T2::saveData(){
	int max_score = CCUserDefault::sharedUserDefault()->getIntegerForKey("MAXSCORE");
	if(current_score>=max_score){

		CCUserDefault::sharedUserDefault()->setIntegerForKey("MAXSCORE",current_score);
		cclbmf_max_score->setString(  (CCString::createWithFormat("%d",current_score))->getCString()   );
	}
	CCUserDefault::sharedUserDefault()->flush();
	current_score = 0;
}

//
//inline void WStrToUTF8(std::string& dest, const wstring& src){
//
//dest.clear();
//
//for (size_t i = 0; i < src.size(); i++){
//
//wchar_t w = src[i];
//
//if (w <= 0x7f)
//
//dest.push_back((char)w);
//
//else if (w <= 0x7ff){
//
//dest.push_back(0xc0 | ((w >> 6)& 0x1f));
//
//dest.push_back(0x80| (w & 0x3f));
//
//}
//
//else if (w <= 0xffff){
//
//dest.push_back(0xe0 | ((w >> 12)& 0x0f));
//
//dest.push_back(0x80| ((w >> 6) & 0x3f));
//
//dest.push_back(0x80| (w & 0x3f));
//
//}
//
//else if (sizeof(wchar_t) > 2 && w <= 0x10ffff){
//
//dest.push_back(0xf0 | ((w >> 18)& 0x07)); // wchar_t 4-bytes situation
//
//dest.push_back(0x80| ((w >> 12) & 0x3f));
//
//dest.push_back(0x80| ((w >> 6) & 0x3f));
//
//dest.push_back(0x80| (w & 0x3f));
//
//}
//
//else
//
//dest.push_back('?');
//
//}
//
//}
//
//
//inline std::string WStrToUTF8(const std::wstring& str){
//
//std::string result;
//
//WStrToUTF8(result, str);
//
//return result;
//
//}

