#include "Scene.h"
#include "Shader.h"
#include "Camera.h"
#include "Mesh.h"
#include "MeshLoad.h"
#include "Light.h"
#include "Physic.h"
#include "Player.h"
#include "LoadModel.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xPos, double yPos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window, Player &player);
GLFWwindow* init();

const unsigned int SCR_WIDTH = 1920*0.5f;
const unsigned int SCR_HEIGHT = 1080*0.5f;
float resolution = SCR_WIDTH / SCR_HEIGHT;

float fov = 50.0f;
float deltatime = 0.0f;
float lastFrame = 0.0f;

bool update = true;

bool firstScreen = true;
float lastX = SCR_WIDTH / 2;
float lastY = SCR_HEIGHT / 2;

bool pressed = true;

Camera camera;

int createScene()
{

    GLFWwindow* window = init();

    //Model backpack("resources/backpack/backpack.obj");
    //Model sphere_m("resources/sphere/sphere.obj");
    //Model grass("resources/grass/grass3.obj");
    Model maison("resources/maison/cottage_blender.obj");

    //Shader shader2D("Shader/2Dvertex.v", "Shader/2Dfragment.f");
    Shader shader("Shader/vertexShader.vs", "Shader/fragmentShader.fs");
    Shader shader_flame("Shader/vertex_motion.vs", "Shader/fragment_motion.fs");
    Shader shader_model("Shader/vertex.v", "Shader/fragment.f");

    MeshLoad cone = createCone(10, 5.0f, 10.0f, vec3(0.0f), vec3(0.0f));
    MeshLoad cone1 = createCone(10, 5.0f, 10.0f, vec3(10.0f, 0.0f,0.0f), vec3(0.0f));
    MeshLoad cone2 = createCone(10, 5.0f, 10.0f, vec3(10.0f, 0.0f,10.0f), vec3(0.0f));
    MeshLoad rectangle = createRectangle(10, 10, 10, vec3(1.0f, 10.0f, 0.0f), vec3(0.0f), 10, 10, 10);
    MeshLoad sphere = createIcosphere(2, 10.0f, vec3(1.0f, 50.0f, 0.0f));
    MeshLoad sphere1 = createIcosphere(2, 10.0f, vec3(5.0f, 50.0f, 0.0f));
    MeshLoad plane = createPlane(10, 10, vec3(0.0f), vec3(0.0f), 200.0f, 200.0f);

    Light lampeLight(0.5, 0.75, glm::vec3(0.75f), glm::vec3(1.0f), glm::vec3(20.0f,2.0f,0.0f));
    Light lampeLight1(0.5, 0.75, glm::vec3(0.75f), glm::vec3(1.0f), glm::vec3(20.0f,2.0f,20.0f));
    Light sun(0.5, 0.75, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.5,0.0,0.5), glm::vec3(0.0f,10.0f,0.0f));

    std::vector <Light> list_light;

    //list_light.emplace_back(lampeLight);
    //list_light.emplace_back(lampeLight1);
    list_light.emplace_back(sun);

    Plane *plane_p = &(ObjectManager::getInstance().createObject<Plane>(vec3(0.0f), 1.0f, false, 25.0f, 25.0f, vec3(0.0f, 1.0f, 0.0f)));
    Sphere *sphere_p = &(ObjectManager::getInstance().createObject<Sphere>(vec3(10.0f, 50.0f, 0.0f), 10.0f, true,  10.0f));
    Sphere *sphere1_p = &(ObjectManager::getInstance().createObject<Sphere>(vec3(10.0f, 20.0f, 0.0f), 10.0f, true,  10.0f));
    
    stbi_set_flip_vertically_on_load(true);


    Player player;

    glfwSetWindowUserPointer(window, &player);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window))
    {

        float currentFrame = glfwGetTime();
        deltatime = currentFrame - lastFrame;
        if (deltatime > 0.1)
            deltatime = 0.1;
        lastFrame = currentFrame;

        processInput(window, player);


        glClearColor(0.2f, 0.3f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_DEPTH_BUFFER_BIT);

        glfwSetCursorPosCallback(window, mouse_callback);
        glfwSetScrollCallback(window, scroll_callback);
        
        
        shader.useShader();
        plane.DrawObject(shader, player, glm::vec3(0.43f,0.25f,0.6f),glm::vec3(0.0f,0.7f,0.0f), 16.0f, currentFrame);
        AddLight(list_light, shader);
        
        shader_flame.useShader();
        AddLight(list_light, shader_flame);

        //sphere.UpdateMesh(sphere_p->coord);
        //sphere1.UpdateMesh(sphere1_p->coord);

        cone1.DrawObject(shader_flame, player, glm::vec3(0.8f,1.0f,0.6f),glm::vec3(0.0f,0.7f,0.0f), 8.0f, currentFrame);
        cone2.DrawObject(shader_flame, player, glm::vec3(0.8f,1.0f,0.6f),glm::vec3(0.0f,0.7f,0.0f), 16.0f, currentFrame);
        

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = player.getViewMatrix();

        shader_model.useShader();
        
        glm::mat4 model = glm::mat4(1.0f);
        /*
        model = glm::translate(model, camera.Position); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down

        shader_model.setMat4("projection", projection);
        shader_model.setMat4("view", view);
        shader_model.setMat4("model", model);

        backpack.Model::DrawModel(shader_model);
        sphere_m.Model::DrawModel(shader_model);
       */
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down

        shader_model.setMat4("projection", projection);
        shader_model.setMat4("view", view);
        shader_model.setMat4("model", model);

        maison.Model::DrawModel(shader_model);

        model = glm::rotate(model, 45.0f, glm::vec3(1, 0, 0));
        shader_model.setMat4("model", model);

        //grass.Model::DrawModel(shader_model);

        //sphere.DrawObject(shader, player, glm::vec3(0.43f,0.25f,0.6f),glm::vec3(0.0f,0.7f,0.0f), 16.0f, currentFrame);
        //sphere1.DrawObject(shader, player, glm::vec3(0.43f,0.25f,0.6f),glm::vec3(0.0f,0.7f,0.0f), 16.0f, currentFrame);

        player.PlayerUpdate();
        ObjectManager::getInstance().updateAllObjects(deltatime);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}
