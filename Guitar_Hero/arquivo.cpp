#include "MyMethods.h"
#include "Flames.cpp"
#define PRESSED_BUTTON 2
#define FREE_BUTTON 0
#define GAME_SPEED 10

int main()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Init(IMG_INIT_PNG);
	SDL_Surface *screen = SDL_SetVideoMode(640, 480, 16, SDL_SWSURFACE);
	SDL_Surface *background = SDL_LoadBMP("resources/Background.bmp");
	SDL_Surface *startgameimage = IMG_Load("resources/neckchoosetext.png");
	SDL_Rect destino;
	bool nexttape = false;
	SDL_Event lastevent;

	SDL_FillRect(screen, NULL, 0x0);	
	SDL_BlitSurface(background, NULL, screen, NULL);
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
				nexttape = MyMethods::MouseIsInsideZoom(startgameimage, &destino, &lastevent, 1.3, screen, background);
			}
		}
	}
	SDL_FreeSurface(startgameimage);

//=================================================GAME============================================================================================
	float timecounter = 0;
	nexttape = false;
	int flames_counter[5] = {0,0,0,0,0};
	SDL_UpdateRect(screen, 0,0,0,0);
	SDL_Surface *estera = IMG_Load("resources/estera.png");
	SDL_Surface *buttons = IMG_Load("resources/fretbuttons.png");
	int buttonstate[5]; //usado para controlar os botões que sobem
	buttons = zoomSurface(buttons, 0.5, 0.5, SMOOTHING_ON); //regulando tamanho
	SDL_Rect spritesheet_buttons[3][5]; //[mode][color]
	SDL_Rect buttonsposition[5];
	Flames* flames[5];
	flames[0] = new Flames(COLOR_GREEN);
	flames[1] = new Flames(COLOR_RED);
	flames[2] = new Flames(COLOR_YELLOW);
	flames[3] = new Flames(COLOR_BLUE);
	flames[4] = new Flames(COLOR_ORANGE);

	FilaEncadeada<Gem*> Fila[5];
	Gem* gemGenerator[5]; //cinco ponteiros que alocaam dinamicamente as gem's para que elas sejam colocadas na fila


	for(int i = 0; i < 3; i++)
	{
		for(int j = 0; j<5; j++){//atribui os locais de corte da spritesheet dos botões
			spritesheet_buttons[i][j].w = buttons->w/5;
			spritesheet_buttons[i][j].h = buttons->h/3;
			spritesheet_buttons[i][j].x = (spritesheet_buttons[i][j].w * j + 1);
			spritesheet_buttons[i][j].y = (spritesheet_buttons[i][j].h * i + 1);
		}
	}
	for(int j = 0; j<5; j++){//atribui a posição dos botões na tela e zera o vetor de estado deles
		buttonsposition[j].x = (j == 0) ? 140 : 140 + j*(spritesheet_buttons[0][j].w + 10); //posição horizontal de cada botão espaçados com 10 pixels
		buttonsposition[j].y = 427; // posição vertical dos botôes
		buttonstate[j] = 0; 
	}

	SDL_BlitSurface (estera, NULL, background, NULL); // printando a esteira
	FilaEncadeada<Gem*> GameTrack;
	FilaEncadeada<Gem*> GameField;
	Gem* gAux = new Gem(0, 0);
	Node<Gem*> *nodeAux = NULL;
	bool ok;
	GameTrack.EntraNaFila(gAux, ok);

	gAux = new Gem(2, 100);
	GameTrack.EntraNaFila(gAux, ok);

	gAux = new Gem(3, 200);
	GameTrack.EntraNaFila(gAux, ok);

	gAux = new Gem(4, 300);
	GameTrack.EntraNaFila(gAux, ok);

	gAux = new Gem(1, 400);
	GameTrack.EntraNaFila(gAux, ok);

	gAux = new Gem(2, 150);
	GameTrack.EntraNaFila(gAux, ok);

	gAux = new Gem(3, 250);
	GameTrack.EntraNaFila(gAux, ok);

	gAux = new Gem(4, 350);
	GameTrack.EntraNaFila(gAux, ok);

	gAux = new Gem(1, 450);
	GameTrack.EntraNaFila(gAux, ok);
	while(!nexttape)
	{
		while(!GameTrack.Vazia() && GameTrack.Topo->info->Time <= timecounter)
		{
			GameTrack.SaiDaFila(gAux, ok);
			if(!ok)printf("fodase\n");
			GameField.EntraNaFila(gAux, ok);
			if(!ok)printf("fodase2\n");
		}
		MyMethods::RunGems(&GameField, ok, GAME_SPEED);
		while(SDL_PollEvent(&lastevent)){
			if(lastevent.type == SDL_MOUSEMOTION)  // printf("X: %d, Y: %d\n", lastevent.motion.x, lastevent.motion.y);
			if(lastevent.type == SDL_QUIT){
				nexttape = 1;
			}
			if(lastevent.type == SDL_KEYDOWN || lastevent.type == SDL_KEYUP){
				switch (lastevent.key.keysym.sym){
					case SDLK_a:
						buttonstate[0] = (lastevent.type == SDL_KEYDOWN) ? PRESSED_BUTTON : FREE_BUTTON;
						if(buttonstate[0] == PRESSED_BUTTON){
								ok = MyMethods::GemHit(&GameField, 0);
							if(ok){
								cout<<"cout\n";
								//flames[0]->flamecounter;
								flames[0]->Print(screen);
							}
						}
						break;
					case SDLK_s:
						buttonstate[1] = (lastevent.type == SDL_KEYDOWN) ? PRESSED_BUTTON : FREE_BUTTON;
						if(buttonstate[1] == PRESSED_BUTTON){
							if(!MyMethods::GemHit(&GameField, 1)){
								flames[1]->flamecounter;
							}
						}
						break;
					case SDLK_j:
						buttonstate[2] = (lastevent.type == SDL_KEYDOWN) ? PRESSED_BUTTON : FREE_BUTTON;
						if(buttonstate[2] == PRESSED_BUTTON){
							if(MyMethods::GemHit(&GameField, 2)){
								flames[2]->flamecounter;
							}
						}
						break;
					case SDLK_k:
						buttonstate[3] = (lastevent.type == SDL_KEYDOWN) ? PRESSED_BUTTON : FREE_BUTTON;
						if(buttonstate[3] == PRESSED_BUTTON){
							if(MyMethods::GemHit(&GameField, 3)){
								flames[3]->flamecounter;
							}
						}
						break;
					case SDLK_l:
						buttonstate[4] = (lastevent.type == SDL_KEYDOWN) ? PRESSED_BUTTON : FREE_BUTTON;
						if(buttonstate[4] == PRESSED_BUTTON){
							if(MyMethods::GemHit(&GameField,4)){
								flames[4]->flamecounter;
							}
						}
						break;
					default:
						break;
				}
			}
		}
		timecounter+=GAME_SPEED;
		//preciso fazer um marcador de pontos
		//presico fazer um label mostrando o nome da musica
		//preciso fazer a animação da nota apertada
		//preciso saber como colocar musica

		//desenha na tela
		SDL_BlitSurface(background, NULL, screen, NULL);

		
		//desenha os botoes
		for(int j = 0; j<5; j++){
			SDL_BlitSurface (buttons, &spritesheet_buttons[buttonstate[j]][j], screen, &buttonsposition[j]);
		}

		//desenha as gems
		if(!GameField.Vazia()){
			nodeAux = GameField.Topo;
			while (nodeAux != NULL){
				destino.x = nodeAux->info->Position.x;
				destino.y = nodeAux->info->Position.y;
				//printf("X:%d e Y:%d \n", destino.x, destino.y);
				SDL_BlitSurface(nodeAux->info->image, &nodeAux->info->spritesheet, screen, &destino);
				nodeAux = nodeAux->next;
			}
		}

		//desenah as flames
		for(int i = 0; i < 5; i++){
			flames[i]->Print(screen);
		}

		SDL_UpdateRect(screen, 0,0,0,0);
		if (GameTrack.Vazia() && GameField.Vazia()){
		}
	}
	
	SDL_Quit();
	return 0;
}