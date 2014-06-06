#include "CSVParse.h"

CSVParse::CSVParse(const char* fileName, string sep)
:m_fieldsep(sep)
{
    openFile(fileName);
}

CSVParse::~CSVParse()
{
    for (int i=0; i<m_data.size(); i++) {
        m_data[i].clear();
    }
    m_data.clear();
}

void CSVParse::split(vector<string>& field,string line)
{
    string fld;
    int i, j;
    
    if (line.length() == 0)
        return ;
    i = 0;
    
    do {
        if (i < line.length() && line[i] == '"')
            j = advquoted(line, fld, ++i);
        else
            j = advplain(line, fld, i);
        
        field.push_back(fld);
        i = j + 1;
    } while (j < line.length());
    
}

int CSVParse::advquoted(const string& s, string& fld, int i)
{
    int j;
    
    fld = "";
    for (j = i; j < s.length(); j++)
    {
        if (s[j] == '"' && s[++j] != '"')
        {
            int k = s.find_first_of(m_fieldsep, j);
            if (k > s.length())
                k = s.length();
            for (k -= j; k-- > 0; )
                fld += s[j++];
            break;
        }
        fld += s[j];
    }
    return j;
}

int CSVParse::advplain(const string& s, string& fld, int i)
{
    int j;
    
    j = s.find_first_of(m_fieldsep, i);
    if (j > s.length()) 
        j = s.length();
    fld = string(s, i, j-i);
    return j;
}


const char* CSVParse::getData(int m,int n)
{
    if ( m<0 || m>=m_data.size() || n<0 || n>=m_data[m].size() ) {
        return "";
    }
    
    //printf("%d,%d,%s\n", m, n, m_data[m][n].c_str());
    
    return m_data[m][n].c_str();
}


void CSVParse::copyData(const char* pFileName)
{
	std::string strPath =  cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename(pFileName);
    unsigned long len = 0;
    unsigned char *data = NULL;
 
    data =  cocos2d::CCFileUtils::sharedFileUtils()->getFileData(strPath.c_str(),"r",&len);
	std::string destPath =  cocos2d::CCFileUtils::sharedFileUtils()->getWritablePath();
    destPath += pFileName;
	CCLog("destPath %s",destPath.c_str());
    FILE *fp = fopen(destPath.c_str(),"w+");
    fwrite(data,sizeof(char),len,fp);
    fclose(fp);
    delete []data;
    data = NULL;
}

bool CSVParse::openFile(const char* fileName)
{

	//copyData(fileName);

    //获取全路径
	string pathKey = cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename(fileName);
    
	

    //打开文件
    //    size_t size = 0;
    FILE *fp = fopen(pathKey.c_str(), "r");
    if( !fp ) {
        CCLOG("OPEN FILE %s FAIL", pathKey.c_str());
        return false;
    }
    
    //获取文件字节数
    //    fseek(fp, 0, SEEK_END);
    //    size = ftell(fp);
    //    fseek(fp, 0, SEEK_SET);
    
    //读取内容
    //    unsigned char* out = (unsigned char*)malloc(size);
    //    size_t read = fread(out, 1, size, fp);
    //    if( read != size ) {
    //        CCLOG("读取文件%s失败", pathKey.c_str());
    //        free(out);
    //        *out = NULL;
    //        return false;
    //    }
    
	/* unsigned long size;
	unsigned  char* pData = NULL;
	   pData = CCFileUtils::sharedFileUtils()->getFileData(pathKey.c_str(), "r", &size);
	    //CCString* ccstr = CCString::createWithFormat("%s", pData); 
	   */


    char tmpChar[2048] = {0};
	string s;
	
    
    //去掉\r
    int lineIndex = 0;
    
    //读取第一行
	fgets((char*)tmpChar, 2048, fp);
	while( strlen(tmpChar) > 0 )
    {
		s =tmpChar;
        //printf("%d = %s", lineIndex, tmpChar);
        
        //删除和替换掉多余字符
        deleteChar(&s);
        
        //拆分掉文本
        std::vector<string> field;
        split(field, s);
        
        //第一行和第一列是无用数据,所以不存.
        if(lineIndex > 0){
            field.erase(field.begin());
            m_data.push_back(field);
        }
        lineIndex++;
        
        //读取下一行
        tmpChar[0] = '\0';
        fgets(tmpChar, 2048, fp);
    }
    
    row = m_data.size();
    col = m_data[0].size();
    CCLog("csv row =%d,col=%d", row, col);
    //测试，输出内容
    for (int i=0; i<m_data.size(); i++) {
        for (int k=0; k<m_data[i].size(); k++) {
            CCLOG("csv--------->%s",getData(i, k));
        }
    }
    
    fclose(fp);
    
    return true;
}


void CSVParse::deleteChar(std::string* str){
    string::iterator it;
    int index = 0;
    for (; index < str->size(); )
    {
        it = str->begin()+index;
        if ( *it == '\r' || *it == '\n' )
        {
            str->erase(it);
        } 
        else{
            index++;
        }
    }    
}