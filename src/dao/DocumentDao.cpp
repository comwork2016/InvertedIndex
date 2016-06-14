#include "DocumentDao.h"

DocumentDao::DocumentDao()
{
    //ctor//ctor
    this->m_Host = "localhost:27017";
    this->m_DocDBName = "FP.docs";
    this->m_IndexDBName = "FP.wordindex";
    this->m_Conn.connect(this->m_Host);
}

/**
    查询数据库中的索引信息
    //登陆舰(n,9)—>{a.txt,tf, (124,247,513, 698, 1546, 1611, 1684,2018)}
*/
std::map<DOC_ID,WordIndexRecord*> DocumentDao::QueryIndexOfWord(const std::string& str_Word)
{
    std::map<DOC_ID,WordIndexRecord*> map_WordIndexRecord;
    mongo::Query query = QUERY("word"<<str_Word);
    mongo::BSONObj bo_columns = BSON("docs"<<1);
    mongo::auto_ptr<mongo::DBClientCursor> cursor = this->m_Conn.query(this->m_IndexDBName,query,0,0,&bo_columns);
    while (cursor->more())
    {
        mongo::BSONObj p = cursor->next();
        mongo::BSONArray arrDoc(p.getObjectField("docs"));
        for(mongo::BSONObjIterator it= arrDoc.begin(); it.more();)
        {
            mongo::BSONElement be = it.next();
            mongo::BSONObj bson = be.Obj();
            std::string str_DocID = bson.getStringField("docid");
            WordIndexRecord* wordIndexRecord = new WordIndexRecord(str_DocID);
            mongo::BSONArray arrPos(bson.getObjectField("poss"));
            for(mongo::BSONObjIterator itr = arrPos.begin(); itr.more();)
            {
                mongo::BSONObj bson_Pos = itr.next().Obj();
                int n_WordPos = bson_Pos.getIntField("wordpos");
                int n_NoInDoc = bson_Pos.getIntField("noindoc");
                WordPos wordPos = {n_WordPos, n_NoInDoc};
                wordIndexRecord->AddPosInfo(wordPos);
            }
            map_WordIndexRecord[str_DocID] =wordIndexRecord;
        }
    }
    return map_WordIndexRecord;
}

bool DocumentDao::isWordIndexExists(const std::string& str_Word)
{
    mongo::Query query = QUERY("word"<<str_Word);
    mongo::BSONObj obj = this->m_Conn.findOne(this->m_IndexDBName,query);
    return obj.isEmpty()?false:true;
}

/**
    将文档索引添加到数据库中
*/
int DocumentDao::InsertIndexOfDocument(const Document* doc)
{
    std::map<std::string,WordIndex*> map_WordIndex = doc->GetMapWordIndex();
    //遍历文档中所有的词语
    for(std::map<std::string,WordIndex*>::iterator it = map_WordIndex.begin(); it != map_WordIndex.end(); it++)
    {
        WordIndex* wordIndex = it->second;//每个词的索引
        //构造要插入文档的BSONObj
        WordIndexRecord* record = wordIndex->GetMapDocWordIndex()[doc->GetDocID()];
        mongo::BSONObjBuilder bb_Record;
        bb_Record.append("docid",record->GetDocID());
        bb_Record.appendNumber("tf",record->GetnTermFrequency());
        mongo::BSONArrayBuilder bb_PosArray;
        for(int j=0; j<record->GetVecPos().size(); j++)
        {
            WordPos wordPos = record->GetVecPos()[j];
            mongo::BSONObjBuilder bb_pos;
            bb_pos.append("wordpos",wordPos.wordPos);
            bb_pos.append("noindoc",wordPos.NoInDoc);
            bb_PosArray.append(bb_pos.obj());
        }
        bb_Record.append("poss",bb_PosArray.arr());

        bool b_exists = isWordIndexExists(wordIndex->GetstrWord());
        if(!b_exists)//如果不存在，则新建索引并插入
        {
            mongo::BSONObjBuilder bb;
            bb.append("word",wordIndex->GetstrWord());
            bb.append("length",wordIndex->GetnWordLength());
            bb.append("POS",wordIndex->GetstrPOS());
            mongo::BSONArrayBuilder bab_Docs;
            bab_Docs.append(bb_Record.obj());
            bb.append("docs",bab_Docs.arr());
            this->m_Conn.insert(this->m_IndexDBName,bb.obj());
        }
        else
        {
            //更新单词的文档索引
            mongo::BSONObjBuilder bb_Docs;
            bb_Docs.append("docs",bb_Record.obj());
            mongo::BSONObjBuilder bb;
            bb.append("$push",bb_Docs.obj());
            mongo::Query query = QUERY("word"<<wordIndex->GetstrWord());
            this->m_Conn.update(this->m_IndexDBName,query,bb.obj(),true,false);
        }
    }
    return 0;
}

