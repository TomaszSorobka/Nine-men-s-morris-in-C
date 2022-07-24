#include <string.h>
#include <math.h>
#include <gtk/gtk.h>
#include <stdlib.h>
#include "logikagry.h"
#include "gtkdesign.h"

/*
	Tomasz Sorobka
	Projekt na przedmiot Wstep do Programowania w jezyku C
	Gra Mlynek
	332034
*/

char plansza[7][7] = {
{'P', '+', '+', 'P', '+','+','P'},
{'+', 'P', '+', 'P', '+','P','+'},
{'+', '+', 'P', 'P', 'P','+','+'},
{'P', 'P', 'P', 'X', 'P','P','P'},
{'+', '+', 'P', 'P', 'P','+','+'},
{'+', 'P', '+', 'P', '+','P','+'},
{'P', '+', '+', 'P', '+','+','P'},
};
char reset[7][7];

void planszaInit() {
    memcpy(plansza, reset, sizeof(reset));
}

int czyMlynek(int tens, char litera) {
    int seen = 0, licznik = 0, rzad = tens/7;
        for (int i = tens%7; i<49; i+=7) {
            if (i == tens) seen = 1;
            if (plansza[i/7][i%7] == litera) {
                licznik ++;
            }
            if (plansza[i/7][i%7] == 'X') {
                licznik = 0;
                seen = 0;
            }
            if (licznik == 3 && seen == 1) return 1;
        }
        seen = 0;
        licznik = 0;
        for (int i = 0; i<7; i++) {
            if (i+rzad*7 == tens) seen = 1;
            if (plansza[rzad][i] == litera) {
                licznik ++;
            }
            if (plansza[rzad][i] == 'X') {
                licznik = 0;
                seen = 0;
            }
            if (licznik == 3 && seen == 1) return 1;
        }
    return 0;
}

int czyMlynekWyj(int tens, char litera) {
    for (int i = 0; i<7; i++) {
        for (int j = 0; j<7; j++) {
            if (plansza[i][j] == litera) {
                if (!czyMlynek(i*7+j, litera)) return czyMlynek(tens, litera);
            }
        }
    }
    return 0;
}

int czyPoprawnyRuch(int a, int b, int znaczniki) {

    if (znaczniki < 4) return 1;
    if (a/7 != b/7 && a%7 != b%7) return 0;
    if (a/7 == b/7) {
        if (a/7 == 0 || a/7 == 6) {
            if(abs(a-b)>3) return 0;
        } else if (a/7 == 1 || a/7 == 5) {
            if(abs(a-b)>2) return 0;
        } else {
            if(abs(a-b)>1) return 0;
        }
    } else {
        if (a%7 == 0 || a%7 == 6) {
            if(abs(a/7-b/7)>3) return 0;
        } else if (a%7 == 1 || a%7 == 5) {
            if(abs(a/7-b/7)>2) return 0;
        } else {
            if(abs(a/7-b/7)>1) return 0;
        }
    }
    return 1;
}



