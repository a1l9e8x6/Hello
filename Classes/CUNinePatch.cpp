#include "CUNinePatch.h"

using namespace cocos2d;
 
const unsigned int kLucencyTag = 255;	//透明像素的取值大于等于这个值，就是黑线标记 
   
CUNinePatch::CUNinePatch(){
}
CUNinePatch::~CUNinePatch(){
}
cocos2d::CCSprite * CUNinePatch::New(const char * file_name ,cocos2d::CCSize size){
	CUNinePatch * nine_patch = new CUNinePatch();
	cocos2d::CCSprite * temp_sprite = NULL;
	if( nine_patch ){
	  temp_sprite =	nine_patch->Init( file_name,size );
	}
	CC_SAFE_DELETE(nine_patch);
	return temp_sprite;
}
cocos2d::CCSprite * CUNinePatch::New(const char * file_name,cocos2d::CCSize size ,cocos2d::CCRect rect){
	CUNinePatch * nine_patch = new CUNinePatch();
	cocos2d::CCSprite * temp_sprite = NULL;
	if( nine_patch ){
		temp_sprite =	nine_patch->Init( file_name,size ,rect );
	}
	CC_SAFE_DELETE(nine_patch);
	return temp_sprite;
}

cocos2d::CCSprite * CUNinePatch::Init(const char * file_name ,cocos2d::CCSize size){ 
	 //读取当前精灵
	 cocos2d::CCSprite * temp_sprite = cocos2d::CCSprite::create( file_name );
	 if(!temp_sprite)
		 return NULL; 
	 cocos2d::CCRect temp_rect =  GetIncisionRect( temp_sprite );	//得到矩形
   CCScale9Sprite * tmpSprite  = GetScaleSprite( file_name,  temp_sprite ,temp_rect);
	 if(!tmpSprite)
		 return NULL;
	 addChild(tmpSprite);	//最后会释放
	

	 //需要设置缩放精灵缩放后的大小    并且 矫正大小和缩放
	 ModulationSize(tmpSprite, temp_sprite->getContentSize() , size); 
	 tmpSprite->setAnchorPoint(ccp(0,0));	//需要设置锚点位置  
	 cocos2d::CCSprite * n_sprite = GetSpriteWithScale( tmpSprite );

	 tmpSprite->removeAllChildrenWithCleanup(true);
	 temp_sprite->removeAllChildrenWithCleanup(true);
	 temp_sprite->removeFromParentAndCleanup(true);
	 tmpSprite->removeFromParentAndCleanup(true);
	 return n_sprite;
}
cocos2d::CCSprite * CUNinePatch::Init(const char * file_name ,cocos2d::CCSize size,cocos2d::CCRect rect){ 
	//读取当前精灵
	cocos2d::CCSprite * temp_sprite = cocos2d::CCSprite::create( file_name );
	if(!temp_sprite)
		return NULL; 
	CCScale9Sprite * tmpSprite  = GetScaleSprite( file_name,  temp_sprite ,rect);
	if(!tmpSprite)
		return NULL;
	addChild(tmpSprite);	//最后会释放
	//需要设置缩放精灵缩放后的大小    并且 矫正大小和缩放
	ModulationSize(tmpSprite, temp_sprite->getContentSize() , size); 
	tmpSprite->setAnchorPoint(ccp(0,0));	//需要设置锚点位置  
	cocos2d::CCSprite * n_sprite = GetSpriteWithScale( tmpSprite );
	tmpSprite->removeAllChildrenWithCleanup(true);
	temp_sprite->removeAllChildrenWithCleanup(true);
	temp_sprite->removeFromParentAndCleanup(true);
	tmpSprite->removeFromParentAndCleanup(true);
	return n_sprite;
}


 //得到图片的 切割矩形  左上角为基准点
 cocos2d::CCRect CUNinePatch::GetIncisionRect( cocos2d::CCSprite * sprite ){
	 cocos2d::CCRect temp_rect;
	 if(!sprite)
		 return temp_rect;
	 //需要手动设置锯齿和锚点坐标  否则读取非抗锯齿像素不精准
	 cocos2d::CCTexture2D * texture = sprite->getTexture();
	 texture->setAliasTexParameters(); 
	 sprite->setAnchorPoint(ccp(0,0));
	 cocos2d::CCRenderTexture * render_texture = cocos2d::CCRenderTexture::create(
		 (int)sprite->getContentSize().width,(int)sprite->getContentSize().height );
	 if(!render_texture)
		 return temp_rect; 
	 render_texture->begin();
	 sprite->visit();
	 render_texture->end(); 

	 cocos2d::CCImage* final_image = render_texture->newCCImage();
	 if(!final_image)
		 return temp_rect;
	 unsigned char * pData = final_image->getData();

	 int i_index = 0;
	 //左上角到右下角，图片的坐标点
	  
	 unsigned int left_begin = 0;
	 unsigned int left_end = 0;
	 unsigned int down_begin = 0;
	 unsigned int down_end = 0;

	 for (unsigned int i = 0; i < final_image->getHeight(); i ++) {
		 for (unsigned int j = 0; j < final_image->getWidth(); j ++) {
			 //判断合法区域
			 if( i != 0 && j != 0 ){
				 //不是上面第一层，也不是左面第一层//略过这一排
				 //左上角数 一个像素4个int，包括rgbo所以 增加4个
				 i_index += 4;
				 continue;	 
			 }
			 //读取纹理，rgbo
			 int i_pos = i_index;
		//	 unsigned int iB = pData[i_index];
			 i_index ++;
		//	 unsigned int iG = pData[i_index];
			 i_index ++;
		//	 unsigned int iR = pData[i_index];
			 i_index ++;
			 unsigned int iO = pData[i_index];
			 i_index ++;

			 //判断是否满足条件
			 if(  iO < kLucencyTag )	//255就是非透明像素
				 continue;
			 //高方向
			 if( i == 0 ){
				 //记录横方向
				 if( left_begin == 0 ){
					 left_begin = j+1;	//记录第一个出现的标记点，因为左边上面增加了，所以左边+1下面的+1 
				 }
				 left_end++ ;	//按照点数，从第一个位置累加，其实可以记录最后一个点的位置减去起始位置点，目前累加
			 }else if( j == 0 ){	//宽方向 
				 //记录纵方向
				 if(  down_begin == 0 ){
					 down_begin = i +1;
				 }
				 down_end++; 
			 } 
		 }
	 }
	 temp_rect.setRect( left_begin,down_begin,left_end,down_end );
	 render_texture->removeAllChildrenWithCleanup(true);
	 render_texture->removeFromParentAndCleanup(true);
	 return temp_rect;
 }
 //缩放精灵
 CCScale9Sprite * CUNinePatch::GetScaleSprite( const char * file_name,  cocos2d::CCSprite * sprite,cocos2d::CCRect rect ){
	 CCScale9Sprite* scale_sprite = NULL;
	 //贴图大小
	 cocos2d::CCSize sprite_size = sprite->getContentSize();
	 //创建原始的图像对应大小矩形区域。
	 cocos2d::CCRect full_rect = cocos2d::CCRect(0,0, sprite_size.width, sprite_size.height);
	 //得到精灵切图矩形，锚点是左上角为基准点，宽高为缩放大小
	 cocos2d::CCRect inset_rect = rect;// GetIncisionRect( sprite );
	 if(inset_rect.size.width == 0 || inset_rect.size.height == 0)
		 return NULL;
	 //得到一个缩放后的精灵
	 scale_sprite = CCScale9Sprite::create(file_name,full_rect,inset_rect);
	 if(!scale_sprite)
		 return NULL;
	 return scale_sprite;
 }
 //得到切割后精灵
 cocos2d::CCSprite * CUNinePatch::GetSpriteWithScale(CCScale9Sprite * scale_sprite){
	 //得到渲染精灵
	 cocos2d::CCRenderTexture * render_texture = cocos2d::CCRenderTexture::create((int)scale_sprite->boundingBox().size.width,(int)scale_sprite->boundingBox().size.height);
	 if(!render_texture)
		 return NULL;
	 render_texture->begin();
	 scale_sprite->visit();
	 render_texture->end(); 
	 //得到选然后的图片
	 cocos2d::CCImage* t_image = render_texture->newCCImage();
	 if(!t_image)
		 return NULL;
	 //得到图片像素点
	 unsigned char * p_data = t_image->getData();
	 //做一个切边下标，遍历的时候，使用
	 int next_index = 0;

	 for (unsigned int i = 0; i < t_image->getHeight(); i ++) {
		 for (unsigned int j = 0; j < t_image->getWidth(); j ++) {
			 //判断合法区域
			 if( i != 0 && j != 0 ){
				 //不是上面第一层，也不是左面第一层//略过这一排
				 //左上角数 一个像素4个int，包括rgbo所以 增加4个
				 next_index += 4;
				 continue;	 
			 }
			 //读取纹理，rgbo
			 int i_pos = next_index;
			// unsigned int iB = pData2[n_iIndex];
			 next_index ++;
			// unsigned int iG = pData2[n_iIndex];
			 next_index ++;
		//	 unsigned int iR = pData2[n_iIndex];
			 next_index ++;
			 unsigned int iO = p_data[next_index];
			 next_index ++;
			 //判断是否满足条件 
			 if(  iO < kLucencyTag )
				 continue; 
			 //修改像素  
			 //将黑边切掉了，切成透明的
			 p_data[i_pos] = 0;// (unsigned char)iB;
			 p_data[i_pos + 1] = 0;// (unsigned char)iG;
			 p_data[i_pos + 2] = 0;// (unsigned char)iR;
			 p_data[i_pos + 3] = 0; 
		 }
	 }


	 cocos2d::CCTexture2D *texture = new cocos2d::CCTexture2D();
	 if(!texture)
		 return NULL; 
	 texture->initWithImage(t_image);
	 texture->setAntiAliasTexParameters();
	 cocos2d::CCSprite* newSprite = cocos2d::CCSprite::createWithTexture(texture);
	 if(!newSprite)
		 return NULL;
	 delete t_image;
	 texture->release();
	 render_texture->removeAllChildrenWithCleanup(true);
	 render_texture->removeFromParentAndCleanup(true);
	 return newSprite;
 }
 //基准坐标
 void CUNinePatch::ModulationSize(CCScale9Sprite * scale_sprite , cocos2d::CCSize old_size, cocos2d::CCSize new_size ){
	 if(!scale_sprite)
		 return;

	 cocos2d::CCSize find_size ;
	 float temp_scalex = 1.0f;
	 float temp_scaley = 1.0f;

	 // 如果是 00 的大小，就是原图
	 if( new_size.width == 0 && new_size.height == 0  )	{
		 find_size = old_size;
	 }else if( old_size.width < new_size.width && old_size.height < new_size.height ){
		 //如果新的宽高大于原图大小 ，就是新的的宽高大小
		 find_size = new_size;
	 }else if( old_size.width > new_size.width && old_size.height > new_size.height ){
		 //新宽高小于旧宽高  旧高度，并且缩放
		find_size = old_size;
		temp_scalex = new_size.width / old_size.width ;
		temp_scaley = new_size.height / old_size.height;
	 }else if( old_size.width > new_size.width  ){
		 //旧大与新的宽
		 find_size.width = old_size.width;		//宽保持一致
		 find_size.height = new_size.height;	//高来放大
		 temp_scalex = new_size.width / old_size.width ;	//保持一致的宽缩小
	 }else{
		 //旧大与新的高
		 find_size.width = new_size.width;		//宽来放大
		 find_size.height = old_size.height;	//高保持一致 
		 temp_scaley = new_size.height / old_size.height ;	//保持一致的高缩小
	 } 
	 //设置大小，很重要！ scale必须设置大小
	 scale_sprite->setContentSize( find_size );
	 scale_sprite->setScaleX(temp_scalex);
	 scale_sprite->setScaleY(temp_scaley);
 }





















