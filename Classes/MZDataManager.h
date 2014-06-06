//
//  MZDataManager.h
//  mzdemo_client
//
//  Created by stevenLi on 13-12-20.
//
//

#ifndef __client1__MZDataManager__
#define __client1__MZDataManager__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "Micro_Loading.h"
#include "Micro.h"
//#include "json.h"
using namespace cocos2d;
using namespace std;

using namespace extension;
using namespace CSJson;


class MZDataManager
{
public:
    static MZDataManager* sharedDataManager();
    void writeFile(std::string filename, std::string value);    
    std::string readFile(std::string filename);

    std::string getLanguageString(char* strName);
	void initLanguageString(char* jsonName);
    
    std::vector<std::string> split(std::string str,std::string pattern);
	
	CCArray* splitstr( const char*srcStr,const char*sSep );
	//Micro* micro;

	void showLoading(CCNode* currentnode);
	void delLoading();

    
    
    int myRand(int start,int end); //[start,end]
    void resetRandSeed();
    
private:
    CSJson::Value languagedata;
  
    MZDataManager();
	~MZDataManager();

	Micro_Loading* _loading;
	bool bShowLoading;
};
#endif /* defined(__client1__MZDataManager__) */
