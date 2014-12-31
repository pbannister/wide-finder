#include "common.h"
#include "ZTrace.h"

int ZTrace::nPad = 1;
int ZTrace::nMax = 64;
const char* ZTrace::sPad = "                                                      ";
//                          123456789012345678901234567890123456789012345678901234

const char* ZTrace::pad() {
	int i = nMax - ((2*nPad) % nMax);
	return sPad + i;
}
ZTrace::ZTrace(const char* s) :
	sName(s) {
	fprintf(stderr, "%s> %s\n", pad(), sName);
	++nPad;
}
ZTrace::~ZTrace() {
	--nPad;
	fprintf(stderr, "%s< %s\n", pad(), sName);
}
void ZTrace::log(const char* s) {
	fprintf(stderr, "%s%s\n", pad(), s);
}
void ZTrace::log(const char* s,const char* v) {
	fprintf(stderr, "%s%s: %s\n", pad(), s, v);
}
void ZTrace::log(const char* s,int v) {
	fprintf(stderr, "%s%s: %d\n", pad(), s, v);
}
