#include <iostream>

using namespace std;

class Main_singleton
{
    public:
        static Main_singleton& Get_instance()
        {
            static Main_singleton instance;
            return instance;
        }
        void print_address()
        {
            cout << this << endl;
        }
    private:        
            Main_singleton() {};
            Main_singleton(const Main_singleton&) = delete;
            Main_singleton& operator=(const Main_singleton&) = delete;
            ~Main_singleton() {}
};

int main()
{
    Main_singleton& instance1 = Main_singleton::Get_instance();
    instance1.print_address();
}