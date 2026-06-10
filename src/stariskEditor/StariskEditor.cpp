#include "StariskEditor.h"
#include "../core/Starisk.h"




Starisk* starisk = nullptr;


static void chained_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // std::cout << "[INPUT] Key event: " << key << " action: " << action << "\n";
    // Forward to ImGui first
    ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);
    // Then forward to Starisk's keyboard system
    Keyboard::keyCallback(window, key, scancode, action, mods);
}

static void chained_mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);
    Mouse::mouseButtonCallback(window, button, action, mods);
}

static void chained_scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
    Mouse::mouseWheelCallback(window, xoffset, yoffset);
}

static void chained_cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
    ImGui_ImplGlfw_CursorPosCallback(window, xpos, ypos);
    Mouse::cursorPosCallback(window, xpos, ypos);
}

static void editor_framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);

    // Retrieve the StariskEditor instance and update its dimensions
    StariskEditor* editor = static_cast<StariskEditor*>(glfwGetWindowUserPointer(window));
    if(editor)
    {
        editor->WINDOW_WIDTH = width;
        editor->WINDOW_HEIGHT = height;
    }
}




void StariskEditor::createWindow()
{

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Starisk**", nullptr, NULL);

    if(window == NULL){
        std::cout << "Window creaion failed" << "\n";
        glfwTerminate();
        
    }

    glfwMakeContextCurrent(window);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Failed to initialize glad" << '\n';
        glfwTerminate();
        
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    
    io = &ImGui::GetIO();
    ctx = ImGui::GetCurrentContext();
    io->ConfigFlags |= ImGuiConfigFlags_DockingEnable; // optional

    ImGui::StyleColorsDark();

    // glfwSetKeyCallback(window, Keyboard::keyCallback);
    // glfwSetCursorPosCallback(window, Mouse::cursorPosCallback);
    // glfwSetMouseButtonCallback(window, Mouse::mouseButtonCallback);
    // glfwSetScrollCallback(window, Mouse::mouseWheelCallback);

    glfwSetKeyCallback(window, chained_key_callback);
    glfwSetMouseButtonCallback(window, chained_mouse_button_callback);
    glfwSetScrollCallback(window, chained_scroll_callback);
    glfwSetCursorPosCallback(window, chained_cursor_pos_callback);


    ImGui_ImplGlfw_InitForOpenGL(window, false);
    ImGui_ImplOpenGL3_Init("#version 330");


    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* version = glGetString(GL_VERSION);
    std::cout << "GPU: " << renderer << "\n";
    std::cout << "OpenGL version: " << version << "\n";

    //set view port
    
    glViewport(0,0, 800, 600);
    // glEnable(GL_BLEND);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, editor_framebuffer_size_callback);
    glfwMaximizeWindow(window);

    
    initGameFramebuffer(800, 500);
}


void StariskEditor::processInput(){
    // if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    //     glfwSetWindowShouldClose(window, true);
    // if (ImGui::IsKeyPressed(ImGuiKey_Space))
    // {
    //     std::cout << "Space pressed\n";
    // }

};

