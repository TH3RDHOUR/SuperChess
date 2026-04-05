This is an implementation of a chess engine in C++

Starting out I would like to make just a simple chess engine and maybe get more fancy as I progress in knowledge of both chess and C++

When/if you switch computers it can be a good idea sometimes to clean the build file by deleting it then rebuilding it:

```
rm -rf build
```

Each time you make a change please use the following commands from the source directory to run the program:

## To Configure the project:

```
cmake -S . -B build
```

_-S (source directory) and -B (binary directory) make for a cleaner workflow_

## To Build the project:

```
cmake --build build
```

## To Run in Windows:

```
.\build\Debug\chess_engine.exe
```

## To run in MacOS or Linux:

```
./build/chess_engine.exe
```
