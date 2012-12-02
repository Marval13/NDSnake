/* * * * * * * * * * * * * * * * * * * *
 * main.cpp                            *
 * Main module of NDSnake              *
 *                                     *
 * 2012 (c) David Barozzini (Marval13) *
 * * * * * * * * * * * * * * * * * * * */

#include "snake.h"

int main() {
	game g;
	consoleDemoInit();
	videoSetMode(MODE_FB0);
	vramSetBankA(VRAM_A_LCD);
	g = game_create();
	drawGame(g);
	debugInfo(g);

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
	while(1);
	return 0;
}