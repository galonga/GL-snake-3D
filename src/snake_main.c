#include "snake_main.h"

void initlichter() //Lichter setzten usw
{
	//Add ambient light
    GLfloat ambientColor[] = {1, 1, 1, 1.0}; 
    //Add positioned light
    GLfloat lightColor0[] = {0.1, 0.5, 0.2, 1.0}; 
    GLfloat lightPos0[] = {4.0, 0.0, 8.0, 1.0}; 
    //Add directed light
    GLfloat lightColor1[] = {0.5, 0.2, 0.2, 1.0}; 
    //Coming from the direction (-1, 0.5, 0.5)
    GLfloat lightPos1[] = {-1.0, 0.5, 0.5, 0.0};

	//Add ambient light
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);

    //Add positioned light
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
	
    //Add directed light
    glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor1);
    glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);
}

/**
  Initalisiert die ersten Konfigurationen
*/
void Initialize(void) 
{
  glEnable(GL_DEPTH_TEST);
  glClearColor(0.2f, 0.2f, 0.2f, 1.0f); //Löschfarbe definieren R.G.B.Alpha | 1.0,1.0,1.0 = Weiß
  glEnable(GL_COLOR_MATERIAL); //Enable color
  glEnable(GL_LIGHTING); //Enable lighting
  glEnable(GL_LIGHT0); //Enable light #0
  glEnable(GL_LIGHT1); //Enable light #1
  glEnable(GL_NORMALIZE); //Automatically normalize normals 
  glEnable(GL_DEPTH_TEST); //Z-Buffer testen
}

/*** Zeichne Koordinatenkreuz */
void zeichneKS()
{
	glPushMatrix();
	glColor3f (1.0, 1.0, 0.8);
	glScalef(10,0.05,0.05);
	glutSolidCube(0.5);
	glPopMatrix();
	glPushMatrix();
	glScalef(0.05,10,0.05);
	glutSolidCube(0.5);
	glPopMatrix();
	glPushMatrix();
	glScalef(0.05,0.05,10);
	glutSolidCube(0.5);
	glPopMatrix();
}

//Nachfolgende Modelle transoformieren in XYZ Richtung
void blickrichtung(){
  //glPushMatrix();
  glRotatef(angley,1.0,0.0,0.0); //Würfel drehen x
  glRotatef(anglex,0.0,1.0,0.0); //Würfel drehen y
  glRotatef(anglez,0.0,0.0,1.0); //Würfel drehen z
  //glPopMatrix();
}

void zeichneSpielfeld() //update 12.1.12 - 11:40
{
    GLfloat dicke = 0.1;
    GLfloat laenge = groesse_spielfeld; //änderung hier skaliert das Spielfeld
    GLfloat hoehe = 0.6;
    GLfloat faktor = laenge/2;
    GLfloat nachoben = 0.2;

    //Malt das Spielfeld
    //Grundflaeche
    glPushMatrix();
    glColor3f(0.5, 0.5, 0.5); // Farbe Spielfeld
    glScalef(laenge, 0.2, laenge);
    glutSolidCube(1);
    glPopMatrix();
    //Rahmen
    glPushMatrix();
    glTranslatef(faktor, nachoben, 0.0);
    glScalef(dicke,hoehe,laenge);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-faktor, nachoben, 0.0);
    glScalef(dicke,hoehe,laenge);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, nachoben, faktor);
    glScalef(laenge,hoehe,dicke);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, nachoben, -faktor);
    glScalef(laenge,hoehe,dicke);
    glutSolidCube(1);
    glPopMatrix();    

    //Zeichnet die Anzeigentafel
    glPushMatrix();
    glTranslatef(0, nachoben, -faktor);
    glScalef(1,5,dicke);
    glutSolidCube(1);
    glPopMatrix(); 

    glPushMatrix();
    glTranslatef(0, nachoben+2, -faktor);
    glScalef(2,1,dicke);
    glutSolidCube(1);
    glPopMatrix();
}

/**
 * Funktion um den Hasen zu zeichnen
 */
