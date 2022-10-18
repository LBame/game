#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <Windows.h>

#define WIDTH 800
#define HEIGHT 640

#define TEXT_SIZE 10

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

void deInit(int error)
{
	if (renderer != NULL) SDL_DestroyRenderer(renderer);

	if (window != NULL) SDL_DestroyWindow(window);

	IMG_Quit();

	SDL_Quit();

	exit(error);
}

void init()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		printf("Error: %s", SDL_GetError());
		system("pause");
		deInit(1);
	}

	int img;

	if ((img = IMG_Init(IMG_INIT_PNG)) == 0)
	{
		printf("Error: %s", SDL_GetError());
		system("pause");
		deInit(1);
	}

	if (img and IMG_INIT_PNG) printf("PNG +");
	else printf("PNG -");

	window = SDL_CreateWindow("Battleships", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);

	if (window == NULL)
	{
		printf("Error: %s", SDL_GetError());
		system("pause");
		deInit(1);
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	if (renderer == NULL)
	{
		printf("Error: %s", SDL_GetError());
		system("pause");
		deInit(1);
	}
}

SDL_Texture* loadTextureFromFile(const char* filename, SDL_Rect* rect)
{
	SDL_Surface* surface = IMG_Load(filename);

	if (surface == NULL)
	{
		printf("Error: %s - %s", SDL_GetError(), filename);
		system("pause");
		deInit(1);
	}

	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

	*rect = { 0, 0, surface->w, surface->h };

	SDL_FreeSurface(surface);

	return texture;
}

struct Png {
	SDL_Rect desk_rect;
	SDL_Texture* desk_texture = loadTextureFromFile("desk.png", &desk_rect);
	//SDL_Rect desk_rect_dst = { WIDTH / 2 - desk_rect.w / 2, (HEIGHT / 4) * 1 - desk_rect.h, desk_rect.w, desk_rect.h };
};

struct Mouse {
	int x = 0;
	int y = 0;
};

struct Loop {
	bool launched = true;
};

struct Game {
	Png png;
	Mouse mouse;
	Loop loop;
	SDL_Event event;
	const int fps = 60;
};

void hideConsole(bool flag)
{
	HWND hide_console;

	AllocConsole();

	hide_console = FindWindowA("ConsoleWindowClass", NULL);

	ShowWindow(hide_console, !flag);
}


void events(Game& game)
{
	while (SDL_PollEvent(&game.event))
	{
		switch (game.event.type)
		{
		case SDL_QUIT:
			game.loop.launched = false;
			break;
		case SDL_MOUSEMOTION:
			game.mouse.x = game.event.motion.x;
			game.mouse.y = game.event.motion.y;
			break;
		}
	}
}

void destroyTexture(Game& game)
{
	SDL_DestroyTexture(game.png.desk_texture);
}

#undef main
int main()
{
	srand(time(NULL));
	init();
	hideConsole(false);

	Game game;

	while (game.loop.launched)
	{
		events(game);
		SDL_RenderCopy(renderer, game.png.desk_texture, &game.png.desk_rect, NULL);
		SDL_RenderPresent(renderer);
		SDL_Delay(1000 / game.fps);
	}

	destroyTexture(game);

	deInit(0);
}