#include "WordSimilarity.h"

WordSimilarity::WordSimilarity()
{
    //ctor
    InitSemVec("dat/whole.dat");
    dao = new GlossaryDao();
}

/**
    把基本义原从文件读入vector
*/
void WordSimilarity::InitSemVec(const std::string str_filename)
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

/**
    计算词语之间的相似度
*/
double WordSimilarity::CalcWordSimilarity(const std::string str_word1,const std::string str_word2)
{
    if(str_word1 == str_word2)
    {
        return 1.0;
    }
    double d_similarity=0.0;
    std::vector<std::string> vec1,vec2;     //两个词语的概念分别存放在vec1和vec2中
    vec1 = dao->SelectSememe(str_word1);
    vec2 = dao->SelectSememe(str_word2);
    int len1=vec1.size();
    int len2=vec2.size();
    if(len1==0)
    {
        std::cout<<"word1 "<<str_word1<<" not in dict"<<std::endl;
    }
    if(len2==0)
    {
        std::cout<<"word2 "<<str_word2<<" not in dict"<<std::endl;
    }
    //每个概念之间进行相似性计算，最大相似度作为词语的相似度
    for(int i=0; i<len1; ++i)
    {
        for(int j=0; j<len2; ++j)
        {
            double sim=CalConceptSim(vec1[i],vec2[j]);
            if(sim > d_similarity)
            {
                d_similarity=sim;
            }
        }
    }
    //std::cout<<str_word1<<"\t"<<str_word2<<"\t"<<d_similarity<<std::endl;
    return d_similarity;
}

/**
    计算两个“概念”的相似度
    unit|单位


    &weight|重量
*/
double WordSimilarity::CalConceptSim(const std::string str_concept1,const std::string str_concept2)
{
    //std::cout<<"将要计算概念["<<str_concept1<<"]和["<<str_concept2<<"]的相似度"<<std::endl;
    if(str_concept1[0]=='{')    //概念1是虚词
    {
        if(str_concept2[0]!='{')    //概念2是实词
        {
            return 0;   //虚词和实词之间的相似度为0；
        }
        else//概念2是虚词
        {
            std::string str_sem1=str_concept1.substr(1,str_concept1.size()-2);   //去掉"{"和"}"
            std::string str_sem2=str_concept2.substr(1,str_concept2.size()-2);
            std::string::size_type p1=str_sem1.find("=");
            std::string::size_type p2=str_sem2.find("=");
            if(p1==std::string::npos ^ p2==std::string::npos) //一个句法义原，一个是关系义原
            {
                return 0;
            }
            else if(p1==std::string::npos && p2==std::string::npos) //都是句法义原
            {
                //可能是由两个基本义原组成的，如{modality|语气,neg|否,#KeepSilence|不说}和{firstPerson|我,female|女}，计算时一对一，然后加权平均
                std::vector<std::string> vec_strsem1 = StringUtil::SplitString(str_sem1,",");
                std::vector<std::string> vec_strsem2 = StringUtil::SplitString(str_sem2,",");
                std::vector<double> vec_sim;
                for(int i=0; i<vec_strsem1.size() && i<vec_strsem2.size(); i++)
                {
                    char c1=vec_strsem1[i][0];
                    char c2=vec_strsem2[i][0];
                    bool b_base1 = false;
                    bool b_base2 = false;
                    if((c1>64&&c1<91) || (c1>96&&c1<123))//基本义原
                    {
                        b_base1 = true;
                    }
                    if((c2>64&&c1<91) || (c2>96&&c1<123))//基本义原
                    {
                        b_base2 = true;
                    }
                    if(b_base1 && b_base2)//都是基本义原
                    {
                        double sim = CalSimBase(vec_strsem1[i],vec_strsem2[i]);
                        vec_sim.push_back(sim);
                    }
                    else if(!b_base1 && !b_base2)//都是符号义原
                    {
                        double sim = CalSim4(vec_strsem1[i],vec_strsem2[i]);
                        vec_sim.push_back(sim);
                    }
                }
                //计算加权平均相似度
                if(vec_sim.size()==0)
                {
                    return 0.0;
                }
                double sum=0.0;
                std::vector<double>::const_iterator it=vec_sim.begin();
                while(it!=vec_sim.end())
                {
                    sum+=*it;
                    it++;
                }
                return sum/vec_sim.size();
            }
            else //都是关系义原
            {
                return CalSimReal(str_sem1,str_sem2);
            }
        }
    }
    else//概念1是实词
    {
        if(str_concept2[0]=='{')//概念2是虚词
        {
            return 0;
        }
        else //概念2是实词
        {
            //分别计算4种描述式的相似度
            double sim1=0.0;
            double sim2=0.0;
            double sim3=0.0;
            double sim4=0.0;
            std::string::size_type pos11,pos12,pos21,pos22;
            pos11=pos21=0;
            for(int i=0; i<4; ++i)
            {
                pos12=str_concept1.find("\n",pos11);
                pos22=str_concept2.find("\n",pos21);
                std::string str_sem1=str_concept1.substr(pos11,pos12-pos11);
                std::string str_sem2=str_concept2.substr(pos21,pos22-pos21);
                switch(i)
                {
                case 0:
                    sim1=CalSim1(str_sem1,str_sem2);
                    break;
                case 1:
                    sim2=CalSim2(str_sem1,str_sem2);
                    break;
                case 2:
                    sim3=CalSim3(str_sem1,str_sem2);
                    break;
                case 3:
                    sim4=CalSim4(str_sem1,str_sem2);
                    break;
                default:
                    break;
                }
                pos11=pos12+1;
                pos21=pos22+1;
            }
            //4部分的加权和作为整体的相似度
            //std::cout<<sim1<<"\t"<<sim2<<"\t"<<sim3<<"\t"<<sim4<<std::endl;
            return beta1*sim1+
                   beta2*sim1*sim2+
                   beta3*sim1*sim2*sim3+
                   beta4*sim1*sim2*sim3*sim4;
        }
    }
}

