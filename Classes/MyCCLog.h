#ifndef __MYCCLOG_H__
#define __MYCCLOG_H__
#include "cocos2d.h"
enum
{
        MYCCLOG_VERBOSE=0,
        MYCCLOG_DEBUG,
        MYCCLOG_INFO,
        MYCCLOG_WARN,
        MYCCLOG_ERROR,
        MYCCLOG_COUNT,
};
 

const std::string LOG_NAME[MYCCLOG_COUNT]={"(verbose)\t","(debug)\t\t","(info)\t\t","(warn)\t\t","(error)\t\t"};
 
const std::string LOG_ANDROID_METHOD[MYCCLOG_COUNT]={"v","d","i","w","e"};

class MyCCLog :public cocos2d::CCObject
{
public:
        MyCCLog(void);
        ~MyCCLog(void);
        //一般常用的打印信息
        static void verbose(const char * pszFormat,...);
        //调试信息
        static void debug(const char * pszFormat,...);
        //一般信息
        static void info(const char * pszFormat,...);
        //警告信息
        static void warn(const char * pszFormat,...);
        //错误信息
        static void error(const char *pszFormat,...);

		static	bool  get_showinfo();
		static void  set_showinfo(bool temp);
	
private:
        static const int LOG_VALUE;
        static void printLog(int type,const char * format,va_list ap);
        static void printAndroidLog(const char * methodName,const char * log);
		static bool showinfo ;//定义静态的成员变量除了在头文件里声明以外   还需要在cpp重新写一份并且赋值
};
#endif