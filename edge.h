#ifndef EDGE_H
#define EDGE_H

enum { CON, EXP };

struct Point
{
    double X;
    double Y;
    Point (void);
    Point (double, double);
    bool operator==(Point p);
    Point operator+(Point p);
    Point operator-(Point p);
    friend Point operator*(double d, Point p);
    double distance(Point p);
    Point between(Point p);
    Point correction(int dir,double corX, double corY, double corR);
    double resVal(Point a, Point b);
};

struct Edge
{
    double A;
    double B;
    double C;
    Point _p0;
    Point _p1;
    Edge(void);
    Edge(double _A, double _B, double _C);
    Edge(Point p0, Point p1);
    Edge(Point p);
    Edge extrapolation(void);
    Edge rotate(void);
    Point point(double t);
    Point normalCross(Edge e);
    Point essential(Point cen);
};

#endif // EDGE_H

