#include <fstream>
#include <SDL/SDL_image.h>
#include "FilaEncadeada.cpp"

using namespace std;



bool load_notes(FilaEncadeada<Gem*> *F, SDL_Surface *image){

	ifstream file("notas.txt");
	string str;
	string color, time, delimiter;
	int color_int, time_int;
	bool ok;
	Gem* gAux;
	delimiter = ":";
	while(getline(file, str)){
		//cout << str << '\n';
		color = str.substr(0, str.find(delimiter));
		time = str.substr(str.find(delimiter)+1, str.length());
		//cout << color << ':' << time << '\n';
		color_int = stoi(color);
		time_int = stoi(time);
		//cout << "Carregando:" << color_int << ':' << time_int << '\n';

		gAux = new Gem(color_int, time_int, image);
		//cout << sizeof(gAux) << '\n';
		F->EntraNaFila(gAux, ok);
		gAux = NULL;
		if (!ok)
			return false;
	}
	return true;
}