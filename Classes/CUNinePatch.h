#ifndef _NINE__PATCH_H_
#define _NINE__PATCH_H_


#include "cocos2d.h"


//注意：无法使用cocos-ext.h的问题，右击项目属性
//右击项目属性->配置属性->C/C++->常规->附加包含目录->$(SolutionDir)extensions
//右击项目属性->配置属性->连接器->输入->附加依赖项->libExtensions.lib
//using namespace extension;

#include "cocos-ext.h"
USING_NS_CC_EXT;

/************************************************************************/
/*	

		2013.12.13 chuan

		九宫格思想 上下左右，中心，分别为拉伸区域  左上右上左下右下为非拉伸区域
		传入9patch做的文件（注意！ 图片必须是严格像素黑色，透明通道才能是是255
		且必须是255，所以 不能近似黑色，需要下载这个软件），然后读取到切割像素位置
		和切割像素大小，通过大小可以生成切割图片。其中开始需要设置锯齿像素，否则
		像素值不精准，拉伸后，那个左上角切完之后，得还原抗锯齿否则黑边

		利用的是CCScale9Sprite首先 解决了计算缩放位置大小的问题，用工具就可以
		然后解决的是图片缩小的偏移的 bug，虽然是拉伸，但使用者未必需要关心大小

		适用：气泡，自适应大小尺寸，规则图片 节省资源

		注意！！ 这样的适配，对于等比 压缩像素的时候，会出现黑线，这个黑线来自于
		系统使用这个类的时候，会有float的转化问题，1.1f和1.9f对计算机来说，转为int
		都是1，这对于像素操作是需要注意的 
		CCEGLView::sharedOpenGLView()->setDesignResolutionSize


*/
/************************************************************************/


class CUNinePatch : public cocos2d::CCNode{
public:
	virtual ~CUNinePatch();

	//图片 大小
	static cocos2d::CCSprite * New(const char * file_name ,cocos2d::CCSize size);
	//手动计算输入 图片的左上角宽高的点，缩放到size大小
	static cocos2d::CCSprite * New(const char * file_name ,cocos2d::CCSize size ,cocos2d::CCRect rect);
	 
protected: 
	//大小
	cocos2d::CCSprite * Init(const char * file_name ,cocos2d::CCSize size);
	//位置大小
	cocos2d::CCSprite * Init(const char * file_name ,cocos2d::CCSize size ,cocos2d::CCRect rect);


	//得到切割矩形
	cocos2d::CCRect GetIncisionRect( cocos2d::CCSprite * sprite );
	//得到一个缩放精灵
	CCScale9Sprite * GetScaleSprite( const char * file_name, cocos2d::CCSprite * sprite ,cocos2d::CCRect rect);
	//得到切割后精灵
	cocos2d::CCSprite * GetSpriteWithScale(CCScale9Sprite * scale_sprite);
	//基准坐标 
	void ModulationSize(CCScale9Sprite * scale_sprite, cocos2d::CCSize old_size, cocos2d::CCSize new_size );
 
private:
	CUNinePatch();
};




























