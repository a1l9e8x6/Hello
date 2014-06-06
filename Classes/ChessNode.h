#ifndef _ChessNode_H
#define _ChessNode_H

#include  "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

class ChessNode :public CCSprite{
public:
	ChessNode();
	~ChessNode();
	
//	void ChessNode::ChangeNodeInfo(int row,int col,int value);

	int init_chess_x;
	int init_chess_y;
	int spw;
	int sph;

	static ChessNode* create(int row,int col,int value,bool created);//created 是否是新建出来的
	 bool chessnodeInit(int row,int col,int value,bool created);

	 void  log_ces_info(const char* tip);

protected:
	 CC_SYNTHESIZE(int, row, Row); 
	 CC_SYNTHESIZE(int ,col , Col);
	  CC_SYNTHESIZE(int ,value , Value);
	  CC_SYNTHESIZE(int ,x_moveDis,XMOVEDIS);
	  CC_SYNTHESIZE(int ,y_moveDis,YMOVEDIS);
};

#endif
