#include <stdio.h>
#include <stdlib.h>

char* displayBuffer = NULL;

void DrawGame(unsigned int gameWidth, unsigned int gameHeight) {
	// system("cls");

	for (int y = 1; y < gameHeight-1; y++) {
		for (int x = 1; x < gameWidth-1; x++) {
			int index = y * gameWidth + x;

			if (/*check if snake should be drawn in here*/0) {

			}
			else {
				displayBuffer[index] = ' ';
			}
		}
		displayBuffer[(y + 1) * gameWidth] = '\n';
	}

	printf(displayBuffer);
}

int main(void) {
	unsigned int gameWidth = 120, gameHeight = 40;

	printf("Set the width of the game (default is %i):", gameWidth);
	scanf_s("%u", &gameWidth);
	gameWidth+=3;

	printf("Set the height of the game (default is %i):", gameHeight);
	scanf_s("%u", &gameHeight);
	gameHeight+=2;


	displayBuffer = malloc(gameWidth * gameHeight * sizeof(char));
	for (int i = 0; i < gameWidth * gameHeight * sizeof(char); i++)
		displayBuffer[i] = '*';
	// printf(displayBuffer);


	int i = 10;
	while (i > 0) {
		DrawGame(gameWidth, gameHeight);

		i--;
	}

	free(displayBuffer);

	return 0;
}