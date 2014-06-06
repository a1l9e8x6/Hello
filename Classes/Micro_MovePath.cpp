//
//  Micro_MovePath.cpp
//  Hello
//
//  Created by bobo on 14-4-19.
//
//

#include "Micro_MovePath.h"


bool Micro_MovePath::initWithDuration(float duration, const CCPoint& position){

    if (CCActionInterval::initWithDuration(duration))
	{
		m_endPosition = position; //这里得到圆心位置，就是要绕着哪里转
		return true;
	}
    
	return false;

}

void Micro_MovePath:: startWithTarget(CCNode *pTarget){

    CCActionInterval::startWithTarget(pTarget);
	m_startPosition = pTarget->getPosition();//这里得到刚开始的位置

}
void Micro_MovePath::update(float time){
    if (m_pTarget)
	{
		//这里的time就相当于当前时间占总的需要运行时间的百分比
		//这里轨迹就用一个圆 x=sin(t)  y=(sin(t) ,当时间走完就是一圈，2*180=360度

		float tempx=(m_startPosition.x-m_endPosition.x)*(m_startPosition.x-m_endPosition.x);
		float tempy=(m_startPosition.y-m_endPosition.y)*(m_startPosition.y-m_endPosition.y);
		float r=sqrt(tempx+tempy);
		float x=r*sin(2*3.14*time);
		float y=r*cos(2*3.14*time);
		m_pTarget->setPosition(ccpAdd(m_startPosition,ccp(x,y)));

	
	}
    

}

Micro_MovePath* Micro_MovePath::actionWithDuration(float duration, const CCPoint& position){

    Micro_MovePath * pmyAction=new Micro_MovePath();
	pmyAction->initWithDuration(duration, position);
	pmyAction->autorelease();
    
	return pmyAction;

}