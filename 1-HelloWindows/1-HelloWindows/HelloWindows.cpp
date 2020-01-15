#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

int main()
{
    /*
        ���ã���ʼ��GLFW
        Ҫ�󣺵�һ��ִ��
    */
    glfwInit();
    

    /*
        ���ã��Դ��ڽ������� 
        Ҫ�󣺶Դ��ڵ����ñ���Ҫ�ڴ�������ǰ����
        ������
            1.���õĲ������ơ���ͨ��Ϊ�궨���int
            2.���õ�intֵ����int
        ���䣺
            1.��ϸ���òο���https://www.glfw.org/docs/latest/window.html#window_hints
            2.���Ƶĺ���glfwWindowHintString��������string���͵�����
            3.GLFW_CONTEXT_VERSION_MAJOR��GLFW_CONTEXT_VERSION_MINOR�൱�ڰ汾��Ϊ3.3
            4.GLFW_OPENGL_PROFILE��ʾ���е�ģʽ���˴�Ϊ����ģʽ
            5.�˴��������趨ͨ���Ǳ����
    */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /*
        ���ã��������ڶ�����������д洢�������봰����ص�����
        ������
            1.���ȡ���int
            2.�߶ȡ���int
            3.���ơ���const char *
            4.�趨ȫ��Ӧ�õ���Ļ����GLFWwindow *������Ϊnull��
            5.����һ�����ڹ���context����GLFWwindow *������Ϊnull��
    */
    GLFWwindow* window = glfwCreateWindow(800, 600, "1-HelloWindows", NULL, NULL);

    /*
      У��  
    */
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        /*
            ���ã����ٴ���
        */
        glfwTerminate();
        return -1;
    }

    /*
        ���ã����ô��ڵ�������
        Ҫ����ɴ��ں������ĵ�����
        ������
            1.���ڡ���GLFWwindow*
    */
    glfwMakeContextCurrent(window);

    /*
        ���ã��ж�glad�Ƿ���ɼ���
        ������
            1.���򡪡�GLADloadproc
        ����ֵ��
            2.�궨���GL_TRUE����GL_FALSE
    */
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    /*
        ���ã������ӿ�
        ������
            1.2.�ӿ����½ǵ�λ�ã�����ڴ��ڣ�
            3.4.�ӿڵĿ��Ⱥ͸߶�
        ���䣺
            1.ʵ�����ӿڿ��Աȴ���С
            2.���Զ���һ���ص������ڸı䴰�ڴ�С��ʱ��ͬ���ı��ӿڴ�С
    */
    glViewport(0, 0, 800, 600);

    /*
        ���ã����ûص�����
        Ҫ�󣺻ص������Ѿ������Ҳ��������׼
        ������
            1.��صĴ���
            2.�ص��ĺ���
    */
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    // end init

    // ��Ⱦѭ��
    /*
        ���ã���ⴰ���Ƿ��趨Ϊ�ر�
        ������
            1.��صĴ��ڡ���GLFWwindow*
    */
    while (!glfwWindowShouldClose(window))
    {
        // ����
        processInput(window);

        // ��鲢�����¼�����������
        /*
            ���ã���鲢�����¼�
            ��������
            ���䣺
                1.���������Ѷ��������еĻص�����ȫ��ִ�У������ڼ������߳�
                2.��������������κλص������б�����
        */
        glfwPollEvents();
        /*
            ���ã���������
            ������
                1.��صĴ��ڡ���GLFWwindow*
            ���䣺
                1.����������ȴ�GPU�����Ⱦһ��������ٽ��н�������֤���֡����Ⱦ��ɵ�
        */
        glfwSwapBuffers(window);
    }

    //end
    glfwTerminate();

    return 0;
}

/*
    ���ã��ص�������ͬ�������ӿںʹ��ڴ�С
    Ҫ�󣺲������ͱ���Ҫ��window
    ������
        1.��Ӧ��window
        2.3.���Ⱥ͸߶�
*/
void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}


void processInput(GLFWwindow *window)
{
    /*
        ���ã����ĳһ�������Ƿ񱻰���
        ������
            1.��صĴ��ڡ���GLFWwindow*
            2.��������GLFWwindow*
        ����ֵ����������״̬���ֱ��ʾ���£�1����δ���£�0��
    */
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
        /*
            ���ã��趨����״̬
            ������
                1.��صĴ��ڡ���GLFWwindow*
                2.�Ƿ�رա���bool
        */
}