/**
    插入一个文档到数据库中
*/
int DocumentDao::InsertDocument(const Document* doc)
{
    InsertIndexOfDocument(doc);
    mongo::BSONObjBuilder b;
    //保存文档信息
    b.append("docid", doc->GetDocID());
    b.append("filename",doc->GetstrDocName());
    b.append("filepath", doc->GetstrDocPath());
    const char* pch_Contents= doc->GetstrContents().c_str();
    b.append("filelength",static_cast<int>(StringUtil::ConvertCharArraytoWString(pch_Contents).length()));
    b.appendNumber("docsimhash",static_cast<long long>(doc->GetlSimHash()));
    b.appendNumber("docsimhash1",static_cast<long long>(doc->GetlSimHash16_1()));
    b.appendNumber("docsimhash2",static_cast<long long>(doc->GetlSimHash16_2()));
    b.appendNumber("docsimhash3",static_cast<long long>(doc->GetlSimHash16_3()));
    b.appendNumber("docsimhash4",static_cast<long long>(doc->GetlSimHash16_4()));
    this->m_Conn.insert(this->m_DocDBName,b.obj());
    return 0;
}

//删除数据库中的信息
int DocumentDao::DeleteAll()
{
    this->m_Conn.remove(this->m_IndexDBName,mongo::Query());
    this->m_Conn.remove(this->m_DocDBName,mongo::Query());
    return 0;
}

//从数据库中查询simhash值相似的文件名称，没有则返回""
std::string DocumentDao::QuerySIMSimilarity(const Document* doc)
{
    std::cout<<"Query similar simhash of document "<<doc->GetstrDocName()<<std::endl;
    std::string str_SimilarDoc = "";
    mongo::BSONObj bson_condition1 = BSON("docsimhash1"<<static_cast<long long>(doc->GetlSimHash16_1()));
    mongo::BSONObj bson_condition2 = BSON("docsimhash2"<<static_cast<long long>(doc->GetlSimHash16_2()));
    mongo::BSONObj bson_condition3 = BSON("docsimhash3"<<static_cast<long long>(doc->GetlSimHash16_3()));
    mongo::BSONObj bson_condition4 = BSON("docsimhash4"<<static_cast<long long>(doc->GetlSimHash16_4()));
    mongo::BSONObj bson_condition = mongo::OR(bson_condition1,bson_condition2,bson_condition3,bson_condition4);
    mongo::BSONObj bo_columns = BSON("docsimhash"<<1<<"filelength"<<1<<"filepath"<<1);
    mongo::auto_ptr<mongo::DBClientCursor> cursor = this->m_Conn.query(this->m_DocDBName,bson_condition,0,0,&bo_columns);
    while (cursor->more())
    {
        mongo::BSONObj p = cursor->next();
        SIMHASH_TYPE l_SimHash = p.getField("docsimhash").numberLong();
        if(HashUtil::IsSimHashSimilar(doc->GetlSimHash(),l_SimHash))
        {
            //两个近似网页的文章长度差距应在20%以内
            int n_DBFileLength = p.getIntField("filelength");
            const char* pch_Contents= doc->GetstrContents().c_str();
            int n_DocFileLength = StringUtil::ConvertCharArraytoWString(pch_Contents).length();
            float f_LengthSim = (float)(n_DBFileLength-n_DocFileLength)/(n_DocFileLength>n_DBFileLength?n_DocFileLength:n_DBFileLength);
            if( f_LengthSim < 0.2)
            {
                str_SimilarDoc = std::string(p.getStringField("filepath"));
                break;
            }
        }
    }
    return str_SimilarDoc;
}

