#include "Scene.h"
#include "Shader.h"
#include "Camera.h"
#include "Mesh.h"
#include "MeshLoad.h"

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

glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraDessus = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 cameraDevant = glm::vec3(0.0f, 0.0f, -1.0f);

glm::vec3 cameraCible = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraZ = glm::normalize(cameraPosition - cameraCible); // note that it's the inverse of the direction in fact

//to get the positive x-axis:
glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f); // ===> vecteur unitaires
//On prend le vecteur de la camera donc z, on fait le produit vectoriel avec y et on obtien x !! ON FAIT up ^ cameraDirection et pas l'inverse sinon négatif
glm::vec3 cameraX = glm::normalize(glm::cross(up, cameraZ));
//Maintenant que l'on a la caméra selon X, selon Z il manque plus qu'a fair eun produit vectoriel pour obtenir Y
glm::vec3 cameraY = glm::cross(cameraZ, cameraX);

glm::vec3 pointLightPositions[] = {
        glm::vec3(0.0f,  0.0f,  1.0f),
        glm::vec3(0.0f, -1.0f, -1.0f),
        glm::vec3(-1.0f,  1.0f, -1.0f),
        glm::vec3(0.0f,  0.0f, -1.0f)
};

glm::vec3 coord = glm::vec3(0.0f);

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));


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

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Erreur de GLAD" << std::endl;
        return -1;
    }
    Shader shader("Shader/vertexShader.vs", "Shader/fragmentShader.fs");


    Mesh sphere;
    Mesh plane;
    Mesh capsule;

    capsule = MakeCapsule(25, 50.0f, 2.0f, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(45.0f, 0.0f, 0.0f));
    plane = MakePlane(4, 4, glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(45.0f, 0.0f, 0.0f),50,50);
    sphere = MakeIcosphere(4, 5.0f, glm::vec3(0.0f, 0.0f, 0.0f));

    MeshLoad loadPlane(plane);
    MeshLoad loadSphere(sphere);
    MeshLoad loadCapsule(capsule);

    // Unbind VBO (optional, for cleanliness)
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Unbind VAO (optional, but often recommended to avoid accidental modifications)
    glBindVertexArray(0);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window))
    {

        float currentFrame = glfwGetTime();
        deltatime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClearColor(0.2f, 0.3f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_DEPTH_BUFFER_BIT);

        glfwSetCursorPosCallback(window, mouse_callback);
        glfwSetScrollCallback(window, scroll_callback);

        glm::mat4 view;

        view = camera.GetViewMatrix();

        glm::mat4 projection;
        projection = glm::perspective(glm::radians(90.0f), (float)(resolution), 0.1f, 100.0f);
        shader.useShader();
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);
        shader.setVec3("viewPos", camera.Position);
        shader.setVec3("light.position", camera.Position);

        // light properties
        glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
        glm::vec3 diffuseColor = lightColor   * glm::vec3(0.5f); // decrease the influence
        glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // low influence
        shader.setVec3("light.ambient", ambientColor);
        shader.setVec3("light.diffuse", diffuseColor);
        shader.setVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));
        shader.setFloat("light.constant", 0.5f);
        shader.setFloat("light.linear", 0.009f);
        shader.setFloat("light.quadratic", 0.0032f);
        shader.setVec3("light.direction", camera.Front);
        //shader.setFloat("light.cutOff", glm::cos(glm::radians(30.0f)));

        // material properties
        shader.setVec3("material.diffuse", glm::vec3(1.0f, 0.5f, 0.31f));
        shader.setVec3("material.specular", glm::vec3(0.2f, 0.15f, 0.31f)); // specular lighting doesn't have full effect on this object's material
        shader.setFloat("material.shininess", 16.0f);
        // Pour la sphère

        float x = sin(glfwGetTime())* 2;
        sphere.coord.x = x;

        glm::mat4 modelSphere = glm::mat4(1.0f);
        modelSphere = glm::translate(modelSphere, sphere.coord);
        modelSphere = glm::rotate(modelSphere, glm::radians(sphere.rotation.x), glm::vec3(1, 0, 0)); // Rotation autour de l'axe X
        modelSphere = glm::rotate(modelSphere, glm::radians(sphere.rotation.y), glm::vec3(0, 1, 0)); // Rotation autour de l'axe Y
        modelSphere = glm::rotate(modelSphere, glm::radians(sphere.rotation.z), glm::vec3(0, 0, 1)); // Rotation autour de l'axe Z

        shader.setMat4("model", modelSphere);
        loadSphere.Draw(shader);

        shader.useShader();
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);
        shader.setVec3("viewPos", camera.Position);
        shader.setVec3("light.position", camera.Position);

        // light properties
        shader.setVec3("light.ambient", ambientColor);
        shader.setVec3("light.diffuse", diffuseColor);
        shader.setVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));
        shader.setFloat("light.constant", 0.5f);
        shader.setFloat("light.linear", 0.009f);
        shader.setFloat("light.quadratic", 0.0032f);
        shader.setVec3("light.direction", camera.Front);
        //shader.setFloat("light.cutOff", glm::cos(glm::radians(30.0f)));

        // material properties
        shader.setVec3("material.diffuse", glm::vec3(1.0f, 0.5f, 0.31f));
        shader.setVec3("material.specular", glm::vec3(0.2f, 0.15f, 0.31f)); // specular lighting doesn't have full effect on this object's material
        shader.setFloat("material.shininess", 16.0f);

        glm::mat4 modelPlane = glm::mat4(1.0f);
        modelPlane = glm::translate(modelPlane, plane.coord);
        modelPlane = glm::rotate(modelPlane, glm::radians(plane.rotation.x), glm::vec3(1, 0, 0)); // Rotation autour de l'axe X
        modelPlane = glm::rotate(modelPlane, glm::radians(plane.rotation.y), glm::vec3(0, 1, 0)); // Rotation autour de l'axe Y
        modelPlane = glm::rotate(modelPlane, glm::radians(plane.rotation.z), glm::vec3(0, 0, 1)); // Rotation autour de l'axe Z

        shader.setMat4("model", modelPlane);
        loadPlane.Draw(shader);

        shader.useShader();
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);
        shader.setVec3("viewPos", camera.Position);
        shader.setVec3("light.position", camera.Position);

        // light properties
        shader.setVec3("light.ambient", ambientColor);
        shader.setVec3("light.diffuse", diffuseColor);
        shader.setVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));
        shader.setFloat("light.constant", 0.5f);
        shader.setFloat("light.linear", 0.009f);
        shader.setFloat("light.quadratic", 0.0032f);
        shader.setVec3("light.direction", camera.Front);
        //shader.setFloat("light.cutOff", glm::cos(glm::radians(30.0f)));

        // material properties
        shader.setVec3("material.diffuse", glm::vec3(1.0f, 0.5f, 0.31f));
        shader.setVec3("material.specular", glm::vec3(0.2f, 0.15f, 0.31f)); // specular lighting doesn't have full effect on this object's material
        shader.setFloat("material.shininess", 16.0f);

        glm::mat4 modelCapsule = glm::mat4(1.0f);
        modelCapsule = glm::translate(modelCapsule, capsule.coord);
        modelCapsule = glm::rotate(modelCapsule, glm::radians(capsule.rotation.x), glm::vec3(1, 0, 0)); // Rotation autour de l'axe X
        modelCapsule = glm::rotate(modelCapsule, glm::radians(capsule.rotation.y), glm::vec3(0, 1, 0)); // Rotation autour de l'axe Y
        modelCapsule = glm::rotate(modelCapsule, glm::radians(capsule.rotation.z), glm::vec3(0, 0, 1)); // Rotation autour de l'axe Z

        shader.setMat4("model", modelCapsule);
        loadCapsule.Draw(shader);







        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    loadSphere.Delete();
    loadPlane.Delete();
    loadCapsule.Delete();
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
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}