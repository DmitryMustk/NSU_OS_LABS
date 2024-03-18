#ifndef TASK1_REVERSE_H
#define TASK1_REVERSE_H
#include "util.h"

int is_path_correct(const char* path_to_dir);
void get_dir_name(const char* path_to_dir, char* dir_name);
int find_dir_name_start_index(const char* path_to_dir);
char* reverse_name(char* name, error_code* error);
char* create_reverse_dir(char* dir_name, error_code* error);
int reverse_dir_content(char* path_to_dir, char* reversed_dir_name, error_code* error);
int reverse_regular_file(char* file_name, char* path, char* reversed_dir_name, error_code* error);
int reverse_file_content(char* src_path, char* dst_path, error_code* error);
char* create_file_path(char* file_name, char* path, error_code* error);
#endif