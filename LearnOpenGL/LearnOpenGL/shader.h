//
//  shader.h
//  LearnOpenGL
//
//  Created by 王啸川 on 2018/8/26.
//  Copyright © 2018年 王啸川. All rights reserved.
//

#ifndef shader_h
#define shader_h

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
    unsigned int ID;
    Shader(const char* vertexPath,const char* fragmentPath)
    {
        // 从文件路径中读取顶点/片段着色器
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        // 保证ifstream对象可以抛出异常
        vShaderFile.exceptions(std::ifstream::failbit|std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit|std::ifstream::badbit);
        try
        {
            // 打开文件
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderStream, fShaderStream;
            // 读取文件的缓冲内容到数据流中
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            // 关闭文件处理器
            vShaderFile.close();
            fShaderFile.close();
            // 转换数据流到string
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        } catch (std::ifstream::failure e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        }
        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();
        
        unsigned int vertex,fragment;
        int success;
        char infoLog[512];
        // 链接&编译顶点着色器
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex,1,&vShaderCode,NULL);
        glCompileShader(vertex);
        
        glGetShaderiv(vertex,GL_COMPILE_STATUS,&success);
        if(!success)
        {
            glGetShaderInfoLog(vertex,512,NULL,infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        };
        // 链接&编译片段着色器
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment,1,&fShaderCode,NULL);
        glCompileShader(fragment);
        
        ID = glCreateProgram();
        glAttachShader(ID,vertex);
        glAttachShader(ID,fragment);
        glLinkProgram(ID);
        
        glGetProgramiv(ID,GL_LINK_STATUS,&success);
        if(!success)
        {
            glGetProgramInfoLog(ID,512,NULL,infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        }
        // 删除着色器
        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }
    void use()
    {
        glUseProgram(ID);
    }
    void setBool(const std::string &name,bool value) const
    {
        glUniform1i(glGetUniformLocation(ID,name.c_str()),(int)value);
    }
    void setInt(const std::string &name, int value) const
    {
        glUniform1i(glGetUniformLocation(ID,name.c_str()),value);
    }
    void setFloat(const std::string &name, float value) const
    {
        glUniform1f(glGetUniformLocation(ID,name.c_str()),value);
    }

};



#endif /* shader_h */
