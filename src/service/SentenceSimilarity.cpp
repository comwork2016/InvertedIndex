#include "SentenceSimilarity.h"

SentenceSimilarity::SentenceSimilarity()
{
    //ctor
}

double SentenceSimilarity::CalcSentenceSimilarity(const std::string str_Sen1,const std::string str_Sen2)
{
    //初始化分词组件
    if(!NLPIR_Init(0,UTF8_CODE,0))
    {
        std::cout<<"ICTCLAS INIT FAILED!"<<std::endl;
    }
    std::vector<std::string> vec_NWord1, vec_VWord1, vec_AWord1, vec_MWord1, vec_QWord1, vec_TWord1;
    std::vector<std::string> vec_NWord2, vec_VWord2, vec_AWord2, vec_MWord2, vec_QWord2, vec_TWord2;
    SplitSentenceToVectors(str_Sen1,vec_NWord1, vec_VWord1, vec_AWord1, vec_MWord1, vec_QWord1, vec_TWord1);
    SplitSentenceToVectors(str_Sen2,vec_NWord2, vec_VWord2, vec_AWord2, vec_MWord2, vec_QWord2, vec_TWord2);
    //分别计算各种词性的相似度
    double d_N = CalcVectorSimilarity(vec_NWord1,vec_NWord2);
    double d_V = CalcVectorSimilarity(vec_VWord1,vec_VWord2);
    double d_A = CalcVectorSimilarity(vec_AWord1,vec_AWord2);
    double d_M = CalcVectorSimilarity(vec_MWord1,vec_MWord2);
    double d_Q = CalcVectorSimilarity(vec_QWord1,vec_QWord2);
    double d_T = CalcVectorSimilarity(vec_TWord1,vec_TWord2);
    return (d_N + d_V + d_A + d_M + d_Q + d_T)/6;
}

/**
    计算两个集合中词语的平均相似度
*/
double SentenceSimilarity::CalcVectorSimilarity(std::vector<std::string>& vec1,std::vector<std::string>& vec2)
{
    WordSimilarity* wordSimilarity = new WordSimilarity();
    std::vector<double> vec_maxsim;
    int len1=vec1.size();
    int len2=vec2.size();
    while(len1 && len2)
    {
        int m,n;
        double max_sim=0.0;
        for(int i=0; i<len1; ++i)
        {
            for(int j=0; j<len2; ++j)
            {
                double simil = wordSimilarity->CalcWordSimilarity(vec1[i],vec2[j]);
                //std::cout<<vec1[i]<<"\t"<<vec2[j]<<"\t"<<simil<<std::endl;
                if(simil>max_sim)
                {
                    //std::cout<<vec1[i]<<","<<vec2[j]<<","<<simil<<","<<max_sim<<std::endl;
                    m=i;
                    n=j;
                    max_sim=simil;
                }
            }
        }
        if(max_sim==0.0)//所有的词语都不相似
        {
            break;
        }
        vec_maxsim.push_back(max_sim);
        vec1.erase(vec1.begin()+m);
        vec2.erase(vec2.begin()+n);
        len1=vec1.size();
        len2=vec2.size();
    }
    //对于剩下的词语，与空格计算相似度并加入到相似度向量中
    int n_rest = len1>len2?len1:len2;
    for(int i=0; i<n_rest; i++)
    {
        double d_NullSim = delta;
        vec_maxsim.push_back(d_NullSim);
    }
    //把整体相似度还原为部分相似度的加权平均,这里权值取一样，即计算算术平均
    if(vec_maxsim.size()==0)
    {
        return 0.0;
    }
    double sum=0.0;
    std::vector<double>::const_iterator it=vec_maxsim.begin();
    while(it!=vec_maxsim.end())
    {
        sum+=*it;
        it++;
    }
    return sum/vec_maxsim.size();
}

void SentenceSimilarity::SplitSentenceToVectors(const std::string& str,std::vector<std::string>& vec_NWord,std::vector<std::string>& vec_VWord,std::vector<std::string>& vec_AWord,std::vector<std::string>& vec_MWord,std::vector<std::string>& vec_QWord,std::vector<std::string>& vec_TWord)
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
        //筛选词性名词(N)、动词(V)、形容词 (A)、数词(M)、量词(Q)和时间(T)
        if(StringUtil::ConvertCharArraytoWString(str_HitsWord).length()>1)
        {
            char ch_pos = pVecResult[i].sPOS[0];
            switch(ch_pos)
            {
            case 'n':
                vec_NWord.push_back(str_HitsWord);
                break;
            case 'v':
                vec_VWord.push_back(str_HitsWord);
                break;
            case 'a':
                vec_VWord.push_back(str_HitsWord);
                break;
            case 'm':
                vec_VWord.push_back(str_HitsWord);
                break;
            case 'q':
                vec_VWord.push_back(str_HitsWord);
                break;
            case 't':
                vec_VWord.push_back(str_HitsWord);
                break;
            default:
                break;
            }
        }
    }
}

SentenceSimilarity::~SentenceSimilarity()
{
    //dtor
}
