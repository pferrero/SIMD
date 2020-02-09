#include <stdio.h>
#include <stdlib.h>
#include "imagen.h"

void enmascarar_c(Byte *a, Byte *b, Byte *mask, int cant) {
  
  for( int i = HEADER_SIZE; i < cant; i++ )
    *(a + i) = transformar_byte(*(a + i), *(b + i), *(mask + i));

}

Byte transformar_byte(Byte a, Byte b, Byte mascara) {
  return (a & ~mascara) | (b & mascara);
}
