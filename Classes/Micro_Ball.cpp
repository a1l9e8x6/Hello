#include "Micro_Ball.h"
#include "MZDataManager.h"
const float ball_speed = 4.0f;

Micro_ball::Micro_ball(){
	ball_dir = BALL_NONE;
    delta_x_withmap = delta_y_withmap = 0.0f;
	particle_ball  = NULL;
}

Micro_ball::~Micro_ball(){
	CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, MAP_NOTIFICATION); 
}

Micro_ball* Micro_ball::create(CCTMXTiledMap* _tileMap,const char* filename,const char* groupName,const char* objectName)
{
	if(NULL == _tileMap)
		return NULL;

	Micro_ball *pRet = new Micro_ball();
	if (pRet && pRet->ballInit(_tileMap,filename,groupName,objectName)) {
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}


// EMITTER BALL
bool Micro_ball::ballInit(CCTMXTiledMap* _tileMap,const char* filename,const char* groupName,const char* objectName){
	if(!CCSprite::init())
		return false;


	this->setAnchorPoint(ccp(0.5,0.5));
	this->initWithFile(filename);
	//对象层
	CCTMXObjectGroup *objects=_tileMap->objectGroupNamed(groupName);
	if(objects){
		CCPoint heroPoint;
		CCDictionary  *spawnPoint=objects->objectNamed(objectName);
		heroPoint.x=spawnPoint->valueForKey("x")->floatValue();
		heroPoint.y=spawnPoint->valueForKey("y")->floatValue();

		ball_dir = spawnPoint->valueForKey("move_dir")->intValue();
		CCLog("ssssssssssssssssssssssssssssstttttttt  =%d",ball_dir);
		//以tilemap里地图的左下为zero的，固定坐标


		//heroPoint 最终是添加到 tilemap上所以 永远是对于map的相对坐标 ，不是屏幕的实际坐标
		CCLog("----needADD---ballInitPoint--needADD----%.2f,%.2f",heroPoint.x,heroPoint.y);

		this->setPosition(ccp(heroPoint.x+tile_one_w/2,heroPoint.y+tile_one_h/2));

		
	particle_ball = CCParticleSystemQuad::create("mirco/lizi/guiji.plist");
	particle_ball->setAnchorPoint(ccp(0.5,0.5));
	particle_ball->setPosition(   ccp(tile_one_w/4,tile_one_h/4) );
	CCLog("particle_ball an x=%.2f,y=%.2f",particle_ball->getPositionX(),particle_ball->getPositionY());
	CCLog("particle_ball contentsize w =%.2f,h=%.2f",particle_ball->getContentSize().width,particle_ball->getContentSize().height);
	this->addChild(particle_ball);
	}

	set_check_tool(false);


    Micro* micro  = (Micro*)(_tileMap->getParent());
    mapw =  micro->map_total_w;
    maph = micro->map_total_h;

    
    //第一个参数是监听消息的对象，第二个参数是消息发来的时候调用的函数，第三个参数消息的名字 //第四个参数消息体
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this,
                                                                  callfuncO_selector(Micro_ball::getNotification),
                                                                  MAP_NOTIFICATION,NULL);

    
   // CCNotificationCenter:sharedNotificationCenter()
    //:registerScriptObserver(self,handler(self,self.showPreview),"changeClose")
    
    
	return true;

}


void Micro_ball::getNotification(CCObject* obj){

	CCString* delta = (CCString*)obj;
	string delta_str = delta->m_sString;
	CCLog("%s",delta_str.c_str());

	std::vector<string> vect_str_delta;
	vect_str_delta =  MZDataManager::sharedDataManager()->split(delta_str, "|");

	string s1 = vect_str_delta.front();
	vect_str_delta.erase(vect_str_delta.begin());
	string s2 = vect_str_delta.front();
	CCLog("%s--%s",s1.c_str(),s2.c_str());

	///CCString* s1_str = CCString::createWithFormat("%s",s1);
	double woca_delta_x_withmap =  std::atof(s1.c_str());
	double woca_delta_y_withmap = std::atof(s2.c_str());

	//delta_x_withmap = woca_delta_x_withmap;
	//delta_y_withmap = woca_delta_y_withmap;

	//delta_x_withmap = atof( vect_str_delta.front().c_str() );
	//	delta_y_withmap = atof(vect_str_delta.back().c_str());
	//	delta_y_withmap = atof( vect_str_delta.at(vect_str_delta.size()-1).c_str() );

	//delta_x_withmap =std::atof( (*vect_str_delta.begin() ).c_str() );
	//delta_y_withmap= std::atof( (*(vect_str_delta.end()-1) ).c_str() );

	CCLog("getNotification -delta_x_withmap ＝%.2f,",delta_x_withmap);
	CCLog("getNotification-delta_y_withmap＝%.2f,",delta_y_withmap);

}