void zeichneHasen()
{
	glPushMatrix();
  
	//glTranslatef(hase_pos_x, 0.2, hase_pos_z);
  glTranslatef(hase_pos_x, 0.2 + springen_hase, hase_pos_z);
	glScalef(0.3, 0.3, 0.3);
  glRotatef(drehung_hase,0, 1.0, 0);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); //Wie eine Textur beleuchtet wird.. DECAL nur die eingene Texturfarbe
	glEnable(GL_TEXTURE_2D); //aktiv Textur
	drawbunny(); // zeichnet den Hasen
	glDisable(GL_TEXTURE_2D); // dektiviert textur
	glPopMatrix();
}

/**
  Aendert die Position der Hasenvariablen auf eine zufaellige Position
*/
void veraendere_hasen_pos()
{
  int i = 0;
  while (i == 0)
  {
    i=1;
    hase_pos_x = (rand() % 10)- spielfeld_laenge/10 ;
	  hase_pos_z = (rand() % 10)- spielfeld_laenge/10 ;
    
    if (schlange_pos_x == hase_pos_x && schlange_pos_z == hase_pos_z)
    {
      printf("pos kopf == hase");
    }
    else
    {
      if (kollision_mit_koeper(hase_pos_x, hase_pos_z))
      {
        i = 0;
      }
    }
  }	
} 

/**
  Zeichnet die gesamte Schlange (Kopf und Koerpersegmente)
*/

void zeichneSchlange()
{
  int i = 0;

	glPushMatrix();
	glTranslatef(schlange_pos_x, 0.2, schlange_pos_z); //pos. muss vor scale verändert werden!
    //Kopf der Schlange		
	glScalef(0.3, 0.3, 0.3);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); //Wie eine Textur beleuchtet wird.. DECAL nur die eingene Texturfarbe
	glEnable(GL_TEXTURE_2D); //aktiv Textur
	//Verschiebt die SChlange um eine bestimmtes Stueck
	glRotatef(headRotation, 0.0, 1.0, 0.0);
	glTranslatef(0.5,0,0); //anpassung des Schlangenkopfen an Schwanz da 3D Modell nicht ganz mittig war
	drawschlangenkopf(); // zeichne Schlangenkopf  
	glDisable(GL_TEXTURE_2D); // dektiviert textur
	glPopMatrix();

  //zeichnet die Koerpersegmente
  for (i = 0; i < schlangen_laenge; i++)
  {
    glPushMatrix();
    glTranslatef(schlangenkoerper[i][0], 0.8, schlangenkoerper[i][1]); //pos. muss vor scale verändert werden!
    glColor3f(0,0.3,0);//Farbe Gruen
    glScalef(0.3, 0.3, 0.3);
    glutSolidSphere(1,20,20);
    glPopMatrix();
  }
}

/**
  Prueft ob die Schlange den Hasen gefressen hat.
  @return 1, falls die SChlange den Hasen gefressen hat, ansonsten 0
*/

int hat_schlange_hasen_gefressen()
{
  if(hase_pos_x == schlange_pos_x && hase_pos_z == schlange_pos_z)
	{
    return 1;
	}
  else
  {
    return 0;
  }
}

/**
  Kollisionserkennung Hase und Schlange
  Falls die Positionen der Schlange mit der Position des Hasen uebereinstimmt, dann wird der Hase auf eine zufaellige Position gesetzt.
*/

void schlange_frist_hase()
{
	if(hase_pos_x == schlange_pos_x && hase_pos_z == schlange_pos_z)
	{
		//printf(" Schlange wächst \n");
		score+=100;
		schlangen_laenge++;
		//verschiebe_koerper(-50,-50);
		veraendere_hasen_pos();
	}
}

/**
  Zeichnet das Spiel inkl. aller Inhalte
*/
void zeichneSnakeSpiel()
{
	glPushMatrix();
	blickrichtung();

	//zeichneKS();
  zeichneSchlange(); //zeichnet das Spielfeld
	zeichneSpielfeld();     //zeichnet das Spielfeld
	zeichneHasen();         //zeichnet den Hasen
	//schlange_frist_hase(); //test auf treffer  
  zeichne_score();
	//printf("hase posx= %i hase posz= %i \n schlange posx= %i posz=%i \n",hase_pos_x,hase_pos_z,schlange_pos_x,schlange_pos_z); //debug pos hase & schlange
  
	glPopMatrix();
}

