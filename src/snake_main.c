#include "snake_main.h"

void initLights()
{
	//ambient light
	GLfloat ambientColor[] = { 1, 1, 1, 1.0 };
	//Add positioned light
	GLfloat lightColor0[] = { 0.1, 0.5, 0.2, 1.0 };
	GLfloat lightPos0[] = { 4.0, 0.0, 8.0, 1.0 };
	//Add directed light
	GLfloat lightColor1[] = { 0.5, 0.2, 0.2, 1.0 };
	//Coming from the direction (-1, 0.5, 0.5)
	GLfloat lightPos1[] = { -1.0, 0.5, 0.5, 0.0 };

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
  Initialize the game
*/
void Initialize(void)
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f); //L�schfarbe definieren R.G.B.Alpha | 1.0,1.0,1.0 = Wei�
	glEnable(GL_COLOR_MATERIAL); //Enable color
	glEnable(GL_LIGHTING); //Enable lighting
	glEnable(GL_LIGHT0); //Enable light #0
	glEnable(GL_LIGHT1); //Enable light #1
	glEnable(GL_NORMALIZE); //Automatically normalize normals
	glEnable(GL_DEPTH_TEST); //Z-Buffer testen
}

void drawMatrix()
{
	glPushMatrix();
	glColor3f(1.0, 1.0, 0.8);
	glScalef(10, 0.05, 0.05);
	glutSolidCube(0.5);
	glPopMatrix();
	glPushMatrix();
	glScalef(0.05, 10, 0.05);
	glutSolidCube(0.5);
	glPopMatrix();
	glPushMatrix();
	glScalef(0.05, 0.05, 10);
	glutSolidCube(0.5);
	glPopMatrix();
}

void cameraDirection() {
	glRotatef(angley, 1.0, 0.0, 0.0);
	glRotatef(anglex, 0.0, 1.0, 0.0);
	glRotatef(anglez, 0.0, 0.0, 1.0);
}

void drawGameDesk()
{
	GLfloat thickness = 0.1;
	GLfloat lenght = sizeOfGameDesk;
	GLfloat hight = 0.6;
	GLfloat factor = lenght / 2;
	GLfloat translateUp = 0.2;

	//draw base
	glPushMatrix();
	glColor3f(0.5, 0.5, 0.5); // Farbe Spielfeld
	glScalef(lenght, 0.2, lenght);
	glutSolidCube(1);
	glPopMatrix();

	//draw border
	glPushMatrix();
	glTranslatef(factor, translateUp, 0.0);
	glScalef(thickness, hight, lenght);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-factor, translateUp, 0.0);
	glScalef(thickness, hight, lenght);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, translateUp, factor);
	glScalef(lenght, hight, thickness);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, translateUp, -factor);
	glScalef(lenght, hight, thickness);
	glutSolidCube(1);
	glPopMatrix();

	//draw score
	glPushMatrix();
	glTranslatef(0, translateUp, -factor);
	glScalef(1, 5, thickness);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, translateUp + 2, -factor);
	glScalef(2, 1, thickness);
	glutSolidCube(1);
	glPopMatrix();
}

/**
 * Draw Rabbit
 */
