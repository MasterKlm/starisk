#include "Starisk.h"

const double targetFPS = 60.0;
const auto targetFrameTime = std::chrono::duration<double>(1.0 / targetFPS);


   

Starisk::Starisk()
{
    

}

Starisk::Starisk(GLFWwindow* existingWindow)
{

    std::cout << "[DEBUG] Starisk(window) called\n";
    window = existingWindow;
    if (!glfwGetCurrentContext()) {
        init();
    }
    std::cout << "[DEBUG] Calling initStariskBaseSystems...\n";
    initStariskBaseSystems();
    std::cout << "[DEBUG] initStariskBaseSystems done\n";
}

void Starisk::initGLAD(GLADloadproc loaderFunc)
{
    if (!gladLoadGLLoader(loaderFunc)) {
        std::cout << "[Starisk] GLAD re-init failed\n";
    } else {
        std::cout << "[Starisk] GLAD re-init success\n";
    }
}

void Starisk::initImGuiContext(ImGuiContext* ctx)
{
    ImGui::SetCurrentContext(ctx);
}

void Starisk::init()
{
    //intialize glfw

    glfwInit();

    //version 3.4
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //apple only
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
}


void Starisk::createWindow(int width, int height, const char* title)
{
    if (!glfwGetCurrentContext()) {
        init();
    }
    
    StariskSettings::WINDOW_WIDTH = width;
    StariskSettings::WINDOW_HEIGHT = height;

    window = glfwCreateWindow(StariskSettings::WINDOW_WIDTH, StariskSettings::WINDOW_HEIGHT, title, NULL, NULL);

    if(window == NULL){
        std::cout << "Window creation failed" << "\n";
        glfwTerminate();
        
    }

    glfwMakeContextCurrent(window);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Failed to initialize glad" << '\n';
        glfwTerminate();
        
    }

    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* version = glGetString(GL_VERSION);
    std::cout << "GPU: " << renderer << "\n";
    std::cout << "OpenGL version: " << version << "\n";

    //set view port
    
    glViewport(0,0, 800, 600);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, Keyboard::keyCallback);
    glfwSetCursorPosCallback(window, Mouse::cursorPosCallback);
    glfwSetMouseButtonCallback(window, Mouse::mouseButtonCallback);
    glfwSetScrollCallback(window, Mouse::mouseWheelCallback);
    
    initStariskBaseSystems();
    // auto& kms = sem.addSystem<KeyboardMovementSystem>();
}

void Starisk::initStariskBaseSystems()
{
    std::cout << "[DEBUG] Creating StarBatchManager...\n";
    sbm = new StarBatchManager();
    std::cout << "[DEBUG] StarBatchManager created\n";
    auto& ts = sem.addSystem<TransformSystem>();
    std::cout << "[DEBUG] TransformSystem added\n";
}

void Starisk::processInput(){
    if(window == nullptr) 
    {
        std::cout << "Create Window Using Starisk.createWindow() First before calling Starisk.processInput()" << "\n";
        return;
    }

    if(glfwGetKey(window, GLFW_KEY_ESCAPE)==GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }
}


Entity& Starisk::CreateEntity(float xpos, float ypos, int width, int height, const char* texturePath)
{   
    
    Entity& e = sem.addEntity();
    e.addComponent<TransformComponent>(xpos, ypos);
    e.quad = CreateQuad(xpos, ypos, width, height, texturePath);
    return e;
    
}

Entity& Starisk::CreateEntity(float xpos, float ypos, int width, int height)
{   
    
    Entity& e = sem.addEntity();
    const char* t = "assets/img/texture_placeholder.png";
    e.addComponent<TransformComponent>(xpos, ypos);
    e.quad = CreateQuad(xpos, ypos, width, height, t);
    return e;
    
}

Entity& Starisk::CreateEntity(float xpos, float ypos, int width, int height,  glm::vec4 rgba)
{   
    
    Entity& e = sem.addEntity();
    e.addComponent<TransformComponent>(xpos, ypos);
    e.quad = CreateQuad(xpos, ypos, width, height, rgba);
    return e;
    
}


