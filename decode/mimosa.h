#ifndef __MIMOSA_mimosa_h__
#define __MIMOSA_mimosa_h__ 0
#include<exception>


class FileCannotBeOpen:std::exception{};
class FileFinishedUnexpectedly:std::exception{};
class FileFormatError:std::exception{};

#endif
