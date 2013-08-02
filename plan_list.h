#ifndef _GENIUS_PLAN_LIST_H
#define _GENIUS_PLAN_LIST_H

#ifdef _WIN32
#include <windows.h>
#include <Mmsystem.h>
#elif __linux
#include <unistd.h>
#include <sys/time.h>
#include <sys/signal.h>
#endif

#include "plan.h"

#include "thread.h"
#include <map>
#include <string>
using std::string;
using std::map;
using std::iterator;

namespace xlog{
class PlanList{
public:
  ~PlanList();
	static PlanList *get_instance();
	
	bool add_plan(string, Plan *);
	void del_plan(string);
	Plan *get_plan(string);
	inline map<string, Plan *> * getPlans(){
		return &plans;
	}
private:
	PlanList();
	void set_timer();

	int _interval;
	
	map<string, Plan *> plans;
	static PlanList *_list;

#ifdef _WIN32
	MMRESULT timer_id;
#endif
};
}

#endif