Entity& Starisk::CreateEntity(float xpos, float ypos, int width, int height, const char* texturePath, float speed)
{   
    
    Entity& e = sem.addEntity();
    e.addComponent<TransformComponent>(xpos, ypos, speed);
    e.quad = CreateQuad(xpos, ypos, width, height, texturePath);
    return e;
    
}

Entity& Starisk::CreateEntity(float xpos, float ypos, int width, int height, float speed)
{   
    
    Entity& e = sem.addEntity();
    const char* t = "assets/img/texture_placeholder.png";
    e.addComponent<TransformComponent>(xpos, ypos, speed);
    e.quad = CreateQuad(xpos, ypos, width, height, t);
    return e;
    
}

Entity& Starisk::CreateEntity(float xpos, float ypos, int width, int height,  glm::vec4 rgba, float speed)
{   
    
    Entity& e = sem.addEntity();
    e.addComponent<TransformComponent>(xpos, ypos, speed);
    e.quad = CreateQuad(xpos, ypos, width, height, rgba);
    return e;
    
}

StarQuad Starisk::CreateQuad(float x_pos, float y_pos, float width, float height, const char* texturePath)
{
    StarQuad quad{x_pos, y_pos, width, height, texturePath};
    quad.sbm_data = sbm->add(quad.vertices);

    return quad;
}

StarQuad Starisk::CreateQuad(float x_pos, float y_pos, float width, float height, glm::vec4 rgba)
{
    StarQuad quad{x_pos, y_pos, width, height, rgba};
    quad.sbm_data = sbm->add(quad.vertices);
    //std::cout << "Quad batch id: " << quad.sbm_data.starBatchIndex << "\n";
    //std::cout << "Buffer start index: " << quad.sbm_data.startBufferIndex << "\n";

    return quad;
}


StarLine Starisk::CreateLine(StarVec2D p1, StarVec2D p2)
{
    StarLine line(p1, p2);
    line.sbm_data = sbm->drawLine(line.p1, line.p2, StariskSettings::WINDOW_WIDTH, StariskSettings::WINDOW_HEIGHT);

    return line;
}

void Starisk::injectKeyEvent(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    Keyboard::keyCallback(window, key, scancode, action, mods);
}

void Starisk::mainLoop(std::function<void()> fn, bool isEditor)
{

    if(window == nullptr) 
    {
        std::cout << "Create Window Using Starisk.createWindow() First\n";
        return;
    }

    if(isEditor)
    {
        
        sbm->update();
        sem.update(sbm);
        fn();
        return;
    }

    while(!glfwWindowShouldClose(window))
    {
        auto frameStart = std::chrono::high_resolution_clock::now();
        processInput();

        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        sbm->update();
        sem.update(sbm);
        fn();


        auto frameEnd = std::chrono::high_resolution_clock::now();
        auto elapsed = frameEnd - frameStart;

        if (elapsed < targetFrameTime)
        {
            std::this_thread::sleep_for(targetFrameTime - elapsed);
        }

        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

   
}


void Starisk::mainLoop(bool isEditor)
{
    if(window == nullptr) 
    {
        std::cout << "Create Window Using Starisk.createWindow() First\n";
        return;
    }

    if(isEditor)
    {
       
        sbm->update();
        sem.update(sbm);
        return;
    }

    // Standalone mode
    while(!glfwWindowShouldClose(window))
    {
        auto frameStart = std::chrono::high_resolution_clock::now();
        processInput();

        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        sbm->update();
        sem.update(sbm);

        auto frameEnd = std::chrono::high_resolution_clock::now();
        auto elapsed = frameEnd - frameStart;

        if (elapsed < targetFrameTime)
        {
            std::this_thread::sleep_for(targetFrameTime - elapsed);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
}

void Starisk::setGameRunning(bool v)
{
    gameRunning = v;
}

bool Starisk::isGameRunning(){ return gameRunning; }




Starisk::~Starisk(){
    delete sbm;
}