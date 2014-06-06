#include "ChessNode.h"
#define CIZE_ 	CCDirector::sharedDirector()->getWinSize()

ChessNode::ChessNode(){
	spw = sph = 110;
		init_chess_x = (CIZE_.width-spw*4)/2;
	init_chess_y = (CIZE_.height-sph*4)/2;
}
ChessNode::~ChessNode(){

}


/**自己的create函数，因为要带参数，那个子弹类也可以这么写*/
ChessNode* ChessNode::create(int row,int col,int value,bool created)
{
    ChessNode *pRet = new ChessNode();
    if (pRet && pRet->chessnodeInit(row,col,value,created)) {
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

bool ChessNode::chessnodeInit(int row,int col,int value,bool created){
	if(!CCSprite::init())
		return false;


	setRow(row);
	setCol(col);
	setValue(value);

	
	char str[20] = {0};//图片名
	sprintf(str,"2048/but%d.png",value);
	this->initWithFile(str);
	this->setPosition(ccp(init_chess_x+spw*col+getContentSize().width/2,init_chess_y+sph*row+getContentSize().height/2));
//	this->setAnchorPoint(ccp(0.5,0.5));
	if(created){
		
		this->setScale(0.5f);
		CCActionInterval * scaleto = CCScaleTo ::create(0.3f, 1);
		this->runAction(scaleto);
	}
	return true;

}

void  ChessNode::log_ces_info(const char* tip)
{
	CCLog("%s row = % d col = %d value = %d",tip,row,col,value);
}

