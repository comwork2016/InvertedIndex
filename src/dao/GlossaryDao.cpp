#include "GlossaryDao.h"

GlossaryDao::GlossaryDao()
{
    //ctor
    this->m_Host = "localhost:27017";
    this->m_DBName = "FP.glossary";
    this->m_Conn.connect(this->m_Host);
}

/**
    将glossary.dat文件中的概念信息读入到数据库中
*/
int GlossaryDao::ReadGlossaryToDB(const std::string str_path)
{
    std::ifstream ifs_glossary(str_path.c_str());   //打开词典文件
    std::string str_line;
    RemoveAll();
    while(getline(ifs_glossary,str_line))        //逐行读取词典文件
    {
        std::istringstream iss(str_line);
        std::string str_word,str_pos,str_sememe;
        iss>>str_word>>str_pos>>str_sememe;  //由空白把一行分割成：词、词性、义原集合
        std::string str_set;
        if(str_sememe[0]=='{')  //该行是虚词，因为虚词的描述只有“{句法义原}”或“{关系义原}”
        {
            str_set=str_sememe;
        }
        else
        {
            std::string str1,str2,str3,str4;//分别是 基本义原描述式\n其他义原描述式\n关系义原描述式\n关系符号义原描述式
            std::string::size_type pos1,pos2;
            pos1=0;
            bool flag=true;
            while(flag)
            {
                pos2=str_sememe.find(",",pos1);
                std::string str_sem;
                if(std::string::npos==pos2)      //已是最后一个义原
                {
                    flag=false;
                    str_sem=str_sememe.substr(pos1);    //提取最后一个义原
                }
                else
                {
                    str_sem=str_sememe.substr(pos1,pos2-pos1);  //提取下一个义原
                }//sem是pos1之后的第一个逗号之前的义原
                pos1=pos2+1;
                if(str_sem.find("=")!=std::string::npos) //关系义原,加入str3
                {
                    str3+=str_sem+",";
                }
                else
                {
                    char c=str_sem[0];
                    //义原以大/小写英文字母开始，或者是具体词--单独在小括号里，属于其他义原，加入str2。40是"("的ASCII值
                    if((c>64&&c<91) || (c>96&&c<123) || (c==40))
                    {
                        str2+=str_sem+",";
                    }
                    else //关系符号义原，加入str4
                    {
                        str4+=str_sem+",";
                    }
                }
            }
            //把str2中的第一条取出来，赋给str1
            std::string::size_type pos3=str2.find(",");
            if(pos3!=std::string::npos)
            {
                str1=str2.substr(0,pos3);
                str2.erase(0,pos3+1);
            }
            if(str2!="")
            {
                str2.erase(str2.length()-1,1);
            }
            if(str3!="")
            {
                str3.erase(str3.length()-1,1);
            }
            if(str4!="")
            {
                str4.erase(str4.length()-1,1);
            }
            str_set=str1+"\n"+str2+"\n"+str3+"\n"+str4;
        }
        Insert(str_word,str_pos,str_set);
    }
    return 0;
}

/**
    插入一行glossary数据到数据库中
*/
int GlossaryDao::Insert(const std::string str_word, const std::string str_pos,const std::string str_sememe)
{
    mongo::BSONObjBuilder b;
    //保存文档信息
    b.append("word",str_word);
    b.append("pos", str_pos);
    b.append("sememe", str_sememe);
    this->m_Conn.insert(this->m_DBName,b.obj());
    return 0;
}

/**
    删除所有数据
*/
int GlossaryDao::RemoveAll()
{
    this->m_Conn.remove(this->m_DBName,mongo::Query());
    return 0;
}

/**
    查询词语的概念
*/
std::vector<std::string> GlossaryDao::SelectSememe(const std::string str)
{
    std::vector<std::string> vec_Sememe;
    mongo::Query query = QUERY("word"<<str);
    mongo::BSONObj bo_columns = BSON("sememe"<<1);
    mongo::auto_ptr<mongo::DBClientCursor> cursor = this->m_Conn.query(this->m_DBName,query,0,0,&bo_columns);
    while(cursor->more())
    {
        mongo::BSONObj p = cursor->next();
        std::string str_sem = p.getStringField("sememe");
        vec_Sememe.push_back(str_sem);
    }
    return vec_Sememe;
}

GlossaryDao::~GlossaryDao()
{
    //dtor
}