void drawRabbit()
{
	glPushMatrix();
	glTranslatef(rabbitPosX, 0.2 + rabbitJump, rabbitPosZ);
	glScalef(0.3, 0.3, 0.3);
	glRotatef(rotationRabbit, 0, 1.0, 0);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glEnable(GL_TEXTURE_2D);
	drawbunny();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

/**
  Random rabbit position
*/
void moveRabbit()
{
	int i = 0;
	while (i == 0)
	{
		i = 1;
		rabbitPosX = (rand() % 10) - lenghtOfGameDesk / 10;
		rabbitPosZ = (rand() % 10) - lenghtOfGameDesk / 10;

		if (snakePosX == rabbitPosX && snakePosZ == rabbitPosZ)
		{
			printf("Snake (meets) Rabbit");
		}
		else {
			if (collisionWithSnakeBody(rabbitPosX, rabbitPosZ))
			{
				i = 0;
			}
		}
	}
}

/**
  Draw full snake
*/
void drawSnake()
{
	int i = 0;

	glPushMatrix();
	glTranslatef(snakePosX, 0.2, snakePosZ);

	//head of the snake
	glScalef(0.3, 0.3, 0.3);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glEnable(GL_TEXTURE_2D);
	glRotatef(headRotation, 0.0, 1.0, 0.0);
	glTranslatef(0.5, 0, 0);
	drawSnakeHead();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//body of the snake
	for (i = 0; i < snakeLenght; i++)
	{
		glPushMatrix();
		glTranslatef(snakeBody[i][0], 0.8, snakeBody[i][1]);
		glColor3f(0, 0.3, 0);
		glScalef(0.3, 0.3, 0.3);
		glutSolidSphere(1, 20, 20);
		glPopMatrix();
	}
}

/**
  Check if collision - kill rabbit
  @return 1 = true else 0 = false^
*/
int snakeFinishedRabbit()
{
	if (rabbitPosX == snakePosX && rabbitPosZ == snakePosZ)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/**
  Check if collision - score++ & add new rabbit
*/
void snakeEatsRabbit()
{
	if (rabbitPosX == snakePosX && rabbitPosZ == snakePosZ)
	{
		score += 100;
		snakeLenght++;

		moveRabbit();
	}
}

/**
  Draw game with all content
*/
void drawGame()
{
	glPushMatrix();
	cameraDirection();

	drawGameDesk();
	drawSnake();
	drawRabbit();
	drawScore();

	printf("Rabbit posx= %i posz= %i \n Snake posx= %i posz=%i \n", rabbitPosX, rabbitPosZ, snakePosX, snakePosZ); //debug pos hase & schlange

	glPopMatrix();
}

/**
  * timer func
  * Do timer processing
  * maybe glutPostRedisplay(), if necessary
  * call back again after elapsedUSecs have passed
*/
void timer(int value)
{
	moveSnakePos();
	moveFinished = TRUE;
	glutTimerFunc(500, timer, 0);

	printf("Score: %i\n", score);
}

/**
  * timer func
  * Rotates the rabbit
*/
void rotateRabbitTimer(int value)
{
	double maxJumpHight = 0.6;
	double factorJumpHight = 0.05;

	rotationRabbit += 1.5;

	if (rabbitJump >= maxJumpHight) {
		maxJumpHightTouched = 1;
	}
	else if (rabbitJump <= 0)
	{
		maxJumpHightTouched = 0;
	}

	if (maxJumpHightTouched == 0)
	{
		rabbitJump += factorJumpHight;
	}
	else {
		rabbitJump -= factorJumpHight;
	}

	drawRabbit();

	glutTimerFunc(50, rotateRabbitTimer, 0);
}



/**
  Reset Game
*/
void resetGame()
{
	//Reset snakeBody
	int i = 0;
	for (i = 0; i < snakeLenght; i++)
	{
		snakeBody[i][0] = 0;
		snakeBody[i][1] = 0;
	}

	snakeLenght = 0;
	snakePosX = 0;
	snakePosZ = 0;
	moveRabbit();
	zoom = 7.0;
	rotate = 0;
	score = 0;
	direction = 0;
}

/**
  start new game
*/
void newGame()
{
	Sleep(1000);
	resetGame();
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	initLights();
	gluLookAt(rotate, zoom + 5, zoom, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	if (menuSelection == 1) {
		cameraActive = TRUE;
		drawGame();
	}
	else {
		if (menuSelection == 2) {
			cameraActive = FALSE;
			timeTick(0);
			drawReadme();
		}
		else {
			cameraActive = FALSE;
			drawMenu();
		}
	}

	glutPostRedisplay();
	glutSwapBuffers();
}

/**
  draw Score
*/
void drawScore()
{
	char buf[6];
	int len;
	int i;

	glPushMatrix();
	itoa(score, buf, 10);
	glColor3f(0.0, 1.0, 0.0);
	glRasterPos3f(-0.3, 2.5, -6);

	for (i = 0; i < 5; i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, buf[i]);
	}

	glPopMatrix();
}

void reshape(int w, int h)
{
	glViewport(10, 10, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-1.0, 1.0, -1.0, 1.0, 1.5, 200.0);
	glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 27: //"ESC"
	   //exit(0);
		menuSelection = -1;
		resetGame();
		break;
	case  'w':
		if (zoom >= 2 && cameraActive == TRUE) {
			zoom -= 1;
			glutPostRedisplay();
		}
		else {
			printf("Max. Zoomlevel!\n");
		}
		break;
	case  's':
		if (zoom <= 15 && cameraActive == TRUE) {
			zoom += 1;
			glutPostRedisplay();
		}
		else {
			printf("Max. Zoomlevel!\n");
		}
		break;
	case  'a':
		if (rotate >= -7 && cameraActive == TRUE) {
			rotate -= 1;
			glutPostRedisplay();
		}
		else {
			printf("Max. rotation\n");
		}
		break;
	case  'd':
		if (rotate <= 7 && cameraActive == TRUE) {
			rotate += 1;
			glutPostRedisplay();
		}
		else {
			printf("Max. rotation\n");
		}
		break;
	}
}

/**
  move the Snake!
*/
void moveSnakePos()
{
	if (snakeFinishedRabbit() == TRUE)
	{
		snakeEatsRabbit();
		drawSnake();
	}
	moveSnakeBody(snakePosX, snakePosZ);

	if (direction == RIGHT) {
		snakePosX++;
		printf("RIGHT");
		if (snakePosX > ((sizeOfGameDesk - 1) / 2)) {
			newGame();
		}
	}
	else if (direction == LEFT) {
		snakePosX--;
		printf("LEFT");
		if ((-1 * snakePosX) > ((sizeOfGameDesk - 1) / 2)) {
			newGame();
		}
	}
	else if (direction == UP) {
		snakePosZ--;
		printf("UP");
		if (-1 * snakePosZ > ((sizeOfGameDesk - 1) / 2))
		{
			newGame();
		}
	}
	else if (direction == DOWN) {
		snakePosZ++;
		printf("RUNTER");
		if ((snakePosZ) > ((sizeOfGameDesk - 1) / 2))
		{
			newGame();
		}
	}

	if (collisionWithSnakeBody(snakePosX, snakePosZ)) {
		newGame();
	}
}

/**
  if collision with snakebody^
  @param x x-Position
  @param z z-Position
  @return boolean
*/
int collisionWithSnakeBody(int x, int z)
{
	int i = 0;
	for (i = 0; i < snakeLenght; i++) {
		if (snakeBody[i][0] == x && snakeBody[i][1] == z) {

			return TRUE;
		}
	}

	return FALSE;
}

void moveSnakeBody(int oldSnakeX, int oldSnakeZ)
{
	int tempBodyPartX = 0;
	int tempBodyPartZ = 0;

	int currentX = oldSnakeX;
	int currentZ = oldSnakeZ;

	int i = 0;
	for (i = 0; i < snakeLenght; i++) {
		tempBodyPartX = snakeBody[i][0]; //merk dir die alte Position des alten
		tempBodyPartZ = snakeBody[i][1];

		snakeBody[i][0] = currentX;
		snakeBody[i][1] = currentZ;

		currentX = tempBodyPartX;
		currentZ = tempBodyPartZ;
	}
}

void special(int key, int x, int y)
{
	if (moveFinished == TRUE)
	{
		switch (key) {
		case GLUT_KEY_RIGHT:
			if (direction != LEFT) {
				direction = RIGHT;
				headRotation = 90;
			}
			break;
		case GLUT_KEY_LEFT:
			if (direction != RIGHT) {
				direction = LEFT;
				headRotation = -90;
			}
			break;
		case GLUT_KEY_UP:
			if (direction != DOWN) {
				direction = UP;
				headRotation = 180;
			}
			break;
		case GLUT_KEY_DOWN:
			if (direction != UP) {
				direction = DOWN;
				headRotation = 0;
			}
			break;
		}
		moveFinished = FALSE;
	}
}

void timeTick(int value)
{
	if (scrollSpeed < -400) {
		view -= 0.000001;
	}

	if (view < 0) {
		view = 20;
		scrollSpeed = -50.0;
	}

	scrollSpeed -= 0.015;
	drawReadme();
	glutTimerFunc(5000, timeTick, 0);
}

void drawReadme()
{
	int numberOfQuotes = 4;
	int l, lenghOfQuote, i;

	const char *string[] = { "Willkommen zu Snake 3D","Anleitung:",
	  "Die Schange wird ueber das Steuerkreuz der Tastatur gesteuert.",
	  "Die Kamera kann ueber w,s zoomen und mit a,d schwenken.",
	  "Viel Spa� beim spielen.", "(c) Andrej Peters und Timo Arndt" };


	glTranslatef(0.0, -140, scrollSpeed);
	glRotatef(240, 1.0, 0.0, 0.0);
	glScalef(0.04, 0.07, 0.07);

	for (l = 0; l < numberOfQuotes; l++) {
		lenghOfQuote = (int)strlen(string[l]);
		glPushMatrix();
		glTranslatef(-(lenghOfQuote * 37), -(l * 200), 0.0);
		for (i = 0; i < lenghOfQuote; i++) {
			glColor3f(1, 1, 0.0);
			glutStrokeCharacter(GLUT_STROKE_ROMAN, string[l][i]);
		}
		glPopMatrix();
	}
	glLoadIdentity();
}

void drawMenu()
{
	if (menuSelect == 0) {
		drawText(0, 0, 0, "Starten", 0);
	}
	else {
		drawText(0, 0, 0, "Starten", 1);
	}
	if (menuSelect == 1) {
		drawText(0, -2, 0, "Anleitung anzeigen", 0);
	}
	else {
		drawText(0, -2, 0, "Anleitung anzeigen", 1);
	}
	if (menuSelect == 2) {
		drawText(0, -5, 0, "Beenden", 0);
	}
	else {
		drawText(0, -5, 0, "Beenden", 1);
	}
}

void drawText(int x, int y, int z, char *string, int green)
{
	int len;
	int i;

	glPushMatrix();

	if (green == 1) {
		glColor3f(0.0, 1.0, 0.0);
	}
	else {
		glColor3f(1.0, 0.0, 0.0);
	}

	glRasterPos3f(x, y, z);
	len = (int)strlen(string);

	for (i = 0; i < len; i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string[i]);
	}

	glPopMatrix();
}