void Micro_ball::ball_move(){
	scheduleUpdate();
  //  this->schedule( schedule_selector(Micro_ball::updateballpos), 0.02f,kCCRepeatForever, 0.0f);
}

void  Micro_ball::changeballdir(){
	 //碰到板子了
	if(ball_dir == BALL_LEFT){  //dir = 1 ballmoveleft
		if(check_tool){
			if(tool_dir == TOOL_DIR_LEFT){
					ball_dir = BALL_DOWN;
			
			}
			else if(tool_dir == TOOL_DIR_RIGHT)
			{
					ball_dir = BALL_UP;
			}
		}
	}
	else if(ball_dir == BALL_RIGHT){ //dir = 2 ballmoveright
		if(check_tool){
			if(tool_dir == TOOL_DIR_LEFT){
				ball_dir = BALL_UP;
			}
			else if(tool_dir == TOOL_DIR_RIGHT)
			{
				ball_dir = BALL_DOWN;
			}
		}

	}
	else if(ball_dir == BALL_UP){ //dir = 3 ballup

		if(check_tool){
			if(tool_dir == TOOL_DIR_LEFT){
				ball_dir = BALL_RIGHT;
			}
			else if(tool_dir == TOOL_DIR_RIGHT)
			{
				// 最左面还需要 再减一个格子的宽
				ball_dir = BALL_LEFT;
			}
		}

	}
	else if(ball_dir == BALL_DOWN){ //dir = 4 ballmovedown
		
		if(check_tool){
			if(tool_dir == TOOL_DIR_LEFT){
				ball_dir = BALL_LEFT;
			}
			else if(tool_dir == TOOL_DIR_RIGHT)
			{
				ball_dir = BALL_RIGHT;
			}
		}
	}
    
}

void Micro_ball::update(float t){
    
//    if(delta_x_withmap != 0){
//        CCLog("PPPPPPPPP");
//        unscheduleUpdate();
//        return;
//    }
    
	if(ball_dir == BALL_LEFT){  //dir = 1 ballmoveleft
		// 最左面还需要 再减一个格子的宽
        if(getPositionX() >=  -tile_one_w){
            int curx = getPositionX();
            curx -= (ball_speed+delta_x_withmap);
          //  CCLog("delta_x_withmap update=%f",delta_x_withmap);
            setPositionX(curx);
			
        }
	}
	else if(ball_dir == BALL_RIGHT){ //dir = 2 ballmoveright

			//最右边还需要加一个格子的宽度
		if(getPositionX() <= ( mapw + tile_one_w )){
				int curx = getPositionX();
				curx += ball_speed;
				setPositionX(curx+delta_x_withmap);
			}
	}
	else if(ball_dir == BALL_UP){ //dir = 3 ballup

	/*	float hh =  MZDataManager::sharedDataManager()->micro->map_total_h;
		CCLog("hh ===%f",hh);*/

		if(getPositionY() <= ( maph+ tile_one_h )  )  {
				int cury = getPositionY();
				cury += ball_speed;
				setPositionY(cury+delta_y_withmap);
			}
	}
	else if(ball_dir == BALL_DOWN){ //dir = 4 ballmovedown
        if(getPositionY() >= -tile_one_h )  {
            int cury = getPositionY();
            cury -= ball_speed;
            setPositionY(cury+delta_y_withmap);
        }
	}
    
}


void Micro_ball::set_tool_dir(int dir){
	this->tool_dir = dir;
}


int Micro_ball::get_tool_dir()
{
	return tool_dir;
}


int  Micro_ball::getball_dir(){
	return ball_dir;
}

void Micro_ball::setball_dir(int dir){
	this->ball_dir = dir;
}

bool  Micro_ball::get_check_tool(){
	return check_tool;
}

void Micro_ball::set_check_tool(bool check){
	this->check_tool = check;
}