// const int kTagNinePatchBatchNode      = 0xF0000100;
// #define CU_APPROXIMATE_ZERO_FLOAT (float)(1E-5)
// const int kZOrderMin =  -100;	//最小
// 
// CUNinePatch::CUNinePatch(){
// 	m_bIsNinePatch = false;
// 	m_bIsFixNinePatch = false;
// }
// 
// CUNinePatch::~CUNinePatch(){
// }
// 
// CUNinePatch *CUNinePatch::New(const std::string &name,bool bIsNinePatch) {
// 	return New(name, cocos2d::CCSizeMake(0.0,0.0),bIsNinePatch);
// }
// 
// CUNinePatch *CUNinePatch::New(const std::string &name, const cocos2d::CCSize &size,bool bIsNinePatch) {	
// 	CUNinePatch * np = new CUNinePatch();
// 	
// 	if(!np->InitWithName(name, size)){
// 		CC_SAFE_DELETE(np);
// 		return NULL;
// 	}
// 	np->autorelease();
// 	np->m_bIsNinePatch = bIsNinePatch;
// 	return np;
// }
// 
// CUNinePatch *CUNinePatch::NewWithClientSize(const std::string &name, const cocos2d::CCSize &client_size,bool bIsNinePatch) {
// 	CUNinePatch * np = new CUNinePatch();
// 	np->autorelease();
// 	np->m_bIsNinePatch = bIsNinePatch;
// 	if(!np->InitWithNameAndClientSize(name, client_size.width, client_size.height) ){
// 		return NULL;
// 	}  
// 	return np;
// }
// 
// CUNinePatch *CUNinePatch::NewWithClientWidth(const std::string &name, float client_width, float height,bool bIsNinePatch) {
// 	CUNinePatch * np = new CUNinePatch();
// 	np->autorelease();
// 	np->m_bIsNinePatch = bIsNinePatch;
// 	if(!np->InitWithNameAndClientSize(name, (float)client_width, 0.0, 0.0, (float)height) ){    
// 		return NULL;
// 	}  
// 	return np;
// }
// 
// cocos2d::CCTexture2D * CUNinePatch::GetTexture(const std::string &name) {
// 	cuPushTextureFormat(kCCTexture2DPixelFormat_RGBA8888);
// 	std::string fname = CCFileUtils::sharedFileUtils()->fullPathForFilename(name.c_str());
// 	cocos2d::CCTexture2D *texture = cocos2d::CCTextureCache::sharedTextureCache()->addImage(fname.c_str());
// 	cuPopTextureFormat();
// 
// 
// 	if( !texture )
// 		return NULL;
// 
// 	if(m_bIsNinePatch && !m_bIsFixNinePatch){		
// 		cocos2d::CCImage *image = new cocos2d::CCImage();
// 		if(image->initWithImageFile( (fname).c_str() )){
// 			fixNinePatch(image);
// 		} else { 
// #ifdef engine_debug
// 			cocos2d::CCMessageBox("failed to read image: %s","title" );
// 			CCLOG("failed to read image: %s", name.c_str());
// #endif   
// 		}
// 		CC_SAFE_DELETE(image);
// 	}	
// 	if(m_bIsNinePatch && !m_bIsFixNinePatch) {
// #ifdef engine_debug
// 		cocos2d::CCMessageBox("not nine patch texture","title" );
// 		CCLOG("not nine patch texture!");
// #endif    
// 		return NULL;
// 	}
// 	return texture;
// }
// 
// bool CUNinePatch::InitWithNameAndClientSize(const std::string &name, float client_width, float client_height, float width, float height) {
// 	image_name = name;
// 	CCTexture2D *texture = this->GetTexture(name);
// 	if(!texture)
// 		return false;
// 	CCSize size = CalculateContentSizeByClientSize(texture, CCSizeMake(client_width, client_height), CCSizeMake(width, height));
// 	InitWithTexture(texture, size);
// 	return true;
// }
// 
// cocos2d::CCSize CUNinePatch::CalculateContentSizeByClientSize(CCTexture2D *texture, const cocos2d::CCSize &client_size, const cocos2d::CCSize &size) {
// 	CCRect padding_rect = m_rectNinePatchStretch;  
// 	CCRect stretch_rect = m_rectNinePatchPadding; 
// 
// 	CCSize newsize = size;
// 	if(client_size.width!=0.0) {  //这里没问题
// 		newsize.width = CalculateContentSizeByClientSize(texture->getContentSize().width, client_size.width,
// 			stretch_rect.origin.x, stretch_rect.origin.x + stretch_rect.size.width,
// 			padding_rect.origin.x, padding_rect.origin.x + padding_rect.size.width);
// 	}
// 	if(newsize.width==0.0)  //这里没问题
// 		newsize.width = texture->getContentSize().width;
// 
// 	if(client_size.height!=0.0){ //这里没问题
// 		newsize.height = CalculateContentSizeByClientSize(texture->getContentSize().height, client_size.height,
// 			stretch_rect.origin.y, stretch_rect.origin.y + stretch_rect.size.height,
// 			padding_rect.origin.y, padding_rect.origin.y + padding_rect.size.height);
// 	}
// 	if(newsize.height==0.0) //这里没问题
// 		newsize.height = texture->getContentSize().height;
// 
// 	return newsize;
// }
// 
// void CUNinePatch::SetContentSizeByClientSize(const cocos2d::CCSize &size) {
// 	cocos2d::CCSpriteBatchNode * batch_node = dynamic_cast<cocos2d::CCSpriteBatchNode *>(this->getChildByTag(kTagNinePatchBatchNode));
// 	if(!batch_node) 
// 		return ;
// 	CCSize newsize = CalculateContentSizeByClientSize(batch_node->getTexture(), size, getContentSize());
// 	setContentSize(newsize);
// }
// 
// bool CUNinePatch::InitWithName(const std::string &name, const CCSize &size) {
// 	image_name = name;
// 	CCTexture2D *texture = this->GetTexture(name);
// 	if(!texture)
// 		return false;
// 	return InitWithTexture(texture, size);
// }
// 
// bool CUNinePatch::InitWithTexture(CCTexture2D *texture, const cocos2d::CCSize &size) {  
// 	CCSize texture_size = texture->getContentSize();  
// 	CCSize final_size = size;
// 	// 使用原始尺寸
// 	float f_w = size.width - 0;
// 	float f_h = size.height - 0;
// 	if ( !(f_w > CU_APPROXIMATE_ZERO_FLOAT || f_w < -CU_APPROXIMATE_ZERO_FLOAT) ){
// 		final_size.width = texture_size.width-2;
// 	}
// 	if( !(f_h > CU_APPROXIMATE_ZERO_FLOAT || f_h < -CU_APPROXIMATE_ZERO_FLOAT)  ) { 
// 		final_size.height = texture_size.height - 2;
// 	}
// 
// 	CCSpriteBatchNode *batch_node = CCSpriteBatchNode::createWithTexture(texture, 9);  
// 	if(!batch_node)
// 		return false;
// 	batch_node->setAnchorPoint(cocos2d::CCPointZero);
// 	addChild(batch_node, kZOrderMin, kTagNinePatchBatchNode);
// 	 
// 	setContentSize(final_size);
// 	return true;
// }
// 
// void CUNinePatch::setContentSize(const CCSize &size){ 
// 	CCNode::setContentSize(size);
// 	cocos2d::CCSpriteBatchNode * batch_node = dynamic_cast<cocos2d::CCSpriteBatchNode *>(this->getChildByTag(kTagNinePatchBatchNode));
// 	if(!batch_node) 
// 		return ;
// 	//删除子节点
// 	batch_node->removeAllChildrenWithCleanup(true);
// 	if(size.width==0 || size.height==0)
// 		return;
// 	CCTexture2D *texture = batch_node->getTexture();
// 	if( !texture ) {
// #ifdef engine_debug
// 		cocos2d::CCMessageBox("texture is NULL","title" );
// 		CCLOG("texture is NULL!");
// #endif     
// 		return;
// 	}
// 
// 	CCSprite *child;
// 	CCRect padding_rect = m_rectNinePatchPadding;
// 
// 	CCSize texture_size = texture->getContentSize();
// 	// 尺寸相同，无需切割(去除上下左右各2个像素的边)
// 	if( size.width==texture_size.width-2 && size.height==texture_size.height-2 ) {
// 		child = CCSprite::createWithTexture(texture, CCRectMake(0.0, 0.0, size.width, size.height));
// 		//child = CCSprite::spriteWithTexture(texture, CCRectMake(0.0, 0.0, size.width+2, size.height+2));
// 		if(child) {
// 			child->setAnchorPoint(ccp(0.0,0.0));
// 			batch_node->addChild(child);
// 		}
// 		client_rect = padding_rect; //CCRectMake(0.0, 0.0, padding_rect..width, final_size.height);
// 		//原图处理结束
// 		return;
// 	}
// 	float w, h,w2,h2;  
// 	CCRect stretch_rect = m_rectNinePatchStretch; //texture->getNinePatchStreachRect();  
// 	//texture的上下左右各有2像素的边应该去掉，而stretch_rect是没有计算边框的(border stripped)
// 	//所以应该是width/height-4
// 	//注意：虽然理论是可以的，下面的是进行过缩放的，也就是缩小的内容小于本身的区域不会裁剪，而是拉伸
// 	float xx[] = {0.0, stretch_rect.origin.x, stretch_rect.origin.x + stretch_rect.size.width, texture_size.width-2};
// 	float yy[] = {0.0, stretch_rect.origin.y, stretch_rect.origin.y + stretch_rect.size.height, texture_size.height-2};
// 
// 	// 如果目标尺寸比9patch的最小还要小，就只能整体scale down了  
// 	//Update: NO,不能scale down! 当contentsize小于texture size时，应该不缩小，而是裁剪！ TODO v1.0
// 	bool is_can_scale = false;
// 	if( (w = xx[1] + xx[3]-xx[2]) > size.width ) {
// 		if( xx[1] >= size.width ) { // 最左侧的一块的一部分就足够了( [xx[0], size.width] )
// 			xx[2] = xx[3] = xx[1] = size.width;
// 		} else {  // 左侧的一块，以及右侧的一块的一部分 ( [xx[2], size.width-xx[1]] )
// 			xx[3] = xx[2] + size.width - xx[1];
// 		}
// 		is_can_scale = true;	//这是没有进行裁剪！！ 而是拉伸  TODO v2
// 	}
// 	if( (h = yy[1] + yy[3]-yy[2]) > size.height) {
// 		if( yy[1] >= size.height) {
// 			yy[2] = yy[3] = yy[1] = size.height;
// 		} else {
// 			yy[3] = yy[2] + size.height - yy[1];
// 		}
// 		is_can_scale = true;
// 	}
// 
// 
// 	if( is_can_scale ){
// 		//左面一个像素，上面一个像素，宽的就是左边1+右面1
// 		child = CCSprite::createWithTexture(texture, CCRectMake(1.0, 1.0, texture_size.width - 2, texture_size.height-2));	
// 		if(!child){
// #ifdef engine_debug
// 			cocos2d::CCMessageBox("ninepatch texture is NULL","title" );
// 			CCLOG("ninepatch texture is NULL!");
// #endif   
// 			return;
// 		}
// 
// 		child->setAnchorPoint(ccp(0.0,0.0));
// 		batch_node->addChild(child);
// 
// 		cocos2d::CCSize limited_size	= size;	
// 		cocos2d::CCSize org_size = texture_size;
// 		float scale_x=1, scale_y=1;
// 		if(limited_size.width!=0)
// 			scale_x = MIN(1, limited_size.width/org_size.width);
// 		if(limited_size.height!=0)
// 			scale_y = MIN(1, limited_size.height/org_size.height);
// 		child->setScaleX(scale_x);
// 		child->setScaleY(scale_y);   
// 		client_rect = padding_rect;
// 		//缩小处理结束
// 		return;
// 	}
// 
// 	float xx2[] = {xx[0], xx[1], size.width-(xx[3]-xx[2]), size.width};
// 	float yy2[] = {yy[0], yy[1], size.height-(yy[3]-yy[2]), size.height};
// 
// 
// 	// 添加子节点
// 	for(int i = 0; i<3; i ++){ //y
// 		h = yy[i+1]-yy[i]; h2 = yy2[i+1]-yy2[i];
// 		if(h==0)continue;
// 		for(int j = 0;j<3;j++){ //x
// 			w = xx[j+1]-xx[j]; w2 = xx2[j+1]-xx2[j]; 
// 			if( w<=0|| h<=0 ) continue;
// 
// 			//注意，spriteWithTexture参数中的rect，是从左上角开始计算的，而不是左下角!!! 坑爹啊!!!
// 			//并且要去除1像素边框，所以。。。
// 			//(texture_size.height-4)为实际高度（去除边框)
// 			//-1-yy[i]为y轴反转后坐标（此时高度为-）
// 			//-h+1则将高度转为正
// 			//最后+2加上边框
// 			child = CCSprite::createWithTexture(texture, CCRectMake(xx[j]+1,((texture_size.height-4)-1-yy[i]-h+1)+3,w,h));
// 			if(child){        
// 				child->setAnchorPoint(ccp(0.0,0.0));
// 				child->setPosition(ccp(xx2[j],yy2[i]));
// 				child->setScaleX(w2/w);
// 				child->setScaleY(h2/h);
// 				batch_node->addChild(child);
// 			}else{
// #ifdef engine_debug
// 				CCLOG("failed to create sprite in nine patch: %d %d", i, j);
// #endif    
// 			}
// 		}
// 	}     
// 	//计算 client rect  
// 	float x1 = CalculateStretchedPosition(xx, xx2, padding_rect.origin.x);
// 	float x2 = CalculateStretchedPosition(xx, xx2, padding_rect.origin.x + padding_rect.size.width);
// 	float y1 = CalculateStretchedPosition(yy, yy2, padding_rect.origin.y);
// 	float y2 = CalculateStretchedPosition(yy, yy2, padding_rect.origin.y + padding_rect.size.height);
// 	client_rect = CCRectMake(x1, y1, x2-x1+1, y2-y1+1);
// }
// 
// 
// 
// 
// void CUNinePatch::LimitContentWidth(float max_width, float min_width) {
// 	float width = getContentSize().width;
// 	if( max_width && width > max_width ) {
// 		setContentSize(CCSizeMake(max_width, getContentSize().height));
// 	} 
// 	else if( min_width && width < min_width ) {
// 		setContentSize(CCSizeMake(min_width, getContentSize().height));
// 	}
// }
// 
// float CUNinePatch::CalculateStretchedPosition(float xx[], float xx2[], float pos) {
// 	float scale;
// 	if( pos <= xx[1]) { //左
// 		scale = (float)(xx[1]==xx[0] ? 1.0 : (xx2[1]-xx2[0])/(xx[1]-xx[0]));
// 		return pos * scale;
// 	}  
// 	if( pos <= xx[2] ) { //中
// 		scale = (float)(xx[2]==xx[1] ? 1.0 : (xx2[2]-xx2[1])/(xx[2]-xx[1]));
// 		return xx2[1] + (pos-xx[1]) * scale;
// 	}
// 	//右
// 	scale = (float)(xx[3]==xx[2] ? 1.0 : (xx2[3]-xx2[2])/(xx[3]-xx[2]));
// 	return xx2[2] + (pos-xx[2]) * scale;
// }
// 
// float CUNinePatch::CalculateContentSizeByClientSize(float org_size, float client_size, 
// 	float stretch1, float stretch2, float padding1, float padding2) {
// 		/************************************************************************
// 		0       p1   s1             p2    s2               org_size
// 		________|____|_______________|____|__________________
// 		|                                                   |
// 
// 		0       p1   s1                   p2     s2               new_size
// 		________|____|_____________________|______|__________________
// 		|                                                           |
// 		|<------ client_size ----->|
// 		************************************************************************/
// 
// 		//padding size固定的，只能返回原始尺寸
// 		if((padding1<=stretch1 && padding2<=stretch1) || (padding1>=stretch2 && padding2>=stretch2)) {    
// #ifdef engine_debug
// 			CCLOG("padding size is not stretchable" );
// #endif    
// 			return org_size;
// 		}
// 
// 		// 去掉左侧无法stretch部分
// 		if(padding1 < stretch1) {
// 			client_size -= stretch1 - padding1;
// 			//padding1 = stretch1;
// 		}
// 		// 去掉右侧无法stretch部分
// 		if(padding2 > stretch2) {
// 			client_size -= padding2 - stretch2;
// 			//padding2 = stretch2;
// 		}
// 		return stretch1 + (org_size - stretch2) + client_size;
// 		//return stretch1 + (org_size - padding1) + client_size;
// }
// 
// 
// 
// 
// void CUNinePatch::fixNinePatch(CCImage *image)
// {
// 	if( !m_bIsNinePatch || m_bIsFixNinePatch )
// 		return;
// 
// 	//	if(m_pTexture2D)
// 	{
// 		CCSize imgSize = CCSizeMake(image->getWidth(),image->getHeight());
// 		calculateNinePatchInfo(image->getData(), imgSize, kCCTexture2DPixelFormat_RGBA8888);
// 	}
// }
// 
// void CUNinePatch::calculateNinePatchInfo(const unsigned char *data, const CCSize &imgSize, CCTexture2DPixelFormat format)
// {    
// 	int bytesPerPixel = 0;      //bits per pixel
// 	int markMask = 0;
// 	int markValue = 0;
// 	switch(format){    
// 	case kCCTexture2DPixelFormat_RGBA8888:   // 0xAABBGGRRR, in fact RGBA in memory
// 		bytesPerPixel = 4; 
// 		markMask  = 0xFFFFFFFF;
// 		markValue = 0xFF000000;          
// 		break;
// 	case kCCTexture2DPixelFormat_RGB888:    
// 		bytesPerPixel = 3; 
// 		markMask  = 0xffffff;    
// 		markValue = 0;
// 		break;
// 	case kCCTexture2DPixelFormat_RGBA4444:   // 0xRGBA, in fact BARG in memory
// 		bytesPerPixel = 2;    
// 		markMask  = 0xFFFF;
// 		markValue = 0xF;
// 		break;
// 	case kCCTexture2DPixelFormat_RGB5A1:
// 		bytesPerPixel = 2;
// 		markMask = 0x1;
// 		break;
// 	case kCCTexture2DPixelFormat_RGB565:
// 		bytesPerPixel = 2;
// 		markMask  = 0xFFFF;
// 		markValue = 0;
// 		break;
// 	case kCCTexture2DPixelFormat_AI88:  // doesn't support nine patch
// 		bytesPerPixel = 2; 
// 		markMask = 0x0;
// 		markValue = 0x0;
// 		break;
// 	case kCCTexture2DPixelFormat_A8:  // doesn't support nine patch
// 		bytesPerPixel = 1;    
// 		markMask = 0x0;
// 		markValue = 0x0;
// 		break;
// 	default:    
// 		CCAssert(0, "NSInternalInconsistencyException");
// 	}
// 
// 	int x1,x2,y1,y2;
// 	int w = (int)(imgSize.width), h = (int)(imgSize.height);
// 	if( w<=4 || h<=4 )  // invalid 9-patch format
// 		return;
// 	//CCLog("be careful: data size if m_uPixelsWide * m_uPixelsHigh, not m_tContentSize!!!")
// 	int scan_line = (int)imgSize.width;
// 
// 	//top    
// 	searchMark(data+bytesPerPixel, bytesPerPixel, w-2, markMask, markValue, &x1, &x2);
// 	//left
// 	//m_uPixelsWide, not w!
// 	searchMark(data+bytesPerPixel * (scan_line), bytesPerPixel*scan_line, h-2, markMask, markValue, &y1, &y2);
// 	// h-4-y2-1, not y1!
// 	m_rectNinePatchStretch = CCRectMake((float)(x1+1),(float)(h-2-y2-1),(float)(x2-x1+1),(float)(y2-y1+1));
// 	//bottom
// 	searchMark(data+bytesPerPixel*scan_line*(h-1)+bytesPerPixel,bytesPerPixel, w-2, markMask, markValue, &x2, &x1);
// 	//right
// 	searchMark(data+bytesPerPixel*(w-1)+bytesPerPixel*scan_line, bytesPerPixel*scan_line, h-2, markMask, markValue, &y1, &y2);
// 	m_rectNinePatchPadding = CCRectMake((float)(x1+1), (float)(h-2-y2-1), (float)(x2-x1+1), (float)(y2-y1+1));
// 	m_bIsFixNinePatch = true;
// }
// 
// 
// void CUNinePatch::searchMark(const unsigned char *startpos, int step, int size, int markMask, int markValue, int *first, int *last) {  
// 	int i = -1;
// 	const unsigned char *pos;
// 	// left to right
// 	for(i = 0, pos = startpos; i < size; i ++, pos+=step) {
// 		if( ((*(int *)pos) & markMask) == markValue ){
// 			*first = i;
// 			break;
// 		}
// 	}  
// 	// can't find mark
// 	if( i == size ) {
// 		*first = 0;
// 		*last = size-1;
// 		return;
// 	}
// 	// right to left
// 	for(i = size-1, pos=startpos+(size-1)*step; i >= 0; i --, pos-=step) {
// 		if( ((*(int *)pos) & markMask) == markValue ){
// 			*last = i;
// 			break;
// 		}
// 	}
// 	CCAssert(i>=0, "searchMark error");
// }
// 
// 
// 
// std::vector<cocos2d::CCTexture2DPixelFormat> *texture_format_stack = NULL;
// 
// void CUNinePatch::cuPushTextureFormat(cocos2d::CCTexture2DPixelFormat format) {
// 	if(!texture_format_stack) {
// 		texture_format_stack = new std::vector<cocos2d::CCTexture2DPixelFormat>();
// 	}
// 	cocos2d::CCTexture2DPixelFormat old = cocos2d::CCTexture2D::defaultAlphaPixelFormat();
// 	texture_format_stack->push_back(old);
// 	cocos2d::CCTexture2D::setDefaultAlphaPixelFormat(format);
// }
// 
// void CUNinePatch::cuPopTextureFormat() {
// 	if(!texture_format_stack || texture_format_stack->size()==0)
// 		return;
// 	cocos2d::CCTexture2D::setDefaultAlphaPixelFormat((*texture_format_stack)[texture_format_stack->size()-1]);
// 	texture_format_stack->pop_back();
// }