/**
  Dise Funktion ruft immer wieder sich selber in einem gewissen Intervall auf. So laesst die Schlangenposition veraendern.
*/

void timer(int value)
{
  /* Do timer processing */
   /* maybe glutPostRedisplay(), if necessary */

   /* call back again after elapsedUSecs have passed */
  veraendere_schlangen_pos();
  zug_beendet = TRUE;
  //printf("Score: %i\n",score);
  glutTimerFunc(500, timer, 0);
}

void timer_hase_drehen(int value)
{
  double max_springen_hase = 0.6;
  double faktor_springen_hase = 0.05;
  
  drehung_hase+=1.5;
  //printf("Springen_hase: %lf\n", temp);
  
  if (springen_hase >= max_springen_hase)
  {
    max_sprung_punkt_erreicht = 1;
  }
  else
    if (springen_hase <= 0)
    {
      max_sprung_punkt_erreicht = 0;
    }

  if (max_sprung_punkt_erreicht == 0)
  {
    springen_hase += faktor_springen_hase;
  }
  else
  {
    springen_hase -= faktor_springen_hase;
  }
  
  zeichneHasen();
  glutTimerFunc(50, timer_hase_drehen, 0);
}



/**
  Setzt alle relevanten Werte des Spieles zureuck. Die Koerpersegmente werden auf 0 gesetzt, sowie die Schlangenposition und die Hasenposition auf einen Standardwert.
*/

void reset()
{
  //resettet die Koepersegmente
  int i = 0;
  for (i = 0; i < schlangen_laenge; i++)
  {
    schlangenkoerper[i][0] = 0;
    schlangenkoerper[i][1] = 0;
  }

  schlangen_laenge = 0;
  schlange_pos_x = 0;
  schlange_pos_z = 0;
  veraendere_hasen_pos();
  zoom=7.0 ;
  rotate=0 ;
  score=0;
  direction=0;
}

/**
  Beginnt eine neues Spiel
*/

void neuesSpiel()
{
  Sleep(1000);
  reset();
}

void display(void)
{
   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Bildschirm löschen 1. löscht farben 2. löscht zbuffer
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity ();             //Einheitsmatrix - keine änderungen
   initlichter(); //Licht Aufrufen
   //glTranslatef (-60.0, 40.0, zoom);
   //glTranslatef (-5, -5,-5); //Ansicht einstellen // glTranslatef (-60.0, 40.0, zoom); //Ansicht einstellen
   gluLookAt (rotate, zoom+5, zoom, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0); //Darstellung auf dem Bildschirm bei Betrachterposition 
   
   //mappe_textur_aus_spielfeld();

   //zeichneKS();

   //### zeichne schlange + Spielfeld
   
   
   
   if (menueantwort == 1)
   {     
	 Kamera_on = TRUE;
     zeichneSnakeSpiel();      
   }
   else
   {
     if (menueantwort == 2)
     {
	   Kamera_on = FALSE;
       timeTick(0);
       zeichne_anleitung();
     }
     else
     {
	   Kamera_on = FALSE;
       zeichne_menue();     
     }
   }  
   
   //###
   //glCallList(list);

   //glFlush (); //Bild ist zu Ende und soll angezeigt werden bei SingleBuffer
   glutPostRedisplay();
   glutSwapBuffers(); // Doubble Buffering zeichenanweisung
}

/**
  Zeichnet die Punkte in das Fenster
*/

void zeichne_score()
{
  char buf[6];
  int len;
  int i;

  glPushMatrix();
  itoa(score, buf, 10);
  glColor3f(0.0, 1.0, 0.0); // Gruen
  glRasterPos3f(-0.3, 2.5, -6);
  //glRotatef(90, 1.0, 0, 0);
  
  for (i = 0; i < 5; i++)
  {
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, buf[i]);
  }
  glPopMatrix();
}

