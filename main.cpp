#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXIM_SALI 10
#define MAXIM_REZERVARI 50

typedef struct {
    char nume[50];
    int capacitate;
    int disponibila;
} Sala;

typedef struct {
    char nume_sala[50];
    char data[15];
    char ora[10];
} Rezervare;

Sala sali[MAXIM_SALI];
int numar_sali = 0;

Rezervare rezervari[MAXIM_REZERVARI];
int numar_rezervari = 0;

void clear_screen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void afisare_header(const char *titlu) {
    clear_screen();
    printf("\033[1;36m==================================\033[0m\n");
    printf("\033[1;32m         %s\n\033[0m", titlu);
    printf("\033[1;36m==================================\033[0m\n\n");
}

void salveaza_sali_in_fisier() {
    FILE *fisier = fopen("sali.txt", "w");
    if (!fisier) return;

    for (int i = 0; i < numar_sali; i++) {
        fprintf(fisier, "%s;%d;%d\n", sali[i].nume, sali[i].capacitate, sali[i].disponibila);
    }

    fclose(fisier);
}

void incarca_sali_din_fisier() {
    FILE *fisier = fopen("sali.txt", "r");
    if (!fisier) return;

    Sala temp;
    while (fscanf(fisier, "%49[^;];%d;%d\n", temp.nume, &temp.capacitate, &temp.disponibila) == 3) {
        if (numar_sali < MAXIM_SALI) {
            sali[numar_sali++] = temp;
        }
    }

    fclose(fisier);
}

void salveaza_rezervari_in_fisier() {
    FILE *fisier = fopen("rezervari.txt", "w");
    if (!fisier) return;

    for (int i = 0; i < numar_rezervari; i++) {
        fprintf(fisier, "%s;%s;%s\n", rezervari[i].nume_sala, rezervari[i].data, rezervari[i].ora);
    }

    fclose(fisier);
}

void incarca_rezervari_din_fisier() {
    FILE *fisier = fopen("rezervari.txt", "r");
    if (!fisier) return;

    Rezervare temp;
    while (fscanf(fisier, "%49[^;];%14[^;];%9[^\n]\n", temp.nume_sala, temp.data, temp.ora) == 3) {
        int sala_valida = 0;
        for (int i = 0; i < numar_sali; i++) {
            if (strcmp(sali[i].nume, temp.nume_sala) == 0) {
                sala_valida = 1;
                break;
            }
        }
        if (sala_valida && numar_rezervari < MAXIM_REZERVARI) {
            rezervari[numar_rezervari++] = temp;
        }
    }

    fclose(fisier);
}

void afisare_sali_disponibile() {
    int inapoi;
    do {
        afisare_header("SALILE DISPONIBILE");

        int gasit = 0;
        for (int i = 0; i < numar_sali; i++) {
            if (sali[i].disponibila == 1) {
                gasit = 1;
                printf("\033[1;34m%-25s\033[0m - Capacitate: \033[1;33m%-5d\033[0m locuri\n", sali[i].nume, sali[i].capacitate);
            }
        }

        if (!gasit) {
            printf("Nu exista sali disponibile momentan.\n");
        }

        printf("\n0 - Inapoi\n");
        printf("Alege optiunea: ");
        scanf("%d", &inapoi);
    } while (inapoi != 0);
}

