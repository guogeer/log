#include "roll_file.h"

#include "plan.h"
#ifdef _WIN32
#include <direct.h>
#include <io.h>
#include <windows.h>
#elif __linux
#include <unistd.h>
#endif

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>

#include <fstream>
#include <iostream>
using std::ios;
using std::cerr;
using std::endl;
using std::ofstream;

#ifdef _WIN32
#define MKDIR(dir) _mkdir(dir)
#define ACCESS(dir, mode) _access(dir, mode)
#elif __linux
#define MKDIR(dir) mkdir(dir, 0755)
#define ACCESS(dir, mode) access(dir, mode)
#endif

using namespace xlog;

//创建路径 
bool CreateDirectory(const char *path){
  //路径为空 
  if (path[0] == '\0' || path == NULL)
    return false;

  int str_len = strlen(path);
  char *buf = (char *)malloc(sizeof(char)*(str_len+10));
  memcpy(buf, path, sizeof(char)*(str_len+1));
  //判断末字符是否为分隔符'/'或'\' 
  char c = buf[str_len-1];
  if(c != '\\' && c != '/'){
    buf[str_len++] = '/';
    buf[str_len] = '\0';
  }  

  for (int i=0; buf[i] != 0; i++){
    c = buf[i];
    if(c == '\\' || c == '/'){
      if(i == 0) continue;
      buf[i] = '\0';
      int flg = 0;
      //判断文件路径是否存在 
      if(0 != ACCESS(buf, 0)){
        flg = MKDIR(buf);
      }
      buf[i] = c;
      //若最终路径创建失败 
      if(i == str_len-1 && flg != 0)
        return false;
    }
  }
  return true;
}

RollFile::RollFile(const char *file_name, 
		 const char *dir, 
		 int max_file_size, 
		 int max_file_num, 
		 int mode):
		 file_name(file_name),
		 max_file_size(max_file_size), 
		 max_file_num(max_file_num),
		 file_index(0),
		 Plan(mode){
	if(file_name == NULL) return;

	if(dir == NULL || dir[0] == '\0'){
		this->dir = "";
	}else{
		this->dir += dir;
		if(CreateDirectory(dir) == false){
			cerr << "Create dirctory error" << endl;
			return;
		}
	}
	
	string path = this->dir;
	if(false == path.empty()) path += "/";
	path += this->file_name;

	out.open(path.c_str(), ios::out|ios::trunc);
	if(!out.is_open()){
		cerr << "open log file error" << endl;
	}

	mutex_init(mtx);
}

RollFile::~RollFile(){
	out.flush();
}

void RollFile::roll_next_file(){
	string path = this->dir;
	if(false == path.empty()) path += "/";
	path += this->file_name;

	string src, dest;
	src = path;
	dest = path;
	dest += ".";
	
	char tmp[32];
	sprintf(tmp, "%d", file_index);
	dest += tmp;

	out.close();
	out.clear();
	//当前日志文件写满，则准备写下一个
	remove(dest.c_str());
	rename(src.c_str(), dest.c_str());

	out.open(path.c_str(), ios::out|ios::trunc);
	if(!out.is_open()){
		cerr << "open log file error" << endl;
		return;
	}

	file_index = (file_index+1)%max_file_num;
}

//msg 信息为空时，不输出任何信息
void RollFile::notify(const char *msg, int level, bool isNow){
  if(NULL == msg || !is_enable_notify(level)) return;
	
	mutex_lock(mtx);

	int len = 0;
  if(NULL != msg)len = strlen(msg);

	if(len > 0)
	{
		char *buf = new char[len+256];
		format(buf, msg, level);

		out << buf;
		delete [] buf;
	}

	int cur_file_size = out.tellp();
	
	if(cur_file_size/1024 >= max_file_size){
		out.flush();
		roll_next_file();
	}

	if(isNow){
		out.flush();
	}
	mutex_unlock(mtx);
}
