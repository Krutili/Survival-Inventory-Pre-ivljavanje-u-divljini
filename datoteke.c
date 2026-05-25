#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>

#include "datoteke.h"

#define DATOTEKA_SPREMANJA "savegame.bin"

void spremiIgru(Igrac* igrac) {

    FILE* datoteka = fopen(DATOTEKA_SPREMANJA, "wb");

    if (datoteka == NULL) {
        printf("Greska kod spremanja.\n");
        return;
    }

    fwrite(igrac, sizeof(Igrac), 1, datoteka);

    fwrite(
        igrac->inventory,
        sizeof(Predmet),
        igrac->brojPredmeta,
        datoteka
    );

    fclose(datoteka);

    printf("Igra je spremljena.\n");
}

int ucitajIgru(Igrac* igrac) {

    FILE* datoteka = fopen(DATOTEKA_SPREMANJA, "rb");

    if (datoteka == NULL) {
        return 0;
    }

    fread(igrac, sizeof(Igrac), 1, datoteka);

    igrac->inventory = malloc(
        igrac->kapacitet * sizeof(Predmet)
    );

    fread(
        igrac->inventory,
        sizeof(Predmet),
        igrac->brojPredmeta,
        datoteka
    );

    fclose(datoteka);

    return 1;
}