@echo off
copy "C:\Users\Escritorio2\Downloads\ManipuladorSacaria.png" .
ren "ManipuladorSacaria.png" "image1.png
copy "C:\Users\Escritorio2\Downloads\ManipuladorMovel.png" .
ren "ManipuladorMovel.png" "image2.png
ffmpeg -f concat -safe 0 -i files.txt -vf "pad=696:564:(ow-iw)/2:(oh-ih)/2" -t 15 C:\Users\Escritorio2\Downloads\heyhe.gif
del image1.png
del palette.png
del image2.png
del palette.png
