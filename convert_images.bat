@echo off
copy "C:\Users\Escritorio2\Downloads\Captura de tela 2024-06-24 103950.png" .
ren "Captura de tela 2024-06-24 103950.png" "image1.png
copy "C:\Users\Escritorio2\Downloads\ManipuladorBaseMovel.png" .
ren "ManipuladorBaseMovel.png" "image2.png
ffmpeg -f concat -safe 0 -i files.txt -vf "fps=10,pad=778:660:(ow-iw)/2:(oh-ih)/2" -loop 0 C:\Users\Escritorio2\Downloads\huh.gif
del image1.png
del palette.png
del image2.png
del palette.png
explorer C:\Users\Escritorio2\Downloads\huh.gif
