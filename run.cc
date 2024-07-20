#include "run.hh"

unsigned int SCREEN_WIDTH = 800, SCREEN_HEIGHT = 600, NUM_PLAYERS;

Game* CardGame;

int main(int argc, char *argv[]) {
    (void) argc;
    (void) argv;
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Dobbs loves my King of Clubs", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    // OpenGL state
    // ------------
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    // compile and setup the shader
    // ----------------------------
    Shader shader("../src/text_vertex.vert", "../src/text_fragment.frag");
    TextRenderer tr(shader);
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(SCREEN_WIDTH), 0.0f, static_cast<float>(SCREEN_HEIGHT));
    shader.Use();
    shader.SetMatrix4("projection", projection);
    CardGame = new Game(tr, window);
    ResourceManager::Shaders["text"] = shader;

    // const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

    // SCREEN_WIDTH = 800; // mode->width;
    // SCREEN_HEIGHT = 600; // mode->height;

    glfwSetKeyCallback(window, keyCallback);
    glfwSetCharCallback(window, characterCallback);
    glfwSetMouseButtonCallback(window, mouseCallback);
    glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);

    // initialize game
    // ---------------
    // this is hardcoded now, but we'll have to (somehow) call
    // it from within the game loop when creating the game ...
    NUM_PLAYERS = 3;
    CardGame->Init();

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

        // manage user input
        // -----------------
        CardGame->ProcessInput(deltaTime);

        // update game state
        // -----------------
        CardGame->Update(deltaTime);
        if (CardGame->MouseInput[0].first) {
            // set xpos, ypos
            glfwGetCursorPos(window, &(CardGame->MouseInput[0].second.first), &(CardGame->MouseInput[0].second.second));
        }

        // render
        // ------
        switch (CardGame->State) {
            using enum Game::GameState;

            case MENU:
            glClearColor(0.133, 0.125, 0.141, 1.0f);
            break;

            case JOIN:
            glClearColor(0.722f, 0.251f, 0.204f, 1.0f);
            break;

            case WIN:
            glClearColor(0.196f, 0.659f, 0.322f, 1.0f);
            break;

            case ACTIVE:
            glClearColor(0.666f, 0.770f, 0.950f, 1.0f);
            break;
        }
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        CardGame->Render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // delete all resources as loaded using the resource manager
    // ---------------------------------------------------------
    ResourceManager::Clear();
    delete CardGame;
    glfwTerminate();

    exit(EXIT_SUCCESS);
}

void mouseCallback(GLFWwindow* window, int button, int state, int mods) {
    (void) mods;
    CardGame->MouseInput[button].first = state; // set isPressed

    glfwGetCursorPos(window,
        &(CardGame->MouseInput[button].second.first), // set xpos
        &(CardGame->MouseInput[button].second.second));// set ypos
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    (void) window;
    (void) scancode;
    (void) mode;

    bool any_selected = false; // if none, use default behavior

    for (auto& [name, tb] : CardGame->text_boxes) {
        if (tb->selected) {
            tb->keyCallback(key, action);
            any_selected = true;
        }
    }

    if (!any_selected && action == GLFW_PRESS) {
        switch (key) {
            case GLFW_KEY_ESCAPE:
            CardGame->OnEscape();
            break;
        }
    }
}

void characterCallback(GLFWwindow* window, unsigned int codepoint) {
    (void) window;

    for (auto& [name, tb] : CardGame->text_boxes) {
        if (tb->selected) {
            tb->characterCallback(codepoint);
        }
    }
}

void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
    (void) window;
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
    SCREEN_WIDTH = width;
    SCREEN_HEIGHT = height;
}