void processInput(GLFWwindow *window, Player &player)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        player.ProcessKeyboard(FORWARD, deltatime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        player.ProcessKeyboard(BACKWARD, deltatime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        player.ProcessKeyboard(LEFT, deltatime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        player.ProcessKeyboard(RIGHT, deltatime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        player.ProcessKeyboard(UP, deltatime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        player.ProcessKeyboard(DOWN, deltatime);
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
if(glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS){
    if(update)
        update = false;
    else
        update = true;
}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{

    Player* player = static_cast<Player*>(glfwGetWindowUserPointer(window));

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

    player->ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    Player* player = static_cast<Player*>(glfwGetWindowUserPointer(window));

    player->ProcessMouseScroll(static_cast<float>(yoffset));
}

GLFWwindow* init()
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
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Erreur de GLAD" << std::endl;
    }

    return window;
}
/*
float loadProgress = 0.0f;
const float totalTasks = 5.0f; // Assume we have 5 assets to load

void updateLoadProgress() {
    loadProgress += 1.0f;
    // Here, you'd normally call the function to render the loading bar with the new progress
    renderLoadingBar(loadProgress / totalTasks);
}
void drawRectangle(float x, float y, float width, float height, glm::vec3 color) {
    // Rectangle vertices (two triangles)
    float vertices[] = {
        x, y, // Top-left
        x + width, y, // Top-right
        x, y + height, // Bottom-left
        x, y + height, // Bottom-left
        x + width, y, // Top-right
        x + width, y + height // Bottom-right
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Use your shader program
    shader2D.use();
    // Set the color uniform
    yourShader.setVec3("color", color);
    // Set the projection matrix
    yourShader.setMat4("projection", projection);

    // Draw the rectangle
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // Cleanup
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void renderLoadingBar(float progress, GLFWwindow* window) {
    // Setup your 2D orthographic projection to draw the bar
    // This is just a conceptual placeholder

    // Background rectangle
    drawRectangle(0.2f, 0.5f, 0.6f, 0.1f, glm::vec3(0.5f, 0.5f, 0.5f)); // Grey background
    // Foreground rectangle (progress)
    drawRectangle(0.2f, 0.5f, 0.6f * progress, 0.1f, glm::vec3(0.0f, 1.0f, 0.0f)); // Green progress

    // Swap buffers to display the loading bar
    glfwSwapBuffers(window); // Assuming 'window' is your GLFWwindow*
    glfwPollEvents();
}

void loadAssets() {
    // Before loading assets, setup the loading bar (clear screen, etc.)

    // Example: Load 5 assets
    for (int i = 0; i < 5; i++) {
        // Load an asset here (e.g., texture, model)
        updateLoadProgress(); // Update progress which also updates the bar
    }

    // Assets are loaded, you can proceed with the rest of your initialization...
}

*/