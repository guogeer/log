#if 1
#ifndef _GITCOM_XLOG_H
#define _GITCOM_XLOG_H

#include "thread.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <fstream>
#include <string>

#include "plan.h"
#include "plan_list.h"

#ifdef _WIN32
#define __vsnprintf(buf, size, format, args) \
_vsnprintf(buf, size, format, args)
#else
#define __vsnprintf(buf, size, format, args) \
vsnprintf(buf, size, format, args)
#endif
using std::string;
using std::ofstream;
namespace xlog{
class Log{
public:
  Log(string, Plan *);
	Log(string);
	void write(const char *format, ...);
  void write(const int lvl, const char *format, ...);
  void write3(const char *msg, int lvl=LOG_NORMAL, bool flag = false);
  //清空日志缓存
  void flush();
private:
	string _key;
	Plan *_plan;
};
}
#endif
#endif
