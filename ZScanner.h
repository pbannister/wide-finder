#ifndef ZSCANNER_H_
#define ZSCANNER_H_

class ZScanner {
public:
    int nBuffers;
    long long cbFile;
    double cbTotal;
protected:
    int nKids;
    int* pOut;
public:
    bool openTo(int, const char*, const char* s = 0);
    void onFile(const char*);
    void onBuffer(const char*,int);
    void onEOF();
    void onError(const char*, int);
    void closeWorkers();
public:
    ZScanner() : nBuffers(0), cbFile(0), cbTotal(0), nKids(0), pOut(0) {}
    ~ZScanner();
};

#endif /*ZSCANNER_H_*/
