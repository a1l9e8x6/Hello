#ifndef _T2_H
#define _T2_H

#include  "cocos2d.h"
#include "cocos-ext.h"
#include "math.h"
#include "ChessNode.h"
#include "AudioManager.h"

USING_NS_CC;
USING_NS_CC_EXT;

#define SCR_SIZE 	CCDirector::sharedDirector()->getWinSize()

typedef enum {
	DIR_NONE = 0,
	DIR_LEFT = 1 ,//1 
	DIR_RIGHT = 2, //2
	DIR_UP = 3, //3
	DIR_DOWN = 4 //4
}GESTURE_DIR;

typedef enum {
	MOVE_NONE = 0,
	MOVE_YD1 ,//1
	MOVE_HETI,
	MOVE_YD2,
	MOVE_OVER,
}MOVE_STATE;

typedef enum {
	GAME_RUN = 0,
	GAME_OVER ,//1
}GAME_STATE;



// 
const static int MAX_ROW = 4;
// 
const static int MAX_COL = 4;

class T2 : public CCLayer
{
public:
	T2();
	~T2();
	static CCScene* scene();
    virtual bool init();
	void setupView();
	CREATE_FUNC(T2);
	virtual void ccTouchesBegan(CCSet* touch,CCEvent* event);
	virtual void ccTouchesMoved(CCSet* touch,CCEvent* event);
	virtual void ccTouchesEnded(CCSet* touch,CCEvent* event);

	//void registerWithTouchDispatcher();
	
	void update(float t);
	void draw();

	void callback_hecheng(CCNode* node );
	void addNewChess();



protected:
	void resetRandSeed();

private:

    void menuGameOverCallback(CCObject* pSender);    // a selector callback
	void menuGameExitCallback(CCObject* pSender);
	void initChess(int start ,int end);//[start,end]
	int dir_guester;//滑动方向
	CCArray *node_array;//主显示ccarray 注意不要在CCARRAY_FOREACH中对CCArray中的元素进行删除或添加。
	
	CCNode* cnode;

	void logcat_chessArr(char* movestate);
	
	bool isMoving; //用来区分啥时候生成新节点（很不好的做法，没办法了）
	void nodeMove(int dir_guester,int desRow,int desCol,int count);
	void callback_EleMove(CCNode* node );//menu_selcetor -> ccobject
    void callback_EleMove_YD2(CCNode* node);
	 void update_moveyd1();

	  bool bemoved_yd1;
	  bool becomposed;

	 void change(int dir,int row,int col,int value);
	 void removeChessnode(int dir,int row,int col);

	 bool isChessFull();//检测棋盘填满没
	int myRand(int start,int end);
	void getDiffRandNumer(int start,int end);//得到[STRART,END]2个不重复的随机数
	int move_state ;//棋子状态

	int game_state;

    void change2number(int& a,int& b);
	 void checkMove(int dir_guester);

	 //
	 void resetGame();
	bool checkFull();
	bool checkGameOver();

	CCLayerColor*  GAME_OVER_LAYER;

	bool isRightOver;
	bool over_check_move_right;
	bool over_check_heti_right;
	
	bool isLeftOver;
	bool over_check_move_left;
	bool over_check_heti_left;

	bool isUpOver;
	bool over_check_move_up;
	bool over_check_heti_up;

	bool isDownOver;
	bool over_check_move_down;
	bool over_check_heti_down;
    
    void gameover_btnCallback(CCNode* node);
	CCLabelBMFont* cclbmf_score;
	CCLabelBMFont* cclbmf_max_score;

	int current_score;
	int max_score;
	void addScore(int addnum);
	void saveData();
};

#endif