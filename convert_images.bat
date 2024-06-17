@echo off
copy "C:\Users\Escritorio2\Downloads\ManipuladorSacaria.png" .
ren "ManipuladorSacaria.png" "image1.png
copy "C:\Users\Escritorio2\Downloads\ManipuladorBaseMovel2.png" .
ren "ManipuladorBaseMovel2.png" "image2.png
copy "C:\Users\Escritorio2\Downloads\ManipuladorBaseMovel.png" .
ren "ManipuladorBaseMovel.png" "image3.png
ffmpeg -f concat -safe 0 -i files.txt -vf "pad=696:564:(ow-iw)/2:(oh-ih)/2" -t 15 -loop 1C:\Users\Escritorio2\Downloads\heyo.gif
del image1.png
del palette.png
del image2.png
del palette.png
del image3.png
del palette.png
explorer C:\Users\Escritorio2\Downloads\heyo.gif
