#include <iostream>

using namespace std;

class Singleton
{
    private:
        Singleton()
        {
            data = 11;
        }
        Singleton(int value)
        {
            data = value;
        } 
    public:
        int data;
        int Get_data()
        {
            return data;
        }
        Singleton&  Get_instance()
        {
            static Singleton instance;
            return instance;
        }
};

int main()
{
    Singleton a = c.Get_instance();
    Singleton b = c.Get_instance();
    // cout << endl << "Classic Singleton Demo" << endl;
    // cout << "1st getInstance() call: "<< a << endl;
    // cout << "2nd getInstance() call: "<< b << endl;
}