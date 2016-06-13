#include "ReadCorpus.h"

ReadCorpus::ReadCorpus()
{
    //ctor
}

/**
    从语料库cvs文件中读取词频信息
*/
void ReadCorpus::ReadCorpusTF(const std::string& str_CorpusPath)
{
    std::cout<<"Reading corpus!"<<std::endl;
    std::ifstream fin;
    fin.open(str_CorpusPath.c_str(),std::ios::in);
    if(!fin.is_open())
    {
        std::cout<<"Read file: "<<str_CorpusPath<<" error!"<<std::endl;
    }
    std::string line;
    while (getline(fin, line))
    {
        std::vector<std::string> fields = StringUtil::SplitString(line,",");
        std::string term = StringUtil::Trim(fields[0]);
        std::string str_frequency = StringUtil::Trim(fields[1]);
        double frequency = atof(str_frequency.c_str())/100;
        map_CorpusTF[term] = frequency;
    }
    fin.close();
    std::cout<<"Reading corpus done!"<<std::endl;
}
/**
    读取停用词
*/
void ReadCorpus::ReadStopTerm(const std::string& str_Path)
{
    std::cout<<"Reading stopWords!"<<std::endl;
    std::ifstream fin;
    fin.open(str_Path.c_str(),std::ios::in);
    if(!fin.is_open())
    {
        std::cout<<"Read file: "<<str_Path<<" error!"<<std::endl;
    }
    std::string line;
    while (getline(fin, line))
    {
        set_StopTerm.insert(line);
    }
    fin.close();
    std::cout<<"Reading stop words done!"<<std::endl;
}

/**
    把基本义原从文件读入vector
*/
void ReadCorpus::ReadSememe(const std::string str_filename)
{
    std::ifstream ifs(str_filename.c_str());
    std::string str_line;
    while(getline(ifs,str_line))
    {
        std::istringstream iss(str_line);
        int index,pind;
        std::string str_sememe;
        iss>>index>>str_sememe>>pind;
        Sememe sem(index,str_sememe,pind);
        vec_Sememe.push_back(sem);
    }
    ifs.close();
}

ReadCorpus::~ReadCorpus()
{
    //dtor
}
