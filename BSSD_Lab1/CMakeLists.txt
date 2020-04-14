cmake_minimum_required(VERSION 3.15)
project(OBRPO_lab1 C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "-fno-stack-protector -D_FORTIFY_SOURCE=0")

add_executable(OBRPO_lab1 main.c)