void StariskEditor::displayProject(const char* projectPath)
{
    if(projectDLL != nullptr)
    {
        FreeLibrary(projectDLL);
        projectDLL = nullptr;
        if(starisk) { delete starisk; starisk = nullptr; }
    }

    std::string sourcePath = projectPath;
    std::string outputPath = sourcePath;

    size_t extPos = outputPath.find_last_of(".");
    if (extPos != std::string::npos) {
        outputPath = outputPath.substr(0, extPos) + ".dll";
    }

    SetEnvironmentVariableA("STARISK_SRC", sourcePath.c_str());
    SetEnvironmentVariableA("STARISK_OUT", outputPath.c_str());

    int result = std::system("compile_project.bat");

    if (result != 0)
    {
        std::cout << "Compilation failed for: " << projectPath << "\n";
        return;
    }

    std::cout << "Compilation successful! Loading " << outputPath << "...\n";
    Sleep(500);

    // FIX 2: Convert relative path to absolute path for LoadLibraryA
    char absolutePath[MAX_PATH];
    GetFullPathNameA(outputPath.c_str(), MAX_PATH, absolutePath, nullptr);

    projectDLL = LoadLibraryA(absolutePath);
    if(!projectDLL)
    {
        std::cout << "Failed to load DLL. Error: " << GetLastError() << "\n";
        return;
    }

    typedef void(*RunProjectFn)(Starisk*, ImGuiContext*);
    RunProjectFn runProject = (RunProjectFn)GetProcAddress(projectDLL, "runProject");

    if(runProject)
    {
        glfwMakeContextCurrent(window);
        std::cout << "[DEBUG] Re-initializing GLAD in starisk_core...\n";
        Starisk::initGLAD((GLADloadproc)glfwGetProcAddress);
        std::cout << "[DEBUG] Creating Starisk...\n";
        starisk = new Starisk(window);
        StariskSettings::WINDOW_WIDTH = gameViewWidth; 
        StariskSettings::WINDOW_HEIGHT = gameViewHeight;
        std::cout << "[DEBUG] Starisk created. Calling runProject...\n";
        runProject(starisk, ctx);
        std::cout << "Running project: " << projectPath << "\n";
    }
    else
    {
        std::cout << "Could not find runProject() in DLL\n";
    }
}

void StariskEditor::initGameFramebuffer(int w, int h)
{
    gameViewWidth = w; gameViewHeight = h;

    glGenFramebuffers(1, &gameFramebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, gameFramebuffer);

    glGenTextures(1, &gameColorTexture);
    glBindTexture(GL_TEXTURE_2D, gameColorTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gameColorTexture, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void StariskEditor::mainLoop()
{
    while(!glfwWindowShouldClose(window))
        {
            //proccess input
            processInput();

            //render
            glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            //render
            // Render game into FBO
            if (starisk != nullptr) {
                glBindFramebuffer(GL_FRAMEBUFFER, gameFramebuffer);
                glViewport(0, 0, gameViewWidth, gameViewHeight);
                glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT);

                starisk->mainLoop();

                glBindFramebuffer(GL_FRAMEBUFFER, 0);
                glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
            }


           
            ImGui::SetNextWindowPos(ImVec2(0,0));
            ImGui::Begin("Game View");
            ImGui::Image((ImTextureID)(intptr_t)gameColorTexture,
                        ImVec2(gameViewWidth, gameViewHeight),
                        ImVec2(0,1), ImVec2(1,0)); // flipped UV because OpenGL
            ImGui::End();

        
            ImGui::SetNextWindowPos(ImVec2(0,WINDOW_HEIGHT - (WINDOW_HEIGHT * 0.30)));
            ImGui::SetNextWindowSize(ImVec2(WINDOW_WIDTH, (WINDOW_HEIGHT * 0.30)));
            ImGui::Begin("DockSpace", nullptr, 
                ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove |
                ImGuiWindowFlags_NoResize   | ImGuiWindowFlags_NoBringToFrontOnFocus
            );
                ImGui::DockSpace(ImGui::GetID("MyDockspace"), ImVec2(0,0));
            ImGui::End();
                
            // int debugPanelWidth = WINDOW_WIDTH * 0.10;
            // ImGui::SetNextWindowPos(ImVec2(WINDOW_WIDTH - debugPanelWidth,0));
            // ImGui::SetNextWindowSize(ImVec2(debugPanelWidth,0));

            ImGui::Begin("Debug");
            ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
            ImGui::End();

            ImGui::Begin("Files");
            if(ImGui::Button("Open Project"))
            {
                displayProject("./src/projects/pong/main.cpp");
            }
            ImGui::End();

           
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        



            glfwSwapBuffers(window);
            glfwPollEvents();


        }

        

    glfwTerminate();
   
}




StariskEditor::~StariskEditor()
{
    if(projectDLL) FreeLibrary(projectDLL);
    if(starisk) delete starisk;
}