void cautare_sala() {
    char nume_cautat[50];
    int capacitate_cautata, optiune_cautare, inapoi;

    do {
        afisare_header("CAUTARE SALA");

        // Meniu pentru alegerea criteriului de căutare
        printf("Cum doriti sa cautati?\n");
        printf("\033[1;32m1.\033[0m Dupa nume\n");
        printf("\033[1;32m2.\033[0m Dupa capacitate\n");
        printf("\033[1;32m3.\033[0m Dupa nume si capacitate\n");
        printf("\033[1;31m0.\033[0m Inapoi\n");
        printf("Alegeti optiunea: ");
        scanf("%d", &optiune_cautare);

        if (optiune_cautare == 0) {
            break; // Ieșim din funcție dacă utilizatorul a ales "Inapoi"
        }

        int gasit = 0;
        switch (optiune_cautare) {
            case 1: // Căutare după nume
                printf("Introdu numele complet sau partial al salii: ");
                scanf(" %[^\n]", nume_cautat);

                for (int i = 0; i < numar_sali; i++) {
                    if (strstr(sali[i].nume, nume_cautat)) {
                        gasit = 1;
                        printf("\033[1;34m%-25s\033[0m - Capacitate: \033[1;33m%-5d\033[0m - Disponibila: %s\n",
                               sali[i].nume, sali[i].capacitate, sali[i].disponibila ? "Da" : "Nu");
                    }
                }
                break;

            case 2: // Căutare după capacitate
                printf("Introdu capacitatea salii: ");
                scanf("%d", &capacitate_cautata);

                for (int i = 0; i < numar_sali; i++) {
                    if (sali[i].capacitate == capacitate_cautata) {
                        gasit = 1;
                        printf("\033[1;34m%-25s\033[0m - Capacitate: \033[1;33m%-5d\033[0m - Disponibila: %s\n",
                               sali[i].nume, sali[i].capacitate, sali[i].disponibila ? "Da" : "Nu");
                    }
                }
                break;

            case 3: // Căutare după nume și capacitate
                printf("Introdu numele complet sau partial al salii: ");
                scanf(" %[^\n]", nume_cautat);
                printf("Introdu capacitatea salii: ");
                scanf("%d", &capacitate_cautata);

                for (int i = 0; i < numar_sali; i++) {
                    if (strstr(sali[i].nume, nume_cautat) && sali[i].capacitate == capacitate_cautata) {
                        gasit = 1;
                        printf("\033[1;34m%-25s\033[0m - Capacitate: \033[1;33m%-5d\033[0m - Disponibila: %s\n",
                               sali[i].nume, sali[i].capacitate, sali[i].disponibila ? "Da" : "Nu");
                    }
                }
                break;

            default:
                printf("\nOptiune invalida!\n");
                getchar(); getchar();
                continue;
        }

        if (!gasit) {
            printf("Nu exista sali care sa corespunda cautarii.\n");
        }

        printf("\n0 - Inapoi\n");
        printf("Alege optiunea: ");
        scanf("%d", &inapoi);
    } while (inapoi != 0);
}

void rezervare_sala() {
    char nume_sala[50], data[15], ora[10];
    int inapoi, deja_rezervata = 0, sala_gasita = 0;

    do {
        afisare_header("REZERVARE SALA");

        printf("Numele salii pentru rezervare: ");
        scanf(" %[^\n]", nume_sala);

        sala_gasita = 0;
        for (int i = 0; i < numar_sali; i++) {
            if (strcmp(sali[i].nume, nume_sala) == 0) {
                sala_gasita = 1;
                break;
            }
        }

        if (!sala_gasita) {
            printf("\n\033[1;31mEroare: Sala introdusa nu exista!\033[0m\n");
        } else {
            printf("Data rezervarii (dd/mm/yyyy): ");
            scanf("%s", data);
            printf("Ora rezervarii (hh:mm): ");
            scanf("%s", ora);

            deja_rezervata = 0;
            for (int i = 0; i < numar_rezervari; i++) {
                if (strcmp(rezervari[i].nume_sala, nume_sala) == 0 &&
                    strcmp(rezervari[i].data, data) == 0 &&
                    strcmp(rezervari[i].ora, ora) == 0) {
                    deja_rezervata = 1;
                    break;
                }
            }

            if (deja_rezervata) {
                printf("\n\033[1;31mEroare: Sala este deja rezervata pentru acea data si ora!\033[0m\n");
            } else {
                strcpy(rezervari[numar_rezervari].nume_sala, nume_sala);
                strcpy(rezervari[numar_rezervari].data, data);
                strcpy(rezervari[numar_rezervari].ora, ora);
                numar_rezervari++;
                salveaza_rezervari_in_fisier();

                printf("\n\033[1;32mRezervare efectuata cu succes!\033[0m\n");
            }
        }

        printf("\n0 - Inapoi\n");
        printf("Alege optiunea: ");
        scanf("%d", &inapoi);
    } while (inapoi != 0);
}

