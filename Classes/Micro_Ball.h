#pragma once
#ifndef _MICRO_BALL_H
#define _MICRO_BALL_H

#include  "cocos2d.h"
#include "cocos-ext.h"
#include "Micro_Tool.h"

USING_NS_CC;
USING_NS_CC_EXT;


extern const float ball_speed;//声明在这里 定义在cpp

typedef enum {
	BALL_NONE = 0,
	BALL_UP ,
	BALL_DOWN ,
	BALL_LEFT ,
	BALL_RIGHT ,

}BALL_MOVE_DIR;

//typedef enum {
//	BALL_NONE = 0,
//	BALL_LEFT = 1,
//	BALL_RIGHT = 2,
//	BALL_UP = 3,
//	BALL_DOWN = 4,
//}BALL_MOVE_DIR;


class Micro_ball :public CCSprite{
public:
	Micro_ball();
	~Micro_ball();
	
	//static Micro_ball* create(CCTMXTiledMap* _tileMap,const char* filename,const CCPoint& point);
	static Micro_ball* create(CCTMXTiledMap* _tileMap,const char* filename,const char* groupName,const char* objectName); 

	 bool ballInit(CCTMXTiledMap* _tileMap,const char* filename,const char* groupName,const char* objectName);




	// void ballchangedir(CCTMXTiledMap* _tileMap,Micro_Tool* tool);
    
	 void changeballdir();

	 //void ball_delta(CCPoint delta);

    void ball_move();
    void update(float t);//schedule_selector 函数必须是float t 

	int getball_dir();
	void setball_dir(int dir);
protected:
	CC_PROPERTY(int,tool_dir,_tool_dir);
	CC_PROPERTY(bool,check_tool,_check_tool);//和道具发生碰撞

private:
    
	CCParticleSystemQuad *particle_ball;
    void getNotification(CCObject* obj);//拖动地图时捕获消息
    double delta_x_withmap;
    double delta_y_withmap;
	 int  ball_dir;
    
   //goushi xie fa fuck!
    int mapw;
    int maph;

};

#endif
