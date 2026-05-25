#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "igra.h"

void ocistiUlaz() {

    int znak;

    while ((znak = getchar()) != '\n' && znak != EOF);
}

void prikaziGlavniIzbornik() {

    printf("=================================\n");
    printf("      SURVIVAL INVENTORY\n");
    printf("=================================\n");

    printf("1. Nova igra\n");
    printf("2. Ucitaj igru\n");
    printf("3. Prikazi igraca\n");
    printf("4. Prikazi inventory\n");
    printf("5. Istrazi podrucje\n");
    printf("6. Koristi predmet\n");
    printf("7. Spremi igru\n");
    printf("8. Novi dan\n");
    printf("9. Sortiraj inventory po nazivu\n");
    printf("10. Pretrazi predmet po nazivu\n");
    printf("11. Obrisi predmet\n");
    printf("0. Izlaz\n");
}

void novaIgra(Igrac* igrac) {

    printf("Unesi ime igraca: ");
    fgets(igrac->ime, MAKS_NAZIV, stdin);

    igrac->ime[strcspn(igrac->ime, "\n")] = '\0';

    igrac->zdravlje = 100;
    igrac->glad = 0;
    igrac->zed = 0;
    igrac->energija = 100;
    igrac->dan = 1;

    igrac->kapacitet = 5;
    igrac->brojPredmeta = 0;

    igrac->inventory = malloc(igrac->kapacitet * sizeof(Predmet));

    printf("Nova igra je pokrenuta.\n");
}

void prikaziIgraca(Igrac* igrac) {

    printf("Ime: %s\n", igrac->ime);
    printf("Zdravlje: %d\n", igrac->zdravlje);
    printf("Glad: %d\n", igrac->glad);
    printf("Zed: %d\n", igrac->zed);
    printf("Energija: %d\n", igrac->energija);
    printf("Dan: %d\n", igrac->dan);
}

void prikaziInventory(Igrac* igrac) {

    if (igrac->brojPredmeta == 0) {
        printf("Inventory je prazan.\n");
        return;
    }

    for (int i = 0; i < igrac->brojPredmeta; i++) {
        printf(
            "%d. %s | kolicina: %d | vrijednost: %d\n",
            i + 1,
            igrac->inventory[i].naziv,
            igrac->inventory[i].kolicina,
            igrac->inventory[i].vrijednost
        );
    }
}

void dodajPredmet(Igrac* igrac, const char* naziv, VrstaPredmeta vrsta, int kolicina, int vrijednost) {

    if (igrac->brojPredmeta >= igrac->kapacitet) {

        igrac->kapacitet *= 2;

        igrac->inventory = realloc(
            igrac->inventory,
            igrac->kapacitet * sizeof(Predmet)
        );
    }

    Predmet* p = &igrac->inventory[igrac->brojPredmeta];

    p->id = igrac->brojPredmeta + 1;
    strcpy(p->naziv, naziv);
    p->vrsta = vrsta;
    p->kolicina = kolicina;
    p->vrijednost = vrijednost;

    igrac->brojPredmeta++;
}

void istraziPodrucje(Igrac* igrac) {

    int slucajno = rand() % 4;

    if (slucajno == 0) {
        dodajPredmet(igrac, "Hrana", HRANA, 1, 20);
    }
    else if (slucajno == 1) {
        dodajPredmet(igrac, "Voda", VODA, 1, 20);
    }
    else if (slucajno == 2) {
        dodajPredmet(igrac, "Drvo", MATERIJAL, 2, 5);
    }
    else {
        dodajPredmet(igrac, "Lijek", LIJEK, 1, 30);
    }

    igrac->glad += 10;
    igrac->zed += 10;
    igrac->energija -= 10;

    printf("Istrazio si podrucje i pronasao predmet.\n");
}

