//
// Created by 王啸川 on 2019/10/8.
//

#include <glad.h>
#include <glfw3.h>
#include <iostream>

#include "../shader.h"
#include "../stb_image.h"

#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../glm/gtc/type_ptr.hpp"

using namespace std;

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

void framebuffer_size_callback(GLFWwindow* window,int width,int height);
void processInput(GLFWwindow *window);

void framebuffer_size_callback(GLFWwindow* window,int width,int height)
{
    glViewport(0,0,width,height);
}

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window,GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window,true);
}

int main(void)
{

    glfwInit();    // 初始化GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    // Mac OSX 专供
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH,SCR_HEIGHT,"LearnOpenGL",NULL,NULL);

    if(window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window,framebuffer_size_callback);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //glViewport(0,0,800,600);
    //glfwSetFramebufferSizeCallback(window,framebuffer_size_callback);

    Shader ourShader("../Shader/Vertex.glsl","../Shader/Fragment.glsl");


    float vertices[] = {
            // 位置             // 纹理
            0.5f,0.5f,0.0f,    1.0f,1.0f,
            0.5f,-0.5f,0.0f,   1.0f,0.0f,
            -0.5f,-0.5f,0.0f,  0.0f,0.0f,
            -0.5f,0.5f,0.0f,   0.0f,1.0f,
    };

    unsigned int indices[] = {
            0, 1, 3, // first triangle
            1, 2, 3  // second triangle
    };

    // VBO,Vertex Buffer Objects,顶点缓冲对象
    // VAO,Vertex Array Object,顶点数组对象
    // EBO,Element Buffer Object,索引缓冲对象
    unsigned int VBO,VAO,EBO;

    // 生成对应的对象
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1,&VBO);
    glGenBuffers(1,&EBO);

    glBindVertexArray(VAO);

    // 把新创建的缓冲绑定在GL_ARRAY_BUFFER目标上，并利用bufferData函数把定义的定点数据复制到缓存的内存中
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);

    // 绑定EBO，并把索引复制到缓冲里
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);


    // 解析顶点数据，并启用顶点数据，设置顶点属性指针
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,5*sizeof(float),(void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);

    //glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)(6* sizeof(float)));
    //glEnableVertexAttribArray(2);

    unsigned int texture1,texture2;
    glGenTextures(1,&texture1);
    glBindTexture(GL_TEXTURE_2D,texture1);

    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width,height,nrChannels;

    stbi_set_flip_vertically_on_load(true);

    unsigned char *data = stbi_load("../Texture/container.jpg",&width,&height,&nrChannels,0);

    if(data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout<<"Failed to load texture"<<std::endl;
    }

    stbi_image_free(data);

    glGenTextures(1,&texture2);
    glBindTexture(GL_TEXTURE_2D,texture2);

    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    data = stbi_load("../Texture/awesomeface.png",&width,&height,&nrChannels,0);


    if(data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout<<"Failed to load texture"<<std::endl;
    }

    stbi_image_free(data);

    ourShader.use(); // don't forget to activate/use the shader before setting uniforms!
    // either set it manually like so:
    ourShader.setInt("texture1", 0);
    ourShader.setInt("texture2", 1);

    // 线框模式
    //glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

    //glm::vec4 vec(1.0f,0.0f,0.0f,1.0f);

    while(!glfwWindowShouldClose(window))
    {
        // 输入
        processInput(window);

        // 渲染指令
        glClearColor(0.2f,0.3f,0.3f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D,texture2);

        ourShader.use();

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);

        model = glm::rotate(model,glm::radians(-55.0f),glm::vec3(1.0f,0.0f,0.0f));
        view = glm::translate(view,glm::vec3(0.0f,0.0f,-3.0f));
        projection = glm::perspective(glm::radians(45.0f),(float)SCR_WIDTH/(float)SCR_HEIGHT,0.1f,100.0f);

        unsigned int modelLoc = glGetUniformLocation(ourShader.ID,"model");
        unsigned int viewLoc = glGetUniformLocation(ourShader.ID,"view");
        glUniformMatrix4fv(modelLoc,1,GL_FALSE,glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc,1,GL_FALSE,&view[0][0]);
        ourShader.setMat4("projection",projection);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        //glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);

        // 检查并调用事件，交换缓冲
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1,&VAO);
    glDeleteBuffers(1,&VBO);
    glDeleteBuffers(1,&EBO);

    glfwTerminate();
    return 0;
}

