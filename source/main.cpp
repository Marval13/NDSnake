/* * * * * * * * * * * * * * * * * * * *
 * main.cpp                            *
 * Main module of NDSnake              *
 *                                     *
 * 2012 (c) David Barozzini (Marval13) *
 * * * * * * * * * * * * * * * * * * * */

#include "snake.h"

int main() {
	// variables and console stuff
	game g;
	char choice, level = 0;
	consoleDemoInit();
	videoSetMode(MODE_FB0);
	vramSetBankA(VRAM_A_LCD);
	

	// and here we go, game loop!
	while(1) {
		choice = menu();
		switch(choice) {
			case 0: // new game
				clearScreen();
				g = game_create(level);
				drawGame(g);
				//debugInfo(g);

				// debug start
				while(!(keysHeld() & KEY_A))
					scanKeys();

				while(!g->isOver) {
					drawGame(g);
					checkCommand(g);
					updateGame(g);
					debugInfo(g);

					swiWaitForVBlank();
				}
				game_destroy(g);
				break;
			case 1:	// difficulty
				level = diff(level);
				break;
			case 2:	// highscores
				break;
			default:
				// I don't know why you are here, but you really shouldn't 
				exit(-1);
		}
	}
	while(1);
	return 0;
}