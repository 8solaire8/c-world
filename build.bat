@echo off
set "RED=%ESC%[31m"
set "RESET=%ESC%[0m"

if not defined VSCMD_ARG_TGT_ARCH (
    set VSCMD_SKIP_SENDTELEMETRY=1
    call "C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvars64.bat" > nul
)

if exist build rmdir /s /q build
mkdir build

set "BUILD_FLAGS=/DUNICODE /D_UNICODE /W3 /D_WINDLL /Zi /Od /MDd /D_DEBUG /EHsc /I ..\src /I /nologo"

if /i "%1"=="release" (
    set "BUILD_FLAGS=/O2 /MT /DNDEBUG /I"..\src" /nologo"
    ECHO %RED%Release mode%RESET%
)

pushd "build"

cl %BUILD_FLAGS% ..\src\game\game_main.c /Fe:game.exe /link /subsystem:windows /DEBUG 

popd