cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project (PJ_RPI_USER)

add_library(PJ_RPI_USER PJ_RPI_USER.c)

install (TARGETS PJ_RPI_USER DESTINATION lib)

install (FILES PJ_RPI_USER.h DESTINATION include)

