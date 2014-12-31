#ifndef ZTRACE_H_
#define ZTRACE_H_

class ZTrace {
	const char* sName;
	static const char* sPad;
	static int nPad;
	static int nMax;
	static const char* pad();
public:
	ZTrace(const char* s);
	~ZTrace();
	static void log(const char* s);
	static void log(const char* s,const char* v);
	static void log(const char* s,int v);
};

#endif /*ZTRACE_H_*/
