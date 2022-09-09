#include <glad/glad.h>

#include "shader.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"

static const float PI = 3.14159265358979323846f;

Camera camera(glm::vec3(0.5f, 0.5f, 2.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f));
GLFWwindow* window_;
Shader parShader_;
GLuint parVao_;
GLuint parPosVbo_;
GLuint parColorVbo_;
int parNum = 2;


bool is_first_mouse = true;
float last_x = 0.0f;
float last_y = 0.0f;
bool is_first = false;
bool is_press = false;



void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
    //return;
    if (is_first_mouse) {
        last_x = xPos;
        last_y = yPos;
        is_first_mouse = false;
    }

    float offsetX = xPos - last_x;
    float offsetY = yPos - last_y;

    last_x = xPos;
    last_y = yPos;
    if (is_press)
    {
        camera.ProcessMouseMovement(offsetX, offsetY);

    }
}


void ProcessKeyboardInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(Camera_Movement::FORWARD, 0.05f);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(Camera_Movement::BACKWARD, 0.05f);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(Camera_Movement::LEFT, 0.05f);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(Camera_Movement::RIGHT, 0.05f);
    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        is_press = true;
    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
    {
        is_press = false;
    }
}

int initGl(float* parPos, float* parColor)
{
    glfwInit();

    glfwWindowHint(GLFW_SAMPLES, 4);
    window_ = glfwCreateWindow(800, 600, "opengl", NULL, NULL);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    if (window_ == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window_);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    //getchar();

    glViewport(0, 0, 800, 600);

    // ���ڴ�С�ı�Ļص�����
    glfwSetFramebufferSizeCallback(window_, FramebufferSizeCallback);

    // ����ƶ��Ļص�����
    glfwSetCursorPosCallback(window_, MouseCallback);

    // �����ƶ��Ļص�����

    ////���ع�꣬����׽
    //glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    //������Ȼ���
    glEnable(GL_DEPTH_TEST);

    // ������
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_MULTISAMPLE);
    //glHint(GL_POINT_SMOOTH, GL_FASTEST);

    //��������shader
    parShader_ = Shader("E:\\Dev\\openGL\\glMinimal2\\src\\particle.vert", "E:\\Dev\\openGL\\glMinimal2\\src\\particle.frag");
    parShader_.use();

    // ����VAO��VBO
    // ����
    glGenVertexArrays(1, &parVao_);
    glBindVertexArray(parVao_);

    // ����λ��
    glGenBuffers(1, &parPosVbo_);
    glBindBuffer(GL_ARRAY_BUFFER, parPosVbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * parNum, parPos, GL_DYNAMIC_DRAW);  // ���������
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
   

    // ������ɫ
    glGenBuffers(1, &parColorVbo_);
    glBindBuffer(GL_ARRAY_BUFFER, parColorVbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * parNum, parColor, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    


    // �����
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void draw(float* parPos, float* parColor)
{
    while (!glfwWindowShouldClose(window_))
    {
        // ������ɫ
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // ��Ⱦ����
        parShader_.use();

        glBindVertexArray(parVao_);

        //set uniform variables.
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 proj = camera.GetProjectionMatrix();
        glm::mat4 MVP = proj * view * model;		//�����˳�����Ҫ
        glm::mat4 MV = view * model;
        //glUniform1f(glGetUniformLocation(parShader_.ID, "pointScale"), 600.0f / tanf(35.0f * 0.5f * (float)PI / 180.0f));
        //glUniform1f(glGetUniformLocation(parShader_.ID, "pointScale"), 50);
        parShader_.setFloat("pointScale", 50);
        glUniformMatrix4fv(glGetUniformLocation(parShader_.ID, "MVP"), 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(parShader_.ID, "MV"), 1, GL_FALSE, &MV[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(parShader_.ID, "u_Persp"), 1, GL_FALSE, &proj[0][0]);
        glPointSize(20);
        glEnable(GL_DEPTH_TEST);
        
        glDrawArrays(GL_POINTS, 0, parNum);

        glBindVertexArray(0);
        glUseProgram(0);

        ProcessKeyboardInput(window_);
        glfwSwapBuffers(window_);
        glfwPollEvents();
    }
}

int main()
{
    float parPos[6] = { 0.3f,0.3f, 0.3f, 0.6f, 0.6f, 0.6f };
    float parColor[6] = { 1.0f,0.0f, 0.0f, 0.0f, 0.0f, 0.6f };
    initGl(parPos, parColor);

    draw(parPos, parColor);
}