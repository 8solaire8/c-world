@echo off

if not defined VSCMD_ARG_TGT_ARCH (
    set VSCMD_SKIP_SENDTELEMETRY=1
    call "C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat" x64 >nul
)

if exist W:build rmdir /s /q  W:\build
mkdir build

set CC=cl

set BUILD_FILES=W:\main.c W:\src\*.c

set TEST_FILES=W:\test\*.c W:\src\*.c

set BUILD_FLAGS_DEBUG=/DUNICODE /D_UNICODE /D_WINDLL /Zi /Od /MDd /D_DEBUG /EHsc /I W:\src


set LIBS=user32.lib d3d11.lib d3dcompiler.lib dxguid.lib user32.lib /link /subsystem:windows /DEBUG

set BUILD_FLAGE_RELEASE=""

pushd "build"

%CC% %BUILD_FLAGS_DEBUG% %BUILD_FILES% %LIBS%

:: %CC% %BUILD_FLAGS_DEBUG% %TEST_FILES%

popd
