#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "igra.h"
#include "datoteke.h"

int main() {

    Igrac igrac;

    int izbor;
    int radi = 1;

    srand((unsigned int)time(NULL));

    igrac.inventory = NULL;
    igrac.brojPredmeta = 0;
    igrac.kapacitet = 0;

    while (radi) {

        prikaziGlavniIzbornik();

        printf("Odaberi opciju: ");
        scanf("%d", &izbor);

        ocistiUlaz();

        switch (izbor) {

        case NOVA_IGRA:
            novaIgra(&igrac);
            break;

        case UCITAJ_IGRU:
            if (ucitajIgru(&igrac)) {
                printf("Igra je uspjesno ucitana.\n");
            }
            else {
                printf("Spremljena igra ne postoji.\n");
            }
            break;

        case PRIKAZ_IGRACA:
            prikaziIgraca(&igrac);
            break;

        case PRIKAZ_INVENTORYJA:
            prikaziInventory(&igrac);
            break;

        case ISTRAZI:
            istraziPodrucje(&igrac);
            break;

        case KORISTI_PREDMET:
            koristiPredmet(&igrac);
            break;

        case SPREMI_IGRU:
            spremiIgru(&igrac);
            break;

        case NOVI_DAN:

            noviDan(&igrac);

            if (igracJeMrtav(&igrac)) {

                printf("Igrac je umro. Kraj igre.\n");

                radi = 0;
            }

            break;

        case IZLAZ:
            radi = 0;
            break;

        default:
            printf("Neispravan odabir.\n");
            break;
        }

        printf("\n");
    }

    oslobodiMemoriju(&igrac);

    return 0;
}