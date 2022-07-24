#include <string.h>
#include <math.h>
#include <gtk/gtk.h>
#include <stdlib.h>
#include "gtkdesign.h"
#include "logikagry.h"

/*
	Tomasz Sorobka
	Projekt na przedmiot Wstep do Programowania w jezyku C
	Gra Mlynek
	332034
*/
extern GtkWidget *window, *box1, *box0, *boxInstrukcja, *boxGra, *button, *label, *boxInfo;
GtkWidget* tabButton[49];

struct przycisk {
    char *id;
    gint posX,lenX,posY,lenY;
} tab[] = {
    {"@",0,1,0,1}, {"C",3,1,0,1}, {"F",6,1,0,1},
    {"H",1,1,1,1}, {"J",3,1,1,1}, {"L",5,1,1,1},
    {"P",2,1,2,1}, {"Q",3,1,2,1}, {"R",4,1,2,1},
    {"U",0,1,3,1}, {"V",1,1,3,1}, {"W",2,1,3,1}, {"Y",4,1,3,1}, {"Z",5,1,3,1}, {"[",6,1,3,1},
    {"^",2,1,4,1}, {"_",3,1,4,1}, {"`",4,1,4,1},
    {"d",1,1,5,1}, {"f",3,1,5,1}, {"h",5,1,5,1},
    {"j",0,1,6,1}, {"m",3,1,6,1}, {"p",6,1,6,1},
};

struct liniaH {
    int posX,posY;
} tabH[] = {
    {1,0},{2,0},{4,0},{5,0},
    {2,1},{4,1},
    {2,5},{4,5},
    {1,6},{2,6},{4,6},{5,6}
};

struct liniaV {
    int posX,posY;
} tabV[] = {
    {0,1},{6,1},
    {0,2},{1,2},{5,2},{6,2},
    {0,4},{1,4},{5,4},{6,4},
    {0,5},{6,5}
};

void inicjalizacjaMenu()
{
    box0 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(window), box0);
    label = gtk_label_new("Czesc, witaj w mojej grze! Gdy zapoznasz sie z zasadami gry, kliknij przycisk Graj :)");
    gtk_style_context_add_class(gtk_widget_get_style_context(label), "label");
    gtk_label_set_width_chars (GTK_LABEL(label), 50);
    gtk_box_pack_start(GTK_BOX(box0), label, TRUE, TRUE, 0);
    //FIRST BOX
    box1 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_pack_start(GTK_BOX(box0), box1, TRUE, TRUE, 0);

    //IMAGE IN THE FIRST BOX
    GtkWidget *image = gtk_image_new_from_file("abc.png");
    gtk_image_set_pixel_size(GTK_IMAGE(image), 100);
    gtk_box_pack_start(GTK_BOX(box1), image, TRUE, TRUE, 0);
    // BUTTONS IN THE SECOND BOX
    GtkWidget *box2 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    GtkWidget *button = gtk_button_new_with_label("Instrukcja");
    gtk_style_context_add_class(gtk_widget_get_style_context(button), "menuBtn");
    g_signal_connect(G_OBJECT(button), "clicked",G_CALLBACK(instrukcja), NULL);
    gtk_box_pack_start(GTK_BOX(box2), button, TRUE, TRUE, 50);

    GtkWidget *button2 = gtk_button_new_with_label("Graj");
    gtk_style_context_add_class(gtk_widget_get_style_context(button2), "menuBtn");
    g_signal_connect(G_OBJECT(button2), "clicked",G_CALLBACK(graj), NULL);
    gtk_box_pack_start(GTK_BOX(box2), button2, TRUE, TRUE, 50);

    gtk_box_pack_end(GTK_BOX(box1), box2, TRUE, TRUE, 0);
    gtk_widget_show_all(window);
}

void graj(GtkWidget *widget, gpointer data)
{
    gtk_container_remove(GTK_CONTAINER(window), GTK_WIDGET(box0));
    boxGra = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    boxInfo = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_container_add(GTK_CONTAINER(window), boxGra);
    label = gtk_label_new("Faza 1");
    gtk_style_context_add_class(gtk_widget_get_style_context(label), "label");
    button = gtk_button_new_with_label("Poddaj sie");
    gtk_style_context_add_class(gtk_widget_get_style_context(button), "concede");
    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(zmienKolor), "2");
    gtk_box_pack_start(GTK_BOX(boxInfo), label, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(boxInfo), button, TRUE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(boxGra), boxInfo, TRUE, TRUE, 0);
    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 5);
    gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 5);
    gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
    gtk_box_pack_start(GTK_BOX(boxGra), grid, TRUE, TRUE, 0);

    for(int i = 0; i < 12; i++) {
        GtkWidget *image = gtk_image_new_from_file("horizontal.png");
        GdkPixbuf *pb;
        pb = gdk_pixbuf_new_from_file("horizontal.png", NULL);
        pb = gdk_pixbuf_scale_simple(pb,120,120,GDK_INTERP_BILINEAR);
        gtk_image_set_from_pixbuf(GTK_IMAGE(image), pb);
        gtk_grid_attach(GTK_GRID(grid), image, tabH[i].posX, tabH[i].posY, 1, 1);
    }
    for(int i = 0; i < 12; i++) {
        GtkWidget *image = gtk_image_new_from_file("horizontal.png");
        GdkPixbuf *pb;
        pb = gdk_pixbuf_new_from_file("vertical.png", NULL);
        pb = gdk_pixbuf_scale_simple(pb,120,120,GDK_INTERP_BILINEAR);
        gtk_image_set_from_pixbuf(GTK_IMAGE(image), pb);
        gtk_grid_attach(GTK_GRID(grid), image, tabV[i].posX, tabV[i].posY, 1, 1);
    }
    for(int i = 0; i < 24; i++) {
        button = gtk_button_new();
        tabButton[zwrocNumer(i)] = button;
        g_signal_connect(G_OBJECT(button), "clicked",G_CALLBACK(zmienKolor), tab[i].id);
        gtk_style_context_add_class(gtk_widget_get_style_context(button), "circular");
        gtk_grid_attach(GTK_GRID(grid), button, tab[i].posX, tab[i].posY, tab[i].lenX, tab[i].lenY);
    }
  gtk_widget_show_all(boxGra);

}

