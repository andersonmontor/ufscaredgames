#include <SDL/SDL.h>
#include "Vector.h"
#include "Point.h"
#define COLOR_GREEN 0
#define COLOR_RED 1
#define COLOR_YELLOW 2
#define COLOR_BLUE 3
#define COLOR_ORANGE 4

class Gem{
	public:
		SDL_Surface *image;
		Point initialXY;
		Vector direction;
		int color;
		Point Position;
		SDL_Rect spritesheet;
		Gem(int);
};

Gem :: Gem (int color){
	image = IMG_Load("Data/data_downloaded/battle_frets.png");
	spritesheet.w = image->w/5;
	spritesheet.h = image->h/5;
	spritesheet.x = spritesheet.w * color; //color assume {0,1,2,3,4,5} sempre
	spritesheet.y = 0;
	this->initialXY.Y = 0;
	direction.Y = 1;
	switch (color){
		case 0:	
			this->initialXY.X = 258-(image->w/2); // 258 = numero de pixel para a primeira via de gem's
			this->direction.X = -138/480;
			break;
		case 1:
			this->initialXY.X = 288-(image->w/2); // 288 = numero de pixel para a segunda via de gem's
			this->direction.X = -4/48;
			break;
		case 2:
			this->initialXY.X = 318-(image->w/2); // 318 = numero de pixel para a terceira via de gem's
			this->direction.X = 11/480; //esse e os demais vetores foi calculado com base nos pixels de onde a gem deve se originar e onde deve se destruir
			break;
		case 3:
			this->initialXY.X = 348-(image->w/2); // 348 = numero de pixel para a quarta via de gem's
			this->direction.X = 62/48;
			break;
		case 4:
			this->initialXY.X = 378-(image->w/2); // 378 = numero de pixel para a quinta via de gem's
			this->direction.X = 113/48;
			break;
		default:
			break;
	}
	Position.X = initialXY.X;
	Position.Y = initialXY.Y;
}