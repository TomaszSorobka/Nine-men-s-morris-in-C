#include <string.h>
#include <math.h>
#include <gtk/gtk.h>
#include <stdlib.h>
#include "fifo.h"
#include "logikagry.h"
#include "gtkdesign.h"
// windows: gcc -std=c99 -Wall -o mlynek main.c win-fifo.c logikagry.c gtkdesign.c `pkg-config gtk+-3.0 --cflags --libs`
/*
	Tomasz Sorobka
	Projekt na przedmiot Wstep do Programowania w jezyku C
	Gra Mlynek
	332034
*/
extern char plansza[7][7];
extern char reset[7][7];
extern GtkWidget* tabButton[49];
GtkWidget *window, *box1, *box0, *boxInstrukcja, *boxGra, *button, *label, *boxInfo;
int czyMojaTura, faza1 = 1,faza2 = 0, znaczniki = 0, selectedtens = -1, dwieWiad = 0, faza1odb = 1,faza2odb = 0, odebrane = 0, wyslane = 0, mlynek = 0, przeciwnikmlynek = 0, znaczprzeciw = 9;

char *selectedid;
static PipesPtr potoki;
static char *moj_id, *twoj_id;

static void przekaz_tekst( GtkWidget *widget, char *id);
static gboolean pobierz_tekst(gpointer data);
static void zakoncz(GtkWidget *widget, gpointer data);
void instrukcja(GtkWidget *widget, gpointer data);
void powrot(GtkWidget *widget, gpointer data);
void graj(GtkWidget *widget, gpointer data);
void zmienKolor(GtkWidget *widget, char *id);
void inicjalizacjaMenu();
void koniecGry();

void zmienneInit() {
    faza1 = 1; faza2 = 0; znaczniki = 0; selectedtens = -1; dwieWiad = 0; faza1odb = 1; faza2odb = 0; odebrane = 0; wyslane = 0; mlynek = 0; przeciwnikmlynek = 0; znaczprzeciw = 9;
}
void pokazBlad(char *komunikat)
{
    GtkWidget *dialog;
    dialog=gtk_message_dialog_new (GTK_WINDOW(window),GTK_DIALOG_DESTROY_WITH_PARENT,
    GTK_MESSAGE_ERROR,GTK_BUTTONS_CLOSE,"%s",komunikat);
    gtk_dialog_run (GTK_DIALOG (dialog));
    gtk_widget_destroy (dialog);
}

int main(int argc,char *argv[])
{
    zmienneInit();
    memcpy(reset, plansza, sizeof(plansza));
    // Inicjalizacja Pipe
    if ((potoki=initPipes(argc,argv)) == NULL)
        return 1;
    if (argc == 2 && strcmp(argv[1],"A") == 0) { twoj_id="B"; moj_id="A"; czyMojaTura = 1; }
    else { moj_id="B"; twoj_id="A";
           czyMojaTura = 0; }
    // Ladowanie gtk i css
    gtk_init(&argc, &argv);
    load_css();

    gchar naglowek[31];
    sprintf(naglowek,"Zagrajmy w Mlynek, %c!", moj_id[0]);
    //Tworzenie okna
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window),naglowek);
    g_signal_connect(G_OBJECT(window), "destroy",G_CALLBACK(zakoncz), NULL);
    g_signal_connect(G_OBJECT(window), "destroy",G_CALLBACK(zakoncz), NULL);
    gtk_window_set_default_size (GTK_WINDOW (window), 800, 800);

    // Zainicjalizowanie menu
    inicjalizacjaMenu();
    g_timeout_add(100,pobierz_tekst,NULL);
    gtk_main();
    return 0;
}

static void zakoncz(GtkWidget *widget, gpointer data)
{
  closePipes(potoki);
  gtk_main_quit();
}

