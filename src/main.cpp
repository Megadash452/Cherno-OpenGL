#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#define END { glfwTerminate(); return 0; }
#define END_ERR { glfwTerminate(); return -1; }

void draw_triangle(float x, float y, float b, float h, GLbyte R = 100, GLbyte G = 100, GLbyte B = 100)
{
    // Draw triangle with LEGACY OpenGL
    glColor3b(R, G, B);
    glBegin(GL_TRIANGLES);

    glVertex2f(x, y - h); // left corner
    glVertex2f(x + b/2, y); // top corner
    glVertex2f(x + b, y - h); // right corner

    glEnd();
}

void draw_square(float x, float y, float s, GLbyte R=100, GLbyte G=100, GLbyte B=100)
{
    glColor3b(R, G, B);
    glBegin(GL_QUADS);

    glVertex2f(x, y); // top-left
    glVertex2f(x + s, y); // top-right
    glVertex2f(x + s, y - s); // bottom-right
    glVertex2f(x, y - s); // bottom-left

    glEnd();
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
            std::cout << "Error initializing Modern OpenGL:\n" << std::endl
                      << glewGetErrorString(in);
        }  
    }
    std::cout << "OpenGL v" << glGetString(GL_VERSION) << std::endl;

    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window))
    {
        // Render here
        glClear(GL_COLOR_BUFFER_BIT);

        draw_triangle(-1, 1, 1, 1);

        draw_square(0, 1, 1);

        // Swap front and back buffers
        glfwSwapBuffers(window);
        // Poll for and process events
        glfwPollEvents();
    }
    
    END
}