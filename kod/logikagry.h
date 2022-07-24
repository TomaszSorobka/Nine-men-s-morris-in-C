#ifndef LOGIKAGRY_H_INCLUDED
#define LOGIKAGRY_H_INCLUDED

/*
	Tomasz Sorobka
	Projekt na przedmiot Wstep do Programowania w jezyku C
	Gra Mlynek
	332034
*/

int czyPoprawnyRuch(int a, int b, int znaczniki);
int czyMlynek(int tens, char litera);
void planszaInit();
void zmienneInit();
int czyMlynekWyj(int tens, char litera);
void zmienKolor(GtkWidget *widget, char *id);
#endif // LOGIKAGRY_H_INCLUDED
