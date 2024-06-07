@echo off
copy "C:\Users\Escritorio2\Downloads\WhatsApp Image 2024-06-06 at 16.02.31(1).jpeg" .
ren "WhatsApp Image 2024-06-06 at 16.02.31(1).jpeg" "image1.jpeg"
copy "C:\Users\Escritorio2\Downloads\WhatsApp Image 2024-06-06 at 16.02.31.jpeg" .
ren "WhatsApp Image 2024-06-06 at 16.02.31.jpeg" "image2.jpeg"
REM Step 1: Generate list of images for each GIF with specified durations
setlocal EnableDelayedExpansion
for /F "tokens=*" %%A in ('type "timeline.txt"') do (
    set "line=%%A"
    for /F "tokens=1,2" %%B in (!line!) do (
        set "gif_file=%%B"
        set "duration=%%C"
        for /L %%i in (1,1,!duration!) do (
            echo file '^!gif_file^!'
        )
    )
)
endlocal > "files.txt"
ffmpeg -framerate 2 -i image%%d.jpeg -vf "pad=180:100:(ow-iw)/2:(oh-ih)/2" -t 1 output.gif
del image1.jpeg
del image2.jpeg
