//
//  Micro_MovePath.h
//  Hello
//
//  Created by bobo on 14-4-19.
//
//

#ifndef Hello_Micro_MovePath_h
#define Hello_Micro_MovePath_h

#include "cocos-ext.h"
#include "cocos2d.h"

USING_NS_CC;
USING_NS_CC_EXT;

/*
 使用方法
 
 CCFiniteTimeAction *action5= Micro_MovePath::actionWithDuration(10,ccp(x+250,y+150));
 这样调用就可以了
 birdSpite->runAction(action5);
 
 */

class Micro_MovePath :public CCActionInterval{

public:
	/** initializes the action */
	bool initWithDuration(float duration, const CCPoint& position);
    
	virtual void startWithTarget(CCNode *pTarget);
	virtual void update(float time);
    
public:
	/** creates the action */
	static Micro_MovePath* actionWithDuration(float duration, const CCPoint& position);
    
protected:
	CCPoint m_endPosition;
	CCPoint m_startPosition;


};


#endif
