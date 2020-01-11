#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

int main()
{
    /*
        作用：初始化GLFW
        要求：第一个执行
    */
    glfwInit();
    

    /*
        作用：对窗口进行配置 
        要求：对窗口的配置必须要在创建窗口前进行
        参数：
            1.配置的参数名称——通常为宏定义的int
            2.配置的int值——int
        补充：
            1.详细设置参考：https://www.glfw.org/docs/latest/window.html#window_hints
            2.相似的函数glfwWindowHintString可以配置string类型的属性
            3.GLFW_CONTEXT_VERSION_MAJOR和GLFW_CONTEXT_VERSION_MINOR相当于版本号为3.3
            4.GLFW_OPENGL_PROFILE表示运行的模式，此处为核心模式
            5.此处的三个设定通常是必须的
    */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /*
        作用：创建窗口对象，这个对象中存储了所有与窗口相关的数据
        参数：
            1.宽度——int
            2.高度——int
            3.名称——const char *
            4.设定全屏应用的屏幕——GLFWwindow *（可以为null）
            5.与另一个窗口共享context——GLFWwindow *（可以为null）
    */
    GLFWwindow* window = glfwCreateWindow(800, 600, "1-HelloWindows", NULL, NULL);

    /*
      校验  
    */
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        /*
            作用：销毁窗口
        */
        glfwTerminate();
        return -1;
    }

    /*
        作用：设置窗口的上下文
        要求：完成窗口和上下文的配置
        参数：
            1.窗口——GLFWwindow*
    */
    glfwMakeContextCurrent(window);

    /*
        作用：判断glad是否完成加载
        参数：
            1.程序——GLADloadproc
        返回值：
            2.宏定义的GL_TRUE或者GL_FALSE
    */
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    /*
        作用：设置视口
        参数：
            1.2.视口左下角的位置（相对于窗口）
            3.4.视口的宽度和高度
        补充：
            1.实际上视口可以比窗口小
            2.可以定义一个回调函数在改变窗口大小的时候同步改变视口大小
    */
    glViewport(0, 0, 800, 600);

    /*
        作用：配置回调函数
        要求：回调函数已经定义且参数满足标准
        参数：
            1.相关的窗口
            2.回调的函数
    */
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // end init

    // 渲染循环
    /*
        作用：检测窗口是否被设定为关闭
        参数：
            1.相关的窗口——GLFWwindow*
    */
    while (!glfwWindowShouldClose(window))
    {
        // 输入
        processInput(window);

        // 检查并调用事件，交换缓冲
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    //end
    glfwTerminate();

    return 0;
}

/*
    作用：回调函数，同步调整视口和窗口大小
    要求：参数类型必须要带window
    参数：
        1.对应的window
        2.3.宽度和高度
*/
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}


void processInput(GLFWwindow *window)
{
    /*
        作用：检测某一个按键是否被按下
        参数：
            1.相关的窗口——GLFWwindow*
            2.按键——GLFWwindow*
        返回值：返回两个状态，分别表示按下（1）和未按下（0）
    */
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
        /*
            作用：设定窗口状态
            参数：
                1.相关的窗口——GLFWwindow*
                2.是否关闭——bool
        */
}