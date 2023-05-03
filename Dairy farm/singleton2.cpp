#include <iostream>
using namespace std;

class Singleton
{
    private:
        Singleton()
        {
          d = 0;
        }
        static Singleton* _instance;

        int d;
    public:
        static Singleton* Get_instance()
        {
          if (_instance == nullptr)
          {

            _instance = new Singleton();
            return _instance;
          }
          else
          {
            return nullptr;
          }
        }

        int Get()
        {
          return d;
        }

        void Set(int _d)
        {
          d = _d;
        }


        void Print()
        {
          cout << "d = " << d << endl;
        }

        ~Singleton()
        {
          delete _instance;
        }
        };

Singleton* Singleton::_instance = nullptr;

int main()
{
    
    Singleton* obj1 = Singleton::Get_instance();

    if (obj1 != nullptr)
    {
      obj1->Set(255);
      obj1->Print(); // d = 255
    }

    // Попытка создания другого экземляра класса Singleton
    Singleton* obj2 = Singleton::Get_instance(); // obj2 = nullptr

    if (obj2 != nullptr)
    {
      obj2->Set(300);
      obj2->Print();
    }
}
