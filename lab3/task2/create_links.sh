#!/bin/bash

rm -rf links && mkdir -p links
for link_name in "create_dir" "show_dir" "rm_dir" "create_file" "show_file" "rm_file" "create_sym_link" "show_sym_link" "show_file_by_symlink" "rm_sym_link" "create_hard_link" "rm_hard_link" "show_file_permissions" "change_file_permissons"
do
    ln ./build/task2 links/$link_name
done