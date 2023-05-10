#include <iostream>
#include <string>
#include <vector>

using namespace std;

enum Animal_type {Goat_t = 1, Cow_t, Sheep_t};

class Animal
{
    public:
        string name;
        Animal_type type;
        bool has_milk;

        Animal()
        {
            has_milk = true;
        }

        void Milk()
        {
            has_milk = false;
        }
        virtual void Print_info() = 0;
        virtual void Error_message() = 0;
};

class Goat: public Animal
{
    public:
        Goat(string _name = "")
        {
            name = _name;
            type = Goat_t;
        }
        void Print_info()
        {
            cout << "type: goat, name: " << name << ", has milk: " << has_milk << endl;
        }
        void Error_message()
        {
            cout << "Failure: goat " << name << " doesn`t have milk" << endl;
        }
};

class Cow: public Animal
{
    public:
        Cow(string _name = "")
        {
            name = _name;
            type = Cow_t;
        }
        void Print_info()
        {
            cout << "type: cow, name: " << name << ", has milk: " << has_milk << endl;
        }
        void Error_message()
        {
            cout << "Failure: cow " << name << " doesn`t have milk" << endl;
        }
};

class Sheep: public Animal
{
    public:
        Sheep(string _name = "")
        {
            name = _name;
            type = Sheep_t;
        }
        void Print_info()
        {
            cout << "type: sheep, name: " << name << ", has milk: " << has_milk << endl;
        }
        void Error_message()
        {
            cout << "Failure: sheep " << name << " doesn`t have milk" << endl;
        }
};

class Creator
{
    public:
        virtual Animal* Factory_method(string name) = 0;
        static void Kill(Animal* animal)
        {
            delete animal;
        }
};

class Goat_creator: public Creator
{
    public:
        Animal* Factory_method(string name)
        {
            return new Goat(name);
        }
};

class Cow_creator: public Creator
{
    public:
        Animal* Factory_method(string name)
        {
            return new Cow(name);
        }
};

class Sheep_creator: public Creator
{
    public:
        Animal* Factory_method(string name)
        {
            return new Sheep(name);
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
        static Milk_can* Can_maker()
        {
            Milk_can* p = new Milk_can();
            return p;
        }
        static void Can_destroyer(Milk_can* can)
        {
            delete can;
        }
        void Print_info()
        {
            cout << "is empty: " << is_empty << endl;
        }
        void Error_message()
        {
            cout << "Failure: can is full" << endl;
        }

};

class Milker
{
    public:
        void do_milking(Animal* animal, Milk_can* can)
        {
            if (animal->has_milk && can->Is_empty())
            {
                animal->Milk();
                can->Fill_milk_can();
            }
            else if (animal->has_milk == false)
            {
                animal->Error_message();
            }
            else
            {
                can->Error_message();
            }
        }
};

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
        void main()
        {
            Goat_creator gc;
            Cow_creator cc;
            Sheep_creator sc;
            
            vector <Animal*> animals;
            vector <Milk_can*> cans;

            animals.push_back(cc.Factory_method("Margo"));
            animals.push_back(gc.Factory_method("Bella"));

            cans.push_back(Milk_can::Can_maker());
            cans.push_back(Milk_can::Can_maker());
            cans.push_back(Milk_can::Can_maker());
            
            Milker C;

            cout << "~~Milking №1~~" << endl;
            C.do_milking(animals[1], cans[0]);
            animals[1]->Print_info();
            cans[0]->Print_info();
            cout << endl;

            cout << "~~Milking №2~~" << endl;
            C.do_milking(animals[1], cans[1]);
            animals[1]->Print_info();
            cans[1]->Print_info();
            cout << endl;

            cout << "~~Milking №3~~" << endl;
            C.do_milking(animals[0], cans[0]);
            animals[0]->Print_info();
            cans[0]->Print_info();
            cout << endl;

            for (int i = 0; i < animals.size(); i++)
                Creator::Kill(animals[i]);

            for (int i = 0; i < cans.size(); i++)
                Milk_can::Can_destroyer(cans[i]);
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
    instance1.main();
}
