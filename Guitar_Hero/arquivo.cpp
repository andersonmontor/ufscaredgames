#include "MyMethods.h"
#include "notes_loader.cpp"
#include <SDL/SDL_mixer.h>
#include <math.h>
#define PRESSED_BUTTON 2
#define FREE_BUTTON 0
#define FRAMERATE_IDEAL 60
#define DELTA_TIME_DESEJADO 17 // (1/60 segundos) * 1000(milisegundos)
#define GAME_SPEED_FIXO 4

int main()
{
	bool acertou;
	Mix_Music *musica = NULL;
	Mix_Chunk *som_errou = NULL, *som_perdeu = NULL;


	float game_speed = GAME_SPEED_FIXO;
	float initial_frametime, last_frametime, total_frametime, FPS;

	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Init(IMG_INIT_PNG);
	SDL_Surface *screen = SDL_SetVideoMode(640, 480, 16, SDL_SWSURFACE);
	SDL_Surface *background = SDL_LoadBMP("resources/Background.bmp");
	SDL_Surface *startgameimage = IMG_Load("resources/neckchoosetext.png");
	SDL_Rect destino;
	bool nexttape = false;
	SDL_Event lastevent;

	Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096);
	musica =  Mix_LoadMUS("resources/Sons/musica2.wav");
	som_errou = Mix_LoadWAV("resources/Sons/errou.wav");
	som_perdeu = Mix_LoadWAV("resources/Sons/failsound.wav");

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
	int score = 0;
	int indicator= 30;
	int acertadas = 0;
	int Xnotas = 0;
	bool ganhou;
	nexttape = false;
	float framecount = 0, sum_fps = 0;
	bool musica_play = false;
	int flames_counter = 0;
	SDL_UpdateRect(screen, 0,0,0,0);
	SDL_Surface *estera = IMG_Load("resources/estera.png");
	SDL_Surface *grademarcador = IMG_Load("resources/coop_rock_top.png");
	grademarcador = zoomSurface(grademarcador, 0.5, 0.5, SMOOTHING_ON);
	SDL_Surface *fundomarcador = IMG_Load("resources/coop_rockmeter.png");
	fundomarcador = zoomSurface(fundomarcador, 0.5, 0.5, SMOOTHING_ON);
	SDL_Surface *marcadorverde = IMG_Load("resources/coop_rock_hi.png");
	marcadorverde = zoomSurface(marcadorverde, 0.5, 0.5, SMOOTHING_ON);
	SDL_Surface *marcadoramarelo = IMG_Load("resources/coop_rock_med.png");
	marcadoramarelo = zoomSurface(marcadoramarelo, 0.5, 0.5, SMOOTHING_ON);
	SDL_Surface *marcadorvermelho = IMG_Load("resources/coop_rock_low.png");
	marcadorvermelho = zoomSurface(marcadorvermelho, 0.5, 0.5, SMOOTHING_ON);
	SDL_Surface *multiplicador = IMG_Load("resources/mult.png");
	multiplicador= zoomSurface(multiplicador, 0.5, 0.5, SMOOTHING_ON);
	SDL_Surface* flames_image = IMG_Load("resources/animacao_chamas.png");
	SDL_Rect mult_spritesheet;
	mult_spritesheet.w = multiplicador->w;
	mult_spritesheet.h = multiplicador->h/8;
	SDL_Surface *buttons = IMG_Load("resources/fretbuttons.png");
	SDL_Surface *image = IMG_Load("resources/notes.png");
	image = zoomSurface(image, 0.38, 0.38, SMOOTHING_ON);
	int buttonstate[5]; //usado para controlar os botões que sobem
	buttons = zoomSurface(buttons, 0.5, 0.5, SMOOTHING_ON); //regulando tamanho
	SDL_Rect spritesheet_buttons[3][5]; //[mode][color]
	SDL_Rect buttonsposition[5];
	Flames* flames[5];
	flames[0] = new Flames(COLOR_GREEN, flames_image);
	flames[1] = new Flames(COLOR_RED, flames_image);
	flames[2] = new Flames(COLOR_YELLOW, flames_image);
	flames[3] = new Flames(COLOR_BLUE, flames_image);
	flames[4] = new Flames(COLOR_ORANGE, flames_image);

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
	destino.x = INDICATOR_POSITIONX;
	destino.y = INDICATOR_POSITIONY;
	SDL_BlitSurface(fundomarcador, NULL, background, &destino);

	FilaEncadeada<Gem*> GameTrack;
	FilaEncadeada<Gem*> GameField;
	Gem* gAux;
	Node<Gem*> *nodeAux = NULL;
	bool ok;

	//Carrega as notas de um arquivo de texto notas.txt
	cout << "Carregando notas...\n";
	if (!load_notes(&GameTrack, image))
		cout << "Um erro ocorreu carregando as notas";

	
	while(!nexttape)
	{
		initial_frametime = SDL_GetTicks();
		while(!GameTrack.Vazia() && GameTrack.Topo->info->Time <= timecounter)
		{
			GameTrack.SaiDaFila(gAux, ok);
			if(!ok)printf("fodase\n");
			GameField.EntraNaFila(gAux, ok);
			if(!ok)printf("fodase2\n");
		}
		MyMethods::RunGems(&GameField, ok, game_speed, indicator, acertadas, Xnotas);
		while(SDL_PollEvent(&lastevent)){
			if(lastevent.type == SDL_MOUSEMOTION)  // printf("X: %d, Y: %d\n", lastevent.motion.x, lastevent.motion.y);
			if(lastevent.type == SDL_QUIT){
				ganhou = true;
				nexttape = true;
			}
			if(lastevent.type == SDL_KEYDOWN || lastevent.type == SDL_KEYUP){
				switch (lastevent.key.keysym.sym){
					case SDLK_ESCAPE:
						nexttape = 1;
						break;						
					case SDLK_a:
						buttonstate[0] = (lastevent.type == SDL_KEYDOWN) ? PRESSED_BUTTON : FREE_BUTTON;
						if(buttonstate[0] == PRESSED_BUTTON){
							if(MyMethods::GemHit(&GameField, 0)){
								MyMethods::acertou(score, acertadas, indicator, Xnotas, flames[0]);
							}
							else{
								MyMethods::errou(indicator, acertadas, Xnotas);
								Mix_PlayChannel(-1, som_errou, 0);
							}
						}
						break;
					case SDLK_s:
						buttonstate[1] = (lastevent.type == SDL_KEYDOWN) ? PRESSED_BUTTON : FREE_BUTTON;
						if(buttonstate[1] == PRESSED_BUTTON){
              
							if(MyMethods::GemHit(&GameField, 1)){
								MyMethods::acertou(score, acertadas, indicator, Xnotas, flames[1]);
							}
							else{
								MyMethods::errou(indicator, acertadas, Xnotas);
								Mix_PlayChannel(-1, som_errou, 0);
							}
						}
						break;
					case SDLK_j:
						buttonstate[2] = (lastevent.type == SDL_KEYDOWN) ? PRESSED_BUTTON : FREE_BUTTON;
						if(buttonstate[2] == PRESSED_BUTTON){
							if(MyMethods::GemHit(&GameField, 2)){
								MyMethods::acertou(score, acertadas, indicator, Xnotas, flames[2]);
							}
							else{
								MyMethods::errou(indicator, acertadas, Xnotas);
								Mix_PlayChannel(-1, som_errou, 0);
							}
						}
						break;
					case SDLK_k:
						buttonstate[3] = (lastevent.type == SDL_KEYDOWN) ? PRESSED_BUTTON : FREE_BUTTON;
						if(buttonstate[3] == PRESSED_BUTTON){
							if(MyMethods::GemHit(&GameField, 3)){
								MyMethods::acertou(score, acertadas, indicator, Xnotas, flames[3]);
							}
							else{
								MyMethods::errou(indicator, acertadas, Xnotas);
								Mix_PlayChannel(-1, som_errou, 0);
							}
						}
						break;
					case SDLK_l:
						buttonstate[4] = (lastevent.type == SDL_KEYDOWN) ? PRESSED_BUTTON : FREE_BUTTON;
						if(buttonstate[4] == PRESSED_BUTTON){
							if(MyMethods::GemHit(&GameField,4)){
								MyMethods::acertou(score, acertadas, indicator, Xnotas, flames[4]);
							}
							else{
								MyMethods::errou(indicator, acertadas, Xnotas);
								Mix_PlayChannel(-1, som_errou, 0);
							}
						}
						break;
					default:
						break;
				}
			}
		}


			

		timecounter+=game_speed;

		//desenha na tela
		SDL_BlitSurface(background, NULL, screen, NULL);

		//desenha o INDICATOR
		MyMethods::DesenhaIndicador(indicator, Xnotas, marcadorvermelho, marcadoramarelo, marcadorverde, multiplicador, &mult_spritesheet, screen, grademarcador, &destino);

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
		printf("indicator: %d\nacertadas: %d\nXnote: %d\ncore: %d\n\n\n",indicator, acertadas, Xnotas, score);
		//desenah as flames
		for(int i = 0; i < 5; i++){
			flames[i]->ParallelPrint(screen);
		}

		SDL_UpdateRect(screen, 0,0,0,0);
		if (GameTrack.Vazia() && GameField.Vazia()){
			nexttape = true;
		}
		last_frametime = SDL_GetTicks();
		total_frametime = (last_frametime - initial_frametime);
		FPS = pow(total_frametime, -1)*1000;
		game_speed = GAME_SPEED_FIXO * (FRAMERATE_IDEAL/FPS);
		sum_fps += FPS;
		framecount++;

		if (!musica_play){
			Mix_PlayMusic(musica, -1);
			musica_play = true;
		}
		//cout << "FPS: " << FPS << " GAME_SPEED: " << game_speed << '\n';
		//cout << "Average FPS: " << (sum_fps/framecount) << '\n';
		//cout << "Track: " << GameTrack.getNumeroDeElementos() << " Field: " << GameField.getNumeroDeElementos() << '\n';
		if(indicator == 0){
			ganhou = false;
			nexttape = true;
		}

	}

	if(ganhou){
		//mensagem de ganhou e mostra a pontuação
		SDL_Surface* you_win = IMG_Load("resources/yourock.png");
		background = SDL_LoadBMP("resources/Background.bmp");
		SDL_BlitSurface(background, NULL, screen, NULL);
		destino.x = (screen->w/2) - you_win->w/2;
		destino.y = (screen->h/2) - you_win->h/2 + 130;
		SDL_BlitSurface(you_win, NULL,background,&destino);
		SDL_UpdateRect(screen, 0, 0, 0, 0);
		SDL_Delay(5000);
	}
	else{
		//mensagem de perdeu
		Mix_HaltMusic();
		Mix_PlayChannel(-1, som_perdeu, 0);
		SDL_Surface* you_lose = IMG_Load("resources/youfailed.png");
		background = SDL_LoadBMP("resources/Background.bmp");
		SDL_BlitSurface(background, NULL, screen, NULL);
		destino.x = (screen->w/2) - you_lose->w/2;
		destino.y = (screen->h/2) - you_lose->h/2 + 130;
		SDL_BlitSurface(you_lose, NULL, screen,&destino);
		SDL_UpdateRect(screen, 0, 0, 0, 0);
		SDL_Delay(5000);
	}

	SDL_Quit();
	return 0;
}