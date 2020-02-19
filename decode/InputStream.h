#ifndef __MIMOSA_InputStream_h__
#define __MIMOSA_InputStream_h__ 0

#include<fstream>
#include<string>
#include<exception>

class InputStreamIsEmptyWAIT:std::exception{};

class InputStream{
  private:
    std::ifstream filep;
  public:
    InputStream(const std::string& filename);
    ~InputStream();
    void Dump();
    bool HasMoreData(){return !filep.eof();}
    uint32_t Get32();
    uint16_t Get16();
};
#endif
