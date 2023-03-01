#include <iostream>

using namespace std;

template <class T> class Node
{
    public:
        T data;
        Node* next;
};

template <class T> class List
{
    private:
        Node<T>* head;
        int length;
        Node<T>* Move(int i)
        {
            int j = 0;
            Node<T>* ptr = head;
            while (j < i)
            {
                ptr = ptr->next;
                j++;
            }
            return ptr;
        }
    public:
        List()
        {
            head = nullptr;
            length = 0;
        }
        T operator[](int i)
        {
            Node<T>* ptr = Move(i);
            return ptr->data;
        }
        void Insert(T _data, int i)
        {
            Node<T>* new_node = new Node<T>;
            new_node->data = _data;
            if (i == 0)
            {
                new_node->next = head;
                head = new_node;
            }
            else
            {
                Node<T>* ptr = Move(i - 1);
                Node<T>* ptrnext = ptr->next;
                new_node->next = ptrnext;
                ptr->next = new_node;
            }
            length += 1;
        }
        void Remove(int i)
        {
            if (i == 0)
            {
                Node<T>* ptr = head;
                head = head->next;
                delete ptr;
            }
            else
            {
                Node<T>* ptr = Move(i - 1);
                Node<T>* ptrnext = ptr->next;
                ptr->next = ptrnext->next;
                delete ptrnext;
            }
            length -= 1;
        }
        List<T> operator+(List<T> second)
        {
            List<T> res;
            /*
            Node<T>* ptr = head;
            for (int i = 0; i <= length; i++)
            {
                T _data = ptr->data;
                res.Insert(_data, i);
                ptr = ptr->next;
            }
            */
            for (int i = 0; i < second.length; i++)
            {
                res.Insert(second[i], i);
            }
            return res;
        }
        /*void Sort()
        {
            for (int i = 0; i < length; i++)
            {
                for (int j = i + 1; j < length; j++)
                {
                    if 
                    
                }
            }
        }*/
        void Print()
        {
            Node<T>* ptr = head;
            for (int i = 0; i < length; i++)
            {
                cout << ptr->data << ' ';
                ptr = ptr->next;
            }
            cout << endl;
        }
        /*~List()
        {
            while (length > 0)
                Remove(length - 1);
        }*/
};

int main()
{
    List<int> a, b, c;
    a.Insert(1, 0);
    a.Insert(2, 1);
    a.Insert(3, 2);
    a.Insert(4, 3);
    a.Print();
    b.Insert(a[0], 0);
    b.Insert(6, 1);
    b.Insert(7, 2);
    b.Print();
    c = a + b;
    
    c.Print();
}
