#include "CsvUtil.h"
#include "MZDataManager.h"

using namespace cocos2d;

CsvUtil*CsvUtil::mCsvUtil=NULL;
CsvUtil*CsvUtil::sharedCsvUtil(){
	if (mCsvUtil==NULL)
	{
		mCsvUtil=new CsvUtil();
		if (mCsvUtil&&mCsvUtil->init())
		{
			mCsvUtil->retain();
			mCsvUtil->autorelease();
		}else{

			CC_SAFE_DELETE(mCsvUtil);
			mCsvUtil=NULL;
		}
	}
	return mCsvUtil;
}
bool CsvUtil::init()
{
	mCsvDict=CCDictionary::create();
	mCsvDict->retain();
	return true;
}

void CsvUtil::loadFile(const char*sPath)
{
	CCArray*csvStrList=CCArray::create();

	CCArray*lineList=FileLoadUtil::sharedFileLoadUtil()->getDataLines(sPath);

	CCObject* obj;
	CCString* tString=NULL;
	CCARRAY_FOREACH(lineList,obj)
	{
		tString=(CCString*)obj;
		if (tString)
		{
			CCArray*tArr=MZDataManager::sharedDataManager()->splitstr(tString->getCString(),",");
			csvStrList->addObject(tArr);
		}
	}
	mCsvDict->setObject(csvStrList,sPath);
}

const CCSize CsvUtil::getFileRowColNum(const char* csvFilePath)
{
	CCArray *csvStrList=(CCArray*)mCsvDict->objectForKey(csvFilePath);

	int iRowNum=csvStrList->count();
	int iColNum=0;

	CCObject *rowObj=csvStrList->objectAtIndex(0);
	CCArray *rowArr=(CCArray*)rowObj;

	iColNum=rowArr->count();
	CCSize size=CCSize(iRowNum,iColNum);
	return size;
}



const char* CsvUtil::get(int iRow,int iCol,const char*csvFilePath)
{
	CCArray *csvStrList=(CCArray*)mCsvDict->objectForKey(csvFilePath);

	CCSize size=getFileRowColNum(csvFilePath);

	int iRowNum=size.width;
	int iColNum=size.height;

	CCArray *rowArr=(CCArray*)csvStrList->objectAtIndex(iRow);

	CCString *colStr=(CCString*)rowArr->objectAtIndex(iCol);
	return colStr->getCString();
}



