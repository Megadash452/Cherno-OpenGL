#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Renderer.h"
#include "Shapes.h"
#include "Buffers.h"
#include "Shader.h"

#include <iostream>
#include <fstream>
#include <string>
using std::string;

#define END { glfwTerminate(); return 0; }
#define END_ERR { glfwTerminate(); return -1; }


void draw_legacy_triangle(float x, float y, float b, float h, GLbyte R=127, GLbyte G=127, GLbyte B=127)
{
    // Draw triangle with LEGACY OpenGL
    glColor3b(R, G, B);
    glBegin(GL_TRIANGLES);

    glVertex2f(x, y - h);     // left corner
    glVertex2f(x + b/2, y);   // top corner
    glVertex2f(x + b, y - h); // right corner

    glEnd();
}


void draw_legacy_square(float x, float y, float s, GLbyte R=127, GLbyte G=127, GLbyte B=127)
{
    glColor3b(R, G, B);
    glBegin(GL_QUADS);

    glVertex2f(x, y);         // top-left
    glVertex2f(x + s, y);     // top-right
    glVertex2f(x + s, y - s); // bottom-right
    glVertex2f(x, y - s);     // bottom-left

    glEnd();
}

string read_txt_file(const char* path)
{
    string str;

    std::ifstream file(path);
    if (file.is_open())
    {
        std::cout << "Reading file <" << path << "> ...\n";

        while (std::getline(file, str))
        {
            std::cout << str << '\n';
        }

        std::cout << "Done reading file <" << path << "> successfully!\n";
        file.close();
    }
    else
        std::cout << "Error opening file <" << path << ">" << std::endl;

    return str;
}



int main(void)
{

    // Initialize the library
    if (!glfwInit())
        return -1;

    // SETUP CORE PROFILE
    // TODO: remove to use latest
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // set OpenGL to version 3._
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // set OpenGL to version _.3
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a windowed mode window and its OpenGL context
    GLFWwindow* window = glfwCreateWindow(640, 480, "OpenGL", NULL, NULL);
    if (!window)
        END_ERR

    // Make the window's context current
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // set framerate with v-sync

    {
        GLenum in = glewInit();
        if (GLEW_OK != in)
        {
            std::cout << "Error initializing Modern OpenGL:\n"
                      << glewGetErrorString(in);
        }  
    }
    std::cout << "OpenGL v" << glGetString(GL_VERSION) << std::endl;

    /// OpenGL Scope
    {
        // -- using static buffer to create square (square top-left style) with index buffers
        // TODO: bad way of doing it
        float x = -1.0f, y = 1.0f,
            width = 1.0f, height = 1.0f;
        const unsigned int num_verts = 8;
        const unsigned int num_inds = 6;

        float first_pos_Verteces[8] = {
            x        , y - height, // index 0
            x + width, y - height, // index 1
            x + width, y         , // index 2
            x        , y           // index 3
        };

        // using INDEX BUFFERS
        unsigned int first_vert_indeces[6] = {
            0, 1, 2, // first triangle
            2, 3, 0  // second triangle
        };           // 2 triangles to make a square

        ASSERT(num_verts % 2 == 0);
        ASSERT(num_inds % 3 == 0);

        VertexArray first_va{  };
        VertexBuffer first_vb{ first_pos_Verteces, 8 * sizeof(float) };

        VertexBuffer_Layout first_layout;
        first_layout.push<float>(2); // 2D - two dimensional
        first_va.add_buf(first_vb, first_layout);

        // struct IndexBuffer
        IndexBuffer first_ib{ first_vert_indeces, 6 };

        // square's color
        colorRGBA<float> first_color{ 0.3f, 0.5f, 1.0f, 1.0f };
        // true is increasing, false is decreasing
        bool r_state = true;
        bool g_state = true;
        bool b_state = true;

        // shader for square
        Shader first_shader{ DEFAULT_SHADER_FILE };
        first_shader.bind();

        // unbind shaders and buffers to create other shapes after
        first_va.unbind();
        first_shader.undind();
        first_vb.unbind();
        first_ib.unbind();
        // -- 


        // square 2
        /*Shape2D s{ { 0.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 0.0f - 1.0f, 0.0f + 1.0f, 0.0f - 1.0f, 0.0f + 1.0f, 0.0f, 0.0f, 0.0f }, { 0, 1, 2, 2, 3, 1 } };*/
        //Triangle t{ 1.0f, 1.0f, 1.0f, 1.0f };
        //Shape2D s{ coord{ -1.0f, 1.0f }, Vect2<float>{ 2.0f, 2.0f }, 4 };


        // Loop until the user closes the window
        while (!glfwWindowShouldClose(window))
        {
            // Render here
            glClear(GL_COLOR_BUFFER_BIT);

            // --- draw first shape (square)
            first_shader.bind();
            first_shader.set_uniform("u_color", first_color);
            first_va.bind();
            first_ib.bind();

            float dc = 0.004f; // change in color
            // red
            if (r_state) {
                first_color.r += dc;
                if (first_color.r > 1.0f)
                    r_state = false;
            }
            else {
                first_color.r -= dc;
                if (first_color.r < 0.0f)
                    r_state = true;
            }
            // green
            if (g_state) {
                first_color.g += dc * 2;
                if (first_color.g > 1.0f)
                    g_state = false;
            }
            else {
                first_color.g -= dc * 2;
                if (first_color.g < 0.0f)
                    g_state = true;
            }

            GLCALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr)); // draw square with index buffers // nullptr because ibo already bound to gpu
            // ---

            //draw_legacy_triangle(-1, 1, 1, 1);
            //t.draw();
            // glDrawArrays(GL_TRIANGLES, 0, 6); // draw triangle with static buffer


            draw_legacy_square(0.0f, 0.0f, 1);


            // Swap front and back buffers
            glfwSwapBuffers(window);
            // Poll for and process events
            glfwPollEvents();
        }
        // delete heap allocated buffers and shaders

    }
    
    END
}