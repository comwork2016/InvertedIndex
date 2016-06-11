#include <iostream>
#include "Document.h"
#include "ReadCorpus.h"
#include "DocumentOperation.h"

//静态变量的初始化
std::map<std::string,double> ReadCorpus::map_CorpusTF;
std::set<std::string> ReadCorpus::set_StopTerm;

int main()
{
    //设置全局环境本地化
    std::locale::global(std::locale(""));
    clock_t start,finish;
    double duration;
    start = clock();
    //读取语料库中的词频信息
    //ReadCorpus::ReadCorpusTF("./Corpus/Corpus.csv");
    ReadCorpus::ReadStopTerm("./Corpus/StopTerm.txt");

    //DocumentOperation::AddDocument("./in/utf_23.txt");
    std::string str_InputDir = "./in/";
    DocumentOperation::AddDirectoryDocuments(str_InputDir);

    finish = clock();
    duration = (double)(finish - start) / CLOCKS_PER_SEC;
    std::cout<<std::endl<<std::endl<<"cost "<<duration<<" secs"<<std::endl<<std::endl;
    return 0;
}
