/* * * * * * * * * * * * * * * * * * * * * * * *
 * snake.c                                     *
 * Implementation of snake library for NDSnake *
 *                                             *
 * 2012 (c) David Barozzini (Marval13)         *
 * * * * * * * * * * * * * * * * * * * * * * * */

#include "snake.h"


/* * * * * * * * * * * * * * *
 * General structs functions *
 * * * * * * * * * * * * * * */

//constructors
game game_create(int level) {
	game g = (game) malloc(sizeof(_game));
	g->s = snake_create();
	g->f = field_create();
	g->a = apple_create();
	game_init(g, level);
	return g;
}

snake snake_create() {
	snake s = (snake) malloc(sizeof(_snake));
	snake_init(s, 3, 3);
	return s;
}

field field_create() {
	field f = (field) malloc(sizeof(_field));
	field_init(f);
	return f;
}

apple apple_create() {
	apple a = (apple) malloc(sizeof(_apple));
	apple_init(a);
	return a;
}

// initializators
void game_init(game g, int level) {
	srand(time(NULL));
	g->score = 0;
	g->isOver = 0;
	g->lastCommand = 0;
	g->frame = 0;
	g->level = level;
	switch(level) {
		case 0:
			g->framesBeforeAction = 14;
			break;
		case 1:
			g->framesBeforeAction = 11;
			break;
		case 2:
			g->framesBeforeAction = 8;
			break;
		case 3:
			g->framesBeforeAction = 5;
			break;
		case 4:
			g->framesBeforeAction = 3;
			break;
		default:
			exit(-1); // you should stop breaking my programs
	}
	g->block = 6;
	g->bgColor = RGB15(0, 0, 0);
	field_init(g->f);
	snake_init(g->s, 3, 3);
	apple_init(g->a);
	newApple(g);
	return;
}

void snake_init(snake s, int x0, int y0) {
	s->points[0][0] = x0;
	s->points[0][1] = y0;
	s->last[0] = x0;
	s->last[1] = y0;
	for(int i=1; i<MAXLENGHT; i++) {
		for (int j=0; j<2; j++)
			s->points[i][j] = -1;
	}
	s->direction = 0;
	s->color = RGB15(0, 31, 0);
	s->lenght = 1;
	return;
}

void field_init(field f) {
	f->x1 = 8;
	f->y1 = 10;
	f->x2 = SCREEN_WIDTH - 8 +1;
	f->y2 = SCREEN_HEIGHT - 12 -1;
	f->color = RGB15(31, 31, 31);
	return;
}

void apple_init(apple a) {
	a->position[0] = 8;
	a->position[1] = 8;
	a->color = RGB15(31, 0, 0);
	return;
}

// destructors
void game_destroy(game g) {
	snake_destroy(g->s);
	field_destroy(g->f);
	apple_destroy(g->a);
	free(g);
	return;
}

void snake_destroy(snake s) {
	free(s);
	return;
}

void field_destroy(field f) {
	free(f);
	return;
}

void apple_destroy(apple a) {
	free(a);
	return;
}


/* * * * * * * * * * *
 * General functions *
 * * * * * * * * * * */

// output functions
int printXY(int x, int y, u16 color) {
	if(x<0 || x>=SCREEN_WIDTH || y<0 || y>=SCREEN_HEIGHT)
		return -1;
	VRAM_A[x+SCREEN_WIDTH*y] = color;
	return 0;
}

int printBlock(game g, int x, int y, u16 color, int thickness) {
	int x1, y1;
	int i, j;
	if(x<0 || x>=40 || y<0 || y>=28)
		return -1;
	x1 = g->f->x1 + x*thickness +1;
	y1 = g->f->y1 + y*thickness +1;
	for(i=x1; i<x1+thickness; i++)
		for(j=y1; j<y1+thickness; j++)
			printXY(i, j, color);
	return 0;
}

void clearScreen() {
	int i;
	for(i=0; i<SCREEN_WIDTH*SCREEN_HEIGHT; i++)
		VRAM_A[i] = RGB15(0, 0, 0);
	return;
}


/* * * * * * * * * *
 * Game functions  *
 * * * * * * * * * */

// read a valid command from the +pad
void checkCommand(game g) {
	int held;
	scanKeys();
	held = keysHeld();
	if(held & KEY_RIGHT && g->s->direction != 2)
		g->lastCommand = 0;
	if(held & KEY_UP && g->s->direction != 3)
		g->lastCommand = 1;
	if(held & KEY_LEFT && g->s->direction != 0)
		g->lastCommand = 2;
	if(held & KEY_DOWN && g->s->direction != 1)
		g->lastCommand = 3;
	return;
}

// general game data updating
void updateGame(game g) {
	g->frame++;
	if(g->frame == g->framesBeforeAction) {
		g->frame = 0;
		g->s->direction = g->lastCommand;
		updateSnake(g);
		checkCollision(g);
		// checkApple(g);
	}
	return;
}

// snake data updating
void updateSnake(game g) {
	int i;
	g->s->last[0] = g->s->points[g->s->lenght-1][0];
	g->s->last[1] = g->s->points[g->s->lenght-1][1];
	for(i = g->s->lenght-1; i>0; i--) {
		g->s->points[i][0] = g->s->points[i-1][0];
		g->s->points[i][1] = g->s->points[i-1][1];
	}
	switch(g->s->direction) {
		case 0:	// right
			g->s->points[i][0]++;
			break;
		case 1:	// up
			g->s->points[i][1]--;
			break;
		case 2: // left
			g->s->points[i][0]--;
			break;
		case 3:	// down
			g->s->points[i][1]++;
			break;
		default: // I just don't know what went wrong...
			exit(-1);
	}
	return;
}

