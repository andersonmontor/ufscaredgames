#include <SDL/SDL_image.h>
#include <SDL/SDL_rotozoom.h>
#include <SDL/SDL_ttf.h>
#include <stdio.h>
#include "FilaEncadeada.cpp"
#include "Gem.h"
#include "Flames.cpp"
#define INDICATOR_POSITIONX 500
#define INDICATOR_POSITIONY 300
using namespace std;

float modulo(float x){
	if (x < 0) x = -x;
	return x;
}

class MyMethods{
	public:
		static bool MouseIsInside(SDL_Surface*, SDL_Rect*, SDL_Event*);
		static bool MouseIsInsideZoom(SDL_Surface* , SDL_Rect*, SDL_Event*, double, SDL_Surface*, SDL_Surface*);
		static bool PushGem(FilaEncadeada<Gem*> *, Gem *, int);
		static void RunGems(FilaEncadeada<Gem*> *, bool&, float, int& , int& , int&);
		static bool GemHit(FilaEncadeada<Gem*> *, int);
		static void acertou(int&, int&, int&, int&, Flames*);
		static void errou(int&, int&, int&);
		static void DesenhaIndicador(int, int, SDL_Surface*, SDL_Surface*, SDL_Surface*, SDL_Surface* ,SDL_Rect* ,SDL_Surface* ,SDL_Surface*, SDL_Rect*);
		static void drawText(SDL_Surface* , char* ,int, int, int,int, int, int,int, int, int);
};

bool MyMethods::MouseIsInside(SDL_Surface *surface, SDL_Rect* destino, SDL_Event* lastevent)
{

	if(lastevent->type == SDL_MOUSEMOTION){
		return (lastevent->motion.x >= destino->x && lastevent->motion.x <= destino->x + surface->w && lastevent->motion.y >= destino->y && lastevent->motion.y <= destino->y + surface->h);
		//printf("X: %d 	Y: %d\n", lastevent.motion.x, lastevent.motion.y);
		//printf("foi\n");
	}
	else return false;
}

bool MyMethods::MouseIsInsideZoom(SDL_Surface* surface, SDL_Rect* destino, SDL_Event* lastevent, double zoom, SDL_Surface* screen, SDL_Surface* background)
{
	if(MouseIsInside(surface, destino, lastevent)){
		//printf("w: %d\n",surface->w);
		destino->x -= ((surface->w*zoom) - surface->w) /2;
		destino->y -= ((surface->h*zoom) - surface->h) /2;
		surface = zoomSurface(surface, zoom, zoom, SMOOTHING_ON); //dei zoom
		//printf("w: %d\n",surface->w);
		SDL_BlitSurface(background, NULL, screen, NULL);
		SDL_BlitSurface(surface, NULL, screen, destino);
		SDL_UpdateRect(screen, 0,0,0,0);

		while(MouseIsInside(surface, destino, lastevent)){ //espero o mouse sair de cima da imagem
			SDL_PollEvent(lastevent);
			if(lastevent->type == SDL_MOUSEBUTTONDOWN)
			{
				if(lastevent->button.button == SDL_BUTTON_LEFT)
				{
					return true;
				}
			}
		}

		surface = zoomSurface(surface, 1/zoom, 1/zoom, SMOOTHING_ON);
		destino->x += ((surface->w*zoom) - surface->w) /2;
		destino->y += ((surface->h*zoom) - surface->h) /2;
		SDL_BlitSurface(background, NULL, screen, NULL);
		SDL_BlitSurface(surface, NULL, screen, destino);
		SDL_UpdateRect(screen, 0,0,0,0);
		return false;

	}
}

bool MyMethods::PushGem(FilaEncadeada<Gem*> *F, Gem *G, int type){
	
	bool OK ;
	F->EntraNaFila(G, OK);
	if(OK == false){
		printf("Não foi possivel colocar gem na fila\n");
		return false;
	}
	else return true;
}

bool MyMethods::GemHit(FilaEncadeada<Gem*> *F, int color){
	int tolerance = 40;
	int hit_ypos = 448;

	//cout<<"GemHit foi chamado\n";

	if(F->Vazia()){
		return false;
	}
	Node<Gem*> *aux = F->Topo;
	 while (aux != NULL && (aux->info->color != color || aux->info->centro.y >= hit_ypos+tolerance)){ //esta ultima condição não deixa pegar notas que ja foram perdidas
		aux = aux->next;
	}//percorre a lista e acha a gem mais antiga da cor correspondente
	if(aux == NULL){
		return false;
	}
	if(aux->info->color == color){
		if((modulo(aux->info->centro.y - hit_ypos)) <= tolerance){ //confere se o centro da gem esta na área tolerada
			F->DeletaElemento(aux);
			return true;
		}else
			return false;
	}	
	else
		return false;
}

