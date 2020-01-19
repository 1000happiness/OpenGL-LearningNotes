#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

int main()
{
    /*
        作用：初始化glfw
    */
    glfwInit();
   

    /*
        作用：配置glfw
        参数：
            1.配置的参数@GLenum
            2.配置的值@int
        补充：
            1.具体的配置属性可以参考 https://www.glfw.org/docs/latest/window.html#window_hints
            2.可以通过glfwWindowHintString配置string类型的变量
            3.至少需要配置这三个类型的变量：GLFW_CONTEXT_VERSION_MAJOR和GLFW_CONTEXT_VERSION_MINOR是版本号，GLFW_OPENGL_PROFILE是核心模式
    */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /*
        作用：创建窗口
        参数：
            1.2.窗口的长度和宽度@int
            3.窗口的名称@const char*
            4.待补充
            5.待补充
    */
    GLFWwindow *window = glfwCreateWindow(800, 600, "1-HelloWindows", NULL, NULL);

    /* 检查窗口是否创建成功 */
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        /*
            作用：结束窗口
        */
        glfwTerminate();
        return -1;
    }

    /*
        作用：配置窗口上下文
    */
    glfwMakeContextCurrent(window);

    /*
        作用：判断glad是否成功加载
    */
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    /*
        作用：设置视口
        参数：
            1.2.设置视口左下角在窗口的位置@int
            3.4.设置视口的大小@int
        补充：
            1.这个函数需要在的调整窗口大小的时候使用回调函数同步调整视口大小时被调用
    */
    glViewport(0, 0, 800, 600);

    /*
        作用：设置回调函数
        参数：
            1.窗口@GLFWwindow *
            2.回调函数指针
    */
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    // end init

    // 进入渲染循环
    /*
        作用：判断窗口是否输入了结束信号
        补充：
            1.窗口输入结束信号的方式有很多，可以是程序内设定，也可以是关闭窗口
    */
    while (!glfwWindowShouldClose(window))
    {
        /* 作用：接受输入*/
        processInput(window);

        /*
            作用：执行事件任务
            补充：
                1.这个函数会执行完队列中的全部函数
                2.这个函数不应在回调函数中被调用
        */
        glfwPollEvents();
        /*
            作用：交换帧缓存
        */
        glfwSwapBuffers(window);
    }

    //end
    glfwTerminate();

    return 0;
}

/*
    作用：自定义的回调函数，用于动态调整
*/
void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}


void processInput(GLFWwindow *window)
{
    /*
        作用：获取键盘输入
        参数：
            1.窗口@GLFWwindow *
            2.输入的按键@GLenum
        补充：
            1.glfw只有两个按键状态，按下（不用松开）和不按下，glfw把长按松开之类的操作交于用户实现
    */
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
        /*
            作用：设定窗口状态
        */
}