/* * * * * * * * * * * * * * * * * * * * * * *
 * snake.h                                   *
 * Declaration of snake library for NDSnake  *
 *                                           *
 * 2012 (c) David Barozzini (Marval13)       *
 * * * * * * * * * * * * * * * * * * * * * * */

#include <nds.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define _VERSION_ "0.9.2"

#define MAXLENGHT 100

struct _snake {
	int points [MAXLENGHT][2];
	int last[2];
	int lenght;
	int direction;	// 1 is right, then it's counterclockwise

	u16 color;
};
typedef _snake* snake;

struct _apple {
	int position[2];
	u16 color;
};
typedef _apple* apple;

struct _field {
	int x1, y1;
	int x2, y2;
	u16 color;
};
typedef _field* field;

struct _game {
	int score;
	int level;
	int isOver;
	int lastCommand; // 1 is right, then it's counterclockwise
	int frame;
	int framesBeforeAction;
	int block;
	u16 bgColor;
	snake s;
	apple a;
	field f;
};
typedef _game* game;


/* * * * * * * * * * * * * * *
 * General structs functions *
 * * * * * * * * * * * * * * */

// constructors
game game_create(int level);
snake snake_create();
field field_create();
apple apple_create();

// initializators
void game_init(game g, int level);
void snake_init(snake s, int x0, int y0);
void field_init(field f);
void apple_init(apple a);

// destructors
void game_destroy(game g);
void snake_destroy(snake s);
void field_destroy(field f);
void apple_destroy(apple a);


/* * * * * * * * * * *
 * General functions *
 * * * * * * * * * * */

// output functions
int printXY(int x, int y, u16 color);
int printBlock(game g, int x, int y, u16 color, int thickness);
void clearScreen();


/* * * * * * * * * *
 * Game functions  *
 * * * * * * * * * */

// read a valid command from the plus-pad
void checkCommand(game g);

// general game data updating
void updateGame(game g);

// snake data updating
void updateSnake(game g);

// check if the snake hit a wall, itself or an apple
void checkCollision(game g);

// generate a new apple (relocate the apple)
void newApple(game g);

// if gnam: new apple, snake++, score++, ???, profit!
void getApple(game g);

// output functions
void drawGame(game g);
void drawField(game g);
void drawSnake(game g);
void drawApple(game g);
void printInfo(game g);

/* * * * * * * * * * * * * *
 * User options functions  *
 * * * * * * * * * * * * * */

// print header
void header();

// draw menu and return choice
char menu();

// select level
int diff(int level);

/* * * * * * * * * *
 * Debug functions *
 * * * * * * * * * */

// display some debug variabiles
void debugInfo(game g);