@echo off

set CC=cl

set BUILD_FILES="W:\*.c"

set BLUILD_FLAGS="/DUNICODE /D_UNICODE /D_WINDLL"

if not exist "build\" mkdir "build"

pushd "build"

%CC% %BLUILD_FLAGS% %BUILD_FILES% user32.lib

popd
