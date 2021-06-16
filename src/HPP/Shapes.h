#pragma once
#include "Structs.h"

#define DEFAULT_SHADER_FILE "res/shaders/basic.shader"
/// NOO, include respective file


struct Shape2D
{
    // might make template to fit other types (int, double, byte)
    // the x, y, w, h Define the square area the shape occupies, but not the shape itself (some exceptions)

    float x, y, width, height;
    colorRGBA<float> col;

    Shape2D(float _x, float _y, float _width, float _height,/* unsigned int num_of_verteces, float pos_verteces[], unsigned int vert_indeces[],*/ colorRGBA<float> _c = { 1.0f, 1.0f, 1.0f, 1.0f });
    Shape2D(coord2D loc, Vect2<float> dimensions,/* unsigned int num_of_verteces, float pos_verteces[], unsigned int vert_indeces[],*/ colorRGBA<float> _c = { 1.0f, 1.0f, 1.0f, 1.0f });
    Shape2D(coord2D loc, float _width, float _height,/* unsigned int num_of_verteces, float pos_verteces[], unsigned int vert_indeces[],*/ colorRGBA<float> _c = { 1.0f, 1.0f, 1.0f, 1.0f });

    virtual void draw(const char* shader_path = DEFAULT_SHADER_FILE);
    //void set_verteces(float verteces[]);
    //void set_vertex(unsigned int vertex_index, float vertex);
    
protected:
    /*VertexBuffer vert_buf;
    IndexBuffer ind_buf;*/


};

struct Triangle : public Shape2D // legacy
{
    float& base = this->width;

    Triangle(coord2D loc, Vect2<float> dimensions, colorRGBA<float> _c = { 1.0f, 1.0f, 1.0f, 1.0f });
    Triangle(coord2D loc, float _width, float _height, colorRGBA<float> _c = { 1.0f, 1.0f, 1.0f, 1.0f });
    Triangle(float _x, float _y, float _width, float _height, colorRGBA<float> _c = { 1.0f, 1.0f, 1.0f, 1.0f });

    void draw(const char* shader_path = DEFAULT_SHADER_FILE) override;
};

struct Square
{

};