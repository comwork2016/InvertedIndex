#include "SentenceSimilarity.h"

SentenceSimilarity::SentenceSimilarity()
{
    //ctor
}

double SentenceSimilarity::CalcSentenceSimilarity(const std::string str_Sen1,const std::string str_Sen2)
{
    //如果两个句子的长度相差一杯，一般是不相似的
    int len1 = str_Sen1.length();
    int len2 = str_Sen2.length();
    double d_LenDiv = len1/(double)len2;
    if(d_LenDiv > 2 || d_LenDiv <0.5 )
    {
        return 0.0;
    }
    //初始化分词组件
    if(!NLPIR_Init(0,UTF8_CODE,0))
    {
        std::cout<<"ICTCLAS INIT FAILED!"<<std::endl;
    }
    std::vector<std::string> vec_Word1,vec_NWord1, vec_VWord1, vec_AWord1, vec_MWord1, vec_QWord1, vec_TWord1;
    std::vector<std::string> vec_Word2,vec_NWord2, vec_VWord2, vec_AWord2, vec_MWord2, vec_QWord2, vec_TWord2;
    SplitSentenceToVectors(str_Sen1,vec_Word1,vec_NWord1, vec_VWord1, vec_AWord1, vec_MWord1, vec_QWord1, vec_TWord1);
    SplitSentenceToVectors(str_Sen2,vec_Word2,vec_NWord2, vec_VWord2, vec_AWord2, vec_MWord2, vec_QWord2, vec_TWord2);
    //便利所有词语，提取词语概念
    GlossaryDao* glossaryDao = new GlossaryDao();
    std::map<std::string,std::vector<std::string> > map_WordConceptsVector;
    for(int i=0; i<vec_Word1.size(); i++)
    {
        std::string str_Word = vec_Word1[i];
        if(map_WordConceptsVector.find(str_Word) == map_WordConceptsVector.end())
        {
            std::vector<std::string> vec1 = glossaryDao->SelectSememe(str_Word);
            map_WordConceptsVector[str_Word] = vec1;
        }
    }
    for(int i=0; i<vec_Word2.size(); i++)
    {
        std::string str_Word = vec_Word2[i];
        if(map_WordConceptsVector.find(str_Word) == map_WordConceptsVector.end())
        {
            std::vector<std::string> vec2 = glossaryDao->SelectSememe(str_Word);
            map_WordConceptsVector[str_Word] = vec2;
        }
    }
    //分别计算各种词性的相似度
    double weight_all = 0;
    double d_N = 0;
    if(!vec_NWord1.empty() || !vec_NWord2.empty())
    {
        d_N = CalcVectorSimilarity(vec_NWord1,vec_NWord2,map_WordConceptsVector);
        weight_all += weight_n;
    }
    double d_V = 0;
    if(!vec_VWord1.empty() || !vec_VWord2.empty())
    {
        d_V = CalcVectorSimilarity(vec_VWord1,vec_VWord2,map_WordConceptsVector);
        weight_all += weight_v;
    }
    double d_A = 0;
    if(!vec_AWord1.empty() || !vec_AWord2.empty())
    {
        d_A = CalcVectorSimilarity(vec_AWord1,vec_AWord2,map_WordConceptsVector);
        weight_all += weight_a;
    }
    double d_M = 0;
    if(!vec_MWord1.empty() || !vec_MWord2.empty())
    {
        d_M = CalcVectorSimilarity(vec_MWord1,vec_MWord2,map_WordConceptsVector);
        weight_all += weight_m;
    }
    double d_Q = 0;
    if(!vec_QWord1.empty() || !vec_QWord2.empty())
    {
        d_Q = CalcVectorSimilarity(vec_QWord1,vec_QWord2,map_WordConceptsVector);
        weight_all += weight_q;
    }
    double d_T = 0;
    if(!vec_TWord1.empty() || !vec_TWord2.empty())
    {
        d_T = CalcVectorSimilarity(vec_TWord1,vec_TWord2,map_WordConceptsVector);
        weight_all += weight_t;
    }
    std::cout<<d_N <<"	"<< d_V <<"	"<< d_A <<"	"<< d_M <<"	"<< d_Q <<"	"<< d_T<<std::endl;
    return (d_N*weight_n/weight_all + d_V*weight_v/weight_all + d_A*weight_a/weight_all + d_M*weight_m/weight_all + d_Q*weight_q/weight_all + d_T*weight_t/weight_all);
}