/**
    计算两个基本义原的相似度
    cause|原因
*/
double WordSimilarity::CalSimBase(const std::string str_sem1,const std::string str_sem2)
{
    if(str_sem1[0]==40 ^ str_sem2[0]==40) //有一个是具体词，而另一个不是
    {
        return 0;
    }
    if(str_sem1[0]==40 && str_sem2[0]==40) //如果两个都是具体词
    {
        if(str_sem1!=str_sem2)
        {
            return 0;
        }
    }
    //两个义原相等
    if(str_sem1==str_sem2)
    {
        return 1;
    }
    Sememe sem1(0,str_sem1,0);
    Sememe sem2(0,str_sem2,0);
    std::vector<Sememe>::iterator it1=find(vec_Sememe.begin(),vec_Sememe.end(),sem1);
    if(it1==vec_Sememe.end())
    {
        //不在词典中
        //std::cout<<"["<<str_sem1<<"]不在词典中"<<std::endl;
        return 0;
    }
    std::vector<Sememe>::iterator it2=find(vec_Sememe.begin(),vec_Sememe.end(),sem2);
    if(it2==vec_Sememe.end())
    {
        //不在词典中
        //std::cout<<"["<<str_sem2<<"]不在词典中"<<std::endl;
        return 0;
    }
    std::stack<std::string> stk1,stk2;
    //把sem1的路径压入栈中
    stk1.push(str_sem1);
    int child1=it1->GetnIndex();
    int parent1=it1->GetnParent();
    while(child1!=parent1)
    {
        stk1.push(vec_Sememe[parent1].GetstrSememe());
        child1=parent1;
        parent1=vec_Sememe[parent1].GetnParent();
    }
    //把sem2的路径压入栈中
    stk2.push(str_sem2);
    int child2=it2->GetnIndex();
    int parent2=it2->GetnParent();
    while(child2!=parent2)
    {
        stk2.push(vec_Sememe[parent2].GetstrSememe());
        child2=parent2;
        parent2=vec_Sememe[parent2].GetnParent();
    }
    if(stk1.top()!=stk2.top())
    {
        //两棵不同子树的根
        //std::cout<<"["<<stk1.top()<<"]和["<<stk2.top()<<"]是两棵不同子树的根"<<std::endl;
        return 0;
    }
    int depth1,depth2;
    depth1 = stk1.size()-1;//树根的深度为0
    depth2 = stk2.size()-1;
    while(!stk1.empty() && !stk2.empty() && stk1.top()==stk2.top())
    {
        stk1.pop();
        stk2.pop();
    }
    int dist=stk1.size()+stk2.size();
    //考虑义原深度
    /*int dist1 = stk1.size() + stk2.size();
    int dist2 = depth1 + depth2;
    int dist = (1-alpha_depth)*dist1 + alpha_depth * dist2;*/
    double result=alpha/(alpha+dist);
    return result;
}


/**
    计算两个基本关系义原的相似度
    cause=addict|嗜好
*/
double WordSimilarity::CalSimReal(std::string str_sem1,std::string str_sem2)
{
    //std::cout<<"将要计算关系义原["<<str_sem1<<"]和["<<str_sem2<<"]的相似度"<<std::endl;
    //如果整体是括在小括号里的，先把小括号去掉
    if(str_sem1[0]==40)
    {
        str_sem1.erase(0,1);
        str_sem1.erase(str_sem1.size()-1,1);
    }
    if(str_sem2[0]==40)
    {
        str_sem2.erase(0,1);
        str_sem2.erase(str_sem2.size()-1,1);
    }
    return CalSim3(str_sem1,str_sem2);
}

/**
    计算第一独立义原描述式的相似度
*/
double WordSimilarity::CalSim1(const std::string str_line1, const std::string str_line2)
{
    if(str_line1=="" && str_line2=="")//两个词语的第一独立义原描述式为空
    {
        return 1;
    }
    else if(str_line1=="" || str_line2=="")//其中一个词语的第一独立义原描述式为空，相似度为一个比较小的常数
    {
        return delta;
    }
    //std::cout<<"将要计算第一独立义原描述式["<<str_line1<<"]和["<<str_line2<<"]的相似度"<<std::endl;
    return CalSimBase(str_line1,str_line2);
}