//#include <string>
//
//
///************************************************************************/
///* 
//
////注意在 ccconfig.h里面找到CC_FIX_ARTIFACTS_BY_STRECHING_TEXEL这个将这个参数设置为1
////注意都需要增加  拉伸区域  使用ninepatch图片
//
//		使用方法：New出来一个对象，推荐使用带size的，默认情况下需要ninepatch 这个可以当作精灵被使用
//
//		通过CentralizeChildAndLimitSize可以限制node在内容区域
//		通过GetClientRect可以得到内容区域，这个区域规定label的大小
//		其他方法可以参考使用
//
//
//		protected 是构造暂时封闭了，公有化使用
//
//*/
///************************************************************************/
//
//class CUNinePatch : public cocos2d::CCNode{
//public:
//  virtual ~CUNinePatch();
//
//	static CUNinePatch *New(const std::string &name,bool bIsNinePatch=true);	//bIsNinePatch 表示是否开启9妹功能	
//	//推荐带size的
//	static CUNinePatch *New(const std::string &name, const cocos2d::CCSize &size,bool bIsNinePatch=true);
//
//
//
//	// 并且限制其大小为内容区域的尺寸
//	inline void CentralizeChildAndLimitSize(cocos2d::CCNode *child, bool lock_ratio = true) {
//		CentralizeChild(child);
//		cocos2d::CCSize limited_size	= GetClientRect().size;
//		cocos2d::CCSize org_size = child->getContentSize();
//		float scale_x=1, scale_y=1;
//		if(limited_size.width!=0)
//			scale_x = MIN(1, limited_size.width/org_size.width);
//		if(limited_size.height!=0)
//			scale_y = MIN(1, limited_size.height/org_size.height);
//
//		if(lock_ratio) {
//			scale_x = scale_y = MIN(scale_x, scale_y);
//		}
//		child->setScaleX(scale_x);
//		child->setScaleY(scale_y);    
//	}  
//
//
//	// 返回内容区域(padding box)
//	inline const cocos2d::CCRect &GetClientRect() const { return client_rect; }
//	// 返回内容区域的中心坐标，相对于整个node的原点(左下角)
//	inline cocos2d::CCPoint GetClientRectCenter() const {
//		return ccp(client_rect.origin.x + client_rect.size.width/2, client_rect.origin.y + client_rect.size.height/2);
//	}
//	// 将子node的中心放在内容区域的正中间
//	inline void CentralizeChild(cocos2d::CCNode *child) {
//		child->setAnchorPoint(ccp(0.5,0.5));
//		child->setPosition(GetClientRectCenter());
//	}
//	// 水平左对齐，垂直居中
//	inline void LeftCenterChild(cocos2d::CCNode *child) {
//		child->setAnchorPoint(ccp(0.0,0.5));
//		child->setPosition(ccp(client_rect.origin.x, GetClientRectCenter().y));
//	}
//	inline void RightCenterChild(cocos2d::CCNode *child) {
//		child->setAnchorPoint(ccp(1.0,0.5));
//		child->setPosition(ccp(client_rect.origin.x+client_rect.size.width, GetClientRectCenter().y));
//	}
//	inline void CenterCenterChild(cocos2d::CCNode *child,float tmp) {
//		child->setAnchorPoint(ccp(0.5,0.5));
//		child->setPosition(ccp(client_rect.origin.x+client_rect.size.width/2, GetClientRectCenter().y +tmp/3));
//	}
//
//	// 重新初始化为指定的大小（如果width或height为0，则不会)
//	virtual void setContentSize(const cocos2d::CCSize &size);
//	void SetContentSizeByClientSize(const cocos2d::CCSize &size);
//	// 将content width限制在min_width, max_width之间
//	// max_width/min_width 如果为0，表示不限制
//	void LimitContentWidth(float max_width, float min_width=0);
//
//protected:
//	//这里可以对外开放使用
//
//	//根据ClientSize，来决定ContentSize
//	static CUNinePatch *NewWithClientSize(const std::string &name, const cocos2d::CCSize &client_size,bool bIsNinePatch=false);
//	//根据client width决定最终宽度 高度则使用指定的高度，如果为0则使用原始图片高度
//	static CUNinePatch *NewWithClientWidth(const std::string &name, float client_width, float height = 0.0,bool bIsNinePatch=false);
//	 
//private:
//	
//	CUNinePatch();
//	CUNinePatch(const CUNinePatch &nine_path);
//	CUNinePatch &operator=(const CUNinePatch &nine_path);
//
//	void fixNinePatch(cocos2d::CCImage *image);
//	void searchMark(const unsigned char *startpos, int step, int size, int markMask, int markValue, int *first, int *last);
//	void calculateNinePatchInfo(const unsigned char *data, const cocos2d::CCSize &dataRectSize, cocos2d::CCTexture2DPixelFormat format);
//
//	bool InitWithName(const std::string &name, const cocos2d::CCSize &size);
//	bool InitWithNameAndClientSize(const std::string &name, float client_width, float client_height, float width=0.0, float height=0.0);
//	bool InitWithTexture(cocos2d::CCTexture2D *texture, const cocos2d::CCSize &size);
//	cocos2d::CCTexture2D *GetTexture(const std::string &name);
//	float CalculateStretchedPosition(float xx[], float xx2[], float pos);
//
//	//计算ContentSize
//	//org_size 9patch图片的原始尺寸(宽或高)
//	//client_size 需要达到的clientrect尺寸
//	//stretch1,stretch2: stretch rect的两个点. stretch1 <= stretch2
//	//padding1,padding2: padding rect的两个点. padding1 <= padding2
//	float CalculateContentSizeByClientSize(float org_size, float client_size, 
//		float stretch1, float stretch2, float padding1, float padding2);
//
//	// 如果client width/height为0，则使用size.width/height， 如果还是为0，则使用图片默认width/height
//	cocos2d::CCSize CalculateContentSizeByClientSize(cocos2d::CCTexture2D *texture, const cocos2d::CCSize &client_size, const cocos2d::CCSize &size);
//	cocos2d::CCRect client_rect;
//	CC_SYNTHESIZE(std::string,image_name,ImageName);
//	 
//	//图片操作
//	void cuPushTextureFormat(cocos2d::CCTexture2DPixelFormat format);
//	void cuPopTextureFormat();
//
//	//记录切割和矩形
//	bool m_bIsNinePatch;
//	bool m_bIsFixNinePatch;
//	cocos2d::CCRect m_rectNinePatchStretch;
//	cocos2d::CCRect m_rectNinePatchPadding;
//};

#endif //_NINE__PATCH_H_

