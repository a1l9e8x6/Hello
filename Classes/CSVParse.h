#pragma once
#ifndef  __CSVParse_H__
#define  __CSVParse_H__

#include <stdio.h>
#include <vector>
#include "cocos2d.h"
USING_NS_CC;
using namespace std;

class CSVParse {
    
public:
    int row;
    int col;
    
public:
    //调用解析
    CSVParse(const char* fileName, string sep = ",");
    ~CSVParse();

	void copyData(const char* pFileName);
    
private:
    /**
     *  分隔符
     */
    string m_fieldsep;
    
    /**
     *  容器，存储从CSV里读取出来的数据
     */
    vector<vector<string> > m_data;
    
private:
    void split(vector<string>& field,string line);
    int advplain(const string& line, string& fld, int);
    int advquoted(const string& line, string& fld, int);
    
    /**
     *  删除替换特定字符
     */
    void deleteChar(std::string* str);
    
public:
    /**
     *  打开文件
     */
    bool openFile(const char* fileName);
    
    /**
     *  取数据
     */
    const char* getData(int m,int n);

    ////得到总共的行数  
    //inline  int     getRows(){return m_data.size();} 
};
#endif