////void Micro_ball::playParticleEffectWithLock()
////{
////	CCParticleSystemQuad* particle_hiteffect = CCParticleSystemQuad::create("mirco/lizi/hitlockeffect.plist");
////	particle_hiteffect->setAnchorPoint(ccp(0.5,0.5));
////	particle_hiteffect->setPosition(   getPosition() );
////	CCLog("particle_hiteffect an x=%.2f,y=%.2f",particle_hiteffect->getPositionX(),particle_hiteffect->getPositionY());
////	this->addChild(particle_hiteffect);
////}

 //		float length =  ccpDistance( CCPointMake(_tileMap->getTileSize().width*(-1), getPositionY()),CCPointMake(getPositionX(),getPositionY()));
  //		float time = length / 40.0f;
   //
    //			CCActionInterval * moveto = CCMoveTo::create(
   //			time,
   //			ccp(_tileMap->getTileSize().width*(-1), getPositionY())  );




//Micro_ball* Micro_ball::create(CCTMXTiledMap* _tileMap,const char* filename,const CCPoint& point)
//{
//	if(NULL == _tileMap)
//		return NULL;
//
//	Micro_ball *pRet = new Micro_ball();
//	if (pRet && pRet->ballInit(_tileMap,filename,point)) {
//		pRet->autorelease();
//		return pRet;
//	}
//	else
//	{
//		delete pRet;
//		pRet = NULL;
//		return NULL;
//	}
//}


//ball
//bool Micro_ball::ballInit(CCTMXTiledMap* _tileMap,const char* filename,const CCPoint& point){
//	if(!CCSprite::init())
//		return false;
//
//	this->setAnchorPoint(ccp(0.5,0.5));
//	this->initWithFile(filename);
//		this->setPosition(ccp(point.x+_tileMap->getTileSize().width/2,
//			point.y+_tileMap->getTileSize().height/2));
//	return true;
//}


//void Micro_ball::ballmove(CCTMXTiledMap* _tileMap)
//{
//
//	/*float scr_ballx = _tileMap->getPositionX()+getPositionX();
//	float scr_bally = _tileMap->getPositionY()+getPositionY();
//	CCLog("scr_ballx ==%.2f ,scr_bally ==%.2f relative ballx = %.2f relative bally = %.2f",scr_ballx,scr_bally,getPositionX(),getPositionY());*/
//
//
//
//	if(ball_dir == BALL_LEFT){  //dir = 1 ballmoveleft
////		float length =  ccpDistance( CCPointMake(_tileMap->getTileSize().width*(-1), getPositionY()),CCPointMake(getPositionX(),getPositionY()));
////		float time = length / 40.0f;
////
////			CCActionInterval * moveto = CCMoveTo::create(
////			time,
////			ccp(_tileMap->getTileSize().width*(-1), getPositionY())  );
//
//		// 最左面还需要 再减一个格子的宽
//		CCActionInterval * moveto = CCMoveTo::create(
//			getPositionX()*0.1f/16.0f,
//			ccp(_tileMap->getTileSize().width*(-1), getPositionY())  );
//		CCFiniteTimeAction * seq = CCSequence::create(moveto,NULL);
//		this->runAction(seq);
//	}
//	else if(ball_dir == BALL_RIGHT){ //dir = 2 ballmoveright
//		//最右边还需要加一个格子的宽度
//		CCActionInterval * moveto = CCMoveTo::create( (_tileMap->getContentSize().width-getPositionX())*0.1f/16.0f, ccp(_tileMap->getContentSize().width+_tileMap->getTileSize().width,getPositionY()));
//		CCFiniteTimeAction * seq = CCSequence::create(moveto,NULL);
//		this->runAction(seq);
//
//	}
//	else if(ball_dir == BALL_UP){ //dir = 3 ballup
//		CCActionInterval * moveto = CCMoveTo::create( (_tileMap->getContentSize().height-getPositionY())*0.1f/16.0f, ccp(getPositionX(),
//			_tileMap->getContentSize().height+_tileMap->getTileSize().height));
//		CCFiniteTimeAction * seq = CCSequence::create(moveto,NULL);
//		this->runAction(seq);
//	}
//	else if(ball_dir == BALL_DOWN){ //dir = 4 ballmovedown
//		//最下边还需要加一个格子的宽度
//		CCActionInterval * moveto = CCMoveTo::create( getPositionY()*0.1f/16.0f,
//			ccp( getPositionX(),_tileMap->getTileSize().height*(-1))  );
//		CCFiniteTimeAction * seq = CCSequence::create(moveto,NULL);
//		this->runAction(seq);
//	}
//
//}

