//
//  Micro_Tool.h
//  Hello
//
//  Created by bobo on 14-4-19.
//
//

#ifndef Hello_Micro_Stone_Wall_h
#define Hello_Micro_Stone_Wall_h


#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

//#define STONE_WALL_TAG 10000

typedef enum {
	WALL_KIND_ONE = 1,
	WALL_KIND_TWO ,
	WALL_KIND_THREE ,
}WALL_KIND;



class Micro_StoneWall :public CCSprite{
public:
	Micro_StoneWall();
	~Micro_StoneWall();
	

	//void showPartichle();
	static Micro_StoneWall* create(CCTMXTiledMap* _tileMap,int wall_kind,const CCPoint& point);
    bool init(CCTMXTiledMap* _tileMap,int wall_kind,const CCPoint& point);
	void changeArmIndex();
	int hitcount;

protected:
	CC_PROPERTY_READONLY(bool,disappear,_disappear);
	CC_SYNTHESIZE_RETAIN(CCArmature*,ccar,_ccar);

};
#endif
