#ifndef GENIUS_SIMPLE_LOG_H_
#define GENIUS_SIMPLE_LOG_H_

#include "log.h"
#include "plan.h"
#include "roll_file.h"

#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_utils.hpp"
#include "rapidxml/rapidxml_print.hpp"

using namespace rapidxml;
using namespace xlog;

void InitConfig(const char *path){
  if(path == NULL) return;

  file<> fdoc(path);
  xml_document<> doc;
  doc.parse<0>(fdoc.data());

  xml_node<>* root = doc.first_node();
  while(root != NULL){
    xml_attribute<> *attr;
    attr = root->first_attribute("type");
    string type = attr->value();  

    attr = root->first_attribute("id");
    string id = attr->value();
    
    xml_node<>* child;
    child = root->first_node("filename");
    string file_name = child->value();

    child = root->first_node("filenum");
    string str_file_num = child->value();

    child = root->first_node("filesize");
    string str_file_size = child->value();

    child = root->first_node("mode");
    string str_mode = child->value();

    child = root->first_node("path");
    string dir = child->value();
  
    root = root->next_sibling();
    int mode, file_size, file_num;
    sscanf(str_mode.c_str(), "%d", &mode);
    sscanf(str_file_size.c_str(), "%d", &file_size);
    sscanf(str_file_num.c_str(), "%d", &file_num);
    if(type != "RollFile")
      continue;

    Plan *new_plan = new RollFile(file_name.c_str(), dir.c_str(), 
                                  file_size, 
                                  file_num,
                                  mode);
    Log Logger(id, new_plan);
  }
}

#define SimpleLog(id, lvl, format)  \
do{     \
  char buf[4096];\
  va_list arglist; \
  va_start(arglist, format);\
  __vsnprintf(buf, 4096, format, arglist);\
  va_end(arglist);\
  Log logger(id);\
  logger.write3(buf, lvl, false);\
}while(0);

//错误信息
void log_error(const char *format, ...){
  SimpleLog("default", LOG_ERROR, format);
 }

void log_error(const char *id, const char *format, ...){
  SimpleLog(id, LOG_ERROR, format);
}

void log_normal(const char *format, ...){ 
  SimpleLog("default", LOG_NORMAL, format);
}
void log_normal(const char *id, const char *format, ...){
  SimpleLog(id, LOG_NORMAL, format);
}

void log_debug(const char *format, ...){ 
  SimpleLog("default", LOG_DEBUG, format);
}
void log_debug(const char *id, const char *format, ...){
  SimpleLog(id, LOG_DEBUG, format);
}

void log_warning(const char *format, ...){ 
  SimpleLog("default", LOG_WARNING, format);
}
void log_warning(const char *id, const char *format, ...){ 
  SimpleLog(id, LOG_WARNING, format);
}

void log_fatal(const char *format, ...) {
  SimpleLog("default", LOG_FATAL, format);
}
void log_fatal(const char *id, const char *format, ...){ 
  SimpleLog(id, LOG_FATAL, format);
}

#endif
