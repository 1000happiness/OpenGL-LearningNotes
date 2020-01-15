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

    // ������ɫ�����˴�������ɫ�����й����¼����������һ����
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

    // ���������Σ�һ��������ĵ������εĶ���ʹ��VAO�Ϳ��Ա�ʶ
    unsigned int ID = 1;

    unsigned int VAO;
    /*
        ���ã�������һID��Ӧ��VAO
        ������
            1.ID����unsigned int
            2.VAO����unsigned int
    */
    glGenVertexArrays(ID, &VAO);

    /*
        ���ã����ɲ���һ��ָ����object
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
    //�˴���ʹ������
    //createTriangle(VAO, VBO, vertices);
    

    //�˴�ʹ������
    
    glBindVertexArray(VAO);
    float vertices2[] = {
    0.9f, 0.9f, 0.0f,   // ���Ͻ�
    0.9f, 0.6f, 0.0f,  // ���½�
    -0.9f, 0.6f, 0.0f, // ���½�
    -0.9f, 0.9f, 0.0f   // ���Ͻ�
    };

    unsigned int indices[] = { // ע��������0��ʼ! 
        0, 1, 3, // ��һ��������
        1, 2, 3  // �ڶ���������
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
        ע�⣺�˴������ߵĽ��˳�����Ҫ��VAO���ǰ����ѡ������߲����VBO��������ΪVAO��glVertexAttribPointer�������εذ���VAO��VBO��������EBO������δ����״̬���������Ҫ�����ֶ����ٴΰ�EBO
    */
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    //��Ⱦѭ��
    while (!glfwWindowShouldClose(window))
    {
        // ����
        processInput(window);

        /*
            ���ã�����buffer����ɫ
            ������
                1.2.3.4.��ɫ����float
            ���䣺
                1.��������ͨ��һ��ʹ�ã��ڶ���������ʾflush
        */
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        /*
            ���ã�ѡ����ɫ����
            ������
                ��ɫ�����򡪡�int
        */
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        /*
            ���ã�����EBO�м�¼��˳��VAO�м�¼�Ľ��ͷ�ʽ��VBO�м�¼�Ķ�����л���
            Ҫ�󣺰�EBO����VAO
            ������
                1.��Ҫ���Ƶ�ͼ��
                2.��Ҫ���ƵĶ�������
                3.EBO����������
                4.EBO�е�ƫ����
            ���䣺
                1.�������ӵ�ж�̬ʵ�֣�Ҳ���Բ���buffer��ѡ��ȥ����������
        */
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    /*
        ���ã�ɾ��VAO����ӦID�İ�
    */
    glDeleteVertexArrays(ID, &VAO);
    /*
        ���ã�ɾ��object����ӦID�İ�
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
        ���ã���VAO
        ������
            1.VAO����unsigned int
    */
    glBindVertexArray(VAO);

    /*
        ���ã���buffer��ָ����object
        ������
            1.buffer���͡���GLenum
            1.object����unsigned int
    */
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    /*
        ���ã������ݸ��Ƶ���ǰ����
        Ҫ���Ѿ�����Ӧ��object����Ӧ�Ļ�����а�
        ������
            1.�󶨵Ļ��塪��GLenum
            2.Դ���ݴ�С����unsigned int
            3.Դ���ݵ�ַ
            4.��д����
        ���䣺
            1.���ĸ�������ʾϣ���Կ���ι�������
                GL_STATIC_DRAW �����ݲ���򼸺�����ı�
                GL_DYNAMIC_DRAW�����ݻᱻ�ı�ܶ�
                GL_STREAM_DRAW ������ÿ�λ���ʱ����ı�
            2.ע��C++��Դ���ݴ�С�Ķ��壬�����float a[9]����ôʵ������9 * sizeof(float)���������ͨ��ָ�룬��ô����4��byte
    */
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), vertices, GL_STATIC_DRAW);

    /*
        ���ã�����OpenGL��ν�����������
        Ҫ���Ѿ�����Ӧ��VAO���а�
        ������
            1.�����ж������Ե�λ��ƫ�ƣ�����ɫ���еĳ������Ӧ������unsigned int
            2.�������ԵĴ�С���˴�һ���������������깹�ɡ���unsigned int
            3.������������͡���enum
            4.�����Ƿ�ϣ�����ݱ���׼������GL_FALSE/GL_TRUE
            5.�������¸�����ĵĳ���λ���뵱ǰ���㿪ͷ�ľ��룩����unsigned int
            6.��ʾλ�������ڻ�������ʼλ�õ�ƫ��������unsigned int
        ���䣺
            1.����1�����6������
            2.������������������������˰󶨵�VAO���������ʹ��VAO�����VBO��ϵ�����ˣ������ں������ͼ��ʱֻ��ҪVAO
    */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    
    /*
        ���ã����ö�������
        ������
            1.�������Ե�λ��ֵ����unsigned int
    */
    glEnableVertexAttribArray(0);

    /*
        ���ã��൱�ڽ��
        ���䣺��������ú󣬾���ֱ�ӽ���ͷ�ռ�õ���Դ
    */
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    /*
        ���ã��൱�ڽ��
        ���䣺���������֮�󣬰�VAO���
    */
    glBindVertexArray(0);
}