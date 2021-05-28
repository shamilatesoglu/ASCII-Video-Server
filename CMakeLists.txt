cmake_minimum_required(VERSION 3.18)
project(Streaming_Server)

set(CMAKE_C_STANDARD 99)

add_executable(sserver src/sserver.c src/string_utils.c src/string_utils.h src/lang_utils.h src/video.c src/video.h)