/**
    计算两个集合中词语的平均相似度
*/
double SentenceSimilarity::CalcVectorSimilarity(std::vector<std::string>& vec1,std::vector<std::string>& vec2,std::map<std::string,std::vector<std::string> > map_WordConceptsVector)
{
    std::vector<double> vec_maxsim;
    int len1=vec1.size();
    int len2=vec2.size();
    if(len1!=0 && len2 !=0)
    {
        //计算由两个向量组成的矩阵中的相似度
        double** matrix;
        //矩阵初始化
        matrix = new double*[len1];
        for(int i=0; i<len1; i++)
        {
            matrix[i] = new double[len2];
        }
        //计算矩阵中的词语相似度
        for(int i=0; i<len1; i++)
        {
            for(int j=0; j<len2; j++)
            {
                if(vec1[i] == vec2[j])
                {
                    matrix[i][j] = 1;
                }
                else
                {
                    std::vector<std::string> vec_concept1 = map_WordConceptsVector[vec1[i]];
                    if(len1==0)
                    {
                        std::cout<<"word1 "<<vec1[i]<<" not in dict"<<std::endl;
                        matrix[i][j] = 0;
                    }
                    else
                    {
                        std::vector<std::string> vec_concept2 = map_WordConceptsVector[vec2[j]];
                        if(len2==0)
                        {
                            std::cout<<"word2 "<<vec2[j]<<" not in dict"<<std::endl;
                            matrix[i][j] = 0;
                        }
                        else
                        {
                            matrix[i][j] = WordSimilarity::CalcWordSimilarityByConcepts(vec_concept1,vec_concept2);
                        }
                    }
                }
            }
        }
        //查找矩阵中最大相似度最大的词语对，加入相似度向量中
        int num = 0;
        while(num<len1 && num <len2)
        {
            double max_sim = -1;
            int m,n;
            for(int i=0; i<len1; i++)
            {
                for(int j=0; j<len2; j++)
                {
                    if(matrix[i][j]>max_sim && matrix[i][j] >0)
                    {
                        max_sim = matrix[i][j];
                        m = i;
                        n = j;
                    }
                }
            }
            //从向量中删除以计算的词语
            for(int i=0; i<len1; i++) //列向置为-1
            {
                matrix[i][n] = -1;
            }
            for(int i=0; i<len2; i++)
            {
                matrix[m][i] = -1;
            }
            vec_maxsim.push_back(max_sim);
            num++;
        }
        //释放matrix矩阵资源
        for(int i=0; i<len1; i++)
        {
            delete[] matrix[i];
        }
        delete[] matrix;
    }
    //对于剩下的词语，与空格计算相似度并加入到相似度向量中
    int n_rest = len1>len2?len1-len2:len2-len1;
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

/**
    将句子分词并存放到不同词性的向量中
*/
void SentenceSimilarity::SplitSentenceToVectors(const std::string& str,std::vector<std::string>& vec_Word,std::vector<std::string>& vec_NWord,std::vector<std::string>& vec_VWord,std::vector<std::string>& vec_AWord,std::vector<std::string>& vec_MWord,std::vector<std::string>& vec_QWord,std::vector<std::string>& vec_TWord)
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
        char ch_pos = pVecResult[i].sPOS[0];
        switch(ch_pos)
        {
        case 'n':
            vec_Word.push_back(str_HitsWord);
            vec_NWord.push_back(str_HitsWord);
            break;
        case 'v':
            vec_Word.push_back(str_HitsWord);
            vec_VWord.push_back(str_HitsWord);
            break;
        case 'a':
            vec_Word.push_back(str_HitsWord);
            vec_AWord.push_back(str_HitsWord);
            break;
        case 'm':
            vec_Word.push_back(str_HitsWord);
            vec_MWord.push_back(str_HitsWord);
            break;
        case 'q':
            vec_Word.push_back(str_HitsWord);
            vec_QWord.push_back(str_HitsWord);
            break;
        case 't':
            vec_Word.push_back(str_HitsWord);
            vec_TWord.push_back(str_HitsWord);
            break;
        default:
            break;
        }
    }
}

SentenceSimilarity::~SentenceSimilarity()
{
    //dtor
    NLPIR_Exit();
}
