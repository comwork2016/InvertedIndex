#include "RangeUtil.h"

RangeUtil::RangeUtil()
{
    //ctor
}

void RangeUtil::MergeRangeToVector(std::vector<DOCRANGETIMES>& vec_DocRangeTimes,DOCRANGETIMES& docRangeTimes)
{
    bool b_MergeRange = false;
    Range range = docRangeTimes.first;
    int times = docRangeTimes.second;
    //std::cout<<std::endl<<"["<<range.begin<<","<<range.end<<"]\t"<<times<<std::endl;
    //std::cout<<"************************"<<std::endl;
    //遍历已保存的所有位置
    for(int i=0; i < vec_DocRangeTimes.size(); i++)
    {
        DOCRANGETIMES docRangeTimes2 = vec_DocRangeTimes[i];
        Range range2 = docRangeTimes2.first;
        int times2 = docRangeTimes2.second;
        //std::cout<<"["<<range2.begin<<","<<range2.end<<"]\t"<<times2<<"\t"<<std::endl;
        //四种情况可以合并，包含，交叉各两种
        //交叉1 range:【6，13】和range2【10，24】
        if(range.begin <= range2.begin && range.end >= range2.begin - SIMILARRANGE && range.end <= range2.end)
        {
            range.end = range2.end;
            b_MergeRange = true;
        }
        //包含1 range:【6，28】和range2【10，24】
        else if(range.begin <= range2.begin && range.end >= range2.end)
        {
            //范围不变
            b_MergeRange = true;
        }
        //包含2 range:【16，18】和range2【10，24】
        else if(range.begin >= range2.begin && range.end <= range2.end)
        {
            range.begin = range2.begin;
            range.end = range2.end;
            b_MergeRange = true;
        }
        //交叉2 range:【22，25】和range2【10，24】
        else if(range.begin >= range2.begin && range.begin <= range2.end + SIMILARRANGE )
        {
            range.begin = range2.begin;
            b_MergeRange = true;
        }
        if(b_MergeRange)//能够合并，删除旧范围，返回
        {
            //更新要添加的集合
            times = times + times2;
            docRangeTimes.first = range;
            docRangeTimes.second = times;
            //删除旧集合中的元素
            vec_DocRangeTimes.erase(vec_DocRangeTimes.begin()+i);
            //继续往前合并
            MergeRangeToVector(vec_DocRangeTimes,docRangeTimes);
            break;
        }
    }
    if(!b_MergeRange)//范围位置偏差太大，则新建一个位置存放词语信息
    {
        vec_DocRangeTimes.push_back(docRangeTimes);
    }
    //std::cin.get();
}

RangeUtil::~RangeUtil()
{
    //dtor
}