void reshape (int w, int h)
{
   glViewport (10, 10, (GLsizei) w, (GLsizei) h); 
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   glFrustum (-1.0, 1.0, -1.0, 1.0, 1.5, 200.0); //z-Puffer sichtweite ändern
   glMatrixMode(GL_MODELVIEW);
}

/* ARGSUSED1 */
void keyboard(unsigned char key, int x, int y) //x&y positon des Mauszeigers bei KeyEvent
{
   switch (key) {
      case 27: //"ESC"
         //exit(0);
         menueantwort = -1;
		 reset();
         break;
	case  'w':
		  if(zoom>=2 && Kamera_on == TRUE)
		  {
			  zoom-=1;
			  glutPostRedisplay();  //Bild neu zeichen
		  }else{
			  printf("Max. Zoomlevel erreicht\n");
		  }
		  break;
	case  's':
		  if(zoom<=15 && Kamera_on == TRUE)
		  {
			  zoom+=1;
			  glutPostRedisplay();  //Bild neu zeichen
		  }else{
			  printf("Max. Zoomlevel erreicht\n");
		  }
		  break;
	case  'a':
		if(rotate>=-7 && Kamera_on == TRUE)
		  {
			  rotate-=1;
			  glutPostRedisplay();  //Bild neu zeichen
		  }else{
			  printf("Max. Schwenklevel erreicht\n");
		  }
		  break;

	case  'd':
		if(rotate<=7 && Kamera_on == TRUE)
		  {
			  rotate+=1;
			  glutPostRedisplay();  //Bild neu zeichen
		  }else{
			  printf("Max. Schwenklevel erreicht\n");
		  }
		  break;
   }
}

/**
  Veraendert die Schlangenposition
*/

void veraendere_schlangen_pos()
{  
  if (hat_schlange_hasen_gefressen() == TRUE)
  {
    schlange_frist_hase();
    zeichneSchlange();
  }
  verschiebe_koerper(schlange_pos_x, schlange_pos_z); //siehe Funktion
	  if (direction == RECHTS)
	  {
		schlange_pos_x++;
		printf("Rechts");
		if (schlange_pos_x > ((groesse_spielfeld-1)/2))
		{
		  printf("Das Spiel beginnnt neu");
		  neuesSpiel();
		}
	  }
	  else
	  {
		if (direction == LINKS)
		{
		  schlange_pos_x--;
		  printf("Links");
		  if ((-1 * schlange_pos_x) > ((groesse_spielfeld-1)/2))
		  {
			neuesSpiel();
		  }
		}
		else
		{
		  if (direction == HOCH)
		  {
			schlange_pos_z--;
			printf("HOCH");
			if (-1 * schlange_pos_z > ((groesse_spielfeld-1)/2))
			{
			  neuesSpiel();
			}
		  }
		  else
		  {
			if (direction == RUNTER)
			{
			  schlange_pos_z++;
			  printf("RUNTER");
			  if ((schlange_pos_z) > ((groesse_spielfeld-1)/2))
			  {
				neuesSpiel();
			  }
			}
		  }
		}
	  
  }  
  if (kollision_mit_koeper(schlange_pos_x, schlange_pos_z))
  {
    neuesSpiel();
  }
  //zeichneSnakeSpiel();
  //printf("Spielfeld wir gezeichnet");
}

/**
  Prueft eine beliebige Position mit Koerpersegmenten der Schlange
  @param x die x-Position 
  @param z die z-Position 
  @return 1, falls Kollision stattfindet, ansonsten 0
*/

int kollision_mit_koeper(int x, int z)
{
  int i = 0;
  for (i = 0; i < schlangen_laenge; i++)
  {
    if (schlangenkoerper[i][0] == x && schlangenkoerper[i][1] == z)
    {
      return TRUE;
    }
  }
  return FALSE;
}

/**
  Verschiebt die Position vom Schlangenkoerper um die Parameter. D.h. das erste Koerperteil nimmt die alte Position des Schlangenkopfes an,
  sofern die Schlangenlaenge > 0 ist. Desweiteren werden die anderen Koerperteile auf die alte Position des vorherigen Koerperteils gesetzt.
  @param pos_x die alte Position des Schlangenkopfes
  @param pos_x die alte Position des Schlangenkopfes
*/

