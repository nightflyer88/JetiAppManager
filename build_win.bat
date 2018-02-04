@echo off


rem ##
 set paths




SET QTDIR=C:\Qt\5.10.0\msvc2015


SET QMAKE=%QTDIR%\bin\qmake.exe
SET MAKE_DIR=%QTDIR%\..\..\Tools\QtCreator\bin
SET MAKE=%MAKE_DIR%\jom.exe


SET NSISDIR=C:\Program Files (x86)\NSIS
SET BASEDIR=%~dp0


SET SOURCEDIR=%BASEDIR%src


SET BUILDDIR=%BASEDIR%build_win




SET EXEDIR=%BASEDIR%build_win\release
SET SETUPDIR=%BASEDIR%build_win\setup



rem ## make build dir

mkdir %BUILDDIR%


mkdir %SETUPDIR%



rem ## build JetiAppManager.exe

pushd %BUILDDIR%


echo "%~dp0"
%QMAKE% %SOURCEDIR%\JetiAppManager.pro -spec win32-msvc
%MAKE%
popd



rem ## copy qt-frameworks and libs to setup dir

copy %EXEDIR%\JetiAppManager.exe %SETUPDIR%
%QTDIR%\bin\windeployqt --qmldir %QTDIR%\qml %SETUPDIR%



rem ## copy openssl lib to setup dir

copy %BASEDIR%lib\*.* %SETUPDIR%



rem ## make installer

pushd %NSISDIR%
makensis.exe /V4 %BASEDIR%JetiAppManager.nsi
popd



rem ## finished ! check if any error

if %errorlevel% neq 0 goto :failed




echo .

echo #####################################

echo JetiAppManager created successfully !

echo .



pause
exit /b


:failed




echo .
echo build failed !

echo .



pause
exit /b


