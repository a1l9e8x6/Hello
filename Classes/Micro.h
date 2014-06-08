#pragma once
#ifndef _Mirco_H
#define _Mirco_H

#include  "cocos2d.h"
#include "cocos-ext.h"

#include "MyCCLog.h"
#include "Micro_Tool.h"
#include "Micro_Ball.h"
#include "Micro_StoneWall.h"
#include "Micro_Lock.h"
#include "ButtonSprite.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

#define SCR_SIZE 	CCDirector::sharedDirector()->getWinSize()
#define SCR_CENTER_POINT CCPointMake(CCDirector::sharedDirector()->getWinSize().width/2,CCDirector::sharedDirector()->getWinSize().height/2)
#define tile_one_w  32
#define tile_one_h  32
#define MEN_TAG 998

#define K_TAG_TIPS 1020

#define max_mscale 1.25
#define min_mscale 0.75

#define MAP_NOTIFICATION "MAP_NOTIFICATION"
#define MICRO_KEY_NOTIFICATION "MICRO_KEY_NOTIFICATION"


//extern const int tile_onew ;//global variable (const can not with static )


//const数据成员的初始化只能在类的构造函数的初始化表中进行。要想建立在整个类中都恒定的常量，应该用类中的枚举常量来实现




typedef enum {
    GAMESTATE_NONE = 0,
	GAMESTATE_RUN ,
	GAMESTATE_OVER ,//2
}GAME_STATE_MICRO;

//size_t writeData(void* buffer,size_t size,size_t nmemb,void* userp);


class Micro : public CCLayer
{
public:

	static CCScene* scene();
    virtual bool init();
	CREATE_FUNC(Micro);

    //static Micro* shareMicroManager();

	std::vector<string> vect_str;

	//暂时没用
	std::vector<Micro_ball*>  vector_ball;
    vector<int>::iterator iter;
    
    virtual void ccTouchesBegan(CCSet* pTouches,CCEvent* event);
	virtual void ccTouchesMoved(CCSet* pTouches,CCEvent* event);
	virtual void ccTouchesEnded(CCSet* pTouches,CCEvent* event);
	
  //  CCPoint tilePosFromLocation( const CCPoint& location, CCTMXTiledMap* tileMap );

    void setViewpointCenter(CCPoint position);
    
	void update(float t);
    
	void draw();

  	 int map_total_w;
    int map_total_h;


private:
	Micro();
	~Micro();

    void TipMove();
	void getLevelNotification(CCObject* obj);//关卡选择时捕获消息

	CCSprite* bg_left_sprite ;//左侧的背景图

	CCSprite* xuanze_sprite;//选择框
    
    bool check_xuanze(const CCPoint& pPoint);//检查是否可以选择
	int xuanze_count;//选择计数器 控制反复点击同一个点的问题


    void menuRetGameCallback(CCObject* pSender);    // a selector callback
	void menuPauseGameCallback(CCObject* pSender);    // a selector callback
	
	  void menuLeftDangBanCallback(CCObject* pSender);
	  void menuRightDangBanCallback(CCObject* pSender);

	CCNode* ccnode_left;//左侧试图节点 --可以去掉
	CCNode* cnodeball; //add ball
	CCNode* ccnodetool;//add tool
	CCNode* ccnode_stonewall;//add stonewall;
	CCNode* ccnodekey;//add key;
	CCNode* ccnodelock;//add lock


	void initGame();

	//门的位置
	CCPoint menPos;
	//例子 获取门的位置
	CCPoint getParitclePos(CCTMXTiledMap* _tileMap,const char* groupName,const char* objectName);

	//检测挡板的碰撞
    bool checkTool(CCTMXTiledMap* _tileMap,Micro_ball* sprite);

	//检测与钥匙的碰撞
	bool checkKey(CCTMXTiledMap* _tileMap,Micro_ball* pball);

	//检测与锁的碰撞
	bool checkLock(CCTMXTiledMap* _tileMap,Micro_ball* pball);

	//
	void loadLock(CCTMXTiledMap* _tileMap);

	//
	void loadStoneWall(CCTMXTiledMap* _tileMap);

	//
	void loadkey(CCTMXTiledMap* _tileMap);

	bool checkCollStoneWall(CCTMXTiledMap* _tileMap,CCSprite* sprite);

	void loadMap(int _LEVEL);
	CCTMXTiledMap* _tileMap;
	int game_state;

    //CC_SYNTHESIZE_RETAIN(cocos2d::CCSprite*, _player, Player);
   
    CCPoint touch_oldPos;
    CCPoint touch_newPos;
	CCSprite*  getTouchSpot(CCTMXTiledMap* _tileMap,const char* layename,const CCPoint& touchCCpoint);
    bool Collision(CCTMXTiledMap* _tileMap,CCSprite* sprite,bool isBall);//isRelative  «∑Ò «œ‡µ±”⁄µÿÕºµƒ◊¯±Í
    
	void gameOver();

	void left_view(int level_);

	void resetGame(int level_);
    //void registerWithTouchDispatcher();
    void gameover_btnCallback(CCNode* node);//游戏结束弹出框

	void nextLevel();

    int init_mapx;
    int init_mapy;
    
	//EmmiterBall
	void herocreate(CCTMXTiledMap* _tileMap,const char* filename,const char* groupName,const char* objectName);

	bool mapisdragorscale;
	bool show_xuanze;//是否可以选中选择框
	void  collision_ball(float t);
    
    int gameState ;
	int level;
    int level_leftpiece;//左边的挡板
	int level_rightpiece;//右侧的挡板
	void readdangbaninfo(int level_);//读取挡板信息
        
   double distance;
   double mscale;
   double deltax;
   double deltay;
    
    void onExit();
    
    
    //tip move
    void tipMoveCallback(CCNode* pSender);
    
public:
    CCNode* BTN_NODE;
    void btncall();
      
    //test
	CCPoint testPoint;
	CCPoint testxuanzePoint;
	CCRect testRect;
	//void saveData();
    
        
};

#endif