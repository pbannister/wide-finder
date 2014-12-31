#ifndef ZREADER_H_
#define ZREADER_H_

#include "ZString.h"

class ZScanner;

class ZReader {
    int file;
    char* pBuffer;
    int nMax;
    int nSize;
public:
    ZString sFilename;
    int iLastError;
public:
    void allocateBuffer(int);
    bool openFile();
    bool scanFile(ZScanner&);
    void closeFile();
public:
    static void scanFiles(ZScanner&, int, int, char**);
public:
    ZReader(const char*);
    ZReader();
    ~ZReader();
};

#endif /*ZREADER_H_*/
