#include <SDL/SDL_image.h>
#include <SDL/SDL_rotozoom.h>
#include <stdio.h>
#include "FilaEncadeada.cpp"
#include "Gem.h"
using namespace std;

class MyMethods{
	public:
		static bool MouseIsInside(SDL_Surface*, SDL_Rect*, SDL_Event*);
		static bool MouseIsInsideZoom(SDL_Surface* , SDL_Rect*, SDL_Event*, double, SDL_Surface*, SDL_Surface*);
		static bool PullGem(FilaEncadeada<Gem*> *, Gem *);
		static void RunGems(FilaEncadeada<Gem*> *, bool&);
};

bool MyMethods::MouseIsInside(SDL_Surface *surface, SDL_Rect* destino, SDL_Event* lastevent)
{

	if(lastevent->type == SDL_MOUSEMOTION){
		if(lastevent->motion.x >= destino->x && lastevent->motion.x <= destino->x + surface->w && lastevent->motion.y >= destino->y && lastevent->motion.y <= destino->y + surface->h)return true;
		else return false;
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

bool MyMethods::PullGem(FilaEncadeada<Gem*> *F, Gem *G){
	bool OK ;
	F->EntraNaFila(G, OK);
	if(OK == false){
		printf("Não foi possivel colocar gem na fila\n");
		return false;
	}
	else return true;
}

void MyMethods::RunGems(FilaEncadeada<Gem*> *F, bool& OK){
	if(F->getTopo() != NULL){ //se não estiver vazia
		Node<Gem*> aux = F->getTopo();
		while(aux->info != NULL){ //percorrer fila até o final
			aux->info->Position = aux->info->Position + aux->info->direction; //incremento o vetor na posição
			aux = aux->next; //próximo elemento da fila
		}
		OK=true;
	}
	else OK=false;
}