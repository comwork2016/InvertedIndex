#include "WordIndex.h"

WordIndex::WordIndex(const std::string& str_Word, const int n_WordLength, const std::string& str_POS)
{
    //ctor
    //this->m_WordID = str_Word;
    this->m_strWord = str_Word;
    this->m_nWordLength = n_WordLength;
    this->m_strPOS = str_POS;
}

int WordIndex::AddDocPosInfo(const DOC_ID docID, const WordPos pos)
{
    WordIndexRecord* wordIndexRecord;
    if(this->m_mapDocWordIndex.find(docID)==this->m_mapDocWordIndex.end())//包含该单词的第一个文档
    {
        wordIndexRecord = new WordIndexRecord(docID);
    }
    else
    {
        wordIndexRecord = this->m_mapDocWordIndex[docID];
    }
    wordIndexRecord->AddPosInfo(pos);
    this->m_mapDocWordIndex[docID] = wordIndexRecord;
    return 0;
}

/**
    显示倒排索引的信息
*/
void WordIndex::Display()
{
    std::cout<<m_strWord<<"("<<this->m_nWordLength<<","<<this->m_strPOS<<")--->";
    for(std::map<DOC_ID,WordIndexRecord*>::iterator it = this->m_mapDocWordIndex.begin(); it != this->m_mapDocWordIndex.end(); it++)
    {
        WordIndexRecord* wordIndexRecord = it->second;
        wordIndexRecord->Display();
    }
    m_mapDocWordIndex;//<doc_id,posInfo>
}

WordIndex::~WordIndex()
{
    //dtor
}