// check if the snake hit a wall, itself or an apple
void checkCollision(game g) {
	int head[2] = {g->s->points[0][0], g->s->points[0][1]};
	int i;
	// check food collisions (yes, they are normal collisions)
	if(head[0] == g->a->position[0] &&
	   head[1] == g->a->position[1]) {
		getApple(g);
		return;
	}
	// check x collisions
	if(head[0] < 0 ||
	   head[0] > 39) {
		g->isOver = 1;
		return;
	}
	// check y collisions
	if(head[1] < 0 ||
	   head[1] > 27) {
		g->isOver = 1;
		return;
	}
	// check collisions with yourself
	for (i=1; i<MAXLENGHT && g->s->points[i][0] != -1; i++) {
		if(head[0] == g->s->points[i][0] &&
		   head[1] == g->s->points[i][1]) {
			g->isOver = 1;
			return;
		}
	}
	return;
}

// generate a new apple (relocate the apple)
void newApple(game g) {
	int i;
	g->a->position[0] = rand() % 40;
	g->a->position[1] = rand() % 28;
	for(i=0; i<g->s->lenght; i++)
		if(g->a->position[0] == g->s->points[i][0] &&
		   g->a->position[1] == g->s->points[i][1])
			newApple(g);
	return;
}

// new apple, snake++, score++, ???, profit!
void getApple(game g) {
	newApple(g);
	g->s->lenght++;
	g->score += g->level+1;
	return;
}

// output functions
void drawGame(game g) {
	drawField(g);
	drawSnake(g);
	drawApple(g);
	return;
}

void drawField(game g){
	int i;
	for(i=g->f->x1; i<=g->f->x2; i++) {
		printXY(i, g->f->y1, g->f->color);
		printXY(i, g->f->y2, g->f->color);
	}
	for(i=g->f->y1; i<=g->f->y2; i++) {
		printXY(g->f->x1, i, g->f->color);
		printXY(g->f->x2, i, g->f->color);
	}
	return;
}

void drawSnake(game g) {
	int i;
	printBlock(g, g->s->last[0], g->s->last[1], g->bgColor, g->block);
	for(i=0; g->s->points[i][0]!=-1 && i<MAXLENGHT; i++)
		printBlock(g, g->s->points[i][0], g->s->points[i][1], g->s->color, g->block);
	return;
}

void drawApple(game g) {
	printBlock(g, g->a->position[0], g->a->position[1], g->a->color, g->block);
	return;
}

void printInfo(game g) {
	header();
	printf("Score: %d\n", g->score);
	return;
}


/* * * * * * * * * * * * * *
 * General user functions  *
 * * * * * * * * * * * * * */

// print header
void header() {
	consoleClear();
	printf("NDSnake v%s (by Marval13)\n", _VERSION_);
	return;
}

// draw menu and return choice
char menu() {
	char choice = 0;
	int i;
	char text[3][15] = {
		" New game",
		" Difficulty",
		" Highscores"
	};
	while(1) {

		// check input
		scanKeys();
		if(keysDown() & KEY_UP)
			choice = (choice + 1) % 2;
		if(keysDown() & KEY_DOWN)
			choice = (choice + 1) % 2;
		if(keysDown() & KEY_A)
			return choice;

		// print menu
		header();
		for(i=0; i<2; i++) { // it will be 3 a day...
			if(choice == i)
				printf("->");
			else
				printf("  ");
			printf("%s\n", text[i]);
		}
		swiWaitForVBlank();
	}
}

// select level
int diff(int level) {
	char choice = level;
	int i;
	while(1) {

		// check input
		scanKeys();
		if(keysDown() & KEY_UP)
			choice = (choice + 4) % 5;
		if(keysDown() & KEY_DOWN)
			choice = (choice + 1) % 5;
		if(keysDown() & KEY_A)
			return choice;

		// print menu
		header();
		printf("Choose your level\n");
		for(i=0; i<5; i++) { // it will be 3 a day...
			if(choice == i)
				printf("->");
			else
				printf("  ");
			printf(" %d\n", i+1);
		}
		swiWaitForVBlank();
	}
}


/* * * * * * * * * *
 * Debug functions *
 * * * * * * * * * */

// display some debug variabiles
void debugInfo(game g) {
	consoleClear();
	printf("g->score: %d\n", g->score);
	printf("g->frame: %d\n", g->frame);
	printf("g->framesBeforeAction: %d\n", g->framesBeforeAction);
	printf("g->isOver: %d\n", g->isOver);
	printf("g->lastCommand: %d\n", g->lastCommand);
	printf("\n");
	printf("g->s->direction: %d\n", g->s->direction);
	printf("g->s->lenght: %d\n", g->s->lenght);
	printf("g->s->points[0][0]: %d\n", g->s->points[0][0]);
	printf("g->s->points[0][1]: %d\n", g->s->points[0][1]);
	printf("g->s->points[1][0]: %d\n", g->s->points[1][0]);
	printf("g->s->points[1][1]: %d\n", g->s->points[1][1]);
	printf("\n");
	printf("g->a->position[0]: %d\n", g->a->position[0]);
	printf("g->a->position[1]: %d\n", g->a->position[1]);
}