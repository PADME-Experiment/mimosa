#ifndef __MIMOSA_BoardData_h__
#define __MIMOSA_BoardData_h__ 0
#include"VData.h"
#include"FrameData.h"
#include<deque>

class BoardData:public VData{
  public:
    BoardData(unsigned brd,InputStream& is):fBoardID(brd),VData(is){Pull();}
    typedef union{
      uint32_t rows[3];
      struct __attribute__((packed)){
        uint32_t row1;
        uint32_t row2;
        uint32_t row3;
      };
      struct __attribute__((packed)){
        uint32_t eventNumber;
        uint32_t triggerNumberFake;
        uint32_t timeStampFake;
      };
    }__attribute__((packed)) header_t;
  public:
    std::deque<FrameData> fFrames;
    header_t fHeader;
    const unsigned int fBoardID;
    void DebugDump();
    void Pull();
};

#endif
