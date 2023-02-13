# Yal

Yet another minimal C++ logger.

A C++ logger is a software component that is used for logging or recording messages and events in a C++ program. 
Loggers are used for debugging, monitoring, and diagnosing issues in software systems. \
They provide a way to output information about the program's behavior and status to a log file, console, or other output destination. Loggers typically have different levels of logging, such as error, warning, information, and debug, to allow developers to control the amount and type of information that is recorded. 
They may also allow for customization of the log output format and destination. 
C++ loggers are often implemented as libraries or classes that can be easily integrated into a C++ application.

This project serves as a way to easily and simply implement a C++ logger library into your project.

## Minimal usage

```c++
#include <Logger.h>

using namespace log;

int main()
{
    Logger * logger = Logger::GetLogger("myLogger");
    logger->Info("Your Log here");
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


### Included Tests:
This repo includes a test folder.

In this folder you can find default tests to help you. 

You can also write your own tests

Example test case:
```
TEST_CASE("A default constructed optional has no value.") {
  const optional_unsigned_int x;
  REQUIRE(x.has_value() == false);
}
```
