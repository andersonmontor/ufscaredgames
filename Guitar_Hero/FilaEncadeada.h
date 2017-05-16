#include "Node.h"

template <typename T>
class FilaEncadeada
{
    //ola sou uma oilha encadeada
    //olha o que eu faço;

    public:
        FilaEncadeada(); //construtor padrão
        FilaEncadeada(T); //constrututor com elemento ja inserido

        void EntraNaFila(T, bool&); //aqui eu coloco um elemento em mim
        void SaiDaFila(T&, bool&); //aqui eu tiro um elemento meu da fila

        bool Cheia(); //verifico se estou cheia, oque é dificil pois sou encadeada
        bool Vazia(); //verifico se tem algo em mim

        void Print();//caso esses elemntos tenham sobrecarga no operador cout<< eu os imprimo na tela

        int getNumeroDeElementos(); // se algiuem quiser ver quantos elementos eu tenho
        //T* getTopo(); //se alguem quiser saber onde eu começo
        //T* getFim(); //se alguem quiser saber onde eu termino

        Node<T>* Topo; //pra eusaber de onde eu tiro alguém
        Node<T>* Fim; //pra eu saber onde eu coloco alguém
        int NumeroDeElementos; //pra eu saber quantos elementos eu tenho
};
