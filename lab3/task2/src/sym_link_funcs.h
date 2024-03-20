#ifndef TASK2_SYM_LINK_FUNCS_H
#define TASK2_SYM_LINK_FUNCS_H
int create_sym_link(char* path_to_file, char* link_name);
int rm_sym_link(char* path_to_link);
int show_sym_link(char* path_to_link);
int show_file_by_symlink(char* path_to_link);
#endif
