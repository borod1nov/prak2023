#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Cow
{
    private:
        string name;
        bool has_milk;
    public:
        Cow()
        {
            name = "";
            has_milk = true;
        }
        Cow(string _name)
        {
            name = _name;
            has_milk = true;
        }
        void Milk_cow()
        {
            has_milk = false;
        }
        string Name()
        {
            return name;
        }
        bool Has_milk()
        {
            return has_milk;
        }
};

class Milk_can
{
    private:
        bool is_empty;
    public:
        Milk_can()
        {
            is_empty = true;
        }
        void Fill_milk_can()
        {
            is_empty = false;
        }
        bool Is_empty()
        {
            return is_empty;
        }

};

class Milker
{
    public:
        void do_milking(Cow & cow, Milk_can & can)
        {
            if (cow.Has_milk() && can.Is_empty())
            {
                cow.Milk_cow();
                can.Fill_milk_can();
            }
            else if (cow.Has_milk() == false)
            {
                cout << "Failure: Cow doesn't have milk" << endl;
            }
            else
            {
                cout << "Failure: Can is full" << endl;
            }
        }
};

int main()
{
    Cow A("Margo"), A1("Bella");
    Milk_can B, B1;
    Milker C;
    // cout << A.Has_milk() << endl;
    // cout << B.Is_empty() << endl;
    // cout << "~~Milking~~" << endl;
    C.do_milking(A, B);
    // cout << A.Has_milk() << endl;
    // cout << B.Is_empty() << endl;

}
