#include "AppDelegate.h"
#include "T2.h"
#include "Micro_Menu.h"
#include "Gundong.h"

//#include "ServerPrizeList.h"


bool isT2 = true;
USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    CCDirector* pDirector = CCDirector::sharedDirector();
    CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();

    pDirector->setOpenGLView(pEGLView);
	
	CCSize framesize = pEGLView->getFrameSize();
	//实际屏幕的宽高比
	float screen_scale = framesize.height/framesize.width;
    if(!isT2){
	float desgin_height = 640;
	float desgin_width = desgin_height/screen_scale;
        CCLOG("AppDelegate %f %f,%f,<%f,%f>",screen_scale,desgin_width,desgin_height,framesize.width,framesize.height);
        
        //pEGLView->setDesignResolutionSize(width,height,kResolutionFixedWidth);
        
        CCEGLView::sharedOpenGLView()->setDesignResolutionSize(desgin_width,desgin_height,kResolutionFixedWidth);

    }else{
        
        float desgin_width = 640;
        float desgin_height = desgin_width*screen_scale;
        CCLOG("AppDelegate %f %f,%f,<%f,%f>",screen_scale,desgin_width,desgin_height,framesize.width,framesize.height);
        CCLog("diaodiao");
        //pEGLView->setDesignResolutionSize(width,height,kResolutionFixedWidth);
        
        CCEGLView::sharedOpenGLView()->setDesignResolutionSize(desgin_width,desgin_height,kResolutionFixedHeight);

    }

	
    // turn on display FPS
    pDirector->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
   // CCScene *pScene = SanX::scene();//ConMap::create();

    if(isT2){
        
        CCScene* pScene = Gundong::scene();
        pDirector->runWithScene(pScene);
    }
    else{
        CCScene* pScene = Micro_Menu::scene();//
        pDirector->runWithScene(pScene);
    
    }
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    CCDirector::sharedDirector()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    CCDirector::sharedDirector()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
