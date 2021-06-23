

cmake_minimum_required(VERSION 3.15)
project(KY_SERVER)


include_directories(/usr/local/include)
link_directories(/usr/local/bin)

file(GLOB_RECURSE src *.c *.h)
add_executable(ky_server ${src})