#include "WordIndexRecord.h"

WordIndexRecord::WordIndexRecord(const DOC_ID docId)
{
    //ctor
    this->m_DocID = docId;
    this->m_nTermFrequency = 0;
    this->m_vecPos.clear();
}

/**
    添加位置信息
*/
int WordIndexRecord::AddPosInfo(const int pos)
{
    this->m_vecPos.push_back(pos);
    this->m_nTermFrequency++;
}

/**
    显示倒排索引某一个文件的信息
*/
void WordIndexRecord::Display()
{
    std::cout<<"{"<<this->m_DocID<<", "<<this->m_nTermFrequency<<", (";
    for(int i=0;i<this->m_vecPos.size();i++)
    {
        std::cout<<this->m_vecPos[i]<<",";
    }
    std::cout<<")}";
}

WordIndexRecord::~WordIndexRecord()
{
    //dtor
}
