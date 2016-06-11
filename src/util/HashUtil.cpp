#include "HashUtil.h"

HashUtil::HashUtil()
{
    //ctor
}

/**
    计算文本的hash值
    BKDR Hash Function
*/
SIMHASH_TYPE HashUtil::CalcStringHash(const std::string& str)
{
    SIMHASH_TYPE l_Hash = 0;
    //判断是否有近义词
    std::wstring wstr = StringUtil::ConvertCharArraytoWString(str);
    //不管数据是否溢出，最终只取结果的一部分作为字符串的hash值
    int seed = 131313; // 31 131 1313 13131 131313 etc..
    for(int i=0; i<wstr.length(); i++)
    {
        wchar_t c = wstr.at(i);
        //将单个字符转换成64位的hash值
        l_Hash = l_Hash * seed +  int(c);
    }
    l_Hash = (l_Hash & 0x7FFFFFFFFFFFFFFF) % MODNUM;
    if(l_Hash * BASE*2<0)
    {
        std::cout<<"less than 0"<<std::endl;
        std::cin.get();
    }
    return l_Hash;
}

/**
    计算文档的SIMHASH
*/
SIMHASH_TYPE HashUtil::CalcDocSimHash(const std::vector<SIMHASH_TYPE>& vec_SimHash)
{
    //初始化表示simhash每一位的权重数组
    short v[SIMHASHBITS-1];
    for(int i=0; i<SIMHASHBITS-1; i++)
    {
        v[i] = 0;
    }
    //遍历hash值列表
    for(int i=0; i<vec_SimHash.size(); i++)
    {
        SIMHASH_TYPE l_Hash = vec_SimHash[i];
        //计算对hash值的每一位，如果为1，则权重数组的相应位+1，为0则-1
        for (int j = 0; j < SIMHASHBITS - 1; j++)
        {
            SIMHASH_TYPE bitmask = (unsigned SIMHASH_TYPE)1 << j; //位的掩码:向左移j位
            SIMHASH_TYPE bit = l_Hash & bitmask;
            if (bit != 0)
            {
                v[j] += 1;
            }
            else
            {
                v[j] -= 1;
            }
        }
    }
    //根据权重数组计算文档的simhash值
    SIMHASH_TYPE l_SimHash = 0;
    for (int i = 0; i < SIMHASHBITS-1; i++)
    {
        if (v[i] > 0)
        {
            SIMHASH_TYPE n_IBit = (unsigned SIMHASH_TYPE)1<<(i);
            l_SimHash += n_IBit;
        }
    }
    return l_SimHash;
}

/**
    计算海明距离
*/
bool HashUtil::IsSimHashSimilar(const SIMHASH_TYPE& l_num1, const SIMHASH_TYPE& l_num2)
{
    int hd = 0;
    SIMHASH_TYPE x = l_num1^l_num2;
    while (x && hd<=HAMMINGDIST)
    {
        hd += 1;
        x = x&(x-1);//减一之后二进制的数字里面会减少一个1
    }
    if(hd<=HAMMINGDIST)
    {
        return true;
    }
    else
    {
        return false;
    }
}


HashUtil::~HashUtil()
{
    //dtor
}
