#ifndef TASK1_REVERSE_H
#define TASK1_REVERSE_H
#include "util.h"

int is_path_correct(const char* path_to_dir);
void get_dir_name(const char* path_to_dir, char* dir_name);
int find_dir_name_start_index(const char* path_to_dir);
char* reverse_name(char* name);
int create_reverse_dir(char* dir_name, error_code error);
#endif