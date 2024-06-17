@echo off
copy "C:\Users\Escritorio2\Downloads\ManipuladorSacaria.png" .
ren "ManipuladorSacaria.png" "image1.png
copy "C:\Users\Escritorio2\Downloads\teste.png" .
ren "teste.png" "image2.png
copy "C:\Users\Escritorio2\Downloads\ManipuladorBaseMovel2.png" .
ren "ManipuladorBaseMovel2.png" "image3.png
copy "C:\Users\Escritorio2\Downloads\ManipuladorBaseMovel.png" .
ren "ManipuladorBaseMovel.png" "image4.png
copy "C:\Users\Escritorio2\Downloads\ManipuladorCimento.png" .
ren "ManipuladorCimento.png" "image5.png
ffmpeg -f concat -safe 0 -i files.txt -vf "pad=1024:635:(ow-iw)/2:(oh-ih)/2" -t 15 C:\Users\Escritorio2\Downloads\whyyy.gif
del image1.png
del palette.png
del image2.png
del palette.png
del image3.png
del palette.png
del image4.png
del palette.png
del image5.png
del palette.png
