@echo off
rem you could also remove the line above, because it might help you to see what happens

rem /i option is needed to avoid the batch file asking you whether destination folder is a file or a folder
rem /e option is needed to copy also all folders and subfolders
rem /y 

del "bin\*"
xcopy "SDL2.dll" ".release" /i
xcopy "SDL2_image.dll" ".release" /i
xcopy "SDL2_ttf.dll" ".release" /i
xcopy "libwinpthread-1.dll" ".release" /i
xcopy "res" ".release/res/" /i /e /y
mingw32-make build DEBUG=false
del "bin\*"
echo Build successful
timeout /t 3