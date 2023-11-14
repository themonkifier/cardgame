#include "run.hh"

unsigned int SCREEN_WIDTH = 800, SCREEN_HEIGHT = 600, NUM_PLAYERS;

Game* CardGame = new Game(SCREEN_WIDTH, SCREEN_HEIGHT);

int main(int argc, char *argv[]) {
    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW" << std::endl;
        exit(EXIT_FAILURE);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #ifdef __APPLE__
      glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    // const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

    // SCREEN_WIDTH = 800; // mode->width;
    // SCREEN_HEIGHT = 600; // mode->height;

    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Card Game Test", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    glfwSetKeyCallback(window, key_callback);
    glfwSetCharCallback(window, character_callback);
    glfwSetMouseButtonCallback(window, mouse_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        exit(EXIT_FAILURE);
    }

    // OpenGL configuration
    // --------------------
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // initialize game
    // ---------------
    // this is hardcoded now, but we'll have to (somehow) call
    // it from within the game loop when creating the game ...
    NUM_PLAYERS = 3;
    CardGame->InitMenu();

    // deltaTime variables
    // -------------------
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    while (!glfwWindowShouldClose(window))
    {
        // calculate delta time
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        glfwPollEvents();

        // manage user input
        // -----------------
        CardGame->ProcessInput(deltaTime);

        // update game state
        // -----------------
        CardGame->Update(deltaTime);
        if (CardGame->MouseInput[0].first) glfwGetCursorPos(window,
                                            &(CardGame->MouseInput[0].second.first), // set xpos
                                            &(CardGame->MouseInput[0].second.second));// set ypos)

        // render
        // ------
        switch (CardGame->State) {
            case Game::GameState::MENU:
            glClearColor(0.133, 0.125, 0.141, 1.0f);
            break;
            
            case Game::GameState::WIN:
            glClearColor(0.196f, 0.659f, 0.322f, 1.0f);
            break;
            
            case Game::GameState::ACTIVE:
            glClearColor(0.666f, 0.770f, 0.950f, 1.0f);
            break;
        }
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        CardGame->Render();

        glfwSwapBuffers(window);
    }

    // delete all resources as loaded using the resource manager
    // ---------------------------------------------------------
    ResourceManager::Clear();
    delete CardGame;

    glfwTerminate();
    exit(EXIT_SUCCESS);
}

void mouse_callback(GLFWwindow* window, int button, int state, int mods) {
    CardGame->MouseInput[button].first = state; // set isPressed

    glfwGetCursorPos(window,
        &(CardGame->MouseInput[button].second.first), // set xpos
        &(CardGame->MouseInput[button].second.second));// set ypos
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    // when a user presses the escape key, we set the WindowShouldClose property to true, closing the application
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    else if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
            CardGame->Keys[key] = true;
        else if (action == GLFW_RELEASE)
            CardGame->Keys[key] = false;
    }
}

void character_callback(GLFWwindow* window, unsigned int codepoint) {
    ;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
    SCREEN_WIDTH = width;
    SCREEN_HEIGHT = height;
}