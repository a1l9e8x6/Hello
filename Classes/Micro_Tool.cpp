//
//  Micro_Tool.cpp
//  Hello
//
//  Created by bobo on 14-4-19.
//
//

#include "Micro_Tool.h"

Micro_Tool::Micro_Tool(){
    
	CCLog("Micro_Tool()Micro_Tool()");
}


Micro_Tool::~Micro_Tool(){
    
        
    
}


Micro_Tool* Micro_Tool::create(CCTMXTiledMap* _tileMap,int tool_kind,const CCPoint& point){
    
    if(NULL == _tileMap){
		CCLog("Micro_Tool  create()   _tileMap IS  NULL ");
		return NULL;
	}
    
    Micro_Tool *pRet = new Micro_Tool();
	if (pRet && pRet->init(_tileMap,tool_kind,point)) {
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



bool Micro_Tool::init(CCTMXTiledMap* _tileMap,int tool_kind,const CCPoint& point){
      if(!CCSprite::init())
		return false;
    
	  //set_toolx(point.x);
	 // set_tooly(point.y);
	  set_tool_kind(tool_kind);
    
	this->setAnchorPoint(ccp(0.5,0.5));
	if(tool_kind == TOOL_DIR_LEFT){
        this->initWithFile("mirco/dangban_left.png");
	}
	else if(tool_kind == TOOL_DIR_RIGHT){
         this->initWithFile("mirco/dangban_right.png");
	}
	
	this->setPosition(point)/*ccp(_tileMap->getTileSize().width/2,_tileMap->getTileSize().height/2)*/;
	CCLog("Micro_Tool::init() -> pointX =%.2f;pointY = %.2f",point.x,point.y);
    
    return  true;
    
}

int Micro_Tool::get_tool_kind()
{
	return tool_kind;
}

void Micro_Tool::set_tool_kind(int toolk)
{
	tool_kind = toolk;
}

//float Micro_Tool::get_toolx()
//{
//	return toolx;
//}
//
//void Micro_Tool::set_toolx(float x)
//{
//	toolx = x;
//}
//
//float Micro_Tool::get_tooly()
//{
//	return tooly;
//}
//
//void Micro_Tool::set_tooly(float y)
//{
//	tooly = y;
//}






