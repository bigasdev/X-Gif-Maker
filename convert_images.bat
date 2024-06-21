@echo off
copy "C:\Users\Escritorio2\Downloads\ManipuladorBaseMovel.png" .
ren "ManipuladorBaseMovel.png" "image1.png
copy "C:\Users\Escritorio2\Downloads\ManipuladorCimento.png" .
ren "ManipuladorCimento.png" "image2.png
copy "C:\Users\Escritorio2\Downloads\ManipuladorMovel.png" .
ren "ManipuladorMovel.png" "image3.png
copy "C:\Users\Escritorio2\Downloads\ManipuladorBaseMovel2.png" .
ren "ManipuladorBaseMovel2.png" "image4.png
ffmpeg -f concat -safe 0 -i files.txt -vf "fps=10,pad=696:564:(ow-iw)/2:(oh-ih)/2" -loop 0 C:\Users\Escritorio2\Downloads\teste15final.gif
del image1.png
del palette.png
del image2.png
del palette.png
del image3.png
del palette.png
del image4.png
del palette.png
explorer C:\Users\Escritorio2\Downloads\teste15final.gif
