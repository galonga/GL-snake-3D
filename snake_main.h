#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>
#include "gltx.h"
#include "glm.h"
#include <time.h>
#include <string.h>
#include <math.h>
#include <Windows.h>


#define HOCH 1
#define RUNTER 2
#define LINKS 3
#define RECHTS 4
#define FALSE   0
#define TRUE    1

//Kameraparameter
GLfloat anglex=0.0, angley=0 ,anglez=0, robrot=0 ,seg1rot=0, seg0rot=0;
double rotateangle=5;
static GLfloat headRotation = 90.0F ;
static GLfloat zoom=7.0 ;
static GLfloat rotate=0 ;
int Kamera_on = FALSE; //spiel ist aktiv


GLuint list; //Liste f�r zu zeichnende Opjekte

//Status  der Schlange
GLbyte  direction  = 0; //Richtung
int schlange_pos_x = 0; //Position der Schlange auf der x-Achse
int schlange_pos_z = 0; //Position der Schlange auf der y-Achse
int zug_beendet = TRUE; //Pruefvariable um ein auf der Stelle drehen zu verhindern

int score = 0; //Punkteanzahl

//Status des Hasen
int hase_pos_x = 3; //die x-Position des Hasen
int hase_pos_z = 2; //die y-Position des Hasen
GLbyte hase_gefressen = TRUE;

//Spielfeld
const int groesse_spielfeld = 13; //Faktor: �ndert spielfeld gr��e, wirkt sich auch auf kollision mit mauer aus ungerade werte nehmen!
const int spielfeld_laenge = 50; //die Spielfeldlaenge
int schlangenkoerper[100][2] = {{0}}; //der Schlangenkoerper der Schlange

int schlangen_laenge = 0;
int menueantwort = -1;
int auf_menuepunkt = -1;
double drehung_hase = 0;
double springen_hase = 0;
int max_sprung_punkt_erreicht = 0;

//Starwars
double scrollgeschwindigkeit = 5, view = 20;

void drawschlangenkopf();
void drawbunny();
void veraendere_schlangen_pos();
void timer(int value);
void zeichneSchlange();
void verschiebe_koerper(int alt_schlangen_pos_x, int alt_schlangen_pos_z);
void neuesSpiel();
void reset();
int kollision_mit_koeper(int x, int z);
void schlange_frist_hase();
int hat_schlange_hasen_gefressen();

//-----------------
void zeichne_text(int x, int y, int z, char *string, int gruen);
void zeichne_menue();
void diemouse(int btn, int state, int x, int y);
void diemouse_bewegt_sich(int x, int y);
void zeichne_score();
void timer_hase_drehen(int value);
void zeichne_anleitung();
void timeTick(int value);

/**
  To-Do:
  -[Geloest] unabhaengige Kamerasteruerung ueber LookAt :: Ueber "w""s" kann der gezoomt werden "a" "d" rotiert, Max. ist begrenzt
  -[Geloest] Farbe der Koerpersegmente
  -[Geloest] Kollisionserkennung: neue Position des Hasen, darf nicht eine Position des Koeprers der Schlange sein
  -[Geloest] Kollisionserkennung: Schlangenkopf und Koerpersegmente
  -[Geloest] reset
  -[Geloest] Popel nicht am Anfang setzen
  -[Geloest] Schlange darf nicht 180 Grad drehbar sein
  -[Geloest] Projekt heisst nun 3DSnake
  -[Geloest] Schlangenkopf leicht versetzt so das Koerper und Kopf eine Linie bilden
  -[Geloest] Score im Spiel anzeigen(�ber score Variable) (noch bissel buggy)
  -[Geloest] Kamerasteuerung nur bei aktiven Spiel moeglich und wird bei ende auf standartwert zurueckgestzt
  -[Geloest] Laufschrift mit Anleitung noch buggy(muss bei neu anzeigen zur�ck gesetzt werden und immer noch ne spiegelung)
  -[Geloest] Menue: das Menue wird nun als erstes aufgerufen und kann mit der Maus bedient werden. (siehe Funktion Maus).  Ueber die Varaibel Menueantwort wird je nach wahl etwas gemacht

                     momentan funktioniert nur Starten und Beenden. Wegen der Optik sollten wir uns mal vielleicht GEdanken machen ....
  -[]Kommentieren



  Extra-Feature:
  -[Geloest] Punkte
  -
*/
