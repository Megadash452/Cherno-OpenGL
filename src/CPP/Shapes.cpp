#include "Shapes.h"
#include "Renderer.h"
#include <iostream>


Shape2D::Shape2D(float _x, float _y, float _width, float _height, std::vector<float> _pos_verts, std::vector<unsigned int> _vert_inds, colorRGBA<float> _c)
    : x(_x), y(_y), width(_width), height(_height), col(_c),
    num_verts(_pos_verts.size()), num_inds(_vert_inds.size()),
    vert_arr(), layout(),
    pos_verts(&_pos_verts[0]), vert_inds(&_vert_inds[0]),
    vert_buf(pos_verts, num_verts * sizeof(float)), ind_buf(vert_inds, num_inds)
{
    // TODO: maybe not use initializer list??
    std::cout << "Shape2D consturctor";

    ASSERT(_pos_verts.size() % 2 == 0);
    ASSERT(_vert_inds.size() % 3 == 0);

    this->layout.push<float>(2); // 2D - two dimensional
    this->vert_arr.add_buf(this->vert_buf, this->layout);

    this->set_shader(DEFAULT_SHADER_FILE);

}
Shape2D::Shape2D(coord2D loc, float _width, float _height, std::vector<float> _pos_verts, std::vector<unsigned int> _vert_inds, colorRGBA<float> _c)
    : Shape2D{ loc.x, loc.y, _width, _height, _pos_verts, _vert_inds, _c }
{}
Shape2D::Shape2D(coord2D loc, Vect2<float> dimensions, std::vector<float> _pos_verts, std::vector<unsigned int> _vert_inds, colorRGBA<float> _c)
    : Shape2D{ loc.x, loc.y, dimensions.x, dimensions.y, _pos_verts, _vert_inds, _c }
{}

Shape2D::~Shape2D()
{
    // TODO: 
}

void Shape2D::draw()
{

}

void Shape2D::set_shader(const char* shader_path)
{
    // get shader from txt file
    // TODO:
}

void Shape2D::add_uniform(const char* u_name)
{
    GLCALL(int u = glGetUniformLocation(this->shader, u_name));
    ASSERT(u != -1); // if cannot set uniform, will return -1
    this->uniforms.push_back(u);
}

std::vector<int> Shape2D::get_uniforms()
{
    return this->uniforms;
}


VertexTriangle::VertexTriangle(float _x, float _y, float _width, float _height, Vect3<Vect2<float>> verteces, colorRGBA<float> _c)
    : Shape2D{ _x, _y, _width, _height,
        {
            verteces.x.x, verteces.x.y,
            verteces.y.x, verteces.y.y,
            verteces.z.x, verteces.z.y
        },
        { 0, 1, 2 }, _c
    }
{}
VertexTriangle::VertexTriangle(coord2D loc, float _width, float _height, Vect3<Vect2<float>> verteces, colorRGBA<float> _c)
    : VertexTriangle{ loc.x, loc.y, _width, _height, verteces, _c }
{}
VertexTriangle::VertexTriangle(coord2D loc, Vect2<float> dimensions, Vect3<Vect2<float>> verteces, colorRGBA<float> _c)
    : VertexTriangle{ loc.x, loc.y, dimensions.x, dimensions.y, verteces, _c }
{}



void VertexTriangle::draw()
{




    // Draw triangle with LEGACY OpenGL
    //glColor3b(this->col.r, this->col.g, this->col.b);
    //glBegin(GL_TRIANGLES);

    //glVertex2f(this->x, this->y - this->height);              // left corner
    //glVertex2f(this->x + this->base / 2, this->y);            // top corner
    //glVertex2f(this->x + this->base, this->y - this->height); // right corner

    //glEnd();
}

const unsigned int VertexTriangle::vert_indeces[3] = { 0, 1, 2 };