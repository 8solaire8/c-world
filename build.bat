@echo off

set CC=cl

set BUILD_FILES="W:\*.c"

set TEST_FILES=W:\test\*.c W:\src\*.c

set BUILD_FLAGS_DEBUG=/DUNICODE /D_UNICODE /D_WINDLL /I W:\src

set BUILD_FLAGE_RELEASE=""

if not exist "build\" mkdir "build"

pushd "build"

%CC% %BUILD_FLAGS_DEBUG% %BUILD_FILES% user32.lib

%CC% %BUILD_FLAGS_DEBUG% %TEST_FILES%

popd