void MyMethods::RunGems(FilaEncadeada<Gem*> *F, bool& OK, float velocidade, int& indicator, int& acertadas, int& Xnotes){
	if(F->Topo != NULL){ //se não estiver vazia
		Node<Gem*> *aux = F->Topo;
		Node<Gem*> *aux2;
		bool deucerto;
		while(aux != NULL){ //percorrer fila até o final
			if (aux->info->Position.y < 480){
				aux->info->Position.x = aux->info->Position.x + (aux->info->vector.x) * velocidade; //incremento o vetor na posição
				aux->info->Position.y = aux->info->Position.y + (aux->info->vector.y) * velocidade;
				aux->info->centro.x = aux->info->Position.x + aux->info->spritesheet.w/2; 
				aux->info->centro.y = aux->info->Position.y + aux->info->spritesheet.h/2;
				aux = aux->next; //próximo elemento da fila
			}
			else{
				aux2 = aux->next;
				F->SaiDaFila(aux->info, deucerto);
				aux = aux2;
				errou(indicator, acertadas, Xnotes);

			}
		}
		OK=true;
	}
	else OK=false;
}

void MyMethods::acertou(int& score, int& acertadas, int& indicator, int& Xnotes, Flames* flame)
{
	if(indicator<60) indicator++;
	acertadas++;
	flame->flamecounter = 0;
	if(acertadas < 30)Xnotes = 0;
	else if(acertadas >= 30 && acertadas < 60)Xnotes = 1;
	else if(acertadas >= 60 && acertadas < 90)Xnotes = 2;
	else if(acertadas >= 90)Xnotes = 3;
	score = (Xnotes != 0) ? score+Xnotes : score+1;
}

void MyMethods::errou(int& indicator, int& acertadas, int& Xnotes){
	if(indicator>=1){
	indicator--;
	acertadas = 0;
	Xnotes = 0;
	}
}

void MyMethods::DesenhaIndicador(	int indicator,
									int Xnotes,
									SDL_Surface* marcadorvermelho,
									SDL_Surface* marcadoramarelo,
									SDL_Surface* marcadorverde,
									SDL_Surface* mult,
									SDL_Rect *mult_spritesheet,
									SDL_Surface* screen,
									SDL_Surface* grademarcador,
									SDL_Rect* destino){
	destino->w = 0;
	destino->h = 0;
	if(indicator < 20){
		destino->x = INDICATOR_POSITIONX;
		destino->y = INDICATOR_POSITIONY+12;
		SDL_BlitSurface(marcadorvermelho, NULL, screen, destino);
	}
	if(indicator >= 20 && indicator < 40){
		destino->x = INDICATOR_POSITIONX+32;
		destino->y = INDICATOR_POSITIONY+12;
		SDL_BlitSurface(marcadoramarelo, NULL, screen, destino);
	}
	if(indicator >= 40){
		destino->x = INDICATOR_POSITIONX+64;
		destino->y = INDICATOR_POSITIONY+12;
		SDL_BlitSurface(marcadorverde, NULL, screen, destino);
	}
	mult_spritesheet->x = 0;
	mult_spritesheet->y = mult_spritesheet->h * Xnotes;
	destino->x = INDICATOR_POSITIONX + 28;
	destino->y = INDICATOR_POSITIONY + 27;
	SDL_BlitSurface(mult, mult_spritesheet, screen, destino);
	destino->y = INDICATOR_POSITIONY;
	destino->x = INDICATOR_POSITIONX;
	SDL_BlitSurface(grademarcador, NULL, screen, destino);

}


// Remember to call TTF_Quit() when done.

void MyMethods::drawText(SDL_Surface* screen, char* string,int size, int x, int y,int fR, int fG, int fB,int bR, int bG, int bB){

TTF_Font* font = TTF_OpenFont("resources/arial.ttf", size);

SDL_Color foregroundColor;
SDL_Color backgroundColor;
foregroundColor.r = fR;
foregroundColor.g = fG;
foregroundColor.b = fB;
backgroundColor.r = bR;
backgroundColor.g = bG;
backgroundColor.b = bB;

SDL_Surface* textSurface = TTF_RenderText_Shaded(font, string, foregroundColor, backgroundColor);
SDL_Rect textLocation;
textLocation.x = x;
textLocation.y = y; 
textLocation.w = 0;
textLocation.h = 0;
SDL_BlitSurface(textSurface, NULL, screen, &textLocation);
SDL_FreeSurface(textSurface);
TTF_CloseFont(font);

}