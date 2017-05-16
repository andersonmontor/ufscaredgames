#include <iostream>
#include "FilaEncadeada.h"

using namespace std;

template <typename T>
FilaEncadeada<T>::FilaEncadeada()
{
    //aqui eu construo
    Topo = nullptr;
    Fim = nullptr;
    NumeroDeElementos = 0;

}

template <typename T>
FilaEncadeada<T>::FilaEncadeada(T inicializador)
{
    //aqui eu construo
    Topo = new Node<T>(inicializador); //crio um Nó apontando para null
    Fim = Topo; //meu fim e meu inicio são o unico elemtento existente em mim
    NumeroDeElementos = 1;
}

template <typename T>
void FilaEncadeada<T>::EntraNaFila(T elemento, bool& ok)
{
    if (Cheia()) ok = false; //caso a heap esteja cheia
    else
    {
        Node<T>* aux = new Node<T>();
        
        aux->next = nullptr;
        if(Fim != nullptr) Fim->next = aux;
        Fim = aux;

        if(Vazia()==true)Topo = aux; //caso esteja vazia o elemento que entrou tambem é primeiro

        Fim->info = elemento;
        NumeroDeElementos++;
        ok = true;
    }
}

template <typename T>
void FilaEncadeada<T>::SaiDaFila(T& destino, bool& ok)
{
    //preciso tirar um elemnto de mim e colocá-lo em alguém
    if(Topo == nullptr) cout << "A fila esta vazia idiota"; ok = false;
    destino = Topo->info; //coloquei em alguém

    Node<T>* aux = Topo;
    Topo = Topo->next; //meu topo agora é o lemento anteiror
    delete(aux); // destruí o elemento em questão com uma basuca pirocuda de protons fluorecentes
    NumeroDeElementos--;
    ok = true;
}

template<typename T>
bool FilaEncadeada<T>::Vazia()
{
    if(Topo == nullptr) return true;
    else return false; 
}

template <typename T>
bool FilaEncadeada<T>::Cheia()
{
    //estou cheia ?
    // vou alocar e desalocar dinamicamente para ver se heap esta cheia
    Node<T>* aux = new Node<T>();
    if(aux == nullptr)
    {
        delete(aux);
        return true;
    }
    else
    {
        delete(aux);
        return false;
    }
}

template <typename T>
void FilaEncadeada<T>::Print() //printo caso haja sobrecarga do operador cout<<
{

    Node<T> *aux = Topo;
    while(aux != nullptr)
    {
        cout << aux->info <<"\n"; //printo o elemento
        aux = aux->next;//passo para o proximo
    }
}

template <typename T>
int FilaEncadeada<T>::getNumeroDeElementos()
{
    return NumeroDeElementos;
}

template <typename T>
T* FilaEncadeada<T>::getTopo()
{
    return Topo;
}

template <typename T>
T* FilaEncadeada<T>::getFim()
{
    return Fim;
}
/*
//vou testar essa merda
int main()
{
    FilaEncadeada<char> charsfila;

    bool ok;

    charsfila.EntraNaFila('a', ok);
    charsfila.EntraNaFila('b', ok);
    charsfila.EntraNaFila('c', ok);

    charsfila.Print();

    char algo;
    charsfila.SaiDaFila(algo, ok);

    cout<<algo<<"\n";

    charsfila.Print(); //deu certo birl :3
}*/