void load_css ( void ) {
    GtkCssProvider *provider;
    GdkDisplay *display;
    GdkScreen *screen;
    // ***
    const gchar *css_style_file = "style.css";
    GFile *css_fp = g_file_new_for_path(css_style_file);
    GError *error = 0;

    provider = gtk_css_provider_new();
    display = gdk_display_get_default();
    screen = gdk_display_get_default_screen(display);

    gtk_style_context_add_provider_for_screen(screen, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    gtk_css_provider_load_from_file(provider, css_fp, &error);

    g_object_unref(provider);
}

void koniecGry() {
    gtk_container_remove(GTK_CONTAINER(window), GTK_WIDGET(boxGra));
    inicjalizacjaMenu();
    zmienneInit();
    planszaInit();
}

void removeStyle(int tens) {
    gtk_style_context_remove_class(gtk_widget_get_style_context(tabButton[tens]), "B");
    gtk_style_context_remove_class(gtk_widget_get_style_context(tabButton[tens]), "A");
    gtk_style_context_remove_class(gtk_widget_get_style_context(tabButton[tens]), "yellow");
}

int zwrocNumer(int i) {
    if (i == 0) return 0; else if (i == 1) return 3; else if (i == 2) return 6;
    if (i == 3) return 8; else if (i == 4) return 10; else if (i == 5) return 12;
    if (i == 6) return 16; else if (i == 7) return 17; else if (i == 8) return 18;
    if (i == 9) return 21; else if (i == 10) return 22; else if (i == 11) return 23; else if (i == 12) return 25; else if (i == 13) return 26; else if (i == 14) return 27;
    if (i == 15) return 30; else if (i == 16) return 31; else if (i == 17) return 32;
    if (i == 18) return 36; else if (i == 19) return 38; else if (i == 20) return 40;
    if (i == 21) return 42; else if (i == 22) return 45; else return 48;
}

void instrukcja(GtkWidget *widget, gpointer data)
{
  gtk_container_remove(GTK_CONTAINER(window), GTK_WIDGET(box0));
  boxInstrukcja = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  gtk_container_add(GTK_CONTAINER(window), boxInstrukcja);
  GtkWidget *label = gtk_label_new("\nZasady gry: \n"
                                   "Gra podzielona jest na 2 fazy \n\n"
                                   "Faza 1: \n"
                                   "Gra rozpoczyna sie przez wybranie przez program rozpoczynajacego.\n"
                                   "Nastepnie gracze rozmieszczaja na przemian swoje 9 pionow na polach planszy.\n"
                                   "Jesli ktoremus uda sie ustawic swoje trzy piony w linii (mlynek),\n"
                                   "moze zdjac z planszy dowolny pion przeciwnika, o ile nie sa one ustawione w mlynku. \n"
                                   "Gdy juz wszystkie piony stoja na planszy, rozpoczyna sie Faza 2 \n\n"
                                   "Faza 2: \n"
                                   "Gracze na zmiane moga przesuwac swoje piony na sasiednie puste pola. \n"
                                   "Za sasiednie pola uznaje sie pola polaczone linia lub bedace bezposrednio obok siebie\n"
                                   "W przypadku, kiedy graczowi pozostaja na planszy tylko trzy piony, moze on przestawiac\n"
                                   "wybrane ze swoich pionow na jakiekolwiek wolne pola, niekoniecznie sasiednie. \n"
                                   "Koniec gry nastepuje w sytuacji, kiedy jeden z graczy pozostaje z dwoma pionami na planszy.\n\n"
                                   "Powodzenia!\n"
                                   );


  gtk_style_context_add_class(gtk_widget_get_style_context(label), "powrotBtn");
  gtk_label_set_line_wrap (GTK_LABEL(label), TRUE);
  gtk_box_pack_start(GTK_BOX(boxInstrukcja), label, TRUE, FALSE, 0);

  GtkWidget *buttonBack = gtk_button_new_with_label("Powrot");
  gtk_style_context_add_class(gtk_widget_get_style_context(buttonBack), "menuBtn");
  g_signal_connect(G_OBJECT(buttonBack), "clicked",G_CALLBACK(powrot), NULL);
  gtk_box_pack_start(GTK_BOX(boxInstrukcja), buttonBack, FALSE, FALSE, 0);
  gtk_widget_show_all(boxInstrukcja);
  gtk_window_set_resizable(GTK_WINDOW (window), FALSE);

}

void powrot(GtkWidget *widget, gpointer data)
{
    gtk_container_remove(GTK_CONTAINER(window), GTK_WIDGET(boxInstrukcja));
    inicjalizacjaMenu();
    gtk_window_set_resizable(GTK_WINDOW (window), TRUE);
}
