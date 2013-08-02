#if 1
#include "log.h"
#include "plan_list.h"

#ifdef _WIN32
#include <windows.h>
#include <io.h>
#include <direct.h>
#elif __linux
#include <unistd.h>
#endif

#include <sys/stat.h>

#include <string.h>
#include <time.h>

#include <iostream>
#include <fstream>
#include <cstdlib>
using std::cerr;
using std::endl;

#ifdef _WIN32
#define MKDIR(dir) _mkdir(dir)
#elif __linux
#define MKDIR(dir) mkdir(dir, 0755)
#endif

using namespace xlog;

Log::Log(string key, Plan *plan){
  _plan = plan;

	PlanList *ls = PlanList::get_instance();
	ls->add_plan(key, plan);
	_key = key;
}

Log::Log(string key){
	PlanList *ls = PlanList::get_instance();
	_plan = ls->get_plan(key);
	_key = key;
}

//写入日志，级别为NORMAL
void Log::write(const char *format, ...){
  char buf[4096];
  va_list arglist;
  va_start(arglist, format);
  __vsnprintf(buf, 4096, format, arglist);
  va_end(arglist);
	_plan->notify(buf, LOG_NORMAL, false);
}

//写入日志 
void Log::write(const int lvl, const char *format, ...){
  char buf[4096];
  va_list arglist;
  va_start(arglist, format);
  __vsnprintf(buf, 4096, format, arglist);
  va_end(arglist);
  _plan->notify(buf, lvl, false);
}

void Log::write3(const char *msg, int lvl, bool flag){
  _plan->notify(msg, lvl, flag);
}

//清空日志缓存
void Log::flush(){
  _plan->notify(NULL, 0, true);
}
#endif