//
//void  Micro_ball ::ballchangedir(CCTMXTiledMap* _tileMap,Micro_Tool* tool){
//
//	CCLog("ballchangedir ball_dir===%d ; toolkind =====%d",ball_dir,tool->get_tool_kind());
//	if(ball_dir == BALL_LEFT){  //dir = 1 ballmoveleft
//
//		if(tool->get_tool_kind() == 1) // tool--LEFT  ball--move--down
//		{
//
//				CCLog("ballchangedirballchangedirballchangedir");
//
//			CCActionInterval * moveto = CCMoveTo::create( (_tileMap->getContentSize().height-tool->get_tooly())*0.1f/16.0f, ccp(tool->get_toolx(),
//				_tileMap->getContentSize().height+_tileMap->getTileSize().height));
//			CCFiniteTimeAction * seq = CCSequence::create(moveto,NULL);
//			this->runAction(seq);
//
//		}
//		else if(tool->get_tool_kind() == 2) // tool--RIGHT  ball--move-up
//		{
//			CCActionInterval * moveto = CCMoveTo::create(  tool->get_tooly()*0.1f/16.0f,
//				ccp( tool->get_toolx(),_tileMap->getTileSize().height*(-1))  );
//			CCFiniteTimeAction * seq = CCSequence::create(moveto,NULL);
//			this->runAction(seq);
//		}
//
//
//	}
//	else if(ball_dir == BALL_RIGHT){ //dir = 2 ballmoveright
//
//		if(tool->get_tool_kind() == 1) // tool--LEFT  ball--up
//		{
//			CCActionInterval * moveto = CCMoveTo::create( tool->get_tooly()*0.1f/16.0f,
//				ccp(  tool->get_toolx(),_tileMap->getTileSize().height*(-1))  );
//			CCFiniteTimeAction * seq = CCSequence::create(moveto,NULL);
//			this->runAction(seq);
//		}
//		else if(tool->get_tool_kind() == 2) // tool--RIGHT  ball--move-down
//		{
//			CCActionInterval * moveto = CCMoveTo::create( (_tileMap->getContentSize().height- tool->get_tooly())*0.1f/16.0f, ccp( tool->get_toolx(),
//				_tileMap->getContentSize().height+_tileMap->getTileSize().height));
//			CCFiniteTimeAction * seq = CCSequence::create(moveto,NULL);
//			this->runAction(seq);
//		}
//
//
//	}
//	else if( ball_dir == BALL_UP){ //dir = 3 ballup
//
//
//		if(tool->get_tool_kind() == 1){ //tool left  ballmoveleft
//			// 最左面还需要 再减一个格子的宽
//			CCActionInterval * moveto = CCMoveTo::create( tool->get_toolx()*0.1f/16.0f,
//				ccp(_tileMap->getTileSize().width*(-1),  tool->get_tooly())  );
//			CCFiniteTimeAction * seq = CCSequence::create(moveto,NULL);
//			this->runAction(seq);
//		}
//
//		else if(tool->get_tool_kind() == 2){ //tool right  ballmoveright
//			//最右边还需要加一个格子的宽度
//			CCActionInterval * moveto = CCMoveTo::create( (_tileMap->getContentSize().width- tool->get_toolx())*0.1f/16.0f, ccp(_tileMap->getContentSize().width+_tileMap->getTileSize().width, tool->get_tooly()));
//			CCFiniteTimeAction * seq = CCSequence::create(moveto,NULL);
//			this->runAction(seq);
//		}
//
//
//	}
//	else if( ball_dir == BALL_DOWN){ //dir = 4 ballmovedown
//
//
//		if(tool->get_tool_kind() == 1) //tool left   ballmove-left
//		{
//			// 最左面还需要 再减一个格子的宽
//			CCActionInterval * moveto = CCMoveTo::create( tool->get_toolx()*0.1f/16.0f,
//				ccp(_tileMap->getTileSize().width*(-1),  tool->get_tooly())  );
//			CCFiniteTimeAction * seq = CCSequence::create(moveto,NULL);
//			this->runAction(seq);
//		}
//		else if(tool->get_tool_kind() == 2) // //tool right   ballmove-right
//		{
//			//最右边还需要加一个格子的宽度
//			CCActionInterval * moveto = CCMoveTo::create( (_tileMap->getContentSize().width- tool->get_toolx())*0.1f/16.0f, ccp(_tileMap->getContentSize().width+_tileMap->getTileSize().width, tool->get_tooly()));
//			CCFiniteTimeAction * seq = CCSequence::create(moveto,NULL);
//			this->runAction(seq);
//		}
//	}
//
//
//
//
//}



