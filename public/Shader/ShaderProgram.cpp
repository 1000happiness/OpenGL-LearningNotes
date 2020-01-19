#include "ShaderProgram.h"

ShaderProgram::ShaderProgram(){
    /*
        作用：生成着色器程序
    */
    ID = glCreateProgram();
}

ShaderProgram::~ShaderProgram(){

}

int ShaderProgram::getID(){
    return ID;
}

bool ShaderProgram::link(){
    int success = 0;
    char infoLog[512];
    /*
        作用：链接着色器程序
    */
    glLinkProgram(ID);
    
    /*
        作用：获取是否链接成功
        参数：
            1.着色器程序ID@int
            2.状态值@GLenum
            3.是否成功链接（地址传递）@int *
    */
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success) {
        /*
            作用：获取详细日志信息
            参数：
                1.着色器程序ID@int
                2.日志字符串的大小@int
                3.待补充
                4.待填写的日志字符串@char *
        */
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        return false;
    }

    for(auto shaderPair = shaderMap.begin(); shaderPair != shaderMap.end(); shaderPair++){
        /*
            作用：删除着色器
        */
        glDeleteShader(shaderPair->second);
    }
}

void ShaderProgram::use(){
    /*
        作用：使用着色器程序
    */
    glUseProgram(ID);
}

bool ShaderProgram::addShader(GLenum shaderType, const char * source, SourceType sourceType){
    if(sourceType == ShaderProgram::SourceType::FILE){
        std::string shaderCode;
        try {
            std::ifstream shaderFile;
            shaderFile.open(source);
            
            std::stringstream shaderStream;
            shaderStream << shaderFile.rdbuf();

            shaderFile.close();

            shaderCode = shaderStream.str();
        }
        catch(std::ifstream::failure e) {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        }
        return addStringShader(shaderType, shaderCode.c_str());
    }
    else {
        return addStringShader(shaderType, source);
    }
}

int ShaderProgram::getShader(GLenum shaderType){
    return shaderMap[shaderType];
}

bool ShaderProgram::setUniformValue(const char * name, bool *value, int number){
    if(number < 1 || number > 4) {
        return false;
    }
    /*
        作用：获取uniform数据的ID，后面可以根据这个ID更改相应的uniform的值
        参数：
            1.着色器程序@int
            2.uniform变量名
        补充：
            1.这个函数如果返回-1表示在程序中不存在这个变量
    */
    int uniformID = glGetUniformLocation(ID, name);
    if(uniformID == -1) {
        return false;
    }

    switch (number) {
        case 1:
            glUniform1i(uniformID, (int)(value[0]));
            break;
        case 2:
            glUniform2i(uniformID, (int)(value[0]), (int)(value[1]));
            break;
        case 3:
            glUniform3i(uniformID, (int)(value[0]), (int)(value[1]), (int)(value[2]));
            break;
        case 4:
            /*
                作用：设定uniform的值
                参数：
                    1.ID
                    2.3.4.5.值
                补充：
                    1.不同类型的uniform用不同的函数进行更改
            */
            glUniform4i(uniformID, (int)(value[0]), (int)(value[1]), (int)(value[2]), (int)(value[3]));
            break;
        default:
            break;
    }
    return true;
}

bool ShaderProgram::setUniformValue(const char * name, int *value, int number){
    if(number < 1 || number > 4) {
        return false;
    }
    
    int uniformID = glGetUniformLocation(ID, name);
    if(uniformID == -1) {
        return false;
    }

    switch (number) {
        case 1:
            glUniform1i(uniformID, (value[0]));
            break;
        case 2:
            glUniform2i(uniformID, (value[0]), (value[1]));
            break;
        case 3:
            glUniform3i(uniformID, (value[0]), (value[1]), (value[2]));
            break;
        case 4:
            glUniform4i(uniformID, (value[0]), (value[1]), (value[2]), (value[3]));
            break;
        default:
            break;
    }
    return true;
}

bool ShaderProgram::setUniformValue(const char * name, float *value, int number){
    if(number < 1 || number > 4) {
        return false;
    }
    
    int uniformID = glGetUniformLocation(ID, name);
    if(uniformID == -1) {
        return false;
    }

    switch (number) {
        case 1:
            glUniform1f(uniformID, (value[0]));
            break;
        case 2:
            glUniform2f(uniformID, (value[0]), (value[1]));
            break;
        case 3:
            glUniform3f(uniformID, (value[0]), (value[1]), (value[2]));
            break;
        case 4:
            glUniform4f(uniformID, (value[0]), (value[1]), (value[2]), (value[3]));
            break;
        default:
            break;
    }
    return true;
}

bool ShaderProgram::addStringShader(GLenum shaderType, const char * source){
    /*
        作用：根据类型生成着色器
    */
    int shader = glCreateShader(shaderType);
    /*
        作用：将源代码附着到着色器上
        参数：
            1.着色器ID@int
            2.着色器程序的数量@int
            3.着色器源程序@char **
            4.待补充
    */
    glShaderSource(shader, 1, &source, NULL);
    /*
        作用：编译着色器
    */
    glCompileShader(shader);
    
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        return false;
    }
    glAttachShader(ID, shader);
    return true;
}