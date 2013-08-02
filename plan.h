#ifndef _GENIUS_XIPLAN_H
#define _GENIUS_XIPLAN_H

#include <string>
using std::string;

#define LOG_NORMAL     1
#define LOG_DEBUG      2
#define LOG_WARNING    3
#define LOG_ERROR      4
#define LOG_FATAL      5

#define MOD_NORMAL     1
#define MOD_DEBUG      2
#define MOD_WARNING    4
#define MOD_ERROR      8
#define MOD_FATAL      16
#define MOD_ALL        31

namespace xlog{
class Plan{
public:
  Plan(int mode);

	virtual void notify(const char *, int, bool) = 0;
	void format(char * dest, const char * src, int lvl=LOG_NORMAL);
protected:
	bool is_enable_notify(int);
	int mode;
};
}

#endif
