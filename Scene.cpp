#include "Scene.h"
#include "Shader.h"
#include "Camera.h"
#include "Mesh.h"
#include "MeshLoad.h"
#include "Light.h"
#include "Physic.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow* window, double xPos, double yPos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow * window);

const unsigned int SCR_WIDTH = 1920*0.75f;
const unsigned int SCR_HEIGHT = 1080*0.75f;
float resolution = SCR_WIDTH / SCR_HEIGHT;

float fov = 50.0f;
float deltatime = 0.0f;
float lastFrame = 0.0f;

bool firstScreen = true;
float lastX = SCR_WIDTH / 2;
float lastY = SCR_HEIGHT / 2;

bool pressed = true;

Camera camera(glm::vec3(10.0f, 10.0f, 0.0f));

int createScene()
{

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Erreur de GLFW" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Erreur de GLAD" << std::endl;
        return -1;
    }
    Shader shader("Shader/vertexShader.vs", "Shader/fragmentShader.fs");

    Mesh sphere;
    Mesh sphere1;
    Mesh plane;
    //Mesh plane1;
    //Mesh capsule;

    //capsule = createCapsule(25, 50.0f, 2.0f, glm::vec3(0.0f, 10.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    plane = createPlane(16, 16, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 0.0f),200, 200);
    //plane1 = createPlane(16, 16, glm::vec3(0.0f,20.0f,0.0f), glm::vec3(0.0f, 0.0f, 0.0f),10, 10);
    sphere = createIcosphere(4, 10.0f, vec3(1.0f, 50.0f, 0.0f));
    sphere1 = createIcosphere(4, 5.0f, vec3(0.0f, 25.0f, 0.0f));

    MeshLoad loadPlane(plane);
    //MeshLoad loadPlane1(plane1);
    MeshLoad loadSphere(sphere);
    MeshLoad loadSphere1(sphere1);
    //MeshLoad loadCapsule(capsule);

    Light light(0.5, 0.75, glm::vec3(0.75f), glm::vec3(1.0f), glm::vec3(25.0f,25.0f,25.0f));

    // physique

    std::vector<Object*> listObject;

    Sphere sphere_p(sphere.coord, 10.0f, true,  10.0f);
    Sphere sphere1_p(sphere1.coord, 10.0f, true,  5.0f);
    Plane plane_p(vec3(0.0f), 10000000.0f, false, 200.0f, 200.0f, plane.norm[2]);
    //Plane plane1_p(plane1.coord, 100.0f, true, 10.0f, 10.0f, plane.norm[2]);

    listObject.push_back(&sphere_p);
    listObject.push_back(&sphere1_p);
    listObject.push_back(&plane_p);
    //listObject.push_back(&plane1_p);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window))
    {

        float currentFrame = glfwGetTime();
        deltatime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        updateAllObjects(listObject, deltatime);

        processInput(window);

        glClearColor(0.2f, 0.3f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_DEPTH_BUFFER_BIT);

        glfwSetCursorPosCallback(window, mouse_callback);
        glfwSetScrollCallback(window, scroll_callback);

        shader.useShader();
        light.AddLight(shader);

        loadSphere.mesh.coord = sphere_p.coord;
        loadSphere1.mesh.coord = sphere1_p.coord;
        //loadPlane1.mesh.coord = plane1_p.coord;

        loadSphere.DrawObject(shader, camera, glm::vec3(1.0f,0.2f,0.8f), glm::vec3(0.7f,0.0f,0.0f));
        loadSphere1.DrawObject(shader, camera, glm::vec3(0.0f,0.2f,0.8f), glm::vec3(0.5f,0.0f,1.0f));
        //loadPlane1.DrawObject(shader, camera, glm::vec3(0.0f,0.8f,0.8f), glm::vec3(0.7f,0.4f,0.4f));

        loadPlane.DrawObject(shader, camera, glm::vec3(0.8f,1.0f,0.6f),glm::vec3(0.0f,0.7f,0.0f));

        //loadCapsule.DrawObject(shader, camera, glm::vec3(0.5f,0.2f,1.0f),glm::vec3(0.0f,0.0f,0.7f));

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    loadSphere.Delete();
    loadPlane.Delete();
    //loadCapsule.Delete();
    shader.deleteShader();


    glfwTerminate();
    return 0;
}
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltatime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltatime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltatime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltatime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera.ProcessKeyboard(UP, deltatime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.ProcessKeyboard(DOWN, deltatime);
    if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS)
    {
        if (pressed) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            pressed = false;
        }
        else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            pressed = true;
        }
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{

    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstScreen)
    {
        lastX = xpos;
        lastY = ypos;
        firstScreen = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}