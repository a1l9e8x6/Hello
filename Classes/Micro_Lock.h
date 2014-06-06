#ifndef _Micro_Lock_h
#define _Micro_Lock_h

#include "cocos2d.h"
#include "cocos-ext.h"
#include "Micro.h"
#include "Micro_Key.h"

USING_NS_CC;
USING_NS_CC_EXT;

typedef enum {
	LOCK_COLOR_YELLOW = 1,
	LOCK_COLOR_RED ,
	LOCK_COLOR_WHITE ,
}LOCK;

typedef enum {
	LOCK_DIR_UP = 1,//SHOOT BALL FROM DOWN - UP DIR
	LOCK_DIR_DOWN ,//SHOOT BALL FROM UP -DOWN DIR
	LOCK_DIR_LEFT ,//SHOOT   BALL  FROM RIGHT - LEFT DIR
	LOCK_DIR_RIGHT//SHOOT BALL  FROM LEFT - RIGHT DIR
}LOCK_DIR;



class Micro_Lock :public CCSprite{
public:
	
	static Micro_Lock* create(CCTMXTiledMap* _tileMap,int lockid,int lockdir,int lockcolorkind,const CCPoint& point);
    bool init(CCTMXTiledMap* _tileMap,int lockid,int lockdir,int lockcolorkind,const CCPoint& point);

	 void getBallHitKeyNotification(CCObject* obj);//响应小球击中key的消息
	 void playParticleEffect();
protected:
	CC_SYNTHESIZE_RETAIN(CCArmature*,ccarlock,_ccarlock);
	CC_PROPERTY(int,lockColor,_lockColor);
	CC_PROPERTY(int,lockdir,_lockdir);
    CC_PROPERTY(int, lockid, _lockid);
	
	CC_PROPERTY(bool,lockstate,_lockstate);

private:
	Micro_Lock();
	~Micro_Lock();

};
#endif
