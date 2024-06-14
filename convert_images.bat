@echo off
copy "C:\Users\Escritorio2\Downloads\Transportador de roletes.png" .
ren "Transportador de roletes.png" "image1.png
copy "C:\Users\Escritorio2\Downloads\ManipuladorSacaria.png" .
ren "ManipuladorSacaria.png" "image2.png
ffmpeg -f concat -safe 0 -i files.txt -vf "pad=388:377:(ow-iw)/2:(oh-ih)/2" -t 15 C:\Users\Escritorio2\Downloads\test.gif
del image1.png
del palette.png
del image2.png
del palette.png
