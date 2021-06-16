#include "Shapes.h"
#include "Renderer.h"
#include <iostream>

Shape2D::Shape2D(float _x, float _y, float _width, float _height, colorRGBA<float> _c)
    : x(_x), y(_y), width(_width),height(_height), col(_c)
{
    std::cout << "Shape2D consturctor";

    // TODO: define pos_vertex array (OpenGL style)
    // TODO: define indices_order array



}
Shape2D::Shape2D(coord2D loc, Vect2<float> dimensions, colorRGBA<float> _c)
    : Shape2D{ loc.x, loc.y, dimensions.x, dimensions.y, _c }
{}
Shape2D::Shape2D(coord2D loc, float _width, float _height, colorRGBA<float> _c)
    : Shape2D{ loc.x, loc.y, _width, _height, _c }
{}

void Shape2D::draw(const char* shader_path)
{

}


Triangle::Triangle(coord2D loc, Vect2<float> dimensions, colorRGBA<float> _c)
    : Shape2D{ loc, dimensions, _c }
{}
Triangle::Triangle(coord2D loc, float _width, float _height, colorRGBA<float> _c)
    : Shape2D{ loc, _width, _height, _c }
{}
Triangle::Triangle(float _x, float _y, float _width, float _height, colorRGBA<float> _c)
    : Shape2D{ _x, _y, _width, _height, _c }
{}

void Triangle::draw(const char* shader_path)
{




    // Draw triangle with LEGACY OpenGL
    //glColor3b(this->col.r, this->col.g, this->col.b);
    //glBegin(GL_TRIANGLES);

    //glVertex2f(this->x, this->y - this->height);              // left corner
    //glVertex2f(this->x + this->base / 2, this->y);            // top corner
    //glVertex2f(this->x + this->base, this->y - this->height); // right corner

    //glEnd();
}