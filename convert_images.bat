@echo off
copy "C:\Users\Escritorio2\Desktop\Redes sociais\2 - Res\3 - Shared\Galões.png" .
ren "Galões.png" "image1.jpeg"
copy "C:\Users\Escritorio2\Desktop\Redes sociais\2 - Res\3 - Shared\icone-barrica.png" .
ren "icone-barrica.png" "image2.jpeg"
copy "C:\Users\Escritorio2\Desktop\Redes sociais\2 - Res\3 - Shared\logo-cta-azul.png" .
ren "logo-cta-azul.png" "image3.jpeg"
ffmpeg -f concat -i files.txt -vf "pad=1600:1200:(ow-iw)/2:(oh-ih)/2" -t 15 C:\Users\Escritorio2\Downloads\teste.mp4
del image1.jpeg
del image2.jpeg
del image3.jpeg
