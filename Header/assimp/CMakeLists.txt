cmake_minimum_required(VERSION 3.12)

aux_source_directory(. DIR_ASSIMP_SRCS)

add_library(ASSIMP ${DIR_ASSIMP_SRCS})