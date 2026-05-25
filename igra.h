#pragma once

#define MAKS_NAZIV 50

typedef enum {
    HRANA = 1,
    VODA,
    MATERIJAL,
    LIJEK,
    ALAT
} VrstaPredmeta;

typedef enum {
    IZLAZ = 0,
    NOVA_IGRA = 1,
    UCITAJ_IGRU,
    PRIKAZ_IGRACA,
    PRIKAZ_INVENTORYJA,
    ISTRAZI,
    KORISTI_PREDMET,
    SPREMI_IGRU,
    NOVI_DAN,
} OpcijaIzbornika;

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

void dodajPredmet(Igrac* igrac, const char* naziv, VrstaPredmeta vrsta, int kolicina, int vrijednost);
void istraziPodrucje(Igrac* igrac);
void koristiPredmet(Igrac* igrac);
void oslobodiMemoriju(Igrac* igrac);

void noviDan(Igrac* igrac);

int igracJeMrtav(const Igrac* igrac);