void mouseClicked(int btn, int state, int x, int y)
{
	if (menuSelection < 0)
	{
		//menu 1
		if (y >= 370 && y <= 399 && x >= 402 && x <= 459) {
			menuSelection = 1;
			printf("Menue:%d\n", menuSelection);
		}

		if (y >= 413 && y <= 425 && x >= 413 && x <= 563) {
			//menu 2
			scrollSpeed = 5;
			view = 20;
			menuSelection = 2;
		}
		//menu 3
		if (y >= 469 && y <= 483 && x >= 402 && x <= 459) {
			menuSelection = -1;
			printf("Menue:%d\n", menuSelection);
			exit(0);
		}
	}
}

/**
  Diese Funktion wird aufgerufen, falls sich die Maus im Fenster bewegt.
  @param die x-Koordiante der Maus im Fenster
  @param die y-Koordiante der Maus im Fenster
*/

void moveMouse(int x, int y)
{
	printf("bewegt sich: x:%d y:%d\n", x, y);
	menuSelect = -1;

	// menu 1
	if (y >= 370 && y <= 399 && x >= 402 && x <= 459) {
		menuSelect = 0;
	}

	// menu 2
	if (y >= 413 && y <= 425 && x >= 413 && x <= 563) {
		menuSelect = 1;
	}

	//menu 3
	if (y >= 469 && y <= 483 && x >= 402 && x <= 459) {
		menuSelect = 2;
	}
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(200, 0);
	glutCreateWindow("Snake 3D");

	//keyboard event
	glutKeyboardFunc(keyboard);
	//keyboard arrow event
	glutSpecialFunc(special);
	//mouseClicked event
	glutMouseFunc(mouseClicked);
	//mouse moved event
	glutPassiveMotionFunc(moveMouse);

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);

	Initialize();

	srand((unsigned)time(NULL));
	timer(0);
	rotateRabbitTimer(0);
	glutMainLoop();

	return 0;
}