void anulare_rezervare() {
    char nume_sala[50], data[15], ora[10];
    int gasit = 0, inapoi;

    do {
        afisare_header("ANULARE REZERVARE");

        printf("Numele salii pentru anulare: ");
        scanf(" %[^\n]", nume_sala);

        printf("Data rezervarii (dd/mm/yyyy): ");
        scanf("%s", data);

        printf("Ora rezervarii (hh:mm): ");
        scanf("%s", ora);

        for (int i = 0; i < numar_rezervari; i++) {
            if (strcmp(rezervari[i].nume_sala, nume_sala) == 0 &&
                strcmp(rezervari[i].data, data) == 0 &&
                strcmp(rezervari[i].ora, ora) == 0) {
                // Ștergerea rezervării
                for (int j = i; j < numar_rezervari - 1; j++) {
                    rezervari[j] = rezervari[j + 1];
                }
                numar_rezervari--;
                salveaza_rezervari_in_fisier();

                printf("\n\033[1;32mRezervarea a fost anulata cu succes!\033[0m\n");
                gasit = 1;
                break;
            }
        }

        if (!gasit) {
            printf("\n\033[1;31mEroare: Nu s-a gasit rezervarea!\033[0m\n");
        }

        printf("\n0 - Inapoi\n");
        printf("Alege optiunea: ");
        scanf("%d", &inapoi);
    } while (inapoi != 0);
}

// Funcția de vizualizare a tuturor rezervărilor
void afisare_rezervari() {
    int inapoi;
    do {
        afisare_header("REZERVARI");

        if (numar_rezervari == 0) {
            printf("Nu exista rezervari in sistem.\n");
        } else {
            for (int i = 0; i < numar_rezervari; i++) {
                printf("\033[1;34m%-25s\033[0m - Data: \033[1;33m%-10s\033[0m - Ora: \033[1;33m%-5s\033[0m\n",
                       rezervari[i].nume_sala, rezervari[i].data, rezervari[i].ora);
            }
        }

        printf("\n0 - Inapoi\n");
        printf("Alege optiunea: ");
        scanf("%d", &inapoi);
    } while (inapoi != 0);
}

int main() {
    incarca_sali_din_fisier();
    incarca_rezervari_din_fisier();

    int alegere;

    do {
        afisare_header("Meniu Principal");

        printf("\033[1;32m1.\033[0m Adauga sala\n");
        printf("\033[1;32m2.\033[0m Afiseaza sali disponibile\n");
        printf("\033[1;32m3.\033[0m Cauta sala\n");
        printf("\033[1;32m4.\033[0m Rezerva sala\n");
        printf("\033[1;32m5.\033[0m Anuleaza rezervare\n");
        printf("\033[1;32m6.\033[0m Vizualizeaza rezervari\n");
        printf("\033[1;31m0.\033[0m Iesire\n");
        printf("Alege optiunea: ");
        scanf("%d", &alegere);

        switch (alegere) {
            case 1:
                // Adăugarea unei săli
                break;
            case 2:
                afisare_sali_disponibile();
                break;
            case 3:
                cautare_sala();
                break;
            case 4:
                rezervare_sala();
                break;
            case 5:
                anulare_rezervare();
                break;
            case 6:
                afisare_rezervari();
                break;
            case 0:
                printf("Iesire din program...\n");
                break;
            default:
                printf("\nOptiune invalida!\n");
        }
    } while (alegere != 0);

    return 0;
}
