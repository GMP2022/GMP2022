cmake_minimum_required (VERSION 2.8)

project(mips)

aux_source_directory(mips_codes SRC_LIST)
add_executable(mips.exe ${SRC_LIST})