void verschiebe_koerper(int alt_schlangen_pos_x, int alt_schlangen_pos_z)
{  
  int temp_koerperteil_x = 0;
  int temp_koerperteil_z = 0;

  int aktuell_x = alt_schlangen_pos_x;
  int aktuell_z = alt_schlangen_pos_z;

  int i = 0; 
  for (i = 0; i < schlangen_laenge; i++)
  {
    temp_koerperteil_x = schlangenkoerper[i][0]; //merk dir die alte Position des alten 
    temp_koerperteil_z = schlangenkoerper[i][1];

    schlangenkoerper[i][0] = aktuell_x;
    schlangenkoerper[i][1] = aktuell_z;

    aktuell_x = temp_koerperteil_x;
    aktuell_z = temp_koerperteil_z;
  }
}

void Special(int key, int x, int y)
{ //ermöglicht die Pfeiltasten zum steuern zu nutzen
  if(zug_beendet == TRUE)
  {	
    switch(key){
    case GLUT_KEY_RIGHT     :
        if(direction != LINKS)
		    {
			    direction = RECHTS;
				headRotation =90;
		    }   
        break;
    case GLUT_KEY_LEFT      :
        if(direction != RECHTS)
        {
          direction = LINKS;
          headRotation =-90;
        }
        break;
    case GLUT_KEY_UP        :
        if(direction != RUNTER)
        {
          direction = HOCH;
          headRotation = 180;
        }
        break;
    case GLUT_KEY_DOWN      :
        if(direction != HOCH)
        {
          direction = RUNTER;
          headRotation = 0;
        }
        break;
    }
	zug_beendet = FALSE;
  }
}

void timeTick(int value)
{
  if (scrollgeschwindigkeit< -400) 
  {
    view-=0.000001;
  }
  if(view < 0) 
  {
    view=20; 
    scrollgeschwindigkeit = -50.0;
  }
  scrollgeschwindigkeit -= 0.015;
  zeichne_anleitung();
  glutTimerFunc(5000, timeTick, 0);
}

void zeichne_anleitung()
{
  int numberOfQuotes = 4;
  int l,lenghOfQuote, i;

  const char *string[]={"Willkommen zu Snake 3D","Anleitung:",
    "Die Schange wird ueber das Steuerkreuz der Tastatur gesteuert.",
	"Die Kamera kann ueber w,s zoomen und mit a,d schwenken.", 
    "Viel Spaß beim spielen.", "(c) Andrej Peters und Timo Arndt"};


	glTranslatef(0.0, -140, scrollgeschwindigkeit);
	glRotatef(240, 1.0, 0.0, 0.0);
	glScalef(0.04, 0.07, 0.07);

	for(l=0; l < numberOfQuotes;l++)
	{
    lenghOfQuote = (int) strlen(string[l]);
		glPushMatrix();
		glTranslatef(-(lenghOfQuote*37), -(l*200), 0.0);
		for (i = 0; i < lenghOfQuote; i++)
		{
		glColor3f(1,1,0.0);	
		glutStrokeCharacter(GLUT_STROKE_ROMAN,  string[l][i]);
		}
		glPopMatrix();
	}
    glLoadIdentity ();
} 

void zeichne_menue()
{
  //glColor3f(0.5, 0.3, 0.3); //Rot
  //glColor3f(0.5f, 0.3f, 0.3f); //Rot
  //glRectd(-1, 1, 3, -1); //Zeichnet ein Recteck
  if (auf_menuepunkt == 0)
  {
    zeichne_text(0, 0, 0,"Starten", 0);
  }
  else
  {
    zeichne_text(0, 0, 0,"Starten", 1);
  }
  if (auf_menuepunkt == 1)
  {
    zeichne_text(0, -2, 0, "Anleitung anzeigen", 0);
  }
  else
  {
    zeichne_text(0, -2, 0, "Anleitung anzeigen", 1);
  } 
  if (auf_menuepunkt == 2)
  {
    zeichne_text(0, -5, 0,"Beenden", 0);
  }
  else
  {
    zeichne_text(0, -5, 0,"Beenden", 1);
  }      
}

