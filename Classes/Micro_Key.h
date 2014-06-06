
#ifndef _Micro_Key_h
#define _Micro_Key_h


#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;


typedef enum {
	KEY_COLOR_YELLOW = 1,
	KEY_COLOR_RED ,
	KEY_COLOR_WHITE ,
}COLOR_KEY;


typedef enum {
	KEY_DIR_UP = 1,//SHOOT BALL FROM DOWN - UP DIR
	KEY_DIR_DOWN ,//SHOOT BALL FROM UP -DOWN DIR
	KEY_DIR_LEFT ,//SHOOT   BALL  FROM RIGHT - LEFT DIR
	KEY_DIR_RIGHT//SHOOT BALL  FROM LEFT - RIGHT DIR
}KEY_DIR;


class Micro_Key :public CCSprite{
public:
	Micro_Key();
	~Micro_Key();

	static Micro_Key* create(CCTMXTiledMap* _tileMap,int keyid,int keydir,int keycolorkind,const CCPoint& point);
    bool init(CCTMXTiledMap* _tileMap,int keyid,int keydir,int keycolorkind,const CCPoint& point);
	void playArmKey();


	bool getisHit();
	void setIsHit(bool p_isHit);
protected:
	CC_SYNTHESIZE_RETAIN(CCArmature*,ccar,_ccar);
	CC_PROPERTY(int,keydir,_keydir);
	CC_PROPERTY(int,keyColor,_keyColor);
    CC_PROPERTY(int, keyid, _keyid);
private:
	bool isHit;

};
#endif
