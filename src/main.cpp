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

typedef unsigned char byte;

struct ShaderSources
{
    string vert, frag;
};

struct Vect2
{
    double x, y;

    Vect2(double _x, double _y)
        : x(_x), y(_y)
    {}
};
struct Vect3
{
    double x, y, z;

    Vect3(double _x, double _y, double _z)
        : x(_x), y(_y), z(_z)
    {}
};
struct colorRGB
{
    byte r, g, b;

    colorRGB(byte _r, byte _g, byte _b)
        : r(_r), g(_g), b(_b)
    {}
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
    unsigned int shader_id = glCreateShader(type);
    const char* src = _src.c_str();
    glShaderSource(shader_id, 1, &src, nullptr);
    glCompileShader(shader_id);

    // ERROR HANDLING
    int result;                                           // to store the possible error code
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &result); // returns 0 if there is an error while COMPILING shader source

    if (result == GL_FALSE)
    {
        int len;
        glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &len);                                    // length of error message
        char* msg = (char*)_malloca(len * sizeof(char));                                       // allocate mem for eror message
        glGetShaderInfoLog(shader_id, len, &len, msg);

        std::cout << "Error compiling shader type <";
        std::cout << ( type == GL_VERTEX_SHADER ? "GL_VERTEX_SHADER" : "GL_FRAGMENT_SHADER" ); // put shader type in string form using ternary operator
        std::cout << ">:\n" << msg << std::endl;                                               // print error message

        glDeleteShader(shader_id);
        return 0;
    }

    return shader_id;
}

static unsigned int CreateShader(const string& vertex_shader, const string& fragment_shader)
{
    // create shaders
    unsigned int prgrm = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertex_shader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragment_shader);

    // combine shaders
    glAttachShader(prgrm, vs);
    glAttachShader(prgrm, fs);
    glLinkProgram(prgrm);
    glValidateProgram(prgrm);

    // delete shaders
#   if NDEBUG
        glDetachShader(prgrm, vs); // remove for debugging
        glDetachShader(prgrm, fs); // remove for debugging
#   endif
    glDeleteShader(vs);
    glDeleteShader(fs);

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
    GLFWwindow* window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
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


    // -- using static buffer to draw triangle (square top-left style)
    // TODO: bad way of doing it
    float x = -0.5f, y = 0.5f,
        base = 1, height = 1;

    float pos[6] = {
        x         , y - height,
        x + base/2, y         ,
        x + base  , y - height
    };

    unsigned int buf;
    glGenBuffers(1, &buf);
    glBindBuffer(GL_ARRAY_BUFFER, buf);
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), pos, GL_STATIC_DRAW); // the positions are static. use GL_DYNAMIC_DRAW for positions that can change

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, nullptr);

    // get shader from txt file
    ShaderSources src = parse_shader_file("res/shaders/basic.shader");

    unsigned int shader = CreateShader(src.vert, src.frag);
    glUseProgram(shader);
    // -- 


    //Triangle t{ -1, 1, 1, 1 , color{ 100, 100, 100 } };

    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window))
    {
        // Render here
        glClear(GL_COLOR_BUFFER_BIT);

        //draw_legacy_triangle(-1, 1, 1, 1);
        //t.draw();
        glDrawArrays(GL_TRIANGLES, 0, 3);

        //draw_legacy_square(0, 1, 1);

        // Swap front and back buffers
        glfwSwapBuffers(window);
        // Poll for and process events
        glfwPollEvents();
    }

    glDeleteProgram(shader);
    
    END
}