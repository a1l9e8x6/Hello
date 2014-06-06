//
//  ServerPrizeList.h
//  Hello
//
//  Created by bobo on 14-5-26.
//
//

#ifndef _ServerPrizeList_
#define _ServerPrizeList_

#include  "cocos2d.h"
#include "cocos-ext.h"
#include "math.h"

USING_NS_CC;
USING_NS_CC_EXT;

#define SCR_SIZE 	CCDirector::sharedDirector()->getWinSize()


class ServerPrizeList: public CCLayer,public CCTableViewDataSource,public CCTableViewDelegate{

public:
    
    ServerPrizeList();
    virtual bool init();
    CREATE_FUNC(ServerPrizeList);
    

    
public:
    virtual void scrollViewDidScroll(CCScrollView* view);
    virtual void scrollViewDidZoom(CCScrollView* view);
    
    
    //触摸到某个cell的事件
    virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
    
    //定制每个cell的size
    virtual cocos2d::CCSize tableCellSizeForIndex(CCTableView *table, unsigned int idx);
    
    //定制每个cell的内容
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
    
    //确定这个tableview的cell行数
    virtual unsigned int numberOfCellsInTableView(CCTableView *table);
    
    
    
    //设置单元格高亮的状态
    virtual void tableCellHighlight(CCTableView* table, CCTableViewCell* cell);
    //设置单元格非高亮的状态
    virtual void tableCellUnhighlight(CCTableView* table, CCTableViewCell* cell);


    //CCSize cellSizeForTable(CCTableView* table);
    //CCSize cellTableSizeForIndex(CCTableView* table,unsigned int idx);

    void menuCallBack(CCObject* sender);
    
private:
    virtual ~ServerPrizeList();
    CC_PROPERTY(int, table_visisbleWidth, _table_visisbleWidth);
    CC_PROPERTY(int, table_visibleHeight, _table_visisbleHeight);
    CC_PROPERTY(int, tablex, _tablex);
    CC_PROPERTY(int, tabley, _tabley);
};


#endif /* defined(__Hello__ServerPrizeList__) */
