#include "MyMethods.h"
#define PRESSED_BUTTON 2
#define FREE_BUTTON 0
#define GAME_SPEED 5

int main()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Init(IMG_INIT_PNG);
	SDL_Surface *screen = SDL_SetVideoMode(640, 480, 16, SDL_SWSURFACE);
	SDL_Surface *background = SDL_LoadBMP("Data/Background.bmp");
	SDL_Surface *startgameimage = IMG_Load("Data/neckchoosetext.png");
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
	int flames_counter = -1;
	SDL_UpdateRect(screen, 0,0,0,0);
	SDL_Surface *estera = IMG_Load("Made_Data/estera.png");
	SDL_Surface *buttons = IMG_Load("Data/data_downloaded/fretbuttons.png");
	SDL_Surface *flames = IMG_Load("Data/animacao_chamas.png");
	int buttonstate[5]; //usado para controlar os botões que sobem
	int flames_selector = -1;
	buttons = zoomSurface(buttons, 0.5, 0.5, SMOOTHING_ON); //regulando tamanho
	SDL_Rect spritesheet[3][5]; //[mode][color]
	SDL_Rect buttonsposition[5];
	SDL_Rect flames_spritesheet;
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
							if(MyMethods::GemHit(&GameField, 0)){
								flames_selector = 0;
							}
						}
						break;
					case SDLK_s:
						buttonstate[1] = (lastevent.type == SDL_KEYDOWN) ? PRESSED_BUTTON : FREE_BUTTON;
						if(buttonstate[1] == PRESSED_BUTTON){
							if(MyMethods::GemHit(&GameField, 1)){
								flames_selector = 1;
							}
						}
						break;
					case SDLK_j:
						buttonstate[2] = (lastevent.type == SDL_KEYDOWN) ? PRESSED_BUTTON : FREE_BUTTON;
						if(buttonstate[2] == PRESSED_BUTTON){
							if(MyMethods::GemHit(&GameField, 2)){
								flames_selector = 2;
							}
						}
						break;
					case SDLK_k:
						buttonstate[3] = (lastevent.type == SDL_KEYDOWN) ? PRESSED_BUTTON : FREE_BUTTON;
						if(buttonstate[3] == PRESSED_BUTTON){
							if(MyMethods::GemHit(&GameField, 3)){
								flames_selector = 3;
							}
						}
						break;
					case SDLK_l:
						buttonstate[4] = (lastevent.type == SDL_KEYDOWN) ? PRESSED_BUTTON : FREE_BUTTON;
						if(buttonstate[4] == PRESSED_BUTTON){
							if(MyMethods::GemHit(&GameField,4)){
									flames_selector = 4;
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

		
		for(int j = 0; j<5; j++){
			SDL_BlitSurface (buttons, &spritesheet[buttonstate[j]][j], screen, &buttonsposition[j]);
		}

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
		if(flames_selector != -1){
			if(flames_counter < 14){
				flames_spritesheet.x = 0;
				flames_spritesheet.y = 0;
				flames_spritesheet.w = (flames->w/13)*(flames_counter+1);
				if(flames_counter == 0)flames_spritesheet.w = flames->h;
				SDL_BlitSurface(flames, &flames_spritesheet, screen, &buttonsposition[flames_selector]);
				flames_counter++;
			}
			else flames_selector = -1;
		}

		SDL_UpdateRect(screen, 0,0,0,0);
		if (GameTrack.Vazia() && GameField.Vazia()){
			nexttape = 1;
		}
	}
	
	SDL_Quit();
	return 0;
}