void zmienKolor(GtkWidget *widget, char *id)
{
    if (id[0] == '2') {
        pokazBlad("Poddales sie");
        przekaz_tekst(widget, "2");
        koniecGry();
        return;
    }
    if (!czyMojaTura) {
            pokazBlad("Nie twoja tura!");
    } else if(faza1 && !mlynek) {
        int tens = (int)id[0] - 64;
        if (plansza[tens/7][tens%7] == 'A' || plansza[tens/7][tens%7] == 'B'){
                pokazBlad("Pole zajete, wybierz inne!");
                return;
        } else {
            plansza[tens/7][tens%7] = moj_id[0];
        }
        removeStyle(tens);
        gtk_style_context_add_class(gtk_widget_get_style_context(widget), moj_id);

        if (czyMlynek(tens, moj_id[0])) {
            mlynek = 1;
            pokazBlad("Brawo, Mlynek!");
        } else {
            czyMojaTura = 0;
            znaczniki++;
            wyslane++;
            if (wyslane==9) {
                faza2 = 1;
                faza1 = 0;
                pokazBlad("Faza 2!");
                gtk_label_set_label(GTK_LABEL(label), "Faza 2");
            }
        }
        przekaz_tekst(widget, id);
    } else if (faza1 && mlynek) {
        int tens = (int)id[0] - 64;
         if (plansza[tens/7][tens%7] != moj_id[0] && plansza[tens/7][tens%7] != 'P' && !czyMlynekWyj(tens, twoj_id[0])) {
                removeStyle(tens);
                plansza[tens/7][tens%7] = 'P';
                czyMojaTura = 0;
                mlynek = 0;
                przekaz_tekst(widget, id);
                znaczprzeciw--;
                znaczniki++;
                wyslane++;
                if (wyslane==9) {
                    faza2 = 1;
                    faza1 = 0;
                    pokazBlad("Faza 2!");
                }
            } else {
                pokazBlad("Wybierz znacznik przeciwnika nie bedacy w mlynku");
                return;
            }
    } else if (faza2) {
        if (selectedtens == -1) {
            int tens = (int)id[0] - 64;

            if (plansza[tens/7][tens%7] != moj_id[0]) {
                pokazBlad("Wybierz swoj znacznik!");
                return;
            }
            selectedtens = tens;
            selectedid = id;
            removeStyle(tens);
            gtk_style_context_add_class(gtk_widget_get_style_context(tabButton[selectedtens]), "yellow");

        } else if (selectedtens != -1 && !mlynek) {
            int tens = (int)id[0] - 64;
            if (plansza[tens/7][tens%7] != 'P' || !czyPoprawnyRuch(selectedtens, tens, znaczniki)) {
                removeStyle(selectedtens);
                gtk_style_context_add_class(gtk_widget_get_style_context(tabButton[selectedtens]), moj_id);
                selectedtens = -1;
                pokazBlad("Wybierz puste pole polaczone sciezka!");
                return;
            }
            removeStyle(selectedtens);
            removeStyle(tens);
            gtk_style_context_add_class(gtk_widget_get_style_context(tabButton[tens]), moj_id);
            plansza[selectedtens/7][selectedtens%7] = 'P';
            plansza[tens/7][tens%7] = moj_id[0];

            przekaz_tekst(widget, selectedid);
            przekaz_tekst(widget, id);

            if (czyMlynek(tens, moj_id[0])) {
                    mlynek = 1;
                    pokazBlad("Brawo, Mlynek!");
            }
            else {
                czyMojaTura = 0;
                selectedtens = -1;
            }
        } else if (mlynek) {
            int tens = (int)id[0] - 64;
            if (plansza[tens/7][tens%7] != moj_id[0] && plansza[tens/7][tens%7] != 'P' && !czyMlynek(tens, twoj_id[0])) {
                removeStyle(tens);
                plansza[tens/7][tens%7] = 'P';
                selectedtens = -1;
                czyMojaTura = 0;
                mlynek = 0;
                przekaz_tekst(widget, id);
                znaczprzeciw--;
                if (znaczprzeciw < 3) {
                    pokazBlad("Wygrales");
                    koniecGry();
                    return;
                }
            } else {
                pokazBlad("Wybierz znacznik przeciwnika nie bedacy w mlynku");
                return;
            }
        }
    }
}

static void przekaz_tekst(GtkWidget *widget, char *id)
{
    sendStringToPipe(potoki, id);
}

static gboolean pobierz_tekst(gpointer data)
{
  char wejscie[3];
  int tens;
  if (getStringFromPipe(potoki,wejscie,2)) {
        if (wejscie[0] == '2') {
            pokazBlad("Przeciwnik sie poddal");
            koniecGry();
            return TRUE;
        }

        if (faza1odb && !przeciwnikmlynek) {
            tens = (int)wejscie[0] -64;
            removeStyle(tens);
            gtk_style_context_add_class(gtk_widget_get_style_context(tabButton[tens]), twoj_id);
            plansza[tens/7][tens%7] = twoj_id[0];

            if (czyMlynek(tens, twoj_id[0])) przeciwnikmlynek = 1;
            else {
                odebrane++;
                if (odebrane==9) {
                    faza2odb = 1;
                    faza1odb = 0;
                }
                czyMojaTura = 1;
        }
    } else if (faza1odb && przeciwnikmlynek) {
        tens = (int)wejscie[0] -64;
            przeciwnikmlynek = 0;
            znaczniki--;
            removeStyle(tens);
            plansza[tens/7][tens%7] = 'P';
            odebrane++;
            if (odebrane==9) {
                faza2odb = 1;
                faza1odb = 0;
            }
            czyMojaTura = 1;

    } else if (faza2odb) {
        tens = (int)wejscie[0] -64;
        if (!dwieWiad) {
            removeStyle(tens);
            plansza[tens/7][tens%7] = 'P';
            if (przeciwnikmlynek) {
                przeciwnikmlynek = 0;
                znaczniki--;
                czyMojaTura = 1;
                if (znaczniki < 3) {
                    //PRZEGRANA
                    pokazBlad("Przegrales");
                    koniecGry();
                    return TRUE;
                }
            } else dwieWiad = 1;
        } else {
            dwieWiad = 0;
            removeStyle(tens);
            gtk_style_context_add_class(gtk_widget_get_style_context(tabButton[tens]), twoj_id);
            plansza[tens/7][tens%7] = twoj_id[0];

            if (czyMlynek(tens, twoj_id[0])) przeciwnikmlynek = 1;
            else czyMojaTura = 1;

            }
        }
  }
  return TRUE;
}
