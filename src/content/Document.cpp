#include "Document.h"

Document::Document(const std::string& str_DocPath, bool b_SplitToSentence, bool b_SplitToWords)
{
    //ctor
    this->m_strDocPath = str_DocPath;
    int n_SeparatorIndex = str_DocPath.find_last_of("/");
    this->m_strDocName = str_DocPath.substr(n_SeparatorIndex+1);
    this->m_strContents = "";
    this->m_lSimHash = 0;
    this->m_nWordCount = 0;
    if(b_SplitToSentence)
    {
        std::cout<<"Reading document "<<this->m_strDocName<<std::endl;
        //读取文档内容
        int n_ReadStats = ReadDocumentAndSplitToSentence();
        if(n_ReadStats == ERROR_OPENFILE)
        {
            std::cout<<"read file "<<this->m_strDocPath<<" error"<<std::endl;
            return;
        }
        if(b_SplitToWords)
        {
            SplitSentenceToWords();
            //CalcDocSimHash();
        }
    }
    else
    {
        ReadDocumentContent();
    }
}

/**
    一次性读取全部的文档内容
*/
int Document::ReadDocumentContent()
{
    std::ifstream ifs_Doc;
    ifs_Doc.open((char *)this->m_strDocPath.c_str(),std::ios_base::in);
    if(!ifs_Doc.is_open())
    {
        return ERROR_OPENFILE;
    }
    std::stringstream ss;
    ss<<ifs_Doc.rdbuf();
    this->m_strContents = ss.str();
    ifs_Doc.close();
    return OK_READFILE;
}


/**
    读取文件内容
    程序中，将一行内容作为一个段落
    并用常见符号将段落分割成句子
*/
int Document::ReadDocumentAndSplitToSentence()
{
    std::ifstream ifs_Doc;
    ifs_Doc.open((char *)this->m_strDocPath.c_str(),std::ios_base::in);
    if(!ifs_Doc.is_open())
    {
        return ERROR_OPENFILE;
    }
    int offset=0;
    while(!ifs_Doc.eof())
    {
        Paragraph para;
        para.textRange.offset_begin = offset;
        //读入一行数据
        std::string str_Line;
        std::getline(ifs_Doc,str_Line);
        int n_LineLength = str_Line.length();
        offset+=n_LineLength;
        this->m_strContents.append(str_Line);
        para.textRange.offset_end = offset;
        // 不是文章结尾时将原文档的换行符加回去
        if(!ifs_Doc.eof())
        {
            this->m_strContents.append(1,'\n');
            offset++;
        }
        if(n_LineLength != 0 && !StringUtil::isStringBlank(str_Line)) //空白行不计为段落
        {
            //对段落进行句子的拆分
            std::string str = this->m_strContents.substr(para.textRange.offset_begin,para.textRange.offset_end - para.textRange.offset_begin);
            SplitParaphToSentence(para,str);
            this->m_vecParagraph.push_back(para);
        }
    }
    ifs_Doc.close();
    return OK_READFILE;
}

/**
    将段落分割成句子
*/
void Document::SplitParaphToSentence(Paragraph& para,const std::string& str)
{
    //将文档用标点符号拆分
    std::vector<std::string> vec_pattern;
    vec_pattern.push_back("。");
    vec_pattern.push_back("？");
    vec_pattern.push_back("！");
    vec_pattern.push_back("；");
    const int strsize=str.size();
    std::string::size_type pos;
    int i = 0;
    while(i<strsize)
    {
        pos = strsize;
        int patternSize=0;
        //查找第一个分隔符的位置
        for(int j=0; j<vec_pattern.size(); j++)
        {
            std::string pattern = vec_pattern[j];
            int index =str.find(pattern,i);
            if(index!=std::string::npos && index < pos)
            {
                pos = index;
                patternSize = pattern.size();
            }
        }
        //std::cout<<i<<","<<pos<<","<<strsize<<std::endl;
        if(pos<=strsize)
        {
            std::string s=str.substr(i,pos-i);
            //std::cout<<s<<std::endl<<std::endl;
            if(!StringUtil::isStringBlank(s))
            {
                Sentence sen;
                sen.textRange.offset_begin = para.textRange.offset_begin + i;
                sen.textRange.offset_end = para.textRange.offset_begin + pos;
                para.vec_Sentences.push_back(sen);
            }
            i=pos+patternSize;
        }
    }
}

