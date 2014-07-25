//
//  CsvParser.h
//  flappyball
//
//  Created by raymon_wang on 14-6-4.
//
//

#ifndef __flappyball__CsvParser__
#define __flappyball__CsvParser__

#include <iostream>
#include <vector>

using namespace std;

class CsvParser
{
public:
//    CsvParser();
    ~CsvParser();
    
    CsvParser(istream& fin=cin, string sep=","):
    fieldsep(sep),
    cols(0)
    {
        
    }
    
    //用以存储数据
    std::vector<std::vector<std::string>> data;
    
private:
    string        fieldsep;
    int            cols;
    
    void StringSplit(const string& str, vector<string>& tokens, const char& delimiters);
    void split(vector<string>& field, string line);
    int advplain(const string& line, string& fld, int);
    int advquoted(const string& line, string& fld, int);
    
public:
    bool parseCsvBuff(unsigned char* pBuffer, int bufferSize);
    const char* getData(unsigned int rows, unsigned int cols);
    int findColsData(int cols, const char* value);
    
    inline int getCols(){return cols;}
    inline int getRows(){return data.size();};
};

#endif /* defined(__flappyball__CsvParser__) */
