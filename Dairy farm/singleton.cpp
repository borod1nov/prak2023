#include <iostream>

using namespace std;

class Singleton
{
    private:
        Singleton()
        {
            data = 11;
        }
    public:
        int data;
        int Get_data()
        {
            return data;
        }
        void Set_data(int value)
        {
            data = value;
        }
        Singleton*  Get_instance()
        {
            static Singleton instance;
            return &instance;
        }
};

int main()
{
    Singleton* a = Singleton::Get_instance();
    Singleton* b = Singleton::Get_instance();
    // cout << endl << "Classic Singleton Demo" << endl;
    // cout << "1st getInstance() call: "<< a << endl;
    // cout << "2nd getInstance() call: "<< b << endl;
}