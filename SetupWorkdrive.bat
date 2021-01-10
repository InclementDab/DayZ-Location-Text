@echo off

cd /D "%~dp0"

IF exist "P:\LocationText\" (
	echo Removing existing link P:\LocationText
	rmdir "P:\LocationText\"
)

echo Creating link P:\LocationText
mklink /J "P:\LocationText\" "%cd%\LocationText\"

echo Done