/**
    将句子分词处理
*/
void Document::SplitSentenceToWords()
{
    SplitUtil* splitUtil = new SplitUtil();
    for(int i=0; i<this->m_vecParagraph.size(); i++)
    {
        Paragraph& para = this->m_vecParagraph[i];
        //对句子进行分词处理
        for(int j = 0; j<para.vec_Sentences.size(); j++)
        {
            Sentence& sen = para.vec_Sentences[j];
            int n_SenLen = sen.textRange.offset_end-sen.textRange.offset_begin;
            std::string str_sentence = this->m_strContents.substr(sen.textRange.offset_begin,n_SenLen);
            splitUtil->SplitTermAndCalcTF(sen,str_sentence,this->m_MapTF,this->m_nWordCount);
            //提取文章标题中的词语
            if(i==0 && j==0) //如果是第一段的第一句话，则为文章的标题
            {
                for(int k =0; k<sen.vec_splitedHits.size(); k++)
                {
                    this->m_vecTitleTerm.push_back(sen.vec_splitedHits[k].words);
                }
            }
        }
    }
    delete splitUtil;
}

void Document::Dispaly()
{
    //输出文件的信息
    std::cout<<this->m_strDocName<<std::endl;

    //输出文章内容
    std::cout<<this->m_strContents<<std::endl;
    //输出段落句子的信息
    for(int i=0; i<this->m_vecParagraph.size(); i++)
    {
        Paragraph& para = m_vecParagraph[i];
        //对句子进行分词并计算simhash
        for(int j = 0; j<para.vec_Sentences.size(); j++)
        {
            Sentence& sen = para.vec_Sentences[j];
            int n_SenLen = sen.textRange.offset_end-sen.textRange.offset_begin;
            std::string str_sentence = this->m_strContents.substr(sen.textRange.offset_begin,n_SenLen);
            std::cout<<"Para "<<i<<" Sentence "<<j<<":["<<sen.textRange.offset_begin<<","<<sen.textRange.offset_end<<"]"<<std::endl;
            std::cout<<str_sentence<<std::endl<<std::endl;
            for(int k=0;k<sen.vec_splitedHits.size();k++)
            {
                SplitedHits sh = sen.vec_splitedHits[k];
                std::cout<<sh.words<<"["<<sh.offset<<","<<sh.length<<"]"<<"\t";
                //std::cout<<sh.words<<"/"<<sh.pos<<"\t";
            }
            std::cout<<std::endl;
        }
    }

    /*//遍历k-gram词组的hash值和文本范围
    for(int i=0; i<this->m_KGramFingerPrints.size(); i++)
    {
        std::cout<<this->m_KGramFingerPrints[i].hashValue<<"\t";
        for(int j=0; j<this->m_KGramFingerPrints[i].vec_splitedHits.size(); j++)
        {
            SplitedHits hits = this->m_KGramFingerPrints[i].vec_splitedHits[j];
            std::cout<<hits.words<<"\t";
            //std::cout<<"["<<hits.hashValue<<"]"<<hits.words<<"\t";
        }
        //std::cout<<"["<<this->m_KGramFingerPrints[i].textRange.offset_begin<<","<<this->m_KGramFingerPrints[i].textRange.offset_end<<"]"<<std::endl;
        std::cout<<std::endl;
    }*/
    //std::cout<<this->m_lSimHash<<std::endl;
}

Document::~Document()
{
    //dtor
}
