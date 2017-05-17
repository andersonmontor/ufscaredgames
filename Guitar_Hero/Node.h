template <typename T>
class Node{

    public:
        T info;
        Node* next;
        Node(T, Node*);
        Node(T);
        Node();
};

template <typename T>
Node<T>::Node(T info, Node* next)
{
    this->info = info;
    this->next = next;
}

template <typename T>
Node<T>::Node(T info)
{
    this->info = info;
    this->next = nullptr;
}

template <typename T>
Node<T>::Node()
{
    this->next = nullptr;
}

