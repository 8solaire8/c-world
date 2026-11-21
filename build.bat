@echo off

if not defined VSCMD_ARG_TGT_ARCH (
    set VSCMD_SKIP_SENDTELEMETRY=1
    call "C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvars64.bat" > nil
)

if exist build rmdir /s /q  build
mkdir build

set BUILD_FLAGS=/DUNICODE /D_UNICODE /W3 /D_WINDLL /Zi /Od /MDd /D_DEBUG /EHsc /I ..\src /I /nologo

pushd "build"

cl %BUILD_FLAGS% ..\src\game\game_main.c /link /subsystem:windows /DEBUG

popd