@echo off
@rem %comspec% /k ""C:\Program Files\Microsoft Visual Studio 10.0\VC\vcvarsall.bat"" x86

:: --------------------------------------------------------------------------------------------
:: File : SetEnv.cmd
::
:: Abstract: This batch file sets the appropriate environment variables for the Windows SDK
::           build environment with respect to OS and platform type.
::
:: Usage : Setenv [/Debug | /Release][/x86 | /x64 | /ia64 ][/vista | /xp | /2003 | /2008 | /win7][-h | /?]
::
::                /Debug   - Create a Debug configuration build environment
::                /Release - Create a Release configuration build environment
::                /x86     - Create 32-bit x86 applications
::                /x64     - Create 64-bit x64 applications
::                /ia64    - Create 64-bit ia64 applications
::                /vista   - Windows Vista applications
::                /xp      - Create Windows XP SP2 applications
::                /2003    - Create Windows Server 2003 applications
::                /2008    - Create Windows Server 2008 or Vista SP1 applications
::                /win7    - Create Windows 7 applications
::
:: Note: This file makes use of delayed expansion to get around the limitations
:: of the current expansion which happens when a line of text is read , not when it is executed.
:: For more information about delayed expansion type: SET /? from the command line
:: --------------------------------------------------------------------------------------------

@call "C:\Program Files\Microsoft SDKs\Windows\v7.1\Bin\SetEnv.cmd" /Debug /win7 2>&1
if %ERRORLEVEL% EQU 0 goto begin
call "C:\Program Files\Microsoft Visual Studio 10.0\VC\vcvarsall.bat" x86 2>&1
if %ERRORLEVEL% GTR 0 goto error_SetEnv
:: nmake
:: pause Press any key
:: if errorlevel goto error_nmakeNotFound

:begin
if "%1" == "clean" goto makeclean
if "%1" == "all" goto makeall

goto end

:makeclean
nmake distclean
del Makefile
qmake -makefile -r -o Makefile
goto end


:makeall
nmake %1
goto end

:error_SetEnv
echo Error: failed to set environment 1>&2
goto end

:error_nmakeNotFound
echo Error: nmake not found 1>&2
goto end


:end