//
//  ServerPrizeList.cpp
//  Hello
//
//  Created by bobo on 14-5-26.
//
//

#include "ServerPrizeList.h"



ServerPrizeList::ServerPrizeList(){
    set_table_visisbleWidth(720);
    set_table_visisbleHeight(450);
    set_tablex(CCDirector::sharedDirector()->getWinSize().width/2-720/2);
    set_tabley(-80);
}

ServerPrizeList::~ServerPrizeList(){
    
    
    
}



bool ServerPrizeList::init(){
    if(!CCLayer::init())
        return false;


    CCLog("get_table_visisbleWidth === %d",get_table_visisbleWidth());
    CCLayerColor* layercolor = CCLayerColor::create(ccc4(230, 20, 0, 100), get_table_visisbleWidth(), get_table_visisbleHeight());
    //layercolor->ignoreAnchorPointForPosition(false);//忽略 cclayercolor的原来左下角（0，0）的中心点；打开注释就是设置为居中，
    layercolor->setPosition(ccp(get_tablex(), get_tabley()));
    this->addChild(layercolor);

    
          
    CCTableView* tableview = CCTableView::create(this, CCSizeMake(get_table_visisbleWidth(), get_table_visisbleHeight()));
    tableview->setDirection(kCCScrollViewDirectionVertical);
    tableview->setDelegate(this);
    // 设置tableview cell的index排列，这里设置是从上到下变大，当然也可以从下到上变大。
     tableview->setVerticalFillOrder(kCCTableViewFillTopDown);
    tableview->setPosition(ccp(get_tablex(), get_tabley()));
    //tableview->reloadData();
    
    this->addChild(tableview);
    
    return true;
    
}



//触摸到某个cell的事件
void ServerPrizeList::tableCellTouched(CCTableView* table, CCTableViewCell* cell){
    int idx = cell->getIdx();
    CCLog("idx ==  %d",idx);
    
}

//定制每个cell的size
CCSize ServerPrizeList::tableCellSizeForIndex(CCTableView *table, unsigned int idx){
    
    return CCSizeMake(210, 100);
}

//定制每个cell的内容
CCTableViewCell* ServerPrizeList::tableCellAtIndex(CCTableView *table, unsigned int idx){
     char stri [20] = {0};
    sprintf(stri, "%d",idx);
    CCString* ccstri = CCString::createWithFormat("index = %d",atoi(stri));
    CCTableViewCell* cell = table->dequeueCell();
    
    if(!cell){
        cell = new CCTableViewCell();
        cell->autorelease();
        
        
        
        
        CCScale9Sprite* ccs9sprite = CCScale9Sprite::create("table/cellbg.png", CCRectMake(0, 0, 210, 91));
        ccs9sprite->setContentSize(CCSizeMake(get_table_visisbleWidth(), ccs9sprite->getContentSize().height));
       // CCSprite* spritebg = CCSprite::create("table/cellbg.png");
        ccs9sprite->setAnchorPoint(CCPointZero);
        ccs9sprite->setPosition(CCPointZero);
        cell->addChild(ccs9sprite,1);
        
        CCLabelTTF* label = CCLabelTTF::create(ccstri->getCString(), "TimesNewRomanPSMT", 25);
        label->setAnchorPoint(CCPointZero);
        label->setPosition(ccp(100,ccs9sprite->getContentSize().height/2-label->getContentSize().height));
        label->setTag(123);
        cell->addChild(label,2);
        
        CCLabelTTF* label2 = CCLabelTTF::create("脑残粉", "Helvetica-Bold", 25);
        label2->setAnchorPoint(CCPointZero);
        label2->setPosition(ccp(300,ccs9sprite->getContentSize().height/2-label->getContentSize().height));
        label2->setTag(123);
        cell->addChild(label2,2);

        
        
        
        CCMenuItemImage* image = CCMenuItemImage::create("table/change.png", "table/change2.png",this,menu_selector(ServerPrizeList::menuCallBack));
        CCMenu* pMenu = CCMenu::create(image,NULL);
        pMenu->setAnchorPoint(CCPointZero);
        pMenu->setPosition(ccp(-image->getContentSize().width/2+ccs9sprite->getContentSize().width,ccs9sprite->getContentSize().height/2));
        cell->addChild(pMenu,5);
        
    }else{
        CCLabelTTF* ttfs = (CCLabelTTF*)cell->getChildByTag(123);
        ttfs->setString(ccstri->getCString());
    }
    return cell;
}

void ServerPrizeList::menuCallBack(CCObject* sender){
    CCTableViewCell* cell = (CCTableViewCell*)((CCMenuItemImage*)sender)->getParent()->getParent();
    CCLog("menu idx === %d",cell->getIdx());
    
    
}

//确定这个tableview的cell行数
unsigned int ServerPrizeList::numberOfCellsInTableView(CCTableView *table){
    
    return 10;
    
}



//设置单元格高亮的状态
void ServerPrizeList::tableCellHighlight(CCTableView* table, CCTableViewCell* cell){
    
    
    CCScale9Sprite* h_ccs9sprite = CCScale9Sprite::create("table/cellbgh.png", CCRectMake(0, 0, 210, 91));
    h_ccs9sprite->setContentSize(CCSizeMake(get_table_visisbleWidth() , h_ccs9sprite->getContentSize().height));

    
    //CCSprite* hsprite = CCSprite::create("table/cellbgh.png");
    h_ccs9sprite->setTag(32);
    h_ccs9sprite->setOpacity(120);
    h_ccs9sprite->setAnchorPoint(CCPointZero);
    h_ccs9sprite->cocos2d::CCNode::setPosition(CCPointZero);
    cell->addChild(h_ccs9sprite,2);
    
}
//设置单元格非高亮的状态
void ServerPrizeList::tableCellUnhighlight(CCTableView* table, CCTableViewCell* cell){
    
    CCLog("unlight---");
    cell->removeChildByTag(32);

    

}

int ServerPrizeList::get_table_visisbleWidth(){
    return table_visisbleWidth;
}

void ServerPrizeList::set_table_visisbleWidth(int pWidth){
    this->table_visisbleWidth = pWidth;
}

int ServerPrizeList::get_table_visisbleHeight(){
    return table_visibleHeight;
}

void ServerPrizeList::set_table_visisbleHeight(int pHeight){
    this->table_visibleHeight = pHeight;
}

int ServerPrizeList::get_tablex(){
    return tablex;
}

void ServerPrizeList::set_tablex(int ptablex)
{
    this->tablex = ptablex;
}

int ServerPrizeList::get_tabley(){
    return tabley;
}

void ServerPrizeList::set_tabley(int ptabley){
    this->tabley = ptabley;
}





void ServerPrizeList::scrollViewDidScroll(CCScrollView* view){
    
    
}


void ServerPrizeList::scrollViewDidZoom(CCScrollView* view){
    
    
}

