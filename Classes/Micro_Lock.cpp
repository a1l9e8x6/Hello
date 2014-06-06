#include "Micro_Lock.h"

Micro_Lock::Micro_Lock(){
    
	CCLog("Micro_Lock()Micro_Lock()");
}


Micro_Lock::~Micro_Lock(){
	CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, MICRO_KEY_NOTIFICATION); 
}


Micro_Lock* Micro_Lock::create(CCTMXTiledMap* _tileMap,int lockid,int lockdir,int lockcolorkind,const CCPoint& point){
    if(NULL == _tileMap){
		CCLog("Micro_Lock  create()   _tileMap IS  NULL ");
		return NULL;
	}
    
    Micro_Lock *pRet = new Micro_Lock();
	if (pRet && pRet->init(_tileMap,lockid,lockdir,lockcolorkind,point)) {
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

bool Micro_Lock::init(CCTMXTiledMap* _tileMap,int lockid,int lockdir,int lockcolorkind,const CCPoint& point){
      if(!CCSprite::init())
		return false;
    
	this->setAnchorPoint(ccp(0.5,0.5));
	set_lockstate(false);
	set_lockColor(lockcolorkind);
    set_lockid(lockid);
	set_lockdir(lockdir);
	CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("mirco/Animal/lockeyAIAM/MicroLock0.png",
	"mirco/Animal/lockeyAIAM/MicroLock0.plist","mirco/Animal/lockeyAIAM/MicroLock.ExportJson");
	ccarlock = CCArmature::create("MicroLock");

	switch (get_lockdir())
	{
	case LOCK_DIR_LEFT:
			if(lockcolorkind == LOCK_COLOR_YELLOW){
			ccarlock->getAnimation()->playByIndex(5);
		}
		else if(lockcolorkind == LOCK_COLOR_RED){
			ccarlock->getAnimation()->playByIndex(11);
		}
		break;
	case LOCK_DIR_RIGHT:
		if(lockcolorkind == LOCK_COLOR_YELLOW){
			ccarlock->getAnimation()->playByIndex(2);
		}
		else if(lockcolorkind == LOCK_COLOR_RED){
			ccarlock->getAnimation()->playByIndex(3);
		}
		break;
	case LOCK_DIR_UP:
			if(lockcolorkind == LOCK_COLOR_YELLOW){
			ccarlock->getAnimation()->playByIndex(7);
		}
		else if(lockcolorkind == LOCK_COLOR_RED){
			ccarlock->getAnimation()->playByIndex(13);
		}
		break;
	case LOCK_DIR_DOWN:
			if(lockcolorkind == LOCK_COLOR_YELLOW){
			ccarlock->getAnimation()->playByIndex(9);
		}
		else if(lockcolorkind == LOCK_COLOR_RED){
			ccarlock->getAnimation()->playByIndex(15);
		}
		break;
	default:
		break;
	}

	CCLog("Micro_Lock::init() -> pointX =%.2f;pointY = %.2f",point.x,point.y);
	ccarlock->setPosition(point);
	this->addChild(ccarlock);
	

	 //第一个参数是监听消息的对象，第二个参数是消息发来的时候调用的函数，第三个参数消息的名字 //第四个参数消息体
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this,
		callfuncO_selector(Micro_Lock::getBallHitKeyNotification),
                                                                  MICRO_KEY_NOTIFICATION,NULL);

	
    return  true;
    
}

void Micro_Lock::getBallHitKeyNotification(CCObject* obj){
	CCLog("Micro_Lock----->>>>Micro_Lock-->>>>");
	Micro_Key* key = (Micro_Key*)obj;
	if(get_lockstate())
		return;

	int lock_color =get_lockColor();
	int lock_id = get_lockid();
	CCLog("lock_color==%d",lock_color);
	if(key->getisHit()){
		if(key->get_keyColor()==lock_color && key->get_keyid() == lockid )
		{
			int lockdir = get_lockdir();
			set_lockstate(true);
			switch (lockdir)
			{
			case LOCK_DIR_LEFT:
				if(get_lockColor() == LOCK_COLOR_YELLOW ){
					ccarlock->getAnimation()->playByIndex(4);
				}
				else if(get_lockColor() == LOCK_COLOR_RED){
					ccarlock->getAnimation()->playByIndex(10);
				}
				break;
			case LOCK_DIR_RIGHT:
				if(get_lockColor() == LOCK_COLOR_YELLOW ){
					ccarlock->getAnimation()->playByIndex(0);
				}
				else if(get_lockColor() == LOCK_COLOR_RED){
					ccarlock->getAnimation()->playByIndex(1);
				}
				break;
			case LOCK_DIR_UP:
				if(get_lockColor() == LOCK_COLOR_YELLOW ){
					ccarlock->getAnimation()->playByIndex(6);
				}
				else if(get_lockColor() == LOCK_COLOR_RED){
					ccarlock->getAnimation()->playByIndex(13);
				}
				break;
			case LOCK_DIR_DOWN:
				if(get_lockColor() == LOCK_COLOR_YELLOW ){
					ccarlock->getAnimation()->playByIndex(8);
				}
				else if(get_lockColor() == LOCK_COLOR_RED){
					ccarlock->getAnimation()->playByIndex(15);
				}
				break;
			default:
				break;
			}
		}
	}
}

void Micro_Lock::playParticleEffect(){
	CCParticleSystemQuad* particle_hiteffect = CCParticleSystemQuad::create("mirco/lizi/hitlockeffect.plist");
	particle_hiteffect->setAnchorPoint(ccp(0.5,0.5));
	particle_hiteffect->setPosition(   ccarlock->getPosition() );
	particle_hiteffect->setAutoRemoveOnFinish(true);//每次hit 产生一个例子效果 回收
	CCLog("particle_hiteffect an x=%.2f,y=%.2f",particle_hiteffect->getPositionX(),particle_hiteffect->getPositionY());
	this->addChild(particle_hiteffect);
}


void Micro_Lock::set_lockdir(int pdir){
	this->lockdir = pdir;
}

int Micro_Lock::get_lockdir(){
	return this->lockdir;
}


void Micro_Lock::set_lockColor(int plockcolor){
	this->lockColor = plockcolor;
}

int Micro_Lock::get_lockColor(){
	return this->lockColor;
}

void Micro_Lock::set_lockstate(bool plockstate){
	this->lockstate = plockstate;
}

bool Micro_Lock::get_lockstate(){
	return this->lockstate;
}

int Micro_Lock::get_lockid(){
    return this->lockid;
}

void Micro_Lock::set_lockid(int pid){
    this->lockid = pid;
}