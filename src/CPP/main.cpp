#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Renderer.h"
#include "Shapes.h"
#include "Structs.h"
#include "Buffers.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
using std::string;

#define END { glfwTerminate(); return 0; }
#define END_ERR { glfwTerminate(); return -1; }



struct ShaderSources
{
    string vert, frag;
};

// error handling with glGetError() (legacy way)
static void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}
static bool GLLogCall(const char* file, const char* func, int line)
{
    while (GLenum err = glGetError())
    {
        std::cout << "[OpenGL Error] (code)<" << err << ">\n    @ \""
            << file << "\" > " << func
            << ",\n    @ line " << line << ":\n"
            << err /*get error in word form*/ << std::endl;
        return false;
    }
    return true;
}


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

static unsigned int CompileShader(unsigned int type, const string& _src)
{
    GLCALL(unsigned int shader_id = glCreateShader(type));
    const char* src = _src.c_str();
    GLCALL(glShaderSource(shader_id, 1, &src, nullptr));
    GLCALL(glCompileShader(shader_id));

    // ERROR HANDLING
    int result;                                           // to store the possible error code
    GLCALL(glGetShaderiv(shader_id, GL_COMPILE_STATUS, &result)); // returns 0 if there is an error while COMPILING shader source

    if (result == GL_FALSE)
    {
        int len;
        GLCALL(glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &len));                            // length of error message
        char* msg = (char*)_malloca(len * sizeof(char));                                       // allocate mem for eror message
        GLCALL(glGetShaderInfoLog(shader_id, len, &len, msg));

        std::cout << "Error compiling shader type <";
        std::cout << ( type == GL_VERTEX_SHADER ? "GL_VERTEX_SHADER" : "GL_FRAGMENT_SHADER" ); // put shader type in string form using ternary operator
        std::cout << ">:\n" << msg << std::endl;                                               // print error message

        GLCALL(glDeleteShader(shader_id));
        return 0;
    }

    return shader_id;
}

static unsigned int CreateShader(const string& vertex_shader, const string& fragment_shader)
{
    // create shaders
    GLCALL(unsigned int prgrm = glCreateProgram());
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertex_shader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragment_shader);

    // combine shaders
    GLCALL(glAttachShader(prgrm, vs));
    GLCALL(glAttachShader(prgrm, fs));
    GLCALL(glLinkProgram(prgrm));
    GLCALL(glValidateProgram(prgrm));

    // delete shaders
#   if NDEBUG
        GLCALL(glDetachShader(prgrm, vs)); // remove for debugging
        GLCALL(glDetachShader(prgrm, fs)); // remove for debugging
#   endif
    GLCALL(glDeleteShader(vs));
    GLCALL(glDeleteShader(fs));

    return prgrm;
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


static ShaderSources parse_shader_file(const string& path)
{
    // FILE* file = fopen(path.c_str(), "r"); // c-style open file

    std::ifstream shader_file{path};
    std::stringstream buf[2];

    if (shader_file.is_open())
    {
        std::cout << "Reading file <" << path << "> ... ";

        enum class ShaderType
        {
            NONE = -1, VERTEX, FRAGMENT
        };

        string line;
        ShaderType shader_type = ShaderType::VERTEX; // default type is vertex

        while (getline(shader_file, line))
        {
            // line comment
            if (line[0] == '/' && line[1] == '/')
                continue;

            // shader type identifier
            else if (line.find("#shader") != string::npos)
                if (line.find("vertex") != string::npos)
                    shader_type = ShaderType::VERTEX;
                else if (line.find("fragment") != string::npos)
                    shader_type = ShaderType::FRAGMENT;
                else;

            // line is appended to buffers
            else
                buf[(int)shader_type] << line << '\n';
        }

        std::cout << "Done reading file <" << path << ">\n";
        shader_file.close();
    }
    else 
        std::cout << "Error opening file <" << path << ">\n";

    return { buf[0].str(), buf[1].str() };
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

        float first_pos_Verteces[num_verts] = {
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

        // create vertex array object
        unsigned int first_vao;
        GLCALL(glGenVertexArrays(1, &first_vao));
        GLCALL(glBindVertexArray(first_vao));

        // struct VertexBuffer
        VertexBuffer first_vb{ first_pos_Verteces, num_verts * sizeof(float) };

        GLCALL(glEnableVertexAttribArray(0));
        GLCALL(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, nullptr)); // tell GL the layout of VERTEX BUFFER

        // struct IndexBuffer
        IndexBuffer first_ib{ first_vert_indeces, 6 };

        // get shader from txt file
        ShaderSources src = parse_shader_file(DEFAULT_SHADER_FILE);
        unsigned int basic_shader = CreateShader(src.vert, src.frag);
        GLCALL(glUseProgram(basic_shader)); // do not comment (for defaults)

        // inject an uniform (sep func)
        GLCALL(int u_color = glGetUniformLocation(basic_shader, "u_color"));
        ASSERT(u_color != -1);
        // -- 


        // unbind shaders and buffers
        GLCALL(glBindVertexArray(0));
        GLCALL(glUseProgram(0));
        GLCALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
        GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

        //Triangle t{ 1.0f, 1.0f, 1.0f, 1.0f };
        //Shape2D s{ coord{ -1.0f, 1.0f }, Vect2<float>{ 2.0f, 2.0f }, 4 };

        colorRGBA<float> col{ 0.3f, 0.5f, 1.0f, 1.0f };
        // true is increasing, false is decreasing
        bool r_state = true;
        bool g_state = true;
        bool b_state = true;

        // Loop until the user closes the window
        while (!glfwWindowShouldClose(window))
        {
            // Render here
            glClear(GL_COLOR_BUFFER_BIT);

            // --- draw first shape (square)
            GLCALL(glUseProgram(basic_shader));
            GLCALL(glUniform4f(  // pass white color (vec4) to uniform in fragment shader
                u_color,
                col.r, col.g, col.b, col.a
            ));

            GLCALL(glBindVertexArray(first_vao));
            first_ib.bind();

            GLCALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr)); // draw square with index buffers // nullptr because ibo already bound to gpu

            float dc = 0.004f; // change in color
            // red
            if (r_state) {
                col.r += dc;
                if (col.r > 1.0f)
                    r_state = false;
            }
            else {
                col.r -= dc;
                if (col.r < 0.0f)
                    r_state = true;
            }
            // green
            if (g_state) {
                col.g += dc * 2;
                if (col.g > 1.0f)
                    g_state = false;
            }
            else {
                col.g -= dc * 2;
                if (col.g < 0.0f)
                    g_state = true;
            }
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

        glDeleteProgram(basic_shader);

        // delete heap allocated buffers
    }
    
    END
}