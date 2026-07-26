#include "StariskEditor.h"
#include "StariskEditorUtils.h"
#include "../core/Starisk.h"

const double targetFPS = 60.0;
const auto targetFrameTime = std::chrono::duration<double>(1.0 / targetFPS);



Starisk* starisk = nullptr;

StariskEditor::StariskEditor(){
}

static void chained_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);
    Keyboard::keyCallback(window, key, scancode, action, mods); // keep for editor-side use

    // Also push the event into the loaded game's Starisk instance
    if (starisk)
        starisk->injectKeyEvent(window, key, scancode, action, mods);
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

    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Starisk** Engine", nullptr, NULL);

    if(window == NULL){
        std::cout << "Window creaion failed" << "\n";
        glfwTerminate();
        
    }

    glfwMakeContextCurrent(window);


    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Failed to initialize glad" << '\n';
        glfwTerminate();
        
    }

    icons["folder_icon"] = LoadTextureFromFile("assets/img/folder_icon.png");


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

void StariskEditor::closeCurrentProject()
{
    if(projectDLL != nullptr)
    {
    
        if(starisk != nullptr){
            delete starisk;
            starisk = nullptr;
        }

        FreeLibrary(projectDLL);
        projectDLL = nullptr;
    }
}


void StariskEditor::selectGameProjectUI()
{
    std::string rootProjectsPath = std::filesystem::absolute("./src/projects/").string();
    static std::string selectedPath = rootProjectsPath;
    static std::stack<std::string> prevPathsStack;
    prevPathsStack.push(rootProjectsPath);
    static std::vector<std::string> paths = getChildPaths(rootProjectsPath);

    ImGui::SetNextWindowSize(ImVec2(600, WINDOW_HEIGHT - gameViewHeight));
    ImGui::Begin("Files");


    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
    
    for(auto& p : paths){
        ImGui::PushID(p.c_str());
        if(ImGui::ImageButton("##folder_icon", (ImTextureID)(intptr_t)icons["folder_icon"], ImVec2(64, 64))){
            if(std::filesystem::is_directory(std::filesystem::path(p))){
                paths = getChildPaths(p);
                prevPathsStack.push(p);
                selectedPath = p + "\\main.cpp";

            }

        }

        ImGui::Text(p.c_str());
            
        ImGui::SameLine();
        ImGui::PopID();
    }

    ImGui::PopStyleColor(1);

    if(ImGui::Button("Back")){
        if(prevPathsStack.size() > 0){
            std::string prevPath = prevPathsStack.top();
            std::cout << prevPath << "\n";
            paths = getChildPaths(prevPath);
            prevPathsStack.pop();
        }
    }
    ImGui::SameLine();

    
    std::string currentDisplayedPathMessage = "Open Project: " +  selectedPath;
    if(ImGui::Button(currentDisplayedPathMessage.c_str()))
    {
        if(!selectedPath.empty()) displayProject(selectedPath.c_str());
    }
    if(ImGui::Button("Close Project", ImVec2(95, 20)))
    {
        closeCurrentProject();
    }
    ImGui::End();
}

