#include "Micro_Key.h"


Micro_Key::Micro_Key():isHit(false){

	CCLog("Micro_Key()Micro_Key()");
}


Micro_Key::~Micro_Key(){
}


Micro_Key* Micro_Key::create(CCTMXTiledMap* _tileMap,int keyid,int keydir,int keycolorkind,const CCPoint& point){

	if(NULL == _tileMap){
		CCLog("Micro_Tool  create()   _tileMap IS  NULL ");
		return NULL;
	}

	Micro_Key *pRet = new Micro_Key();
	if (pRet && pRet->init(_tileMap,keyid,keydir,keycolorkind,point)) {
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

bool Micro_Key::init(CCTMXTiledMap* _tileMap,int keyid,int keydir,int keycolorkind,const CCPoint& point){
	if(!CCSprite::init())
		return false;

	this->setAnchorPoint(ccp(0.5,0.5));
	set_keyColor(keycolorkind);
	set_keyid(keyid);
	set_keydir(keydir);
	CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("mirco/Animal/lockeyAIAM/MicroLockKey0.png",
		"mirco/Animal/lockeyAIAM/MicroLockKey0.plist","mirco/Animal/lockeyAIAM/MicroLockKey.ExportJson");
	ccar = CCArmature::create("MicroLockKey");

	switch (keydir)
	{
	case KEY_DIR_UP:
		if(keycolorkind == KEY_COLOR_YELLOW){
			ccar->getAnimation()->playByIndex(0);
		}
		else if(keycolorkind == KEY_COLOR_RED){
			ccar->getAnimation()->playByIndex(2);
		}
		break;
	case KEY_DIR_DOWN:
		if(keycolorkind == KEY_COLOR_YELLOW){
			ccar->getAnimation()->playByIndex(8);
		}
		else if(keycolorkind == KEY_COLOR_RED){
			ccar->getAnimation()->playByIndex(14);
		}
		break;
	case KEY_DIR_LEFT:
		if(keycolorkind == KEY_COLOR_YELLOW){
			ccar->getAnimation()->playByIndex(6);
		}
		else if(keycolorkind == KEY_COLOR_RED){
			ccar->getAnimation()->playByIndex(12);
		}
		break;
	case KEY_DIR_RIGHT:
		if(keycolorkind == KEY_COLOR_YELLOW){
			ccar->getAnimation()->playByIndex(4);
		}
		else if(keycolorkind == KEY_COLOR_RED){
			ccar->getAnimation()->playByIndex(10);
		}
		break;
	default:
		break;
	}
	CCLog("Micro_Key::init() KEY_COLOR_YELLOW-> pointX =%.2f;pointY = %.2f",point.x,point.y);
	ccar->setPosition(point);
	this->addChild(ccar);


	return  true;

}

void Micro_Key::playArmKey(){
	isHit = true;
	int keydir = get_keydir();
	switch (keydir)
	{
	case KEY_DIR_UP:
		if(get_keyColor() == KEY_COLOR_YELLOW){
			ccar->getAnimation()->playByIndex(1);
		}
		else if(get_keyColor() == KEY_COLOR_RED){
			ccar->getAnimation()->playByIndex(3);
		}
		break;
	case KEY_DIR_DOWN:
		if(get_keyColor() == KEY_COLOR_YELLOW){
			ccar->getAnimation()->playByIndex(9);
		}
		else if(get_keyColor() == KEY_COLOR_RED){
			ccar->getAnimation()->playByIndex(15);
		}
		break;
	case KEY_DIR_LEFT:
		if(get_keyColor() == KEY_COLOR_YELLOW){
			ccar->getAnimation()->playByIndex(7);
		}
		else if(get_keyColor() == KEY_COLOR_RED){
			ccar->getAnimation()->playByIndex(13);
		}
		break;
	case KEY_DIR_RIGHT:
		if(get_keyColor() == KEY_COLOR_YELLOW){
			ccar->getAnimation()->playByIndex(5);
		}
		else if(get_keyColor() == KEY_COLOR_RED){
			ccar->getAnimation()->playByIndex(11);
		}
		break;

	default:
		break;
	}


}


void Micro_Key::set_keydir(int pdir){
	this->keydir = pdir;
}

int Micro_Key::get_keydir(){
	return this->keydir;
}


void Micro_Key::set_keyColor(int color){
	this->keyColor = color;
}

int Micro_Key::get_keyColor(){
	return this->keyColor;
}

bool Micro_Key::getisHit(){
	return isHit;
}

void  Micro_Key::setIsHit(bool p_isHit){
	this->isHit = p_isHit;
}

void Micro_Key::set_keyid(int pId){
	this->keyid = pId;
}

int Micro_Key::get_keyid(){
	return this->keyid;
}

