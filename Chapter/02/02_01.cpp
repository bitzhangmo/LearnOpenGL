//
// Created by 王啸川 on 2019/10/8.
//

#include <glad.h>
#include <glfw3.h>
#include <iostream>

#include "../../glm/glm.hpp"
#include "../../glm/gtc/matrix_transform.hpp"
#include "../../glm/gtc/type_ptr.hpp"

#include "../../Header/LocalHeader/shader.h"
#include "../../Header/LocalHeader/stb_image.h"
#include "../../Header/LocalHeader/Camera.h"

using namespace std;

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

Camera camera(glm::vec3(0.0f,0.0f,3.0f));
float lastX = SCR_WIDTH/2.0f;
float lastY = SCR_HEIGHT/2.0f;
bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

glm::vec3 lightPos(1.2f,1.0f,2.0f);

void framebuffer_size_callback(GLFWwindow* window,int width,int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow *window, double xpos,double ypos);
void scroll_callback(GLFWwindow *window,double xoffset,double yoffset);

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
    glfwSetCursorPosCallback(window,mouse_callback);
    glfwSetScrollCallback(window,scroll_callback);

    glfwSetInputMode(window,GLFW_CURSOR,GLFW_CURSOR_DISABLED);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    Shader lightingShader("../Shader/0201Vertex.glsl","../Shader/0201Fragment.glsl");
    Shader lampShader("../Shader/0201LightVertex.glsl","../Shader/0201LightFragment.glsl");

    float vertices[] = {
            -0.5f, -0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,
            0.5f,  0.5f, -0.5f,
            0.5f,  0.5f, -0.5f,
            -0.5f,  0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,

            -0.5f, -0.5f,  0.5f,
            0.5f, -0.5f,  0.5f,
            0.5f,  0.5f,  0.5f,
            0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f,
            -0.5f, -0.5f,  0.5f,

            -0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,
            -0.5f, -0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f,

            0.5f,  0.5f,  0.5f,
            0.5f,  0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,
            0.5f, -0.5f,  0.5f,
            0.5f,  0.5f,  0.5f,

            -0.5f, -0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,
            0.5f, -0.5f,  0.5f,
            0.5f, -0.5f,  0.5f,
            -0.5f, -0.5f,  0.5f,
            -0.5f, -0.5f, -0.5f,

            -0.5f,  0.5f, -0.5f,
            0.5f,  0.5f, -0.5f,
            0.5f,  0.5f,  0.5f,
            0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f, -0.5f,
    };

    // VBO,Vertex Buffer Objects,顶点缓冲对象
    // VAO,Vertex Array Object,顶点数组对象
    // EBO,Element Buffer Object,索引缓冲对象
    unsigned int VBO,cubeVAO;
    glGenVertexArrays(1,&cubeVAO);
    glGenBuffers(1,&VBO);

    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices),vertices,GL_STATIC_DRAW);

    glBindVertexArray(cubeVAO);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*(sizeof(float)),(void*)0);
    glEnableVertexAttribArray(0);

    unsigned int lightVAO;
    glGenVertexArrays(1,&lightVAO);
    glBindVertexArray(lightVAO);

    glBindBuffer(GL_ARRAY_BUFFER,VBO);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*(sizeof(float)),(void*)0);
    glEnableVertexAttribArray(0);

    stbi_set_flip_vertically_on_load(true);

    while(!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // 输入
        processInput(window);

        // 渲染指令
        glClearColor(0.1f,0.1f,0.1f,0.1f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        lightingShader.use();
        lightingShader.setVec3("objectColor",1.0f,0.5f,0.31f);
        lightingShader.setVec3("lightColor",1.0f,1.0f,1.0f);

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view",view);

        glm::mat4 model = glm::mat4(1.0f);
        lightingShader.setMat4("model",model);

        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES,0,36);

        lampShader.use();
        lampShader.setMat4("projection",projection);
        lampShader.setMat4("view",view);
        model = glm::mat4(1.0f);
        model = glm::translate(model,lightPos);
        model = glm::scale(model,glm::vec3(0.2f));
        lampShader.setMat4("model",model);

        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES,0,36);


        // 检查并调用事件，交换缓冲
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightVAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window,int width,int height)
{
    glViewport(0,0,width,height);
}

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window,GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window,true);

    float cameraSpeed = 2.5f * deltaTime;
    if(glfwGetKey(window,GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD,deltaTime);
    if(glfwGetKey(window,GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD,deltaTime);
    if(glfwGetKey(window,GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT,deltaTime);
    if(glfwGetKey(window,GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT,deltaTime);
}

void mouse_callback(GLFWwindow *window,double xpos,double ypos)
{

    if(firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset,yoffset);
}

void scroll_callback(GLFWwindow *window,double xoffset,double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}