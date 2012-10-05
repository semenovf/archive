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
mkdir include
mkdir src
mkdir tests

cd include 
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

:: Prepare clean.bat
:: (TODO see clean.lst)

:: Prepare build.bat
copy %CWT_HOME%\template\build.bat build.bat

:: Prepare .gitignore
copy %CWT_HOME%\template\gitignore + %CWT_HOME%\template\clean.lst .gitignore
echo ^!.gitignore >> .gitignore

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