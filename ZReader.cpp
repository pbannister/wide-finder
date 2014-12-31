#include "common.h"
#include "ZReader.h"
#include "ZScanner.h"
#include <fcntl.h>
#include <unistd.h>

#ifndef O_LARGEFILE
#define O_LARGEFILE 0 // Only needed if defined.
#endif

ZReader::ZReader(const char* fn) : file(-1), pBuffer(0), nMax(0), nSize(0), sFilename(fn), iLastError(0) {
    allocateBuffer(1 * 1024 * 1024);
}

ZReader::ZReader() : file(0), pBuffer(0), nMax(0), nSize(0), sFilename("STDIN"), iLastError(0) {
    allocateBuffer(1 * 1024 * 1024);
}

ZReader::~ZReader() {
    if (0 < file) {
        closeFile();
    }
}

void ZReader::allocateBuffer(int cb) {
    pBuffer = new char[cb+1];
    nMax = cb;
    nSize = 0;
}

bool ZReader::openFile() {
	TRACE_FN(ZReader::openFile);
    file = open(sFilename,O_LARGEFILE);
    if (file < 0) {
        iLastError = errno;
        perror(sFilename);
        return false;
    }
    return true;
}

#if 1
// This is less than optimal. Needed as MacOS and Solaris(?) are missing memrchr().
void* memrchr(void* pBuffer, char c, size_t nBuffer) {
	for (size_t i = nBuffer; 0 < i; --i) {
		const char* p = ((const char*) pBuffer) + i - 1;
		if (c == *p) {
			return (void*) p;
		}
	}
	return 0;
}
#endif

enum { PIPE_QUANTA = 8192 }; 

bool ZReader::scanFile(ZScanner& scanner) {
	TRACE_FN(ZReader::scanFile);
    for (;;) {
        int nWanted = nMax - nSize;
        nWanted &= ~(0xFFFF);     // read in 64KB quanta 
        //fprintf(stderr,"%8d size, %8d wanted\n",nSize,nWanted);
        int nRead = read(file,pBuffer + nSize,nWanted);
        if (nRead <= 0) {
            break;
        }
        nSize += nRead;
        pBuffer[nSize] = 0;
        //fprintf(stderr,"%8d size, %8d read\n",nSize,nRead);
        char* pBase = pBuffer;
        char* pEOB = pBuffer + nSize;
        for (;;) {
            int nLeft = nSize - (int)(pBase - pBuffer);
            if (nLeft < PIPE_QUANTA) {
                memmove(pBuffer,pBase,nLeft);
                nSize = nLeft;
                //fprintf(stderr,"%8d size (fragment quanta)\n",nSize);
                break;
            }
            char* pEOL = (char*) memrchr(pBase,'\n',PIPE_QUANTA);
            if (!pEOL && (pBase == pBuffer)) {
                pEOL = (char*) memchr(pBase,'\n',nLeft);
            }
            if (!pEOL) {
                memmove(pBuffer,pBase,nLeft);
                nSize = nLeft;
                //fprintf(stderr,"%8d size (fragment line)\n",nSize);
                break;
            }
            int n = (int)(pEOL - pBase) + 1;
            scanner.onBuffer(pBase,n);
            pBase += n;
            if (pEOB <= pBase) {
                nSize = 0;
                break;
            }
        }
    }
    if (0 < nSize) {
        //fprintf(stderr,"%8d size (final)\n",nSize);
        scanner.onBuffer(pBuffer,nSize);
    }
    scanner.onEOF();
    return true;
}

void ZReader::closeFile() {
	TRACE_FN(ZReader::closeFile);
    if (0 < file) {
        close(file);
        file = -1;
    }
}

void ZReader::scanFiles(ZScanner& scanner, int iBase, int ac, char** av) {
	TRACE_FN(ZReader::scanFiles);
    for (int i=iBase; i<ac; ++i) {
        const char* sFilename = av[i];
        ZReader reader(sFilename);
        scanner.onFile(sFilename);
        if (!reader.openFile()) {
            scanner.onError(sFilename,reader.iLastError);
            return;
        }
        bool bOK = reader.scanFile(scanner);
        if (!bOK) {
            fprintf(stderr,"FAIL  '%s'\n",sFilename);
        }
        reader.closeFile();
        fprintf(stderr,"Done with: %s\n",sFilename);
        fflush(0);
    }
}
