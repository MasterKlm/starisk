#include "StariskEditor.h"
#include "../core/Starisk.h"




Starisk* starisk = nullptr;

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
    io->ConfigFlags |= ImGuiConfigFlags_DockingEnable | ImGuiConfigFlags_NavEnableKeyboard; // optional

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
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

    // glfwSetKeyCallback(window, Keyboard::keyCallback);
    // glfwSetCursorPosCallback(window, Mouse::cursorPosCallback);
    // glfwSetMouseButtonCallback(window, Mouse::mouseButtonCallback);
    // glfwSetScrollCallback(window, Mouse::mouseWheelCallback);


}


void StariskEditor::processInput(){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
};

void StariskEditor::displayProject(const char* projectPath)
{
    if(projectDLL != nullptr)
    {
        FreeLibrary(projectDLL);
        projectDLL = nullptr;
        if(starisk) { delete starisk; starisk = nullptr; }
    }

    // Compile the specific project file into a DLL
    std::string compileCmd = std::string("C:/starisk/compile_project.bat ") + 
                             projectPath + " C:/starisk/build/project.dll";
    
    int result = system(compileCmd.c_str());
    if(result != 0)
    {
        std::cout << "Compilation failed for: " << projectPath << "\n";
        return;
    }

    // Small delay to ensure file is written
    Sleep(500);

    projectDLL = LoadLibrary("C:/starisk/build/project.dll");
    if(!projectDLL)
    {
        std::cout << "Failed to load DLL. Error: " << GetLastError() << "\n";
        return;
    }

    typedef void(*RunProjectFn)(Starisk*);
    RunProjectFn runProject = (RunProjectFn)GetProcAddress(projectDLL, "runProject");

    if(runProject)
    {
        starisk = new Starisk();
        runProject(starisk);
        std::cout << "Running project: " << projectPath << "\n";
    }
    else
    {
        std::cout << "Could not find runProject() in DLL\n";
    }
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
            if(starisk != nullptr) starisk->mainLoop();


  
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