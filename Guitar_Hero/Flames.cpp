#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#define POSITION_Y 220
#define POSITION_X 110
#define OFFSET_X -50



class Flames{
	public:
		SDL_Surface *surface;
		SDL_Rect spritesheet;
		SDL_Rect destino;
		int qual;
		int flamecounter;
		Flames(int);
		void ParallelPrint(SDL_Surface*);
		void Print(SDL_Surface*);
};

Flames :: Flames(int tipo, SDL_Surface* surface){
	this->surface = surface;
	qual = tipo;
	spritesheet.w = surface->w/13;
	spritesheet.h = surface->h;
	spritesheet.x = 0;
	spritesheet.y = 0;

	destino.x = (qual == 0) ? POSITION_X : POSITION_X + qual * (spritesheet.w + OFFSET_X); //posição horizontal de cada flame espaçados com 10 pixels
	destino.y = POSITION_Y;
	destino.w = 0;
	destino.h = 0;
	flamecounter = -1;

}
void Flames::ParallelPrint(SDL_Surface* screen){
	if(flamecounter != -1){
		if(flamecounter < 6){
			spritesheet.x = (flamecounter * spritesheet.w * 2)+1;
			SDL_BlitSurface(surface, &spritesheet, screen, &destino);
			SDL_UpdateRect(screen, 0, 0, 0, 0);
			flamecounter++;
		}
		else{
			flamecounter = -1;
		}
	}
}
void Flames::Print(SDL_Surface* screen){
	for(int i = 0; i < 13; i++){
		spritesheet.x = (i * spritesheet.w)+1;
			SDL_BlitSurface(surface, &spritesheet, screen, NULL);
			SDL_UpdateRect(screen, 0,0,0,0);
			SDL_Delay(30);
	}
	spritesheet.x = 0;
}

/*int main()
{
	Flames* flame = new Flames(0);
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Init(IMG_INIT_PNG);
	SDL_Surface *screen = SDL_SetVideoMode(640, 480, 16, SDL_SWSURFACE);
	flame->Print(screen);
	SDL_Delay(5000);
	delete(screen);
	delete(flame);
	SDL_Quit();
	return 0;
}*/