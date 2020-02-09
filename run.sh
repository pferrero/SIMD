#!/bin/bash

# variables
ancho=800
alto=600
imagenes=img/
totalIteraciones=15
# compilación
nasm -f elf32 enmascarar_asm.s -o enmascarar_asm.o
gcc -m32 main.c enmascarar_c.c enmascarar_asm.o -o ejec
# ejecución
./ejec img/desierto800x600.rgb img/peces800x600.rgb img/mascara800x600.rgb 800 600
./ejec img/cielo1920x1080.rgb img/tiburon1920x1080.rgb img/mascara1920x1080.rgb 1920 1080
./ejec img/atardecer3840x2160.rgb img/iceberg3840x2160.rgb img/mascara3840x2160.rgb 3840 2160
for escala in $(seq 2 $totalIteraciones)
do
    gm convert ${imagenes}desierto800x600.rgb -resize $((escala*100))% -compress None desierto$((escala*100)).rgb
    gm convert ${imagenes}peces800x600.rgb -resize $((escala*100))% -compress None peces$((escala*100)).rgb
    gm convert ${imagenes}mascara800x600.rgb -resize $((escala*100))% -compress None mascara$((escala*100)).rgb
    ./ejec desierto$((escala*100)).rgb peces$((escala*100)).rgb mascara$((escala*100)).rgb $((ancho*escala)) $((alto*escala))
done
