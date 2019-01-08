#include <cmath>
#include "edge.h"
using namespace std;

Point::Point (void): X(0), Y(0) {};
Point::Point (double _X, double _Y): X(_X), Y(_Y) {};

bool Point::operator==(Point p)
{
    return (X == p.X) && (Y == p.Y);
}

Point Point::operator+(Point p)
{
    return Point(X + p.X, Y + p.Y);
}

Point Point::operator-(Point p)
{
    return Point(X - p.X, Y - p.Y);
}

Point operator *(double d, Point p)
{
    return Point(d * p.X, d * p.Y);
}

double Point::distance(Point p)
{
    return sqrt((X - p.X)*(X - p.X) + (Y - p.Y)*(Y - p.Y));
}

Point Point::between(Point p)
{
    return 0.5*(*this + p);
}

Point Point::correction(int dir,double corX, double corY, double corR)
{
    if (dir == EXP)
        return Point(X*corR + corX, -Y*corR + corY);
    else
        return Point((X - corX) / corR, (-Y + corY) / corR);
}

double Point::resVal(Point a, Point b)
{
    return abs(this->distance(a)-this->distance(b));
}

Edge::Edge(void): A(0), B(0), C(0) {};

Edge::Edge(double _A, double _B, double _C)
{
    A = _A;
    B = _B;
    C = _C;
    if (B == 0)
    {
        _p0.X = -C / A;
        _p1.X = _p0.X;
        _p0.Y = -sqrt(1 - _p0.X*_p0.X);
        _p1.Y = sqrt(1 - _p0.X*_p0.X);
    }
    else if (A == 0)
    {
        _p0.Y = -C / B;
        _p1.Y = _p0.Y;
        _p0.X = -sqrt(1 - _p0.Y*_p0.Y);
        _p1.X = sqrt(1 - _p0.Y*_p0.Y);
    }
    else
    {
        double a = 1 + A*A / (B*B),
            b = 2 * A*C / (B*B),
            c = C*C / (B*B) - 1,
            d = b*b - 4 * a*c;
        _p0.X = (-b - sqrt(d)) / (2 * a);
        _p0.Y = -A / B*_p0.X - C / B;
        _p1.X = (-b + sqrt(d)) / (2 * a);
        _p1.Y = -A / B*_p1.X - C / B;
    }
}

Edge::Edge(Point p0, Point p1)
{
    _p0 = p0;
    _p1 = p1;
    if (p0 == p1)
    {
        A = 0;
        B = 1;
        C = 0;
    }
    else
    {
        A = p1.Y - p0.Y;
        if (A == 0)
        {
            B = 1;
            C = -p0.Y;
        }
        else
        {
            B = p0.X - p1.X;
            if (B == 0)
            {
                A = 1;
                C = -p0.X;
            }
            else
            {
                C = -(A*p0.X + B*p0.Y);
                double d = p0.distance(p1);
                if (C > 0)
                    d = -d;
                A /= d;
                B /= d;
                C /= d;
            }
        }
    }
}

Edge::Edge(Point p)
{
    Edge Edge0(p, Point());
    A = Edge0.A, B = Edge0.B, C = Edge0.C;
    Edge0 = Edge(A, B, C);
    _p0 = Edge0._p0;
    _p1 = Edge0._p1;
}

Edge Edge::extrapolation(void)
{
    return Edge(A, B, C);
}

Edge Edge::rotate(void)
{
    Point P0 = _p0.between(_p1),
        P1 = _p1 - _p0,
        P2 = Point(P1.Y, -P1.X);
    Edge tmp;
    tmp.A = -B;
    tmp.B = A;
    tmp.C = B*P0.X - A*P0.Y;
    tmp._p0 = P0 - 0.5*P2;
    tmp._p1 = P0 + 0.5*P2;
    return tmp;
}

Point Edge::point(double t)
{
    return Point(_p0 + t*(_p1 - _p0));
}

Point Edge::normalCross(Edge e)
{
    Point tmp;
    Edge e1 = this->rotate(),
        e2 = e.rotate();
    if ((this->_p0.X - e._p1.X)*(e._p0.Y - e._p1.Y) != (e._p0.X - e._p1.X)*(this->_p0.Y - e._p1.Y))
    {
        if (abs(e1.B) <= 0.00001)//_!!!_0.0000000000000001
        {
            tmp.X = -e1.C / e1.A;
            tmp.Y = (e1.C*e2.A - e1.A*e2.C) / (e1.A*e2.B);
        }
        else
        {
            tmp.X = (e1.C*e2.B - e1.B*e2.C) / (e1.B*e2.A - e1.A*e2.B);
            tmp.Y = -(e1.A*tmp.X + e1.C) / e1.B;
        }
    }
    else tmp = Point(1, 0);
    return tmp;
}
Point Edge::essential(Point cen)
{
    Point tmp;
    if (cen.distance(_p0) < cen.distance(_p1))
        tmp = _p0;
    else
        tmp = _p1;
    return tmp;
}
