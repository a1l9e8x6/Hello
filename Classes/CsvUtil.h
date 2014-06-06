#ifndef __CsvUTIL_H__
#define __CsvUTIL_H__

#include "cocos2d.h"

#include "Box2D/Box2D.h"

#include "SimpleAudioEngine.h"

#include "FileLoadUtil.h"
using namespace cocos2d;
using namespace std;

class CsvUtil : public CCObject
{
public:
	static CsvUtil*sharedCsvUtil();
	bool init();

	void loadFile(const char*sPath);
	//void releaseFile(const char*sPath);
	const char*get(int iRow,int iCol,const char*csvFilePath);
	//const int*getInt(int iRow,int iCol,const char*csvFilePath);
	//const float*getFloat(int iRow,int iCol,const char*csvFilePath);
	const CCSize getFileRowColNum(const char* csvFilePath);
	//const int findValueInWithLine(const char*chValue,int iValueCol,const char*csvFilePath);

private:
	static CsvUtil*mCsvUtil;
	CCDictionary*mCsvDict;

	CREATE_FUNC(CsvUtil);
};

#endif  // __CsvUTIL_H__