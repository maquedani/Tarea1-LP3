#include <stdio.h>
#include <tiffio.h>

int main (int argc, char** argv)
{
    TIFF* tiff;
    tiff = TIFFOpen (argv[1], "r");

    /* Condición agregada para confirmar que el archivo fue abierto correctamente */
    if (tiff){
        printf("Archivo TIFF abierto correctamente\n");        
    }

    TIFFClose (tiff);
    return 0;
}