/**
  Schreibt einen Text in das Opengl-Fenster an einer bestimmten Position.
  @param x die x-Koordinate des zu schreibenden Textes
  @param y die y-Koordinate des zu schreibenden Textes
  @param string die Zeichenkette, die geschrieben werden soll
  @param gruen 1, falls gruen gezeichnet werden soll, 0 falls rot
*/

void zeichne_text(int x, int y, int z, char *string, int gruen)
{
  int len;
  int i;

  glPushMatrix();
  if (gruen == 1)
  {
    glColor3f(0.0, 1.0, 0.0); // Gruen
  }
  else
  {
    glColor3f(1.0, 0.0, 0.0); // Rot
  }
  
  glRasterPos3f(x, y, z);
  len = (int) strlen(string);

  for (i = 0; i < len; i++)
  {
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string[i]);
  }
  glPopMatrix();
}

/**
  Diese Funktion wird aufgerufen, falls ein Mousebutton aktiivert wird.
  @param btn der Button der gedrueckt wurde
  @param state welcher Zustand des Buttons, gedrueckt oder bereits losgelassen
  @param x die x Koordinate der Maus (Achtung: links: 0; rechts: max)
  @param y die y Koordinate der Maus (Achtung: oben : 0; unten max)
*/

void diemouse(int btn, int state, int x, int y)
{
  //printf("x:%d y:%d\n", x, y);

  if (menueantwort < 0)
  {
    //Menupunkt 1
    if (y >= 370 && y <= 399 && x >= 402 && x <= 459)
    {
      menueantwort = 1;
      printf("Menue:%d\n", menueantwort);
    }
    if (y >= 413 && y <= 425 && x >= 413 && x <= 563)
    {
	//Menuepunkt 2 - es wird auch die Anleitung zurueck gesetzt
	  scrollgeschwindigkeit = 5 ;
      view = 20;
      menueantwort = 2;

    }
    //Menupunkt 3
    if (y >= 469 && y <= 483 && x >= 402 && x <= 459)
    {
      menueantwort = -1;
      printf("Menue:%d\n", menueantwort);
      exit(0);
    }
  }
}

/**
  Diese Funktion wird aufgerufen, falls sich die Maus im Fenster bewegt. 
  @param die x-Koordiante der Maus im Fenster
  @param die y-Koordiante der Maus im Fenster
*/

void diemouse_bewegt_sich(int x, int y)
{
  printf("bewegt sich: x:%d y:%d\n", x, y);
  auf_menuepunkt = -1;
  //Menupunkt 1
  if (y >= 370 && y <= 399 && x >= 402 && x <= 459)
  {
    auf_menuepunkt = 0;
  }
  if (y >= 413 && y <= 425 && x >= 413 && x <= 563)
  {
    auf_menuepunkt = 1;
  }
  //Menupunkt 3
  if (y >= 469 && y <= 483 && x >= 402 && x <= 459)
  {
    auf_menuepunkt = 2;
  }
}

int main(int argc, char** argv) // Einsprungpunkt
{
   
	//glut - universale befehle win/unix
   glutInit(&argc, argv); 
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB ); //OpenGL Aufruf
   glutInitWindowSize (800, 800); //erzeugen von Fenster
   glutInitWindowPosition (200, 0);
   glutCreateWindow ("Snake 3D by Andrej Peters & Timo Arndt"); //Name des Programms

   glutKeyboardFunc(keyboard);//Ereignis Tastenbetätigung
   glutSpecialFunc(Special); //bewegungen über pfeiltasten ermöglichen
   glutDisplayFunc(display); //Ereignis 
   glutReshapeFunc(reshape); //Ereignis Größenänderung
   glutMouseFunc(diemouse); //Funktion fuer die Maus
   glutPassiveMotionFunc(diemouse_bewegt_sich);
   Initialize();  

   
   srand((unsigned) time(NULL)); //Initialisiert den Zufallsgenerator
   timer(0);
   timer_hase_drehen(0);
    glutMainLoop();

   return 0;
}