//void Micro_ball::update(float t){
//
//	if(ball_dir == BALL_LEFT){  //dir = 1 ballmoveleft
//      //碰到板子了
//		if(check_tool){
//			if(tool_dir == TOOL_DIR_LEFT){
//				if(getPositionY() >= -tile_one_h )  {
//					int cury = getPositionY();
//					cury -= ball_speed;
//					setPositionY(cury);
//				}
//			}
//			else if(tool_dir == TOOL_DIR_RIGHT)
//			{
//				if(getPositionY() <= ( maph + tile_one_h )  )  {
//					int cury = getPositionY();
//					cury += ball_speed;
//					setPositionY(cury);
//				}
//			}
//		}
//		else{
//			// 最左面还需要 再减一个格子的宽
//        if(getPositionX() >=  -tile_one_w){
//            int curx = getPositionX();
//            curx -= ball_speed;
//            setPositionX(curx);
//        }
//		}
//
//
//
//
//	}
//	else if(ball_dir == BALL_RIGHT){ //dir = 2 ballmoveright
//
//		if(check_tool){
//			if(tool_dir == TOOL_DIR_LEFT){
//				if(getPositionY() <= ( maph + tile_one_h )  )  {
//					int cury = getPositionY();
//					cury += ball_speed;
//					setPositionY(cury);
//				}
//			}
//			else if(tool_dir == TOOL_DIR_RIGHT)
//			{
//				if(getPositionY() >= -tile_one_h )  {
//					int cury = getPositionY();
//					cury -= ball_speed;
//					setPositionY(cury);
//				}
//			}
//		}
//		else{
//			//最右边还需要加一个格子的宽度
//			if(getPositionX() <= (  mapw+ tile_one_w )){
//				int curx = getPositionX();
//				curx += ball_speed;
//				setPositionX(curx);
//			}
//		}
//
//	}
//	else if(ball_dir == BALL_UP){ //dir = 3 ballup
//
//
//		if(check_tool){
//			if(tool_dir == TOOL_DIR_LEFT){
//				//最右边还需要加一个格子的宽度
//				if(getPositionX() <= (  mapw+ tile_one_w )){
//					int curx = getPositionX();
//					curx += ball_speed;
//					setPositionX(curx);
//				}
//			}
//			else if(tool_dir == TOOL_DIR_RIGHT)
//			{
//				// 最左面还需要 再减一个格子的宽
//				if(getPositionX() >=  -tile_one_w){
//					int curx = getPositionX();
//					curx -= ball_speed;
//					setPositionX(curx);
//				}
//			}
//		}
//		else{
//			if(getPositionY() <= ( maph + tile_one_h )  )  {
//				int cury = getPositionY();
//				cury += ball_speed;
//				setPositionY(cury);
//			}
//
//		}
//
//	}
//	else if(ball_dir == BALL_DOWN){ //dir = 4 ballmovedown
//
//
//		if(check_tool){
//			if(tool_dir == TOOL_DIR_LEFT){
//				// 最左面还需要 再减一个格子的宽
//				if(getPositionX() >=  -tile_one_w){
//					int curx = getPositionX();
//					curx -= ball_speed;
//					setPositionX(curx);
//				}
//			}
//			else if(tool_dir == TOOL_DIR_RIGHT)
//			{
//				//最右边还需要加一个格子的宽度
//				if(getPositionX() <= (  mapw+ tile_one_w )){
//					int curx = getPositionX();
//					curx += ball_speed;
//					setPositionX(curx);
//				}
//			}
//		}
//		else{
//
//        if(getPositionY() >= -tile_one_h )  {
//            int cury = getPositionY();
//            cury -= ball_speed;
//            setPositionY(cury);
//        }
//
//		}
//	}
//
//}


             