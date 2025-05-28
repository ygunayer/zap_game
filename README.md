# Game Sample
A sample project for creating games using [zap](https://github.com/ygunayer/zzz/tree/main/zap).

This project currently serves as my playground for implementing my own renderers so it's under heavy development. Even zap istelf isn't properly added as a submodule, but edited directly within the project for ease of use.

## Running
There's not much to do yet, but if you're still interested, just compile the project using CMake and run it.

On Windows:
```batch
REM change to `release` for release mode
> cmake --preset debug
> cmake --build --preset debug
> .\Bin\Debug\game.exe
```

On MacOS, Linux, BSD, etc.:
```sh
> cmake --preset debug
> cmake --build --preset debug
> ./bin/Debug/game
```