/**
    计算其他独立义原描述式的相似度
*/
double WordSimilarity::CalSim2(const std::string str_line1,const std::string str_line2)
{
    if(str_line1=="" && str_line2=="")
    {
        return 1;
    }
    else if(str_line1=="" || str_line2=="")
    {
        return 0;
    }
    //std::cout<<"将要计算其他独立义原描述式["<<str_line1<<"]和["<<str_line2<<"]的相似度"<<std::endl;
    std::vector<double> vec_maxsim;
    std::vector<std::string> vec1,vec2;
    vec1 = StringUtil::SplitString(str_line1,",");
    vec2 = StringUtil::SplitString(str_line2,",");
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
                double simil=CalSimBase(vec1[i],vec2[j]);
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
        if(max_sim==0.0)//所有的其他独立义原都不相似
        {
            break;
        }
        vec_maxsim.push_back(max_sim);
        vec1.erase(vec1.begin()+m);
        vec2.erase(vec2.begin()+n);
        len1=vec1.size();
        len2=vec2.size();
    }
    //对于剩下的义原，与空格计算相似度并加入到相似度向量中
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

/**
    计算关系义原描述式的相似度
    patient=land|陆地
*/
double WordSimilarity::CalSim3(const std::string str_line1,const std::string str_line2)
{
    if(str_line1=="" && str_line2=="")
    {
        return 1;
    }
    else if(str_line1=="" || str_line2=="")
    {
        return 0;
    }
    //std::cout<<"将要计算关系义原描述式["<<str_line1<<"]和["<<str_line2<<"]的相似度"<<std::endl;
    std::vector<double> vec_sim;
    std::vector<std::string> vec1,vec2;
    //可能有多个关系义原描述式
    vec1 = StringUtil::SplitString(str_line1,",");
    vec2 = StringUtil::SplitString(str_line2,",");
    int len1=vec1.size();
    int len2=vec2.size();
    //对词语1的关系义原描述式拆分
    std::map<std::string,std::string> map_real1;
    for(int i=0; i<len1; i++)
    {
        std::string str = vec1[i];
        std::string::size_type pos_equal = str.find("=");
        std::string str_prop = str.substr(0,pos_equal);
        std::string str_sememe = str.substr(pos_equal+1);
        map_real1[str_prop] = str_sememe;
    }
    //对第二个词语的关系义原描述式拆分，并与第一个词语中的关系计算相似度
    for(int i=0; i<len2; i++)
    {
        std::string str = vec2[i];
        std::string::size_type pos_equal = str.find("=");
        std::string str_prop = str.substr(0,pos_equal);
        std::string str_sememe2 = str.substr(pos_equal+1);
        std::string str_sememe1 = map_real1[str_prop];
        double sim = CalSimBase(str_sememe1,str_sememe2);
        //std::cout<<str_sememe1<<"\t"<<str_sememe2<<"\t"<<sim<<std::endl;
        vec_sim.push_back(sim);
    }
    //计算加权平均相似度
    if(vec_sim.size()==0)
    {
        return 0.0;
    }
    double sum=0.0;
    std::vector<double>::const_iterator it=vec_sim.begin();
    while(it!=vec_sim.end())
    {
        sum+=*it;
        it++;
    }
    return sum/vec_sim.size();
}

/**
    计算符号义原描述式的相似度
    #cure|医治,#disease|疾病
*/
double WordSimilarity::CalSim4(const std::string str_line1,const std::string str_line2)
{
    if(str_line1=="" && str_line2=="")
    {
        return 1;
    }
    else if(str_line1=="" || str_line2=="")
    {
        return 0;
    }
    //std::cout<<"将要计算符号义原描述式["<<str_line1<<"]和["<<str_line2<<"]的相似度"<<std::endl;
    std::vector<double> vec_sim;
    std::vector<std::string> vec1,vec2;
    vec1 = StringUtil::SplitString(str_line1,",");
    vec2 = StringUtil::SplitString(str_line2,",");
    int len1=vec1.size();
    int len2=vec2.size();
    while(len1 && len2)
    {
        char sym1=vec1[len1-1][0];
        for(int j=0; j<len2; ++j)
        {
            char sym2=vec2[j][0];
            if(sym1==sym2)
            {
                std::string str_base1=vec1[len1-1].substr(1);
                std::string str_base2=vec2[j].substr(1);
                vec_sim.push_back(CalSimBase(str_base1,str_base2));
                vec2.erase(vec2.begin()+j);
                break;
            }
        }
        vec1.pop_back();
        len1=vec1.size();
        len2=vec2.size();
    }
    if(vec_sim.size()==0)
    {
        return 0.0;
    }
    double sum=0.0;
    std::vector<double>::const_iterator it=vec_sim.begin();
    while(it!=vec_sim.end())
    {
        sum+=*it;
        it++;
    }
    return sum/vec_sim.size();
}

WordSimilarity::~WordSimilarity()
{
    //dtor
    this->vec_Sememe.clear();
    delete dao;
}
