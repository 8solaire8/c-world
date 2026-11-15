@echo off

if not defined VSCMD_ARG_TGT_ARCH (
    set VSCMD_SKIP_SENDTELEMETRY=1
    call "C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvars64.bat" > nil
)

if exist W:build rmdir /s /q  W:\build
mkdir build

set "BUILD_FILES=..\main.c ..\src\*.c ..\third_party\*.c"

set TEST_FILES=..\test\*.c ..\src\*.c

set BUILD_FLAGS_DEBUG=/DUNICODE /D_UNICODE /W3 /D_WINDLL /Zi /Od /MDd /D_DEBUG /EHsc /I ..\src /I ..\third_party /nologo


set LIBS=user32.lib d3d11.lib d3dcompiler.lib dxguid.lib user32.lib /link /subsystem:windows /DEBUG

set BUILD_FLAGE_RELEASE=""

pushd "build"

cl %BUILD_FLAGS_DEBUG% %BUILD_FILES% %LIBS%

:: %CC% %BUILD_FLAGS_DEBUG% %TEST_FILES%

popd
