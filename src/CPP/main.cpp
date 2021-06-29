#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>

#include <ImGUI-1.60/imgui.h>
#include <ImGUI-1.60/imgui_impl_glfw_gl3.h>

#include "Renderer.h"
#include "Shapes.h"

#include <iostream>
#include <fstream>
#include <string>
using std::string;

#define END { glfwTerminate(); return 0; }
#define END_ERR { glfwTerminate(); return -1; }


/// NOT GOING TO WORK
void draw_legacy_triangle(float x, float y, float b, float h, GLbyte R = 127, GLbyte G = 127, GLbyte B = 127)
{
    // Draw triangle with LEGACY OpenGL
    glColor3b(R, G, B);
    glBegin(GL_TRIANGLES);

    glVertex2f(x, y - h);     // left corner
    glVertex2f(x + b / 2, y);   // top corner
    glVertex2f(x + b, y - h); // right corner

    glEnd();
}

/// NOT GOING TO WORK
void draw_legacy_square(float x, float y, float s, GLbyte R = 127, GLbyte G = 127, GLbyte B = 127)
{
    GLCALL(glColor3b(R, G, B));
    GLCALL(glBegin(GL_QUADS););

    GLCALL(glVertex2f(x, y));         // top-left
    GLCALL(glVertex2f(x + s, y));     // top-right
    GLCALL(glVertex2f(x + s, y - s)); // bottom-right
    GLCALL(glVertex2f(x, y - s));     // bottom-left

    GLCALL(glEnd());
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



/// FOR SQUARE
float x = 0.0f, y = 0.0f,
width = 20.0f, height = 20.0f;
colorRGBA<float> color{ 0.3f, 0.5f, 1.0f, 1.0f };
colorRGBA<float> copy_color{ 1.0f, 0.5f, 1.0f, 1.0f };

//void set_x(float _x, float* v)
//{
//    // x = _x;
//    v[0] = _x;
//    v[4] = _x + width;
//    v[8] = _x + width;
//    v[12] = _x;
//}
//void set_y(float _y, float* v)
//{
//    // y = _y;
//    v[1] = _y;
//    v[5] = _y;
//    v[9] = _y + height;
//    v[13] = _y + height;
//}
//void set_width(float _w, float* v)
//{
//    // width = _w;
//    v[4] = x + _w;
//    v[8] = x + _w;
//}
//void set_height(float _h, float* v)
//{
//    // height = _h;
//    v[9] = y + _h;
//    v[13] = y + _h;
//}



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
    Vect2<int> Resolution{ 640, 480 };
    GLFWwindow* window = glfwCreateWindow(Resolution.x, Resolution.y, "OpenGL", NULL, NULL);
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

    // for textures
    GLCALL(glEnable(GL_BLEND));
    GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    /// OpenGL Scope
    {
        // -- using static buffer to create square (square top-left style) with index buffers

        const unsigned int num_verts = 16;
        const unsigned int num_inds = 6;

        // TODO: On_X/Y/H/W_Change() or Set change only the ones that use that component
        // e.g. => set x(float _x) { arr[0] = _x; arr[4] = _x + this->width; ... }
        float first_pos_Verteces[num_verts] = {       // texture coords
            x        , y         , /* index 0 */ 0.0f, 0.0f, // bottom-left
            x + width, y         , /* index 1 */ 1.0f, 0.0f, // bottom-right
            x + width, y + height, /* index 2 */ 1.0f, 1.0f, // top   -right
            x        , y + height, /* index 3 */ 0.0f, 1.0f  // top   -left
        };

        // using INDEX BUFFERS
        unsigned int first_vert_indeces[num_inds] = {
            0, 1, 2, // first triangle
            2, 3, 0  // second triangle
        };           // 2 triangles to make a square

        ASSERT(num_verts % 2 == 0);
        ASSERT(num_inds % 3 == 0);

        // true is increasing, false is decreasing
        bool r_state = true;
        bool g_state = true;
        bool b_state = true;
        bool a_state = true;

        VertexArray first_va{  };
        VertexBuffer first_vb{ first_pos_Verteces, num_verts * sizeof(float) }; // TODO: find how to edit the values after passed to gpu
        VertexBuffer_Layout first_layout{  };

        Shader texture_shader{ "res/shaders/texture.shader" };
        Shader basic_color{ "res/shaders/basic_color.shader" };
        Shader& first_shader = texture_shader; // shader being used -- above are loaded shaders
        first_shader.bind();

        first_layout.push<float>(2); // 2D - two dimensional
        first_layout.push<float>(2); // 2D Texture coords
        first_va.add_buf(first_vb, first_layout);

        // index buffer must be defined after layout for buffers is set
        IndexBuffer first_ib{ first_vert_indeces, num_inds };

        // OPTIONAL - Texture
        Texture first_texture{ "res/textures/heart.png" };
        first_texture.bind();
        first_shader.set_uniform("u_texture", 0);

        glm::mat4 proj_matrix = glm::ortho(0.0f, (float)Resolution.x, (float)Resolution.y, 0.0f, -1.0f, 1.0f); // origin on top-left
        // TODO: write OnWindowResize() to push matrix uniform (u_MVP) to shader every time the window resolution changes
        // move 0 pixels
        glm::mat4 view_matrix = glm::translate(glm::mat4{ 1.0f }, glm::vec3{ 0, 0, 0 }); // view matrix acts as a camera, translates models
        // move 100 pixels down
        ///glm::mat4 modl_matrix = glm::translate(glm::mat4{ 1.0f }, glm::vec3{ 0, 100, 0 });

        // unbind shaders and buffers to create other shapes after
        first_va.unbind();
        first_vb.unbind();
        first_ib.unbind();
        first_shader.unbind();
        // -- 


        // square 2
        /*Shape2D s{ { 0.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 0.0f - 1.0f, 0.0f + 1.0f, 0.0f - 1.0f, 0.0f + 1.0f, 0.0f, 0.0f, 0.0f }, { 0, 1, 2, 2, 3, 1 } };*/
        //Triangle t{ 1.0f, 1.0f, 1.0f, 1.0f };
        //Shape2D s{ coord{ -1.0f, 1.0f }, Vect2<float>{ 2.0f, 2.0f }, 4 };

        Renderer2D renderer{};

        /// IMGUI
        ImGui::CreateContext();
        ImGui_ImplGlfwGL3_Init(window, true);
        ImGui::StyleColorsDark();

        //glm::vec3 view_pos{ 0.0f, 0.0f, 0.0f };
        glm::vec3 first_position{ 0.0f, 0.0f, 0.0f };
        glm::vec3 copy_position{ 100.0f, 0.0f, 0.0f };

        // Loop until the user closes the window
        while (!glfwWindowShouldClose(window))
        {
            renderer.clear();

            /* -- - draw first shape(square)*/ {
                float dc = 0.004f; // change in color
                // red
                //if (r_state) {
                //    color.r += dc;
                //    if (color.r > 1.0f)
                //        r_state = false;
                //}
                //else {
                //    color.r -= dc;
                //    if (color.r < 0.0f)
                //        r_state = true;
                //}
                //// green
                //if (g_state) {
                //    color.g += dc * 2;
                //    if (color.g > 1.0f)
                //        g_state = false;
                //}
                //else {
                //    color.g -= dc * 2;
                //    if (color.g < 0.0f)
                //        g_state = true;
                //}
                //// alpha
                //if (a_state) {
                //    color.a += dc * 0.4f;
                //    if (color.a > 1.0f)
                //        a_state = false;
                //}
                //else {
                //    color.a -= dc * 0.4f;
                //    if (color.a < 0.0f)
                //        a_state = true;
                //}
            }
            // move the object

            

            // TODO: Hints on how to draw multiple objects with independent properties
            /// draw texture on colored rectangle
            {
                glm::mat4 modl_matrix = glm::translate(glm::mat4{ 1.0f }, first_position);
                glm::mat4 mvp = proj_matrix * view_matrix * modl_matrix;

                basic_color.bind();
                basic_color.set_uniform("u_color", color);
                basic_color.set_uniform("u_MVP", mvp);
                renderer.draw(first_va, first_ib, basic_color);

                texture_shader.bind(); // texture goes on top, after color is drawn
                texture_shader.set_uniform("u_MVP", mvp);
                renderer.draw(first_va, first_ib, texture_shader);
            } /// ---

            /// -- copy of first object --
            {
                glm::mat4 modl_matrix = glm::translate(glm::mat4{ 1.0f }, copy_position);
                glm::mat4 mvp = proj_matrix * view_matrix * modl_matrix;

                basic_color.bind();
                basic_color.set_uniform("u_color", copy_color);
                basic_color.set_uniform("u_MVP", mvp);
                renderer.draw(first_va, first_ib, basic_color);

                texture_shader.bind(); // texture goes on top, after color is drawn
                texture_shader.set_uniform("u_MVP", mvp);
                renderer.draw(first_va, first_ib, texture_shader);
            } /// ---


            //draw_legacy_triangle(-1, 1, 1, 1);
            //t.draw();
            // glDrawArrays(GL_TRIANGLES, 0, 6); // draw triangle with static buffer


            //draw_legacy_square(0.0f, 0.0f, 1);



            /// --- IMGUI
            ImGui_ImplGlfwGL3_NewFrame();
            {
                ImGui::Text("Color");
                ImGui::ColorEdit4("RGBA Color", (float*)&color);
                ImGui::SliderFloat("Opacity", &color.w, 0.0f, 1.0f);

                ImGui::Text("Position");
                ImGui::SliderFloat("X", &first_position.x, 0.0f, (float)(Resolution.x - width));
                ImGui::SliderFloat("Y", &first_position.y, 0.0f, (float)(Resolution.y - height));
                ImGui::SliderFloat("Z", &first_position.z, 0.0f, 1.0f);

                ImGui::Text("Copy Position");
                ImGui::SliderFloat("x", &copy_position.x, 0.0f, (float)(Resolution.x - width));
                ImGui::SliderFloat("y", &copy_position.y, 0.0f, (float)(Resolution.y - height));
                ImGui::SliderFloat("z", &copy_position.z, 0.0f, 1.0f);

                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            }
            ImGui::Render();
            ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
            /// ---



            // Swap front and back buffers
            glfwSwapBuffers(window);
            // Poll for and process events
            glfwPollEvents();
        }
        // delete heap allocated buffers and shaders

    }

    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();
    END
}