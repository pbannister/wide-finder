#include "common.h"
#include "ZScanner.h"
#include <unistd.h>
#include <sys/wait.h>

void ZScanner::closeWorkers() {
    for (int i = 0; i<nKids; ++i) {
        close(pOut[i]);
    }
    while (0 < nKids--) {
        int status = 0;
        int pid = wait(&status);
        fprintf(stderr,"Worker #%d ended with status: %d\n",pid,status);
    }
}

ZScanner::~ZScanner() {
    closeWorkers();
}

bool ZScanner::openTo(int n, const char* sInterpreter, const char* sScript) {
    pOut = new int[n];
    for (int i=0; i<n; ++i) {
        int f[2];
        VERIFY(0 == pipe(f));
        int pid = fork();
        if (0 == pid) {
            dup2(f[0], 0);
            close(f[1]);
            execl(sInterpreter, sInterpreter, sScript, (char*)0);
            onError(sInterpreter, errno);
            exit(2);
        } else {
            close(f[0]);
            pOut[i] = f[1];
            ++nKids;
        }
    }
    return true;
}

void ZScanner::onFile(const char* sFilename) {
    fprintf(stderr,"Scanning: %s\n",sFilename);
    cbFile = 0;
}

void ZScanner::onBuffer(const char* pBuffer,int cbBuffer) {
    ++nBuffers;
    cbFile += cbBuffer;
    if (0 == nKids) {
    	return;	// for testing read performance
    }
    int f = pOut[nBuffers % nKids];
    write(f,pBuffer,cbBuffer);
    //fprintf(stderr,"%8d written to worker\n",cbBuffer);
}

void ZScanner::onEOF() {
    cbTotal += cbFile;
}

void ZScanner::onError(const char* sFilename, int iError) {
    const char* sError = strerror(iError);
    fprintf(stderr,"ERROR(%d) %s: %s\n",iError,sFilename,sError);
}
