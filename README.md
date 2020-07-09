# Yal
Yet another minimal C++ logger.

## Minimal usage
```c++
#include <Logger.h>

using namespace log;

int main()
{
    Logger * logger = Logger::getLogger("myLogger");
    logger->info("Your log here");
}
```

## Build integration
This expects you to have this project in a subdirectory in your project
### Using Cmake
```cmake
cmake_minimum_required(VERSION 3.15)
project(MyProject)

set(CMAKE_CXX_STANDARD 17) # needed for inline variable

add_subdirectory(yal)

add_executable(MyExecutable src/main.cpp)
target_link_libraries(MyExecutable yal)
```
### Using Makefile
```
SRCS = src/main.cpp
OBJS = $(addsuffix .o,$(basename $(SRCS)))

MyExecutable: $(OBJS)
	g++ -o $@ $^

%.o: %.c
	g++ -std=c++17 -Iyal/ -c -o $@ $^

clean:
	rm $(OBJS)

.PHONY: clean
```