void koristiPredmet(Igrac* igrac) {

    int odabir;

    prikaziInventory(igrac);

    if (igrac->brojPredmeta == 0) {
        return;
    }

    printf("Odaberi predmet za koristenje: ");
    scanf("%d", &odabir);

    ocistiUlaz();

    if (odabir < 1 || odabir > igrac->brojPredmeta) {
        printf("Neispravan odabir.\n");
        return;
    }

    Predmet* p = &igrac->inventory[odabir - 1];

    if (p->vrsta == HRANA) {
        igrac->glad -= 20;
    }
    else if (p->vrsta == VODA) {
        igrac->zed -= 20;
    }
    else if (p->vrsta == LIJEK) {
        igrac->zdravlje += 20;
    }

    for (int i = odabir - 1; i < igrac->brojPredmeta - 1; i++) {
        igrac->inventory[i] = igrac->inventory[i + 1];
    }

    igrac->brojPredmeta--;

    printf("Predmet je iskoristen.\n");
}

void oslobodiMemoriju(Igrac* igrac) {

    free(igrac->inventory);
    igrac->inventory = NULL;
}

void noviDan(Igrac* igrac) {

    igrac->dan++;

    igrac->glad += 15;
    igrac->zed += 15;

    igrac->energija += 20;

    if (igrac->energija > 100) {

        igrac->energija = 100;
    }

    if (igrac->glad >= 100) {

        igrac->zdravlje -= 15;

        printf("Prevelika glad! Gubis zdravlje.\n");
    }

    if (igrac->zed >= 100) {

        igrac->zdravlje -= 20;

        printf("Prevelika zed! Gubis zdravlje.\n");
    }

    if (igrac->zdravlje < 0) {

        igrac->zdravlje = 0;
    }

    printf("Prosao je jedan dan.\n");
}

int igracJeMrtav(const Igrac* igrac) {

    if (igrac->zdravlje <= 0) {

        return 1;
    }

    return 0;
}

int usporediPredmetePoNazivu(const void* a, const void* b) {

    const Predmet* predmetA = (const Predmet*)a;
    const Predmet* predmetB = (const Predmet*)b;

    return strcmp(predmetA->naziv, predmetB->naziv);
}

int usporediPredmetSKljucem(const void* kljuc, const void* element) {

    const char* trazeniNaziv = (const char*)kljuc;
    const Predmet* predmet = (const Predmet*)element;

    return strcmp(trazeniNaziv, predmet->naziv);
}

void sortirajInventoryPoNazivu(Igrac* igrac) {

    if (igrac->brojPredmeta == 0) {

        printf("Inventory je prazan.\n");
        return;
    }

    qsort(
        igrac->inventory,
        igrac->brojPredmeta,
        sizeof(Predmet),
        usporediPredmetePoNazivu
    );

    printf("Inventory je sortiran po nazivu.\n");
}

void pretraziPredmetPoNazivu(Igrac* igrac) {

    char trazeniNaziv[MAKS_NAZIV];

    if (igrac->brojPredmeta == 0) {

        printf("Inventory je prazan.\n");
        return;
    }

    sortirajInventoryPoNazivu(igrac);

    printf("Unesi naziv predmeta za pretragu: ");
    fgets(trazeniNaziv, MAKS_NAZIV, stdin);

    trazeniNaziv[strcspn(trazeniNaziv, "\n")] = '\0';

    Predmet* pronadeniPredmet = bsearch(
        trazeniNaziv,
        igrac->inventory,
        igrac->brojPredmeta,
        sizeof(Predmet),
        usporediPredmetSKljucem
    );

    if (pronadeniPredmet != NULL) {

        printf("Predmet je pronaden.\n");
        printf("Naziv: %s\n", pronadeniPredmet->naziv);
        printf("Kolicina: %d\n", pronadeniPredmet->kolicina);
        printf("Vrijednost: %d\n", pronadeniPredmet->vrijednost);
    }
    else {

        printf("Predmet nije pronaden.\n");
    }
}

void obrisiPredmet(Igrac* igrac) {

    int odabir;

    if (igrac->brojPredmeta == 0) {

        printf("Inventory je prazan.\n");
        return;
    }

    prikaziInventory(igrac);

    printf("Odaberi redni broj predmeta za brisanje: ");
    scanf("%d", &odabir);

    ocistiUlaz();

    if (odabir < 1 || odabir > igrac->brojPredmeta) {

        printf("Neispravan odabir.\n");
        return;
    }

    for (int i = odabir - 1; i < igrac->brojPredmeta - 1; i++) {

        igrac->inventory[i] = igrac->inventory[i + 1];
    }

    igrac->brojPredmeta--;

    printf("Predmet je obrisan iz inventoryja.\n");
}