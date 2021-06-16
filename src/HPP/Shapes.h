#pragma once
#include <vector>

#include "Structs.h"
#include "Buffers.h"

#define DEFAULT_SHADER_FILE "res/shaders/basic.shader"
/// NOO, include respective file


struct Shape2D
{
    // might make template to fit other types (int, double, byte)
    // the x, y, w, h Define the square area the shape occupies, but not the shape itself (some exceptions)

    float x, y, width, height;
    colorRGBA<float> col;

    // TODO: do not make copy of vectors
    Shape2D(float _x, float _y, float _width, float _height, std::vector<float> _pos_verts, std::vector<unsigned int> _vert_inds, colorRGBA<float> _c = { 1.0f, 1.0f, 1.0f, 1.0f });
    Shape2D(coord2D loc, float _width, float _height, std::vector<float> _pos_verts, std::vector<unsigned int> _vert_inds, colorRGBA<float> _c = { 1.0f, 1.0f, 1.0f, 1.0f });
    Shape2D(coord2D loc, Vect2<float> dimensions, std::vector<float> _pos_verts, std::vector<unsigned int> _vert_inds, colorRGBA<float> _c = { 1.0f, 1.0f, 1.0f, 1.0f });
    ~Shape2D();

    virtual void draw();
    void set_shader(const char* shader_path);
    void add_uniform(const char* u_name);
    std::vector<int> get_uniforms();
    //void set_verteces(float verteces[]);
    //void set_vertex(unsigned int vertex_index, float vertex);
    
protected:
    const unsigned int num_verts;
    const unsigned int num_inds;

    float* pos_verts;
    unsigned int* vert_inds;

    unsigned int shader;

    VertexBuffer_Layout layout;
    VertexArray  vert_arr;
    VertexBuffer vert_buf;
    IndexBuffer  ind_buf;

    std::vector<int> uniforms;
};

struct Triangle : public Shape2D // legacy
{
    float& base = this->width;


    Triangle(float _x, float _y, float _width, float _height, colorRGBA<float> _c = { 1.0f, 1.0f, 1.0f, 1.0f });
    Triangle(coord2D loc, float _width, float _height, colorRGBA<float> _c = { 1.0f, 1.0f, 1.0f, 1.0f });
    Triangle(coord2D loc, Vect2<float> dimensions, colorRGBA<float> _c = { 1.0f, 1.0f, 1.0f, 1.0f });

    void draw() override;
    static const unsigned int* get_vert_indeces() { return Triangle::vert_indeces; }

private:
    const static unsigned int vert_indeces[3];
};

struct Square
{

};