#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>
#include "gltx.h"
#include "glm.h"
#include <time.h>
#include <string.h>
#include <math.h>
#include <Windows.h>

#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4
#define FALSE 0
#define TRUE 1

//cameraparameter
GLfloat anglex = 0.0, angley = 0 ,anglez = 0, robrot = 0 ,seg1rot = 0, seg0rot = 0;
double rotateangle = 5;
static GLfloat headRotation = 90.0F ;
static GLfloat zoom = 7.0 ;
static GLfloat rotate = 0 ;
int cameraActive = FALSE;

//List of objects
GLuint list;

//current direction of the snake
GLbyte  direction = 0;

//current pos of the snake
int snakePosX = 0;
int snakePosZ = 0;
int snakeBody[100][2] = {{0}};
int snakeLenght = 0;

int moveFinished = TRUE;
int score = 0;

//current pos of the rabbit
int rabbitPosX = 3;
int rabbitPosZ = 2;
GLbyte foundRabbit = TRUE;
double rotationRabbit = 0;
double rabbitJump = 0;
int maxJumpHightTouched = 0;

const int sizeOfGameDesk = 13; //don't use %2
const int lenghtOfGameDesk = 50;

int menuSelect = -1;
int menuSelection = -1;

//Starwars
double scrollSpeed = 5, view = 20;

void drawSnake();
void drawSnakeHead();
void moveSnakePos();
void moveSnakeBody(int oldSnakeX, int oldSnakeZ);
int collisionWithSnakeBody(int x, int z);
void snakeEatsRabbit();
int snakeFinishedRabbit();

void drawRabbit();
void rabbitRotator(int value);

void newGame();
void resetGame();

void drawText(int x, int y, int z, char *string, int green);
void drawMenu();
void drawMouse(int btn, int state, int x, int y);
void moveMouse(int x, int y);
void drawScore();
void drawReadme();

void timeTick(int value);
void timer(int value);
