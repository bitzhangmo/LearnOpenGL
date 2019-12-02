cmake_minimum_required(VERSION 3.12)
# 查找当前目录下的所有源文件
# 并把名称保存在 DIR_HEADER_SRCS 变量中

aux_source_directory(. DIR_LOCAL_HEADER_SRCS)

# 添加链接库
add_library(LOCALHEADER ${DIR_LOCAL_HEADER_SRCS})

