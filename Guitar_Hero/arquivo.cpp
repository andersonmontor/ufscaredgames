#include "MyMethods.h"
#define PRESSED_BUTTON 2
#define FREE_BUTTON 0

int main()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Init(IMG_INIT_PNG);
	SDL_Surface *screen = SDL_SetVideoMode(640, 480, 16, SDL_SWSURFACE);
	SDL_Surface *backgroung = SDL_LoadBMP("Data/Background.bmp");
	SDL_Surface *startgameimage = IMG_Load("Data/neckchoosetext.png");
	SDL_Rect destino;
	bool nexttape = false;
	SDL_Event lastevent;

	SDL_FillRect(screen, NULL, 0x0);	
	SDL_BlitSurface(backgroung, NULL, screen, NULL);
	destino.x = (screen->w/2) - startgameimage->w/2;
	destino.y = (screen->h/2) - startgameimage->h/2 + 130;
	SDL_BlitSurface(startgameimage, NULL, screen, &destino);
	SDL_UpdateRect(screen, 0,0,0,0);

//=================================================OPENING========================================================================================
	while(!nexttape){
		while(SDL_PollEvent(&lastevent))
		{
			if(lastevent.type == SDL_QUIT) SDL_Quit();
			if(lastevent.type == SDL_MOUSEMOTION){
				nexttape = MyMethods::MouseIsInsideZoom(startgameimage, &destino, &lastevent, 1.3, screen, backgroung);
			}
		}
	}
	SDL_FreeSurface(startgameimage);

//=================================================GAME============================================================================================
	nexttape = false;
	SDL_UpdateRect(screen, 0,0,0,0);
	SDL_Surface *estera = IMG_Load("Made_Data/estera.png");
	SDL_Surface *buttons = IMG_Load("Data/data_downloaded/fretbuttons.png");
	int buttonstate[5]; //usado para controlar os botões que sobem
	buttons = zoomSurface(buttons, 0.5, 0.5, SMOOTHING_ON); //regulando tamanho
	SDL_Rect spritesheet[3][5]; //[mode][color]
	SDL_Rect buttonsposition[5];
	FilaEncadeada<Gem*> Fila[5];
	Gem* gemGenerator[5]; //cinco ponteiros que alocaam dinamicamente as gem's para que elas sejam colocadas na fila


	for(int i = 0; i < 3; i++)
	{
		for(int j = 0; j<5; j++){//atribui os locais de corte da spritesheet dos botões
			spritesheet[i][j].w = buttons->w/5;
			spritesheet[i][j].h = buttons->h/3;
			spritesheet[i][j].x = (spritesheet[i][j].w * j + 1);
			spritesheet[i][j].y = (spritesheet[i][j].h * i + 1);
		}
	}
	for(int j = 0; j<5; j++){//atribui a posição dos botões na tela e zera o vetor de estado deles
		buttonsposition[j].x = (j == 0) ? 140 : 140 + j*(spritesheet[0][j].w + 10); //posição horizontal de cada botão espaçados com 10 pixels
		buttonsposition[j].y = 427; // posição vertical dos botôes
		buttonstate[j] = 0; 
	}

	SDL_BlitSurface (estera, NULL, backgroung, NULL); // printando a esteira
	while(!nexttape)
	{
		while(SDL_PollEvent(&lastevent)){
			if(lastevent.type == SDL_MOUSEMOTION)printf("X: %d, Y: %d\n", lastevent.motion.x, lastevent.motion.y);
			if(lastevent.type == SDL_QUIT) SDL_Quit();
			if(lastevent.type == SDL_KEYDOWN || lastevent.type == SDL_KEYUP){
				switch (lastevent.key.keysym.sym){
					case SDLK_a:
						buttonstate[0] = (lastevent.type == SDL_KEYDOWN) ? PRESSED_BUTTON : FREE_BUTTON;
						break;
					case SDLK_s:
						buttonstate[1] = (lastevent.type == SDL_KEYDOWN) ? PRESSED_BUTTON : FREE_BUTTON;
						break;
					case SDLK_j:
						buttonstate[2] = (lastevent.type == SDL_KEYDOWN) ? PRESSED_BUTTON : FREE_BUTTON;
						break;
					case SDLK_k:
						buttonstate[3] = (lastevent.type == SDL_KEYDOWN) ? PRESSED_BUTTON : FREE_BUTTON;
						break;
					case SDLK_l:
						buttonstate[4] = (lastevent.type == SDL_KEYDOWN) ? PRESSED_BUTTON : FREE_BUTTON;
						break;
					default:
						break;
				}
			}
		}
		//preciso fazer um marcador de pontos
		//presico fazer um label mostrando o nome da musica
		//preciso fazer a animação da nota
		//preciso fazer a animação da nota apertada
		//preciso saber como colocar musica
		SDL_BlitSurface(backgroung, NULL, screen, NULL);
		for(int j = 0; j<5; j++){
			SDL_BlitSurface (buttons, &spritesheet[buttonstate[j]][j], screen, &buttonsposition[j]);
		}
		SDL_UpdateRect(screen, 0,0,0,0);
	}
	return 0;
}


