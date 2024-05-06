@echo off

mkdir bin
mkdir .build
mkdir .release

xcopy "SDL2.dll" ".build" 
xcopy "SDL2_image.dll" ".build" /i
xcopy "SDL2_ttf.dll" ".build" /i
xcopy "libwinpthread-1.dll" ".build" /i
xcopy "res" ".build/res/" /i /e /y

xcopy "SDL2.dll" ".release" /i
xcopy "SDL2_image.dll" ".release" /i
xcopy "SDL2_ttf.dll" ".release" /i
xcopy "libwinpthread-1.dll" ".release" /i
xcopy "res" ".release/res/" /i /e /y