#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <iostream>

// this might want to be GLfloat
typedef float color [4];

struct point5d {
public:
    // Destination of this instruction
    double x;
    double y;
    double z;
    double a;
    double b;

    point5d(const point5d& p) :
        x(p.x), y(p.y), z(p.z), a(p.a), b(p.b) {};

    point5d(double x, double y, double z, double a, double b) :
        x(x), y(y), z(z), a(a), b(b) {};

    point5d() :
        x(0), y(0), z(0), a(0), b(0) {};

    void print(std::ostream& os) const {
        os << "(" << x <<", "<< y <<", "<< z <<", "<< a <<", "<< b << ")";
    }

};

// primitive is the base class for all the shapes
// we might wish to hold in a visualModel.
struct primitive
{
    color c;
    virtual ~primitive() {};
};

// segment is a simple line segment between two points
struct segment : public primitive
{
    point5d from;
    point5d to;
    segment(const point5d& f, const point5d& t) :
        from(f), to(t) {};
};


#endif // GEOMETRY_H
