#pragma once
#include <vector>

#include "Renderer.h"
#include "Structs.h"
#include "Texture.h"

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

    virtual void draw(void(*handler)() = [](){});
    void set_shader(const char* shader_path);
    //void set_verteces(float verteces[]);
    //void set_vertex(unsigned int vertex_index, float vertex);
    
protected:
    const unsigned int num_verts;
    const unsigned int num_inds;

    float* pos_verts;
    unsigned int* vert_inds;

    VertexArray  vert_arr;
    VertexBuffer vert_buf;
    VertexBuffer_Layout layout;
    IndexBuffer  ind_buf;
    Shader shader;

    std::vector<int> uniforms;
};

struct VertexTriangle : public Shape2D // legacy
{
    float& base = this->width;


    VertexTriangle(float _x, float _y, float _width, float _height, Vect3<Vect2<float>> verteces, colorRGBA<float> _c = { 1.0f, 1.0f, 1.0f, 1.0f });
    VertexTriangle(coord2D loc, float _width, float _height, Vect3<Vect2<float>> verteces, colorRGBA<float> _c = { 1.0f, 1.0f, 1.0f, 1.0f });
    VertexTriangle(coord2D loc, Vect2<float> dimensions, Vect3<Vect2<float>> verteces, colorRGBA<float> _c = { 1.0f, 1.0f, 1.0f, 1.0f });

    //void draw(void(*handler)() = []() {}) override;
    static const unsigned int* get_vert_indeces() { return VertexTriangle::vert_indeces; }

private:
    const static unsigned int vert_indeces[3];
};

struct Square
{

};