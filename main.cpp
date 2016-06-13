#include <iostream>
#include <ctime>

#include "Document.h"
#include "ReadCorpus.h"
#include "DocumentOperation.h"
#include "GlossaryDao.h"
//to delete
#include "SentenceSimilarity.h"

//静态变量的初始化
std::map<std::string,double> ReadCorpus::map_CorpusTF;
std::set<std::string> ReadCorpus::set_StopTerm;
std::vector<Sememe> ReadCorpus::vec_Sememe;

int main()
{
    //设置全局环境本地化
    std::locale::global(std::locale(""));
    struct timeval start,finish;
    double duration;
    gettimeofday(&start,NULL);
    /*//将词语义项存入数据库中
    GlossaryDao* glossaryDao = new GlossaryDao();
    std::string str_GlossaryPath="./dat/glossary.dat";
    glossaryDao->ReadGlossaryToDB(str_GlossaryPath);
    delete glossaryDao;*/

    //读取语料库中的词频信息
    //ReadCorpus::ReadCorpusTF("./Corpus/Corpus.csv");
    ReadCorpus::ReadStopTerm("./Corpus/StopTerm.txt");
    ReadCorpus::ReadSememe("./dat/whole.dat");

    //DocumentOperation::AddDocument("./in/utf_23.txt");
    std::string str_InputDir = "./in/";
    //DocumentOperation::AddDirectoryDocuments(str_InputDir);

    //查询数据泄露
    std::string str_LeakDoc = "./test/leak.txt";
    DocumentOperation::SearchLeak(str_LeakDoc);

    //Document* doc = new Document("./in/utf_742.txt",true);
    //doc->Display();

    /*std::string str1 = "“龙眼”无人机是由美国海军陆战队士兵在阿富汗地区发射";
    std::string str2 = "海军陆战队战士在阿富汗山区发射“龙眼”无人机";
    //相似度为0.48
    std::string str1 = "这颗树有3米高";
    std::string str2 = "那个石头有2千克重";
    std::cout<<SentenceSimilarity::CalcSentenceSimilarity(str1,str2)<<std::endl;*/

    gettimeofday(&finish,NULL);
    duration = 1000000 * (finish.tv_sec - start.tv_sec) + finish.tv_usec - start.tv_usec;
    duration /= 1000000;
    std::cout<<std::endl<<std::endl<<"cost "<<duration<<" s"<<std::endl<<std::endl;
    return 0;
}
