//
//  Micro_Tool.h
//  Hello
//
//  Created by bobo on 14-4-19.
//
//

#ifndef Hello_Micro_Tool_h
#define Hello_Micro_Tool_h


#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;


typedef enum {
    TOOL_DIR_LEFT = 1,
	TOOL_DIR_RIGHT ,
}TOOL_DIR;


void test();

class Micro_Tool :public CCSprite{
public:
	Micro_Tool();
	~Micro_Tool();
    
	static Micro_Tool* create(CCTMXTiledMap* _tileMap,int tool_kind,const CCPoint& point);
    bool init(CCTMXTiledMap* _tileMap,int tool_kind,const CCPoint& point);
    

protected:
	CC_PROPERTY(int,tool_kind,_tool_kind);
//	CC_PROPERTY(float , toolx,_toolx);
	//CC_PROPERTY(float , tooly,_tooly);

	
	//	CC_SYNTHESIZE_RETAIN(CCString*,nickname,Nickname);
	//	CC_SAFE_RELEASE_NULL(nickname);
};
#endif
