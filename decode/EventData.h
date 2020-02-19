#ifndef __MIMOSA_EventData_h__
#define __MIMOSA_EventData_h__ 0
#include"VData.h"
#include"BoardData.h"
#include<deque>
#include<string>
class EventData:public VData{
  public:
    EventData(InputStream& is):VData(is){Pull();}
    std::deque<BoardData> boards;
    void DebugDump();
    void Pull();
    void PBM(std::string="/tmp/export-");
    void NPixels();
};

#endif
