#include "SplitUtil.h"

SplitUtil::SplitUtil()
{
    //ctor
    //初始化分词组件
    if(!NLPIR_Init(0,UTF8_CODE,0))//数据在当前路径下,默认为 GBK 编码的分词
    {
        std::cout<<"ICTCLAS INIT FAILED!"<<std::endl;
    }
}

/**
    对句子分词并统计词频
*/
void SplitUtil::SplitTermAndCalcTF(Sentence& sen,const std::string& str,std::map<std::string,double>& map_TF,int& n_WordCount)
{
    const result_t *pVecResult;
    int nCount = 0;
    pVecResult = NLPIR_ParagraphProcessA(str.c_str(),&nCount,true);
    for (int i=0; i<nCount; i++)
    {
        std::string str_HitsWord = str.substr(pVecResult[i].start,pVecResult[i].length);
        if(StringUtil::isStringBlank(str_HitsWord) || ReadCorpus::set_StopTerm.find(str_HitsWord)!=ReadCorpus::set_StopTerm.end())
        {
            continue;
        }
        SplitedHits sh_hits =
        {
            str_HitsWord,
            sen.textRange.offset_begin + pVecResult[i].start,//偏移值为最后一个字符的下一个字符所在的位置
            pVecResult[i].length,
            pVecResult[i].sPOS
        };
        sen.vec_splitedHits.push_back(sh_hits);
        n_WordCount++;
        map_TF[str_HitsWord] += 1;
    }

    //std::cout<<sh_hits.words<<"["<<sh_hits.offset<<","<<sh_hits.length<<","<<sh_hits.hashValue<<"]   ";
//std::wcout<<std::endl<<std::endl;
}

/**
    分隔字符串
*/
std::vector<std::string>  SplitUtil::SplitString(const std::string& str_Source,const std::string& str_pattern)
{
    std::vector<std::string> result;
    std::string str = str_Source + str_pattern;
    int strsize=str.size();
    int patternSize = str_pattern.size();
    std::string::size_type pos;
    for(int i=0; i<strsize; i++)
    {
        pos =str.find(str_pattern,i);
        if(pos!=std::string::npos && pos<strsize)
        {
            std::string s=str.substr(i,pos-i);
            result.push_back(s);
            i=pos+patternSize-1;
        }
    }
    return result;
}

SplitUtil::~SplitUtil()
{
    //dtor
    NLPIR_Exit();
}
