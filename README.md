# Getting Started

## Prerequisites
- CPP

## Compiling
When you compile the .cpp files use the following arguments on the cpp compiler. `-DUNICODE` and `-D_UNICODE`
You may use a command like this.<br/>
WINDOWS:
```sh
c++ -g ./Snake.cpp -o ./dist/Snake.exe -DUNICODE -D_UNICODE
```
UNIX:
```sh
c++ -g ./Snake.cpp -o ./dist/Snake.out -DUNICODE -D_UNICODE
```