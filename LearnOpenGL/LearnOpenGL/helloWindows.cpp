//
//  LearnOpenGL
//
//  Created by 王啸川 on 2018/7/28.
//  Copyright © 2018年 王啸川. All rights reserved.
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window,int width,int height);
void processInput(GLFWwindow* window);

const char* vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "gl_Position = vec4(aPos.x,aPos.y,aPos.z,1.0f);\n"
    "}\0";
const char* fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "FragColor = vec4(1.0f,0.5f,0.2f,1.0f);\n"
    "}\0";

int main()
{
    glfwInit(); //初始化GLFW
    // 指定创建的上下文必须与之兼容的客户端的API版本
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);  // 所选客户端API的有效主版本号
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);  // 所选客户端API的有效次要版本号
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 制定要为其创建上下文的OpenGL配置文件
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);    // 制定OpenGL上下文是否应该是向前兼容的
    
    GLFWwindow* window = glfwCreateWindow(800, 600, "learnOpenGL", NULL, NULL);
    if(window == NULL)
    {
        std::cout<<"Failed to create GLFW window"<<std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  // 每当窗口调整大小的时候调用这个函数
    
    // 在调用Opengl的函数之前初始化GLAD用来管理OpenGL的函数指针
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {// 用来加载系统相关的OpenGL函数指针地址的函数
        std::cout<<"Failed to initalize GLAD"<<std::endl;
        return -1;
    }
    
    unsigned int vertexShader;
    vertexShader=glCreateShader(GL_VERTEX_SHADER);  // 创建一个着色器对象
    glShaderSource(vertexShader,1,&vertexShaderSource,NULL);    // 将着色器源码附加到着色器对象上
    glCompileShader(vertexShader);  // 编译
    
    int success;
    char infoLog[512];
    
    glGetShaderiv(vertexShader,GL_COMPILE_STATUS,&success); // 获取vertexShader编译状态
    if(!success)
    {
        glGetShaderInfoLog(vertexShader,512,NULL,infoLog);  // 获取编译失败的信息
        std::cout<<"ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"<<infoLog<<std::endl;
    }
    
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader,1,&fragmentShaderSource,NULL);
    glCompileShader(fragmentShader);
    
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();  // 创建着色器程序
    glAttachShader(shaderProgram,vertexShader); // 将对应的着色器对象链接到着色器程序中
    glAttachShader(shaderProgram,fragmentShader);
    glLinkProgram(shaderProgram);
    
    glGetProgramiv(shaderProgram,GL_LINK_STATUS,&success);
    if(!success)
    {
        glGetProgramInfoLog(shaderProgram,512,NULL,infoLog);
        std::cout<<"ERROR::PROGRAM::LINKED_FAILED\n"<<infoLog<<std::endl;
    }
    // 调用该着色器程序对象
    glUseProgram(shaderProgram);
    
    // 删除着色器对象
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    float vertices[] = {
        -0.5f,-0.5f,0.0f,
        0.5f,-0.5f,0.0f,
        0.0f,0.5f,0.0f
    };  // 顶点数据
    
    unsigned int VBO,VAO;   // 顶点缓冲对象
    glGenVertexArrays(1,&VAO);  // 生成对应ID的VAO对象
    glGenBuffers(1,&VBO);   // 生成对应ID的VBO对象
    glBindVertexArray(VAO); // 绑定VAO对象
    glBindBuffer(GL_ARRAY_BUFFER,VBO);  // 将VBO对象绑定到GL_ARRAY_BUFFER上，任何该目标的缓冲都会配置当前绑定的缓冲（VBO）。
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW); // 将定义的顶点数据复制到缓冲的内存中
    
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);  // 设置顶点属性指针
    glEnableVertexAttribArray(0);   // 启用与0索引相关联的顶点数组
    
    glBindBuffer(GL_ARRAY_BUFFER,0);
    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens.
    // 你可以在之后取消绑定这个VAO，这样做其他的VAO调用将不会意外的修改这个VAO，尽管它很少发生。
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    // 修改其他VAO需要调用glBindVertexArray，所以我们通常不会在没有直接需要的情况下取消绑定VAO（也不是VBO）。
    glBindVertexArray(0);
    
    while(!glfwWindowShouldClose(window))
    {// Render Loop 渲染循环
        processInput(window);
        
        glClearColor(0.2f,0.3f,0.3f,1.0f);  // 状态设置函数
        glClear(GL_COLOR_BUFFER_BIT);   // 状态使用函数
        
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES,0,3);
        
        glfwPollEvents();   // 检查有无触发事件
        glfwSwapBuffers(window);    //交换颜色缓冲
    }
    
    glfwTerminate(); // 清理资源并释放应用程序
    //glViewport(0,0,800,600);  用来设置视口维度
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window,int width,int height)
{// 回调函数以调整窗口大小
    glViewport(0,0,width,height);
}

void processInput(GLFWwindow* window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE)==GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}
