@echo off

if not defined VSCMD_ARG_TGT_ARCH (
    set VSCMD_SKIP_SENDTELEMETRY=1
    call "C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvars64.bat" > nil
)

set TEST_FILES=..\test\*.c ..\src\*.c


:: %CC% %BUILD_FLAGS_DEBUG% %TEST_FILES%