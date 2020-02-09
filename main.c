#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "imagen.h"

void enmascarar_c(Byte*, Byte*, Byte*, int);
void enmascarar_asm(Byte*, Byte*, Byte*, int);

static clock_t inicio_c, fin_c, inicio_asm, fin_asm;
static double total_c = 0, total_asm = 0;
static char SALIDA_C[] = "salida_c";
static char SALIDA_ASM[] = "salida_asm";
static char RGB[] = ".rgb";
static int imagen_size;
static FILE *imagen1;
static FILE *imagen2;
static FILE *mascara;
static FILE *output;
static Byte *buff_imagen1_c1;
static Byte *buff_imagen1_c2;
static Byte *buff_imagen2;
static Byte *buff_mascara;
static char string_buff_c[30];
static char string_buff_asm[30];

/* Recibe 5 argumentos del usuario desde la línea de comandos:
   arg1: imagen1, arg2: imagen2, arg3: mascara, arg4: ancho, arg5: alto */
int main( int argc, char *argv[] ) {
  if( argc != 6 ) {
    fprintf(stderr, "Cantidad de argumentos incorrecta. arg1=imagen1, arg2=imagen2, arg3=mascara, arg4=ancho, arg5=alto.\n");
    exit(1);
  }
  
  imagen1 = fopen(argv[1], LECTURA);
  imagen2 = fopen(argv[2], LECTURA);
  mascara = fopen(argv[3], LECTURA);
  
  if( imagen1 == NULL ) {
    fprintf(stderr, "Imagen 1 no encontrada.\n");
    exit(1);
  }
  if( imagen2 == NULL ) {
    fprintf(stderr, "Imagen 2 no encontrada.\n");
    exit(1);
  }
  if( mascara == NULL ) {
    fprintf(stderr, "Máscara no encontrada.\n");
    exit(1);
  }

  imagen_size = atoi(argv[4]) * atoi(argv[5]) * 3 + 512;
  if( fsize(imagen1) != imagen_size || fsize(imagen2) != imagen_size || fsize(mascara) != imagen_size) {
    fprintf(stderr, "Las imágenes no tienen el mismo tamaño.\n");
    exit(1);
  }

  strcpy(string_buff_c, SALIDA_C);
  strcat(string_buff_c, argv[4]);
  strcat(string_buff_c, "x");
  strcat(string_buff_c, argv[5]);
  strcat(string_buff_c, RGB);  
  
  strcpy(string_buff_asm, SALIDA_ASM);
  strcat(string_buff_asm, argv[4]);
  strcat(string_buff_asm, "x");
  strcat(string_buff_asm, argv[5]);
  strcat(string_buff_asm, RGB);
  
  buff_imagen1_c1 = (Byte *) malloc(imagen_size);
  buff_imagen1_c2 = (Byte *) malloc(imagen_size);
  buff_imagen2 = (Byte *) malloc(imagen_size);
  buff_mascara = (Byte *) malloc(imagen_size);

  fread(buff_imagen1_c1, 1, imagen_size, imagen1);
  fseek(imagen1, 0, SEEK_SET);
  fread(buff_imagen1_c2, 1, imagen_size, imagen1);
  fread(buff_imagen2, 1, imagen_size, imagen2);
  fread(buff_mascara, 1, imagen_size, mascara);

  fclose(imagen1);
  fclose(imagen2);
  fclose(mascara);

  inicio_c = clock();
  enmascarar_c(buff_imagen1_c1, buff_imagen2, buff_mascara,  imagen_size);
  fin_c = clock();
  
  inicio_asm = clock();
  enmascarar_asm(buff_imagen1_c2, buff_imagen2, buff_mascara, imagen_size);
  fin_asm = clock();  

  free(buff_imagen2);
  free(buff_mascara);

  total_c   = (double) (fin_c - inicio_c)     / CLOCKS_PER_SEC;
  total_asm = (double) (fin_asm - inicio_asm) / CLOCKS_PER_SEC;
  
  output = fopen(string_buff_c, ESCRITURA);
  fwrite(buff_imagen1_c1, 1, imagen_size, output);
  fclose(output);

  output = fopen(string_buff_asm, ESCRITURA);
  fwrite(buff_imagen1_c2, 1, imagen_size, output);
  fclose(output);

  free(buff_imagen1_c1);
  free(buff_imagen1_c2);

  // tamaño, tiempo_c, tiempo_asm
  fprintf(stdout, "%d, %f, %f\n", imagen_size, total_c, total_asm);

}

int fsize(FILE *p) {
  fseek(p, 0, SEEK_END);
  long int size = ftell(p);
  fseek(p, 0, SEEK_SET);
  return (int) size;
}
