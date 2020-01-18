#include <iostream>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../../public//Shader//ShaderProgram.h"

/* 着色器语言的相关语法在笔记中呈现，此处只记录如何编译和链接着色器 */

const char *vertexShaderSource = "#version 330 core \n"
"layout(location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"out vec3 ourColor;\n"
"void main(){gl_Position = vec4(aPos, 1.0); ourColor = aColor; }";

const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec3 ourColor;\n"
    "uniform vec3 myColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(myColor, 1.0f);\n"
    "}\n\0";

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    GLFWwindow* window = glfwCreateWindow(800, 600, "1-HelloWindows", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    // end init

    // 配置着色器，此节的重点部分
    ShaderProgram shaderProgram;
    shaderProgram.addShader(GL_VERTEX_SHADER, vertexShaderSource, ShaderProgram::SourceType::STRING);
    shaderProgram.addShader(GL_FRAGMENT_SHADER, fragmentShaderSource, ShaderProgram::SourceType::STRING);
    shaderProgram.link();

    // 创建三角形，一个不会更改的三角形的对象使用VAO就可以标识
    unsigned int ID = 1;
    unsigned int VAO;
    glGenVertexArrays(ID, &VAO);
    unsigned int VBO;
    glGenBuffers(ID, &VBO);
    unsigned int EBO;
    glGenBuffers(ID, &EBO);
    
    glBindVertexArray(VAO);
    float vertices2[] = {
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f
    };

    unsigned int indices[] = { // 注意索引从0开始! 
        0, 1, 2, // 第一个三角形
    };
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), vertices2, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    float value[] = { 0.0f, 0.0f, 0.0f };
    //渲染循环
    while (!glfwWindowShouldClose(window))
    {
        // 输入
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shaderProgram.setUniformValue("myColor", value, 3);

        glUseProgram(shaderProgram.getID());

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(ID, &VAO);
    glDeleteBuffers(ID, &VBO);
    glDeleteBuffers(ID, &EBO);
    //end
    glfwTerminate();

    return 0;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}