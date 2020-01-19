#include <iostream>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Triangle.h"
#include "../../public/Point/Point.h"

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void createTriangle(unsigned int VAO, unsigned int VBO, float * vertices);

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

    // 配置着色器，此处不对着色器进行过多记录，详情在下一节中
    // vertex shader
    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // fragment shader
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // link shaders
    int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // 创建三角形，一个不会更改的三角形的对象使用VAO就可以标识
    unsigned int ID = 1;

    unsigned int VAO;
    /*
        作用：生成这一ID对应的VAO
        参数：
            1.ID——unsigned int
            2.VAO——unsigned int
    */
    glGenVertexArrays(ID, &VAO);

    /*
        作用：生成并绑定一个指定的object
    */
    unsigned int VBO;
    glGenBuffers(ID, &VBO);
    
    float *vertices = new float[9];
    Triangle triangle(
        Point(-0.5f, -0.5f, 0.0f),
        Point(0.5f, -0.5f, 0.0f),
        Point(0.0f, 0.5f, 0.0f)
    );
    triangle.formatVertices(vertices);
    //此处不使用索引
    //createTriangle(VAO, VBO, vertices);
    

    //此处使用索引
    
    glBindVertexArray(VAO);
    float vertices2[] = {
    0.9f, 0.9f, 0.0f,   // 右上角
    0.9f, 0.6f, 0.0f,  // 右下角
    -0.9f, 0.6f, 0.0f, // 左下角
    -0.9f, 0.9f, 0.0f   // 左上角
    };

    unsigned int indices[] = { // 注意索引从0开始! 
        0, 1, 3, // 第一个三角形
        1, 2, 3  // 第二个三角形
    };
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), vertices2, GL_STATIC_DRAW);
    

    unsigned int EBO;
    glGenBuffers(ID, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(0);

    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    /*
        注意：此处这三者的解绑顺序很重要，VAO解绑前可以选择解绑或者不解绑VBO（这是因为VAO的glVertexAttribPointer函数隐形地绑定了VAO和VBO），但是EBO必须是未解绑的状态，否则必须要后面手动地再次绑定EBO
    */
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    //渲染循环
    while (!glfwWindowShouldClose(window))
    {
        // 输入
        processInput(window);

        /*
            作用：重置buffer的颜色
            参数：
                1.2.3.4.颜色——float
            补充：
                1.两个函数通常一起使用，第二个函数表示flush
        */
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        /*
            作用：选择着色程序
            参数：
                着色器程序——int
        */
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        /*
            作用：根据EBO中记录的顺序，VAO中记录的解释方式，VBO中记录的顶点进行绘制
            要求：绑定EBO，绑定VAO
            参数：
                1.需要绘制的图形
                2.需要绘制的顶点数量
                3.EBO的数据类型
                4.EBO中的偏移量
            补充：
                1.这个函数拥有多态实现，也可以不绑定buffer，选择去传索引数组
        */
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    /*
        作用：删除VAO与相应ID的绑定
    */
    glDeleteVertexArrays(ID, &VAO);
    /*
        作用：删除object与相应ID的绑定
    */
    glDeleteBuffers(ID, &VBO);

    //end
    delete vertices;
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

void createTriangle(unsigned int VAO, unsigned int VBO, float *vertices){
    /*
        作用：绑定VAO
        参数：
            1.VAO——unsigned int
    */
    glBindVertexArray(VAO);

    /*
        作用：绑定buffer到指定的object
        参数：
            1.buffer类型——GLenum
            1.object——unsigned int
    */
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    /*
        作用：将数据复制到当前缓冲
        要求：已经将相应的object与相应的缓冲进行绑定
        参数：
            1.绑定的缓冲——GLenum
            2.源数据大小——unsigned int
            3.源数据地址
            4.读写类型
        补充：
            1.第四个参数表示希望显卡如何管理数据
                GL_STATIC_DRAW ：数据不会或几乎不会改变
                GL_DYNAMIC_DRAW：数据会被改变很多
                GL_STREAM_DRAW ：数据每次绘制时都会改变
            2.注意C++对源数据大小的定义，如果是float a[9]，那么实际上是9 * sizeof(float)，如果是普通的指针，那么就是4个byte
    */
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), vertices, GL_STATIC_DRAW);

    /*
        作用：告诉OpenGL如何解析顶点数据
        要求：已经把相应的VAO进行绑定
        参数：
            1.数组中顶点属性的位置偏移（与着色器中的程序相对应）——unsigned int，这个偏移意味着此时绑定的数据对应的是着色器中的哪一个数据
            2.顶点属性的大小，此处一个顶点由三个坐标构成——unsigned int
            3.顶点的数据类型——enum
            4.定义是否希望数据被标准化——GL_FALSE/GL_TRUE
            5.步长（下个顶点的的出现位置与当前顶点开头的距离）——unsigned int
            6.表示位置数据在缓冲中起始位置的偏移量——unsigned int，此处表示在数组中的偏移量
        补充：
            1.参数1与参数6的区别
            2.调用这个函数本质上是配置了绑定的VAO，这个函数使得VAO间接与VBO联系起来了，所以在后面绘制图像时只需要VAO
    */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    
    /*
        作用：启用顶点属性
        参数：
            1.顶点属性的位置值——unsigned int
    */
    glEnableVertexAttribArray(0);

    /*
        作用：相当于解绑
        补充：在完成设置后，尽量直接解绑释放占用的资源
    */
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    /*
        作用：相当于解绑
        补充：在完成设置之后，把VAO解绑
    */
    glBindVertexArray(0);
}