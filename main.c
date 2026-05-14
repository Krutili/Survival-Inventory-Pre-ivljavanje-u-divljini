#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAKS_NAZIV 50
#define DATOTEKA_SPREMANJA "savegame.bin"

typedef enum {
    HRANA = 1,
    VODA,
    MATERIJAL,
    LIJEK,
    ALAT
} VrstaPredmeta;

typedef struct {
    int id;
    char naziv[MAKS_NAZIV];
    VrstaPredmeta vrsta;
    int kolicina;
    int vrijednost;
} Predmet;

typedef struct {
    char ime[MAKS_NAZIV];
    int zdravlje;
    int glad;
    int zed;
    int energija;
    int dan;
    Predmet* inventory;
    int brojPredmeta;
    int kapacitet;
} Igrac;

void ocistiUlaz();
void prikaziGlavniIzbornik();
void novaIgra(Igrac* igrac);
void prikaziIgraca(Igrac* igrac);
void prikaziInventory(Igrac* igrac);
void dodajPredmet(
    Igrac* igrac,
    const char* naziv,
    VrstaPredmeta vrsta,
    int kolicina,
    int vrijednost
);

void istraziPodrucje(Igrac* igrac);
void koristiPredmet(Igrac* igrac);
void spremiIgru(Igrac* igrac);
int ucitajIgru(Igrac* igrac);
void oslobodiMemoriju(Igrac* igrac);

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

        case 1:
            novaIgra(&igrac);
            break;

        case 2:

            if (ucitajIgru(&igrac)) {
                printf("Igra je uspjesno ucitana.\n");
            }
            else {
                printf("Spremljena igra ne postoji.\n");
            }

            break;

        case 3:
            prikaziIgraca(&igrac);
            break;

        case 4:
            prikaziInventory(&igrac);
            break;

        case 5:
            istraziPodrucje(&igrac);
            break;

        case 6:
            koristiPredmet(&igrac);
            break;

        case 7:
            spremiIgru(&igrac);
            break;

        case 0:
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
        printf("%d. %s | kolicina: %d | vrijednost: %d\n",
            i + 1,
            igrac->inventory[i].naziv,
            igrac->inventory[i].kolicina,
            igrac->inventory[i].vrijednost);
    }
}

void dodajPredmet(Igrac* igrac, const char* naziv, VrstaPredmeta vrsta, int kolicina, int vrijednost) {
    if (igrac->brojPredmeta >= igrac->kapacitet) {
        igrac->kapacitet *= 2;
        igrac->inventory = realloc(igrac->inventory, igrac->kapacitet * sizeof(Predmet));
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

    if (slucajno == 0) dodajPredmet(igrac, "Hrana", HRANA, 1, 20);
    else if (slucajno == 1) dodajPredmet(igrac, "Voda", VODA, 1, 20);
    else if (slucajno == 2) dodajPredmet(igrac, "Drvo", MATERIJAL, 2, 5);
    else dodajPredmet(igrac, "Lijek", LIJEK, 1, 30);

    igrac->glad += 10;
    igrac->zed += 10;
    igrac->energija -= 10;

    printf("Istrazio si podrucje i pronasao predmet.\n");
}

void koristiPredmet(Igrac* igrac) {
    int odabir;

    prikaziInventory(igrac);

    if (igrac->brojPredmeta == 0) return;

    printf("Odaberi predmet za koristenje: ");
    scanf("%d", &odabir);
    ocistiUlaz();

    if (odabir < 1 || odabir > igrac->brojPredmeta) {
        printf("Neispravan odabir.\n");
        return;
    }

    Predmet* p = &igrac->inventory[odabir - 1];

    if (p->vrsta == HRANA) igrac->glad -= 20;
    else if (p->vrsta == VODA) igrac->zed -= 20;
    else if (p->vrsta == LIJEK) igrac->zdravlje += 20;

    for (int i = odabir - 1; i < igrac->brojPredmeta - 1; i++) {
        igrac->inventory[i] = igrac->inventory[i + 1];
    }

    igrac->brojPredmeta--;

    printf("Predmet je iskoristen.\n");
}

void spremiIgru(Igrac* igrac) {
    FILE* datoteka = fopen(DATOTEKA_SPREMANJA, "wb");

    if (datoteka == NULL) {
        printf("Greska kod spremanja.\n");
        return;
    }

    fwrite(igrac, sizeof(Igrac), 1, datoteka);
    fwrite(igrac->inventory, sizeof(Predmet), igrac->brojPredmeta, datoteka);

    fclose(datoteka);

    printf("Igra je spremljena.\n");
}

int ucitajIgru(Igrac* igrac) {
    FILE* datoteka = fopen(DATOTEKA_SPREMANJA, "rb");

    if (datoteka == NULL) return 0;

    fread(igrac, sizeof(Igrac), 1, datoteka);

    igrac->inventory = malloc(igrac->kapacitet * sizeof(Predmet));
    fread(igrac->inventory, sizeof(Predmet), igrac->brojPredmeta, datoteka);

    fclose(datoteka);

    return 1;
}

void oslobodiMemoriju(Igrac* igrac) {
    free(igrac->inventory);
    igrac->inventory = NULL;
}