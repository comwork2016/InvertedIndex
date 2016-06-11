#ifndef CONSTANTS_H_INCLUDED
#define CONSTANTS_H_INCLUDED

#include <string>
#include <vector>

typedef std::string DOC_ID;
//typedef std::string WORD_ID;
typedef long long SIMHASH_TYPE;


const int HAMMINGDIST = 3;
const int SIMHASHBITS = 64;

const int SIMHASHKGRAM = 2;
const int BASE = 12;
const SIMHASH_TYPE MODNUM = (((unsigned SIMHASH_TYPE)1 << (SIMHASHBITS-1))- 1)/BASE/2; //不能取太大的值，否则在计算KRHash时会发生溢出，导致结果不准确

const static int ERROR_OPENFILE = 1;
const static int OK_READFILE = 0;

//定义结构体，文档某一段之间的内容
struct TextRange
{
    int offset;//起始偏移值
    int length;//结束偏移值
};

//定义结构体，用来存储分词之后的片段信息
struct SplitedHits
{
    std::string word;//存储词语信息
    TextRange textRange;//词语的偏移位置信息
    std::string POS;//词语的词性
    SIMHASH_TYPE hashValue;//词语的hash值
};

//定义结构体，存储句子信息
struct Sentence
{
    TextRange textRange; //段落范围
    std::vector<SplitedHits> vec_splitedHits; //段落包含的分词信息
};

//定义结构体，存储段落信息
struct Paragraph
{
    TextRange textRange; //段落范围
    std::vector<Sentence> vec_Sentences; //段落包含的分词信息
};

#endif // CONSTANTS_H_INCLUDED
