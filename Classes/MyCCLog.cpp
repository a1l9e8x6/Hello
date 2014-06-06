#include "MyCCLog.h"
 
#if (CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
 
#include "platform/android/jni/JniHelper.h"
#include "platform/android/jni/java_org_cocos2dx_lib_Cocos2dxHelper.h"
#include <jni.h>
 
#endif
 
USING_NS_CC;
 
#define MAX_STRING_LEN (1024*100)
#define LOG_V 1
#define LOG_D 2
#define LOG_I 4
#define LOG_W 8
#define LOG_E 16
 
const int MyCCLog::LOG_VALUE=LOG_V|LOG_D|LOG_I|LOG_W|LOG_E;

bool MyCCLog::showinfo = true;

MyCCLog::MyCCLog(void)
{
}
 
 
MyCCLog::~MyCCLog(void)
{
}
 
void MyCCLog::verbose(const char * pszFormat, ...) {
        if (showinfo&&(LOG_V & LOG_VALUE)) {
                va_list ap;
                va_start(ap, pszFormat);
                MyCCLog::printLog(MYCCLOG_VERBOSE, pszFormat, ap);
                va_end(ap);
        }
}
 
void MyCCLog::debug(const char* pszFormat, ...) {
	if (showinfo&&(LOG_D & LOG_VALUE ) ){
                va_list ap;
                va_start(ap, pszFormat);
                MyCCLog::printLog(MYCCLOG_DEBUG, pszFormat, ap);
                va_end(ap);
        }
}
 
void MyCCLog::info(const char* pszFormat, ...) {
        if (showinfo&&(LOG_I & LOG_VALUE)) {
                va_list ap;
                va_start(ap, pszFormat);
                MyCCLog::printLog(MYCCLOG_INFO, pszFormat, ap);
                va_end(ap);
        }
}
 
void MyCCLog::warn(const char* pszFormat, ...) {
        if (showinfo&&(LOG_W & LOG_VALUE)) {
                va_list ap;
                va_start(ap, pszFormat);
                MyCCLog::printLog(MYCCLOG_WARN, pszFormat, ap);
                va_end(ap);
        }
}
 
void MyCCLog::error(const char* pszFormat, ...) {
        if (showinfo&&(LOG_E & LOG_VALUE)) {
                va_list ap;
                va_start(ap, pszFormat);
                MyCCLog::printLog(MYCCLOG_ERROR, pszFormat, ap);
                va_end(ap);
        }
}
 
void MyCCLog::printLog(int type, const char* format, va_list ap) {
        char* pBuf = (char*) malloc(MAX_STRING_LEN);
        std::string mstr;
        if (pBuf != NULL) {
                vsnprintf(pBuf, MAX_STRING_LEN, format, ap);
                mstr = pBuf;
                free(pBuf);
        }
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        printAndroidLog(LOG_ANDROID_METHOD[type].c_str(), mstr.c_str());
#else
        CCLog("%s :%s", LOG_NAME[type].c_str(), mstr.c_str());
#endif
}
 
void MyCCLog::printAndroidLog(const char* methodName, const char* log) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        JniMethodInfo t;
        bool isHave = JniHelper::getStaticMethodInfo(t,
                "android/util/Log",
                methodName,
                "(Ljava/lang/String;Ljava/lang/String;)I");
        if (isHave)
        {
                jstring jTitle = t.env->NewStringUTF("cocos2d-x");
                jstring jMsg = t.env->NewStringUTF(
                        log);
                t.env->CallStaticVoidMethod(t.classID, t.methodID, jTitle,
                        jMsg);
                t.env->DeleteLocalRef(jTitle);
                t.env->DeleteLocalRef(jMsg);
        }
        else
        {
                CCLog("the jni method is not exits");
        }
#endif
}

bool MyCCLog::get_showinfo(){
	return showinfo;
}

void MyCCLog::set_showinfo(bool temp){
	showinfo = temp;
}