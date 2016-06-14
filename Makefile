objs=obj/ReadCorpus.o obj/StringUtil.o obj/RangeUtil.o obj/HashUtil.o obj/Document.o obj/WordIndexRecord.o obj/WordIndex.o obj/DocumentOperation.o obj/DocumentDao.o  obj/Sememe.o obj/GlossaryDao.o obj/WordSimilarity.o  obj/SentenceSimilarity.o obj/main.o 

CPPINCLUDE=-Iinclude -Iinclude/corpus -Iinclude/content -Iinclude/util -Iinclude/service -Iinclude/dao
BOOSTINCLUDE=-pthread -lmongoclient -lboost_thread -lboost_filesystem -lboost_program_options -lboost_system

vpath %.o obj
vpath %.cpp src/corpus src/util src/content src/service src/dao
vpath %.h include/corpus include/util include/content include/service include/dao

fingerprints: $(objs)
	g++ -o $@ $^ $(BOOSTINCLUDE) -lNLPIR -DOS_LINUX

obj/%.o:%.cpp %.h
	g++ $(CPPINCLUDE)  -o $@ -c $< -lpthread -lNLPIR -DOS_LINUX

obj/main.o:main.cpp
	g++ $(CPPINCLUDE)  -o $@ -c $< -lpthread -lNLPIR -DOS_LINUX

clean:
	-rm -f obj/*.o fingerprints
