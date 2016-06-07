#ifndef CONSTANTS_H_INCLUDED
#define CONSTANTS_H_INCLUDED

#include <vector>

typedef int DOC_ID;
typedef int WORD_ID;
typedef long long SIMHASH_TYPE;

//定义结构体，文档某一段之间的内容
struct TextRange
{
    int offset_begin;//起始偏移值
    int offset_end;//结束偏移值
};

//定义结构体，用来存储分词之后的片段信息
struct SplitedHits
{
    std::string words;//存储词语信息
    int offset;//词语在文档中的偏移量
    int length;//词语所占的字节数
    std::string pos;//词语的词性
    SIMHASH_TYPE hashValue;//词语的hash值
};

//定义结构体，存储句子信息
struct Sentence
{
    TextRange textRange; //段落范围
    SIMHASH_TYPE hashValue;//句子的simhash值
    std::vector<SplitedHits> vec_splitedHits; //段落包含的分词信息
};

//定义结构体，存储段落信息
struct Paragraph
{
    TextRange textRange; //段落范围
    SIMHASH_TYPE hashValue;//段落simhash值
    std::vector<Sentence> vec_Sentences; //段落包含的分词信息
};

#endif // CONSTANTS_H_INCLUDED
