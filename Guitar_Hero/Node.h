template <typename T>
class Node{

        public:
            T info;
            Node* next;


            Node(T info, Node* next)
            {
                this->info = info;
                this->next = next;
            }

            Node(T info)
            {
                this->info = info;
                this->next = nullptr;
            }
            Node()
            {
                this->next = nullptr;
            }
    };
