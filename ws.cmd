@echo off

if "%CWT_HOME%" == "" set CWT_HOME=\

if "%1" == "-create" goto create_begin
if "%1" == "-clear"  goto clear

set PROJECT=%1

if "%PROJECT%" == "" goto usage
goto create

:create_begin
set PROJECT=%2

:create
if "%PROJECT%" == "" goto usage

if exist %CWT_HOME%%PROJECT% goto error_exists

cd %CWT_HOME%
mkdir %PROJECT%
cd %PROJECT%
mkdir .qmake
mkdir .msvc
mkdir include
mkdir tests

cd include 
mkdir %PROJECT%

cd ..\.msvc
mkdir %PROJECT%

cd ..\.qmake
mkdir tests
mkdir %PROJECT%

echo #************************************************************  > %PROJECT%.pro
echo #* Generated automatically by '%0'                            >> %PROJECT%.pro
echo #* script:  %0                                                >> %PROJECT%.pro
echo #* command: %CMDCMDLINE%                                      >> %PROJECT%.pro
echo #* user:    %USERNAME%                                        >> %PROJECT%.pro
echo #* date:    %DATE%                                            >> %PROJECT%.pro
echo #************************************************************ >> %PROJECT%.pro
echo TEMPLATE = subdirs                                            >> %PROJECT%.pro
echo CONFIG  += ordered                                            >> %PROJECT%.pro
echo SUBDIRS  = %PROJECT%                                          >> %PROJECT%.pro

cd ..
echo ^@echo off         > clear.cmd
echo del ^/S ^*.o      >> clear.cmd
echo del ^/S ^*.obj    >> clear.cmd
echo del ^/S ^*.exe    >> clear.cmd
echo del ^/S ^*.swp    >> clear.cmd
echo del ^/S ^*.sdf    >> clear.cmd


echo Project %PROJECT% live in %CWT_HOME%%PROJECT% directory
echo Modyfy .qmake^\%PROJECT%.pro to add new subprojects
goto end

:clear
goto end


:usage
echo Usage:
echo     ws ^[-create^|-clear^] PROJECTNAME
goto end

:error_exists
echo Error: Directory already exists: %CWT_HOME%%PROJECT%
goto end

:end