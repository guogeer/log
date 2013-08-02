#ifndef _GENIUS_ROLL_FILE_H
#define _GENIUS_ROLL_FILE_H

#include "thread.h"
#include "plan.h"

#include <fstream>
#include <iostream>

namespace xlog{
  class RollFile:public Plan{
public:
	RollFile(const char *file_name, const char *dir=NULL, int max_file_size=5*1024/*5MB*/, 
		int max_file_num=8, int mod=MOD_ALL);
	~RollFile();
	void notify(const char *msg, int lvl=LOG_NORMAL, bool isNow = false);
private:
	RollFile();

	void roll_next_file();

	xmutex mtx;
	std::ofstream out;
	
	int max_file_size;
	int max_file_num;
	int file_index;

	string dir;
	string file_name;
};
}

#endif
