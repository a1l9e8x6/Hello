#include "MZDataManager.h"

#define IP "192.168.1.111"
//#define IP "192.168.0.114"
#define PORT            22222

static MZDataManager* _sharedDataManager = NULL;
#define  kd_font "kdfont.fnt"
MZDataManager* MZDataManager::sharedDataManager()
{
    if(!_sharedDataManager)
        _sharedDataManager = new MZDataManager();
    
    return _sharedDataManager;
}

MZDataManager::MZDataManager()
{

    bShowLoading = false;
  /*  micro = new Micro();
	micro->retain();*/
	CCLog("MZDataManagerMZDataManager");
}

MZDataManager::~MZDataManager(){
//	CC_SAFE_RELEASE(micro);
}


void MZDataManager::writeFile(std::string filename, std::string value)
{
    std::string Background = value;
    const char * groundName = Background.c_str();
    std::string doPath = "";
    std::string filenamea = filename;
    std::string filepath = doPath + filename;
    FILE *fp = fopen(filepath.c_str(),"w");
    fputs(groundName,fp);
    fclose(fp);
}




std::string MZDataManager::readFile(std::string filename)
{
    std::string doPath ="";
    std::string filenamea = filename;
    std::string filepath = doPath + filename;
    FILE *fp1 = fopen(filepath.c_str(),"r");
    char* pchBuf = NULL;
    
    int len = 0;
    if (fp1 == NULL) {
        return "nologin";
    }
    fseek(fp1 , 0, SEEK_END);
    len = ftell(fp1);
    
    rewind(fp1);
    pchBuf = (char*)malloc(sizeof(char)*len+1);
    
    if (!pchBuf)
    {
        exit(0);
    }
    
    len = fread(pchBuf,sizeof(char),len,fp1);
    pchBuf[len] = '\0';
    std::string getground = pchBuf;
    fclose(fp1);
    free(pchBuf);
    
    std::stringstream ss(getground);
    std::string pucter;
    for (int i=0 ;i < 1 ;i++)
    {
        getline(ss,pucter,',');
    }
    return pucter.c_str() ;
    
}



std::string MZDataManager::getLanguageString( char* strName )
{
	return languagedata[strName].asString();
}

void MZDataManager::initLanguageString( char* jsonName )
{
	
	//CSJson::Reader reader;
	
	//if (!reader.parse(readJson(jsonName), languagedata)) {
	//	return;
	//}
	
	 //const char* pszFullPath 
	  CSJson::Reader reader;    
     string pszFullPath = "";
     pszFullPath= CCFileUtils::sharedFileUtils()->fullPathForFilename(jsonName);
     	//	CCLOG("bbbbbbbb %s",pszFullPath.c_str());
     unsigned long size;
     //std::string filecontext = "";
     
    unsigned char* pData = NULL;
     
    // char *pFileContent = "";
    pData = CCFileUtils::sharedFileUtils()->getFileData(pszFullPath.c_str(), "r", &size);
    //	filecontext.append((char*)pData);
     	
     	
     std::string jsonfile ;
     jsonfile.assign((char*)pData, size);
     //		CCLOG("jsonfile==== %s",jsonfile.c_str());
     CCAssert( size > 0, "file is open fail!");
     	//CCLOG("eeeeeeeeee");
     CSJson::Value root;
     if (!reader.parse(jsonfile, languagedata, false )){
     //		CCLOG("ffffffffff");
       // CCAssert(false, "Json::Reader Parse error!");
     }
	
	 //记得释放内存
	if(size >0 && pData) delete[] pData;
	 
}

//字符串分割函数
std::vector<std::string> MZDataManager::split(std::string str,std::string pattern)
{
    std::string::size_type pos;
    std::vector<std::string> result;
    str+=pattern;//扩展字符串以方便操作
    int size=str.size();
    
    for(int i=0; i<size; i++)
    {
        pos=str.find(pattern,i);
        if(pos<size)
        {
            std::string s=str.substr(i,pos-i);
            result.push_back(s);
            i=pos+pattern.size()-1;
        }
    }
    return result;
}

CCArray* MZDataManager::splitstr( const char*srcStr,const char*sSep )
{
	CCArray *stringList=CCArray::create();

	int size=strlen(srcStr);

	CCString *str=CCString::create(srcStr);

	int startIndex=0;
	int endIndex=0;
	endIndex=str->m_sString.find(sSep);

	CCString *spliStr=NULL;

	while (endIndex>0)
	{
		spliStr=CCString::create("");

		spliStr->m_sString=str->m_sString.substr(startIndex,endIndex);

		stringList->addObject(spliStr);

		str->m_sString=str->m_sString.substr(endIndex+1,size);

		endIndex=str->m_sString.find(sSep);
	}

	if (str->m_sString.compare("")!=0)
	{
		stringList->addObject(CCString::create(str->m_sString));
	}
	return stringList;
}


void MZDataManager::showLoading( CCNode* currentnode )
{
    CCLog("11111111");
	if(!bShowLoading){
            CCLog("22222");
		 _loading= Micro_Loading::create();
            CCLog("33333");
		currentnode->addChild(_loading);
		bShowLoading = true;
	}
	
}

void MZDataManager::delLoading()
{
	if(bShowLoading){
		_loading->removeFromParent();
		bShowLoading = false;
	}
}
int MZDataManager::myRand(int start,int end){ //[start,end]
	float randnum  = (CCRANDOM_0_1()* (end-start+1)+start);
	return (int)randnum;
}

void MZDataManager::resetRandSeed(){
	cc_timeval psv;
	CCTime::gettimeofdayCocos2d(&psv, NULL);
	unsigned long int rand_seed = psv.tv_sec*1000 + psv.tv_usec/1000+rand();
	srand(rand_seed);
    
}

