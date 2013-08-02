#include "plan.h"
#include <stdio.h>
#include <time.h>

using namespace xlog;

const char *log_level_desp[]={
  "valid", "normal", "debug", "warning", "error", "fatal"
};

Plan::Plan(int mode):mode(mode){
}

void Plan::format(char *dest, const char *src, int lvl){
	time_t cur_sec = time(NULL);
	struct tm *p_tm = gmtime(&cur_sec);
	
	sprintf(dest, "[%02d-%02d-%02d %02d:%02d:%02d %7s] %s\n",
		(p_tm->tm_year+1900)%2000,p_tm->tm_mon+1, p_tm->tm_mday,
		(p_tm->tm_hour+8)%24, p_tm->tm_min, p_tm->tm_sec,
		log_level_desp[lvl], src);
}

bool Plan::is_enable_notify(int lvl){
	if((1<<(lvl-1))&mode) return true;
	return false;
}
