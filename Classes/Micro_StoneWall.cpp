//
//  Micro_Tool.cpp
//  Hello
//
//  Created by bobo on 14-4-19.
//
//

#include "Micro_StoneWall.h"
#include "MyCCLog.h"


Micro_StoneWall::Micro_StoneWall(){
    
	CCLog("Micro_StoneWall()Micro_StoneWall()");
	hitcount = 0;
}


Micro_StoneWall::~Micro_StoneWall(){
   // 	CC_SAFE_RELEASE_NULL(ccar);
}


Micro_StoneWall* Micro_StoneWall::create(CCTMXTiledMap* _tileMap,int wall_kind,const CCPoint& point){
    
    if(NULL == _tileMap){
		CCLog("Micro_Tool  create()   _tileMap IS  NULL ");
		return NULL;
	}
    
    Micro_StoneWall *pRet = new Micro_StoneWall();
	if (pRet && pRet->init(_tileMap,wall_kind,point)) {
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

bool Micro_StoneWall::init(CCTMXTiledMap* _tileMap,int wall_kind,const CCPoint& point){
      if(!CCSprite::init())
		return false;
    
	this->setAnchorPoint(ccp(0.5,0.5));

	CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("mirco/Animal/bigstoneAIAM/Micro0.png","mirco/Animal/bigstoneAIAM/Micro0.plist","mirco/Animal/bigstoneAIAM/Micro.ExportJson");
	ccar = CCArmature::create("Micro");
	ccar->getAnimation()->playByIndex(0);
	ccar->setPosition(point);
	this->addChild(ccar);

	/*CCActionInterval * shaky3D= CCShaky3D::create(1.2,  CCSizeMake(32, 32), 4,false);
    ccar->runAction(shaky3D);*/

	CCLog("Micro_StoneWall::init() -> pointX =%.2f;pointY = %.2f",point.x,point.y);
    disappear = false;
    return  true;
    
}

void Micro_StoneWall::changeArmIndex(){
	
	if(hitcount < 4){
		hitcount = hitcount+1;
		CCLog("changeArmIndex hitcount =%d",hitcount);
		ccar->getAnimation()->playByIndex(hitcount);
	}
	else{
		CCLog("disappeardisappeardisappear");
		hitcount = 0;
		disappear = true;
		ccar->removeFromParentAndCleanup(true);
	}
	CCLog("hitcount ===%d",hitcount);
}

bool Micro_StoneWall::get_disappear(){
	return disappear;
}

