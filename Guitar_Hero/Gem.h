#include <SDL/SDL.h>
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
		Point vector;
		int color;
		Point Position;
		SDL_Rect spritesheet;
		Gem(int, int);
		int Time;
};

Gem :: Gem (int color, int Time){
	image = IMG_Load("Data/data_downloaded/notes.png");
	image = zoomSurface(image, 0.38, 0.38, SMOOTHING_ON);
	spritesheet.w = image->w/5;
	spritesheet.h = image->h/5;
	spritesheet.x = spritesheet.w * color; //color assume {0,1,2,3,4,5} sempre
	spritesheet.y = 0;
	this->initialXY.y = 0;
	vector.y = 	1 ;
	switch (color){
		case COLOR_GREEN:	
			this->initialXY.x = 255-(spritesheet.w/2); // 258 = numero de pixel para a primeira via de gem's
			this->vector.x = (-0.1875);
			break;
		case COLOR_RED:
			this->initialXY.x = 288-(spritesheet.w/2); // 288 = numero de pixel para a segunda via de gem's
			this->vector.x = (-0.0833);
			break;
		case COLOR_YELLOW:
			this->initialXY.x = 318-(spritesheet.w/2); // 318 = numero de pixel para a terceira via de gem's
			this->vector.x = (0.0229); //esse e os demais vetores foi calculado com base nos pixels de onde a gem deve se originar e onde deve se destruir
			break;
		case COLOR_BLUE:
			this->initialXY.x = 348-(spritesheet.w/2); // 348 = numero de pixel para a quarta via de gem's
			this->vector.x = (0.1291);
			break;
		case COLOR_ORANGE:
			this->initialXY.x = 378-(spritesheet.w/2); // 378 = numero de pixel para a quinta via de gem's
			this->vector.x = (0.2354);
			break;
		default:
			break;
	}
	Position.x = initialXY.x;
	Position.y = initialXY.y;
	this->Time = Time;
}