void StariskEditor::displayProject(const char* projectPath)
{
    closeCurrentProject();

    std::string sourcePath = projectPath;
    std::string outputPath = sourcePath;

    size_t extPos = outputPath.find_last_of(".");
    if (extPos != std::string::npos) {
        outputPath = outputPath.substr(0, extPos) + ".dll"; // project filename.dll
    }

    SetEnvironmentVariableA("STARISK_SRC", sourcePath.c_str());
    SetEnvironmentVariableA("STARISK_OUT", outputPath.c_str());

    int result = std::system(std::filesystem::absolute("compile_project.bat").string().c_str());

    if (result != 0)
    {
        std::string message = std::string(projectPath);
        size_t projectsPos = message.find("projects\\");
        if (projectsPos != std::string::npos)
        {
            message = "Compilation failed for: " + message.substr(projectsPos);
        }
        const size_t maxLen = 40; 
        if (message.size() > maxLen)
        {
            message = message.substr(0, maxLen) + "...";
        }

        addPopupMessage(message);
        return;
    }

    std::string compileSuccessMessage = std::string(outputPath);
    size_t projectPos = compileSuccessMessage.find("projects\\");
    if(projectPos != std::string::npos){
        size_t gameNameStartPos = projectPos + std::string("projects\\").size();
        size_t gameNameEndPos = compileSuccessMessage.find("\\main.dll");
        if(gameNameEndPos != std::string::npos){
            compileSuccessMessage = "Opened Game: " + compileSuccessMessage.substr(gameNameStartPos, gameNameEndPos - gameNameStartPos);

        }
    }
    addPopupMessage(compileSuccessMessage);
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

    typedef Starisk*(*RunProjectFn)(GLFWwindow*, ImGuiContext*);
    RunProjectFn runProject = (RunProjectFn)GetProcAddress(projectDLL, "mainProject");
    
    StariskSettings::WINDOW_WIDTH = gameViewWidth; 
    StariskSettings::WINDOW_HEIGHT = gameViewHeight;

    if(runProject)
    {
        glfwMakeContextCurrent(window);
        std::cout << "[DEBUG] Re-initializing GLAD in starisk_core...\n";
        Starisk::initGLAD((GLADloadproc)glfwGetProcAddress);
        std::cout << "[DEBUG] Syncing ImGui context in starisk_core...\n";
        Starisk::initImGuiContext(ctx);
        std::cout << "[DEBUG] Creating Starisk...\n";
        starisk = runProject(window, ctx);
        StariskSettings::WINDOW_WIDTH = gameViewWidth; 
        StariskSettings::WINDOW_HEIGHT = gameViewHeight;
        std::cout << "[DEBUG] Starisk created. Calling runProject...\n";
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
    while(!glfwWindowShouldClose(window)){
        auto frameStart = std::chrono::high_resolution_clock::now();
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

            starisk->mainLoop(true);

            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
        }


        
        ImGui::SetNextWindowPos(ImVec2(200,0));
        ImGui::SetNextWindowSize(ImVec2(gameViewWidth, gameViewHeight));
        ImGui::Begin("Game View");
        ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
        if(starisk != nullptr){

            ImGui::Image((ImTextureID)(intptr_t)gameColorTexture,
                    ImVec2(gameViewWidth, gameViewHeight),
                    ImVec2(0,1), ImVec2(1,0)); // flipped UV because OpenGL
        }
        ImGui::End();

    
        ImGui::SetNextWindowPos(ImVec2(0,WINDOW_HEIGHT - (WINDOW_HEIGHT * 0.30)));
        ImGui::SetNextWindowSize(ImVec2(WINDOW_WIDTH, (WINDOW_HEIGHT * 0.30)));
        ImGui::Begin("DockSpace", nullptr, 
            ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoResize   | ImGuiWindowFlags_NoBringToFrontOnFocus
        );
            ImGui::DockSpace(ImGui::GetID("MyDockspace"), ImVec2(0,WINDOW_HEIGHT - gameViewHeight));
        ImGui::End();
            
        // int debugPanelWidth = WINDOW_WIDTH * 0.10;
        // ImGui::SetNextWindowPos(ImVec2(WINDOW_WIDTH - debugPanelWidth,0));
        // ImGui::SetNextWindowSize(ImVec2(debugPanelWidth,0));

        selectGameProjectUI();
        showPopups();
        
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    


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


void StariskEditor::showPopups()
{
    if(!popupMessagesStack.empty()){
        const std::string id = "Popup##" + popupMessagesStack.top();

        ImGui::SetNextWindowSizeConstraints(
            ImVec2(250, 0),           // min size (no minimum)
            ImVec2(300, FLT_MAX)    // max size: width capped at 400, height unlimited
        );
        ImGui::SetNextWindowPos(ImVec2(WINDOW_WIDTH - 270, WINDOW_HEIGHT - 100));
        ImGui::SetNextWindowBgAlpha(0.9f); // optional: slight transparency, toast-style

        ImGui::Begin(id.c_str(), nullptr,
            ImGuiWindowFlags_NoTitleBar   |
            ImGuiWindowFlags_NoResize     |
            ImGuiWindowFlags_NoMove       |
            ImGuiWindowFlags_AlwaysAutoResize |
            ImGuiWindowFlags_NoSavedSettings
        );

        ImGui::TextWrapped("%s", popupMessagesStack.top().c_str());
        // ImGui::SameLine();
        if(ImGui::Button("OK"))
        {
            popupMessagesStack.pop();
        }

        ImGui::End();
    }
}  
void StariskEditor::addPopupMessage(std::string msg){ popupMessagesStack.push(msg); }

StariskEditor::~StariskEditor()
{
    closeCurrentProject();
    if(starisk) delete starisk;
}