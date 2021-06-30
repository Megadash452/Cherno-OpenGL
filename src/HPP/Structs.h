#pragma once

typedef unsigned char byte;

template<typename number> struct Vect2
{
    number x, y;

    Vect2(number _x, number _y)
        : x(_x), y(_y)
    {}

    Vect2(const Vect2<number>& v)
        : x(v.x), y(v.y)
    {}

    void operator =(const Vect2<number>& v) {
        this->x = v.x;
        this->y = v.y;
    }
};
template<typename number> struct Vect3
{
    number x, y, z;

    Vect3(number _x, number _y, number _z)
        : x(_x), y(_y), z(_z)
    {}

    Vect3(const Vect3<number>& v)
        : x(v.x), y(v.y), z(v.z)
    {}

    void operator =(const Vect2<number>& v) {
        this->x = v.x;
        this->y = v.y;
    }

    void operator =(const Vect3<number>& v) {
        this->x = v.x;
        this->y = v.y;
        this->z = v.z;
    }
};
template<typename number> struct Vect4
{
    number x, y, z, w;

    Vect4(number _x, number _y, number _z, number _w)
        : x(_x), y(_y), z(_z), w(_w)
    {}

    Vect4(const Vect4<number>& v)
        : x(v.x), y(v.y), z(v.z), w(v.w)
    {}

    void operator =(const Vect2<number>& v) {
        this->x = v.x;
        this->y = v.y;
    }

    void operator =(const Vect3<number>& v) {
        this->x = v.x;
        this->y = v.y;
        this->z = v.z;
    }

    void operator =(const Vect4<number>& v) {
        this->x = v.x;
        this->y = v.y;
        this->z = v.z;
        this->w = v.w;
    }
};

template<typename number> struct colorRGB : public Vect3<number>
{
    number& r = this->x;
    number& g = this->y;
    number& b = this->z;

    colorRGB(number _r, number _g, number _b)
        : Vect3<number>{ _r, _g, _b }
    {
        this->r = _r;
        this->g = _g;
        this->b = _b;
    }

    void operator =(const Vect2<number>& v) {
        this->r = v.x;
        this->g = v.y;
    }

    void operator =(const Vect3<number>& v) {
        this->r = v.x;
        this->g = v.y;
        this->b = v.z;
    }

    void operator =(const colorRGB<number>& v) {
        this->r = v.r;
        this->g = v.g;
        this->b = v.b;
    }
};
template<typename number> struct colorRGBA : public Vect4<number>
{
    number& r = this->x;
    number& g = this->y;
    number& b = this->z;
    number& a = this->w;

    colorRGBA(number _r, number _g, number _b, number _a)
        : Vect4<number>{ _r, _g, _b, _a }
    {}
    colorRGBA(const colorRGBA<number>& c)
        : Vect4<number>{ c.r, c.g, c.b, c.a }
    {}

    void operator =(const Vect2<number>& v) {
        this->r = v.x;
        this->g = v.y;
    }

    void operator =(const Vect3<number>& v) {
        this->r = v.x;
        this->g = v.y;
        this->b = v.z;
    }

    void operator =(const Vect4<number>& v) {
        this->r = v.x;
        this->g = v.y;
        this->b = v.z;
        this->a = v.w;
    }

    void operator =(const colorRGBA<number>& v) {
        this->r = v.r;
        this->g = v.g;
        this->b = v.b;
        this->a = v.a;
    }
};

typedef Vect2<float> coord2D;
typedef Vect3<float> coord3D;
//typedef colorRGBA<float> color;

#if DIMENSIONS == 3
    typedef coord3D coord;
#else
#   define DIMENSIONS 2
    typedef coord2D coord;
#endif