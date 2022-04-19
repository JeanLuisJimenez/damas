#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>

void clear_output();
void count_cols ();
FILE* open_file(const char *ruta_nombre, const char *modo);

void clear_output()
{
    system("cls || clear");
}

void count_cols ()
{
    printf("\n\t  ");
    for (int col = 0; col < 8; col++)
        printf("%c ", col + 97);
    printf("\n\n");
}

FILE* open_file(const char *ruta_nombre, const char *modo)
{
    FILE *archivo = fopen(ruta_nombre, modo);
    if(archivo == NULL) exit(1);

    return archivo;
}

#endif // UTILS_H