/**
    查询句子相似的文档
*/
//std::vector<FingerPrintsSimilarDocument> DocumentDao::GetSentenceSimilarDocument(doc)
void DocumentDao::GetSentenceSimilarDocument(const Document* doc)
{
    //对每句话，查询句子范围内相同词语超过20%的句子
    for(int i=0; i<doc->GetvecParagraph().size(); i++)
    {
        Paragraph para = doc->GetvecParagraph()[i];
        for(int j=0; j<para.vec_Sentences.size(); j++)
        {
            Sentence sen = para.vec_Sentences[j];
            int n_SameWordGate = sen.vec_splitedHits.size() * 0.5;
            std::vector<std::map<DOC_ID,WordIndexRecord*> > vec_WordInvertedIndex;//所有词语的倒排索引列表
            std::map<DOC_ID,std::map<int,Range> > map_DocWordNoPosition;//保存所有词语的位置信息。
            for(int k=0; k<sen.vec_splitedHits.size(); k++)
            {
                std::string str_Word = sen.vec_splitedHits[k].word;
                std::map<DOC_ID,WordIndexRecord*> map_WordDocIndexRecord = QueryIndexOfWord(str_Word);//单词索引的文档信息
                //遍历索引信息，保存词语位置信息
                for(std::map<DOC_ID,WordIndexRecord*>::iterator it = map_WordDocIndexRecord.begin(); it != map_WordDocIndexRecord.end(); it++)
                {
                    DOC_ID docID = it->first;
                    WordIndexRecord* record = it->second;
                    std::vector<WordPos> vec_WordPos = record->GetVecPos();
                    //遍历每一个位置范围
                    std::map<int,Range> map_NoPositionInDoc = map_DocWordNoPosition[docID];
                    for(int m=0; m<vec_WordPos.size(); m++)
                    {
                        WordPos wordPos = vec_WordPos[m];
                        Range posRange = {wordPos.wordPos,wordPos.wordPos+str_Word.length()};
                        map_NoPositionInDoc[wordPos.NoInDoc] = posRange;
                    }
                    map_DocWordNoPosition[docID] = map_NoPositionInDoc;
                }
                vec_WordInvertedIndex.push_back(map_WordDocIndexRecord);
            }
            //合并倒排列表，取出出现次数大于阈值并且词语位置相邻的句子范围信息
            std::map<DOC_ID,std::vector<DOCRANGETIMES> > map_DocRangeVector;
            for(int k=0; k<vec_WordInvertedIndex.size(); k++)
            {
                std::map<DOC_ID,WordIndexRecord*> map_WordDocIndexRecord = vec_WordInvertedIndex[k];
                //便利每一个文档索引列表
                for(std::map<DOC_ID,WordIndexRecord*>::iterator it = map_WordDocIndexRecord.begin(); it != map_WordDocIndexRecord.end(); it++)
                {
                    DOC_ID docID = it->first;
                    WordIndexRecord* record = it->second;
                    std::vector<WordPos> vec_WordPos = record->GetVecPos();
                    //遍历每一个位置范围
                    for(int m=0; m<vec_WordPos.size(); m++)
                    {
                        WordPos wordPos = vec_WordPos[m];
                        Range range = {wordPos.NoInDoc,wordPos.NoInDoc};
                        DOCRANGETIMES docRangeTimes(range,1);
                        //对于每个单词的位置，查看是否能够与前面的文档范围合并，如果不能，则新建一个范围
                        //文档第一次出现
                        if(map_DocRangeVector.find(docID) == map_DocRangeVector.end())
                        {
                            std::vector<DOCRANGETIMES> vec_DocRangeTimes;
                            vec_DocRangeTimes.push_back(docRangeTimes);
                            map_DocRangeVector[docID] = vec_DocRangeTimes;
                        }
                        else
                        {
                            std::vector<DOCRANGETIMES> vec_DocRangeTimes = map_DocRangeVector[docID];//某个文档已保存的位置向量
                            RangeUtil::MergeRangeToVector(vec_DocRangeTimes,docRangeTimes);
                            map_DocRangeVector[docID] = vec_DocRangeTimes;
                            //std::cin.get();
                        }
                    }
                }
            }
            //挑选出现次数大于阈值的短语范围
            for(std::map<DOC_ID,std::vector<DOCRANGETIMES> >::iterator it = map_DocRangeVector.begin(); it != map_DocRangeVector.end(); it++)
            {
                DOC_ID docID = it->first;
                std::vector<DOCRANGETIMES> vec_DocRangeTimes = it->second;
                for(int m=0; m<vec_DocRangeTimes.size(); m++)
                {
                    DOCRANGETIMES docRangeTimes = vec_DocRangeTimes[m];
                    if(docRangeTimes.second > 0.5*sen.vec_splitedHits.size())
                    {
                        Range range = docRangeTimes.first;
                        std::map<int,Range> map_NoPositionInDoc = map_DocWordNoPosition[docID];
                        int begin = map_NoPositionInDoc[range.begin].begin;
                        int end = map_NoPositionInDoc[range.end].end;
                        std::cout<<docID<<"\t["<<range.begin<<","<<range.end<<"]"<<std::endl;
                        std::cout<<docID<<"\t["<<begin<<","<<end<<"]"<<std::endl;
                    }
                }
            }
            /*
            //计算含有相同句子的相似度
            for(int k=0; k<vec_DocSen.size(); k++)
            {
                std::string str1;
                std::string str2;
                double d_similarity = SentenceSimilarity::CalcSentenceSimilarity(str1,str2);
                std::cout<<str1<<std::endl<<str2<<std::endl<<d_similarity<<std::endl;
            }*/
        }
    }
}

DocumentDao::~DocumentDao()
{
    //dtor
}
