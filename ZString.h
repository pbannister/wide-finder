#ifndef __ZSTRING_H__
#define __ZSTRING_H__

//
//  Simple string class.
//

class ZString
{

protected:
    char* sBuffer;
    int cbBufferMax;

public:
    ZString() { upsizeTo(1); }
    ZString(const char* s) { upsizeTo(::strlen(s)); ::strcpy(sBuffer,s); }
    ~ZString() { recycle(); }

protected:
    void recycle();
    void upsizeTo(int);
    void sizeTo(int n) {
        if (cbBufferMax <= n) {
            upsizeTo(n);
        }
    }

public:
    operator const char*()          { return sBuffer; }
    char* getBuffer()               { return sBuffer; }
    char* getBuffer(int n)          { sizeTo(n); return sBuffer; }
    int getBufferSize()             { return cbBufferMax; }

public:
    void operator=(const char* s) {
        strcpy(s);
    }

public:
    void strcpy(const char* s) {
        sizeTo(::strlen(s));
        ::strcpy(sBuffer,s);
    }
    void strcpy(const char* s,int n) {
        sizeTo(n);
        ::strncpy(sBuffer,s,n);
        sBuffer[n] = 0;
    }
    void strcat(const char* s) {
        int n1 = ::strlen(sBuffer);
        int n2 = ::strlen(s);
        sizeTo(n1 + n2);
        ::strcpy(sBuffer+n1,s);
    }
    void strcat(const char* s,int n) {
        int n1 = ::strlen(sBuffer);
        sizeTo(n1 + n);
        ::strncpy(sBuffer+n1,s,n);
        sBuffer[n1+n] = 0;
    }

};

#endif
