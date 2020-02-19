#ifndef __MIMOSA_FrameData_h__
#define __MIMOSA_FrameData_h__ 0
#include"VData.h"
#include<vector>
#include<unordered_map>
#include<map>
#include<set>
class FrameData:public VData{
  public:
    typedef union{
      uint32_t rows[4];
      struct __attribute__((packed)){
        uint32_t row1;
        uint32_t row2;
        uint32_t row3;
        uint32_t row4;
      };
      struct __attribute__((packed)){
        uint32_t triggerNumber;
        uint32_t timeStamp;
        uint32_t frameCount;
        uint16_t dataLength1;
        uint16_t dataLength2;
      };
    }__attribute__((packed)) header_t;

    typedef union{
      uint32_t row;
      struct __attribute__((packed)){
        uint32_t stateNb  : 4;
        uint32_t data     :10;
        uint32_t zero1    : 1;
        uint32_t zero2    : 1;
      };
    }__attribute__((packed)) data_t;

    typedef union{
      uint16_t row;
      struct __attribute__((packed)){
        uint16_t nStates  : 4;
        uint16_t line     :10;
        uint16_t zero     : 1;
        uint16_t ovf      : 1;
      };
    }__attribute__((packed)) status_line_t;

    typedef union{
      uint16_t row;
      struct __attribute__((packed)){
        uint16_t nHit  : 2;
        uint16_t column:10;
        uint16_t zeros : 4;
      };
    }__attribute__((packed)) state_t;


    FrameData(unsigned frameid,InputStream& is):fFrameID(frameid),VData(is){
      fData.reserve(1850); // maximum number of 32bit words per frame
      Pull();
      }
    void DebugDump();
    void Pull();

    std::vector<data_t>   fData;
    std::unordered_multimap<uint16_t,std::vector<state_t> > fDatamap;
    std::vector<status_line_t> fStatus;
    std::vector<state_t>       fState;
    header_t              fHeader;
    std::map<uint16_t,std::set<uint16_t> > fMap;
    const unsigned int fFrameID;
};
#endif
