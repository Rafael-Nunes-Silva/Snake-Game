#include <stdio.h>
#include <stdlib.h>

const char BORDER = '*';
const char WHITESPACE = ' ';
const char SNAKE = '#';
const char SNAKE_HEAD = '@';
const char FOOD = '&';

typedef struct vec2 {
	int x, y;
} vec2;

char* displayBuffer = NULL;
vec2* snake = NULL;
int snakeLength = 0;
vec2 food = { 0, 0 };

void DrawGame(int gameWidth, int gameHeight) {
	/*draw borders and whitespace*/
	for (int y = 0; y < gameHeight; y++) {
		for (int x = 0; x < gameWidth - 1; x++) {
			int index = y * gameWidth + x;

			if (y == 0 || x == 0 || y == gameHeight - 1 || x == gameWidth - 2)
				displayBuffer[index] = BORDER;
			else
				displayBuffer[index] = WHITESPACE;
		}
	}

	/*draw snake*/
	for (int s = 0; s < snakeLength; s++)
		displayBuffer[snake[s].y*gameWidth+snake[s].x] = (s==0?SNAKE_HEAD:SNAKE);

	/*draw foor*/
	displayBuffer[food.y * gameWidth + food.x] = FOOD;

	printf(displayBuffer);
}
void DrawControls() {
	printf("%c: Border\n%c: Snake Body\n%c: Snake Head\n%c: Food\n(W) - Up\n(S) - Down\n(A) - Left\n(D) - Right\n(B) - Quit\n", BORDER, SNAKE, SNAKE_HEAD, FOOD);
}

void MoveSnake(vec2 pos) {
	for (int s = snakeLength - 1; s > 0; s--) {
		snake[s].x = snake[s - 1].x;
		snake[s].y = snake[s - 1].y;
	}
	snake[0].x = pos.x;
	snake[0].y = pos.y;
}
void GrowSnake() {
	snakeLength++;
	snake[snakeLength - 1].x = snake[snakeLength - 2].x + (snake[snakeLength - 3].x - snake[snakeLength - 2].x);
	snake[snakeLength - 1].y = snake[snakeLength - 2].y + (snake[snakeLength - 3].y - snake[snakeLength - 2].y);
}

int IsInside(vec2 pos, int gameWidth, int gameHeight) {
	return (pos.x > 0 && pos.x < gameWidth - 2 && pos.y > 0 && pos.y < gameHeight - 1);
}
int IsSnake(vec2 pos) {
	for (int i = 0; i < snakeLength; i++) {
		if (pos.x == snake[i].x && pos.y == snake[i].y)
			return 1;
	}
	return 0;
}

void PlaceFood(int gameWidth, int gameHeight) {
	food.x = rand() % (gameWidth - 3) + 1;
	food.y = rand() % (gameHeight - 2) + 1;

	if (IsSnake(food))
		PlaceFood;
}

void RunGame() {
	int gameWidth = 0, gameHeight = 0;

	printf("Set the width of the game (default is %i):", gameWidth);
	scanf_s("%u", &gameWidth);
	if (gameWidth < 4)
		gameWidth = 120;
	gameWidth += 3; // add 3 more to game width so it can fit 2 borders and a line break

	printf("Set the height of the game (default is %i):", gameHeight);
	scanf_s("%u", &gameHeight);
	if (gameHeight < 4)
		gameHeight = 40;
	gameHeight += 2; // add 2 more to game height so it can fit 2 borders

	int dBufferLength = gameWidth * gameHeight * sizeof(char);

	displayBuffer = malloc(dBufferLength + 1);
	if (!displayBuffer) {
		printf("Couldn't alocate necessary memory for 'displayBuffer'");
		free(displayBuffer);
		return 0;
	}
	displayBuffer[dBufferLength] = '\0';
	for (int j = 1; j <= gameHeight; j++)
		displayBuffer[j * gameWidth - 1] = '\n';

	snake = malloc((gameWidth - 3) * (gameHeight - 2) * sizeof(struct vec2));
	if (!snake) {
		printf("Couldn't alocate necessary memory for 'snake'");
		free(displayBuffer);
		return 0;
	}
	snake[0].x = (gameWidth - 3) / 2;
	snake[0].y = (gameHeight - 2) / 2;
	snake[1].x = snake[0].x + 1;
	snake[1].y = snake[0].y;
	snakeLength = 2;

	food.x = rand() % (gameWidth - 3) + 1;
	food.y = rand() % (gameHeight - 2) + 1;

	int endState = 0; // 1 = win | -1 = lost

	int quit = 0;
	while (!quit) {
		system("cls");
		DrawGame(gameWidth, gameHeight);
		DrawControls();

		char input = ' ';
		scanf_s("%c", &input);

		vec2 newPos = { 0, 0 };
		switch (input) {
		case 'B':
			quit = 1;
			break;
		case 'W':
			newPos.x = snake[0].x;
			newPos.y = snake[0].y - 1;

			quit = (!IsInside(newPos, gameWidth, gameHeight) || IsSnake(newPos) ? 1 : 0);
			endState = -1;
			MoveSnake(newPos);
			break;
		case 'S':
			newPos.x = snake[0].x;
			newPos.y = snake[0].y + 1;

			quit = (!IsInside(newPos, gameWidth, gameHeight) || IsSnake(newPos) ? 1 : 0);
			endState = -1;
			MoveSnake(newPos);
			break;
		case 'A':
			newPos.x = snake[0].x - 1;
			newPos.y = snake[0].y;

			quit = (!IsInside(newPos, gameWidth, gameHeight) || IsSnake(newPos) ? 1 : 0);
			endState = -1;
			MoveSnake(newPos);
			break;
		case 'D':
			newPos.x = snake[0].x + 1;
			newPos.y = snake[0].y;

			quit = (!IsInside(newPos, gameWidth, gameHeight) || IsSnake(newPos) ? 1 : 0);
			endState = -1;
			MoveSnake(newPos);
			break;
		}

		if (snake[0].x == food.x && snake[0].y == food.y) {
			GrowSnake();
			PlaceFood(gameWidth, gameHeight);
		}
	}

	switch (endState) {
	case 1:
		printf("\n***********\n*You Won:)*\n***********\n\n");
		break;
	case -1:
		printf("\n************\n*You Lost:(*\n************\n\n");
		break;
	}

	free(displayBuffer);
	free(snake);

	printf("Play again? (Y/N):\0");
	char again = ' ';
	getchar();
	scanf_s("%c", &again);
	if (again == 'Y') {
		system("cls");
		RunGame();
	}
}

int main(void) {
	int run = 1;

	RunGame();

	return 0;
}