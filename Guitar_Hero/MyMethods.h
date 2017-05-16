#include <SDL/SDL_image.h>
#include <SDL/SDL_rotozoom.h>
#include <stdio.h>
#include "FilaEncadeada.cpp"
#include "Gem.h"
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
		static void RunGems(FilaEncadeada<Gem*> *, bool&, float);
		static bool GemHit(FilaEncadeada<Gem*> *, int);
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
	int tolerance = 300;
	int hit_ypos = 448;
	bool deucerto;
	Point centro_gem;


	Node<Gem*> *aux = F->Topo;
	while (aux != NULL && aux->info->color != color) //&& !(aux->info->Position->y >= hit_ypos + tolerance)
		aux = aux->next;

	printf("%d, x: %f, y: %f", aux->info->color, aux->info->Position.x, aux->info->Position.y);

	if (aux != NULL){
		centro_gem.x = aux->info->Position.x + 25.0;
		centro_gem.y = aux->info->Position.y + 25.0;
		//printf("%f\n", modulo(centro_gem.y - hit_ypos));
	}
	if (aux != NULL && (modulo(centro_gem.y - hit_ypos) <= tolerance)){
		printf("acertou\n");
		F->SaiDaFila(aux->info, deucerto);
		return true;
	}
	else
		return false;
}

void MyMethods::RunGems(FilaEncadeada<Gem*> *F, bool& OK, float velocidade){
	if(F->Topo != NULL){ //se não estiver vazia
		Node<Gem*> *aux = F->Topo;
		bool deucerto;
		while(aux != NULL){ //percorrer fila até o final
			if (aux->info->Position.y < 480){
				aux->info->Position.x = aux->info->Position.x + (aux->info->vector.x) * velocidade; //incremento o vetor na posição
				aux->info->Position.y = aux->info->Position.y + (aux->info->vector.y) * velocidade;
				aux = aux->next; //próximo elemento da fila
			}
			else{
				F->SaiDaFila(aux->info, deucerto);
			}
		}
		OK=true;
	}
	else OK=false;
}