#include <iostream>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../../public//Shader/ShaderProgram.h"
#include "../../public/stb_image/stb_image.h"

/* 着色器语言的具体写法在笔记中记录，此处只记录调用方式 */

const char *vertexShaderSource = "#version 330 core \n"
"layout(location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"layout (location = 2) in vec2 aTexCoord;\n"
"out vec3 ourColor;\n"
"out vec2 TexCoord;\n"
"void main()\n"
"{\n"
"gl_Position = vec4(aPos, 1.0);\n"
"ourColor = aColor;\n"
"TexCoord = aTexCoord;\n"
"}\n\0";

const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 ourColor;\n"
"in vec2 TexCoord;\n"
"uniform sampler2D texture1;\n"
"uniform sampler2D texture2;\n"
"void main()\n"
"{\n"
"   FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.8);\n"
"}\n\0";

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

GLFWwindow* init();
ShaderProgram makeShaderProgramme();

int main()
{
    /* 初始化 */
    GLFWwindow* window = init();
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        return -1;
    }

    /* 注册图形ID */
    unsigned int ID = 1;

    /* 生成着色器程序*/
    ShaderProgram shaderProgram = makeShaderProgramme();

    /* 添加纹理 */
    int width1, height1, nrChannels1;
    /*
        作用：读取图片时将图片翻转
    */
    stbi_set_flip_vertically_on_load(true);
    /*
        作用：读取文件
        参数：
            1.文件名@const char *
            2.3.4.读取图片的高度、宽度、通道数量（指针传递）@int *
            5.待补充
    */
    unsigned char *data1 = stbi_load("../image/awesomeface.png", &width1, &height1, &nrChannels1, 0);
    unsigned int texture1;

    /* 
        作用：将纹理ID与图形ID绑定
    */
    glGenTextures(ID, &texture1);

    /*
        作用：将纹理ID与2D纹理（缓存绑定）
        补充：此处也可能是3D
    */
    glBindTexture(GL_TEXTURE_2D, texture1);
    /*
        作用：为当前绑定的纹理对象设置环绕、过滤方式
        参数：
            1.纹理缓存@GLenum
            2.配置属性@GLenum
            3.属性值@GLenum
    */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    if (data1 != NULL) {
        /*
            作用：利用缓存中的图像生成纹理
            参数：
                1.纹理缓存@GLenum
                2.指定多级渐远纹理的级别@int
                3.指定纹理的存储方式@GLenum
                4.5.设置纹理的宽度和高度@int
                6.该参数总是被设为0（历史遗留问题）
                7.原图像的存储类型@GLenum
                8.原图像的数据类型@GLenum
                9.原图像数据（这个数据与8相关）
            补充：
                1.这个函数的颜色通道设定不能错误，否则会导致错误
        */
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width1, height1, 0, GL_RGBA, GL_UNSIGNED_BYTE, data1);
        /*
            作用：生成多级纹理
        */
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Failed to load texture1" << std::endl;
    }
    stbi_image_free(data1);

    /* 生成第二个纹理 */
    unsigned int texture2;
    int width2, height2, nrChannels2;
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    unsigned char *data2 = stbi_load("../image/container.jpg", &width2, &height2, &nrChannels2, 0);
    if (data2 != NULL) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width2, height2, 0, GL_RGB, GL_UNSIGNED_BYTE, data2);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data2);

    /* 生成三角形 */
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;

    glGenVertexArrays(ID, &VAO);
    glGenBuffers(ID, &VBO);
    glGenBuffers(ID, &EBO);

    glBindVertexArray(VAO);
    float vertices[] = {
        //---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
        0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 右上
        0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 右下
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // 左上
    };

    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3,
    };

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, 32 * sizeof(float), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    /* 配置纹理单元 */
    glUseProgram(shaderProgram.getID());
    int value1[] = { 0 };
    shaderProgram.setUniformValue("texture1", value1, 1);
    int value2[] = { 1 };
    shaderProgram.setUniformValue("texture2", value2, 1);
    

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        glUseProgram(shaderProgram.getID());
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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

GLFWwindow* init() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "1-HelloWindows", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return NULL;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return NULL;
    }

    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    return window;
}

ShaderProgram makeShaderProgramme() {
    ShaderProgram shaderProgram;
    shaderProgram.addShader(GL_VERTEX_SHADER, vertexShaderSource, ShaderProgram::SourceType::STRING);
    shaderProgram.addShader(GL_FRAGMENT_SHADER, fragmentShaderSource, ShaderProgram::SourceType::STRING);
    shaderProgram.link();
    return shaderProgram;
}