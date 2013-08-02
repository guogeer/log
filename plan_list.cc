#include "plan_list.h"

#include "plan.h"
#include "thread.h"

#include <iostream>
#include <string>
using std::cout;
using std::cerr;
using std::endl;
using std::string;

using namespace xlog;

#ifdef _WIN32
#pragma comment(lib, "Winmm.lib")
#endif

#ifdef _WIN32
#define try_sleep(t) Sleep(t)
#elif __linux
#define try_sleep(t) usleep(t*1000)
#endif

PlanList *PlanList::_list = NULL;

PlanList::PlanList(){
  _interval = 1000;
	set_timer();
}

PlanList::~PlanList(){
#ifdef _WIN32
	timeKillEvent(timer_id);
#endif

	map<string, Plan *>::iterator it;
	for(it = plans.begin(); it!=plans.end(); it++){
		if(NULL != it->second) {
			delete it->second;
			plans.erase(it);
		}
	}
}

PlanList * PlanList::get_instance(){
	if(NULL == _list){
		_list = new PlanList;
	}
	return _list;
}

bool PlanList::add_plan(string key, Plan *plan){
	if(NULL == plan){
		cerr << "The plan is NULL" << endl;
		return false;
	}

	if(NULL != plans[key]){
		cerr << "The plan is existed" << endl;
		return false;
	}

	plans[key] = plan;
	return true;
}

void PlanList::del_plan(string key){

	if(NULL == plans[key]){
		cerr << "The plan is not existed" << endl;
		return;
	};

	Plan *plan = plans[key];
	delete plan;

	map<string, Plan *>::iterator it;
	for(it = plans.begin(); it!=plans.end(); it++){
		if(it->first == key){
			plans.erase(it);
			break;
		}
	}
}

Plan* PlanList::get_plan(string key){
	map<string, Plan *>::iterator it;
	for(it = plans.begin(); it!=plans.end(); it++){
		if(it->first == key){
			return it->second;
		}
	}
	return NULL;
}

#ifdef _WIN32
static void WINAPI do_timer(UINT wTimerID, UINT msg,DWORD dwUser,DWORD dwl,DWORD dw2){
	PlanList * pl = (PlanList *)PlanList::get_instance();
	map<string, Plan *> *plans = pl->getPlans();
	map<string, Plan *>::iterator it;
	for(it = plans->begin(); it!=plans->end(); it++){
		if(NULL != it->second)
			it->second->notify("", LOG_NORMAL, true);
	}
}
#elif __linux
static void do_timer(int sig){
	if(SIGALRM == sig){
		PlanList * pl = (PlanList *)PlanList::get_instance();
		map<string, Plan *> *plans = pl->getPlans();
		map<string, Plan *>::iterator it;
		for(it = plans->begin(); it!=plans->end(); it++){
			if(NULL != it->second)
				it->second->notify("", LOG_NORMAL, true);
		}
	}
}
#endif

void PlanList::set_timer(){
#ifdef _WIN32
	timer_id = timeSetEvent(_interval, 1, (LPTIMECALLBACK)do_timer, NULL, TIME_PERIODIC);
#elif __linux
	 struct itimerval val;
     val.it_value.tv_sec=_interval/1000;
     val.it_value.tv_usec=(_interval%1000)*1000;
     val.it_interval.tv_sec=_interval/1000;
     val.it_interval.tv_usec=(_interval%1000)*1000;
     setitimer(ITIMER_REAL,&val,NULL);
     signal(SIGALRM,do_timer);
#endif
}
