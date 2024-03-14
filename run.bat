cd v1\external\box2d\

if exist build\bin\ (
	echo Build folder exists. Check bin folder for binaries.
) else (
	call build.bat
)

cd ../../..

call v1\external\premake\premake5.exe vs2022
PAUSE