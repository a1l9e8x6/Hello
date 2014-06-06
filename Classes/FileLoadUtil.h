#ifndef __FILELOADUTIL_H__
#define __FILELOADUTIL_H__

#include "cocos2d.h"

#include "Box2D/Box2D.h"

#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace std;

class FileLoadUtil : public CCObject
{
public:
	static FileLoadUtil*sharedFileLoadUtil();
	bool init();

	CCArray*getDataLines(const char*sFilePath);

private:
	static FileLoadUtil*mFileLoadUtil;
	CREATE_FUNC(FileLoadUtil);
};

#endif  // __FILELOADUTIL_H__