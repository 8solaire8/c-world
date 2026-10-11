@echo off

set CC=cl

set BUILD_FILES=W:\main.c W:\src\*.c

set TEST_FILES=W:\test\*.c W:\src\*.c

set BUILD_FLAGS_DEBUG=/DUNICODE /D_UNICODE /D_WINDLL /I W:\src


set LIBS=user32.lib d3d11.lib d3dcompiler.lib dxguid.lib user32.lib /link /subsystem:windows

set BUILD_FLAGE_RELEASE=""

if not exist "build\" mkdir "build"

pushd "build"

%CC% %BUILD_FLAGS_DEBUG% %BUILD_FILES% %LIBS%

:: %CC% %BUILD_FLAGS_DEBUG% %TEST_FILES%

popd
