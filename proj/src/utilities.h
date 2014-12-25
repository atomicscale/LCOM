#pragma once

#include "stdio.h"

extern FILE* logfile;
#define LOG_PATH "home/lcom/log.txt"
#define LOG(tag, msg) fprintf(logfile, "%s: %s\n", tag, msg)
#define LOG_VAR(tag, var) fprintf(logfile, "%s: %d\n", tag, var)


const char* getImagePath(const char* image);
