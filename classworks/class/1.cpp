#include <iostream>

using namespace std;

class Point
{
    public:
        double x, y;
        Point()
        {
            x = 0;
            y = 0;
        }
        Point(double _x, double _y)
        {
            x = _x;
            y = _y;
        }
};

class Circle
{
    public:
        Point center;
        double radius;
        Circle()
        {
            radius = 1;
        }
        bool is_point_in_circle(Point a)
        {
            if ((((center.x - a.x) * (center.x - a.x)) + ((center.y - a.y) * (center.y - a.y))) <= radius * radius)
                return true;
            return false;
        }
};

int main()
{
    Point c(1, 0);
    Circle b;
    cout << b.is_point_in_circle(c) << endl;
}