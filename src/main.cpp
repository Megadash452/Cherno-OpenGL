#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
using std::string;

#define DIMENSIONS 2

#define VERT_DEFAULT_FILE ".vert"
#define FRAG_DEFAULT_FILE ".frag"

#define END { glfwTerminate(); return 0; }
#define END_ERR { glfwTerminate(); return -1; }

#define ASSERT(x) if (!(x)) __debugbreak()
#define GLCALL(x) GLClearError(); x; ASSERT(GLLogCall(__FILE__, #x, __LINE__))
// #x will turn x into a string (const char*) e.g. x = glDraw, #x = "glDraw"


typedef unsigned char byte;

struct ShaderSources
{
    string vert, frag;
};

struct Vect2
{
    double x, y;
};
struct Vect3
{
    double x, y, z;
};
struct colorRGB
{
    byte r, g, b;
};
typedef Vect2 coord2D;
typedef Vect3 coord3D;
typedef colorRGB color;

#if DIMENSIONS == 2
    typedef coord2D coord;
#elif DIMENSIONS == 3
    typedef coord3D coord;
#endif


struct Triangle
{
    // TODO: think of making a SHAPE base class

    float x, y, base, height;
    color col;

    Triangle(coord2D loc, float _base, float _height, color _c = color{ 127, 127, 127 })
        : x(loc.x), y(loc.y), base(_base), height(_height), col(_c)
    {}
    Triangle(float _x, float _y, float _base, float _height, color _c = color{ 127, 127, 127 })
        : x(_x), y(_y), base(_base), height(_height), col(_c)
    {}

    void draw()
    {
        // Draw triangle with LEGACY OpenGL
        glColor3b(this->col.r, this->col.g, this->col.b);
        glBegin(GL_TRIANGLES);

        glVertex2f(this->x, this->y - this->height);              // left corner
        glVertex2f(this->x + this->base / 2, this->y);            // top corner
        glVertex2f(this->x + this->base, this->y - this->height); // right corner

        glEnd();
    }
};

// erro handling with glGetError() (legacy way)
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
        ShaderType shader_type = ShaderType::NONE;

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

    // Create a windowed mode window and its OpenGL context
    GLFWwindow* window = glfwCreateWindow(640, 480, "OpenGL", NULL, NULL);
    if (!window)
        END_ERR

    // Make the window's context current
    glfwMakeContextCurrent(window);

    {
        GLenum in = glewInit();
        if (GLEW_OK != in)
        {
            std::cout << "Error initializing Modern OpenGL:\n"
                      << glewGetErrorString(in);
        }  
    }
    std::cout << "OpenGL v" << glGetString(GL_VERSION) << std::endl;


    // -- using static buffer to draw square (square top-left style) with index buffers
    // TODO: bad way of doing it
    float x = -0.5f, y = 0.5f,
        width = 1.0f, height = 1.0f;

    float pos[8] = {
        x        , y - height, // index 0
        x + width, y - height, // index 1
        x + width, y         , // index 2
        x        , y           // index 3
    };

    unsigned int buf;
    GLCALL(glGenBuffers(1, &buf));
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, buf));
    GLCALL(glBufferData(GL_ARRAY_BUFFER, 4*2 * sizeof(float), pos, GL_STATIC_DRAW)); // the positions are static. use GL_DYNAMIC_DRAW for positions that can change

    GLCALL(glEnableVertexAttribArray(0));
    GLCALL(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, nullptr));

    // using INDEX BUFFERS
    unsigned int indeces[6] = {
        0, 1, 2, // first triangle
        2, 3, 0  // second triangle
    };           // 2 triangles to make a square
    // bind the index buffer to GPU
    unsigned int index_buf_obj;
    GLCALL(glGenBuffers(1, &index_buf_obj));
    GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buf_obj));
    GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indeces, GL_STATIC_DRAW));

    // get shader from txt file
    ShaderSources src = parse_shader_file("res/shaders/basic.shader");

    unsigned int shader = CreateShader(src.vert, src.frag);
    GLCALL(glUseProgram(shader));
    // -- 


    //Triangle t{ -1, 1, 1, 1 , color{ 100, 100, 100 } };

    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window))
    {
        // Render here
        glClear(GL_COLOR_BUFFER_BIT);

        //draw_legacy_triangle(-1, 1, 1, 1);
        //t.draw();
        // glDrawArrays(GL_TRIANGLES, 0, 6); // draw triangle with static buffer

        // LEGACY error checking
        GLCALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr)); // draw square with index buffers // nullptr because ibo already bound to gpu

        //draw_legacy_square(0, 1, 1);


        // Swap front and back buffers
        glfwSwapBuffers(window);
        // Poll for and process events
        glfwPollEvents();
    }

    GLCALL(glDeleteProgram(shader));
    
    END
}