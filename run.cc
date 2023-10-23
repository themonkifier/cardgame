#include "run.hh"

const unsigned int SCREEN_WIDTH = 800, SCREEN_HEIGHT = 600;

int main(void)
{
    GLFWwindow* window = setup_window();
    if (window == NULL) return -1;

    run(window);

    glfwTerminate();
    return 0;
}

GLFWwindow* setup_window()
{
    if (!glfwInit())
    {
        printf("Failed to initialize GLFW\n");
        return nullptr;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_COMPAT_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Card Game Test", nullptr, nullptr);
    if (window == nullptr)
    {
        printf("Failed to create GLFW window\n");
        glfwTerminate();
        return nullptr;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  
    
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        printf("Failed to initialize GLAD\n");
        return nullptr;
    }
    
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    
    return window;
}

void setup_project(unsigned int &VAO, unsigned int &VBO, unsigned int &EBO)
{
    float vertices[] = {
        // positions                       // texture coords
        0.35f,  0.5f, 0.0f,   1.0f, 1.0f,   // top right
        0.35f, -0.5f, 0.0f,   1.0f, 0.0f,   // bottom right
       -0.35f, -0.5f, 0.0f,   0.0f, 0.0f,   // bottom left
       -0.35f,  0.5f, 0.0f,   0.0f, 1.0f    // top left 
    };

    float texCoords[] = {
        0.0f, 0.0f,  // lower-left corner  
        1.0f, 0.0f,  // lower-right corner
        0.5f, 1.0f   // top-center corner
    };
    
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    }; 
    
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    /* position attribute */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);
    /* texture coord attribute */
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glBindVertexArray(0);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    /* set texture settings - mirrored repeat, linear interpolation with mipmaps, flip on the y-axis */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    stbi_set_flip_vertically_on_load(true);
}

void run(GLFWwindow* window)
{
    Shader* shader = new Shader("../src/vertex_shader.vert", "../src/fragment_shader.frag");

    unsigned int VAO, VBO, EBO;
    setup_project(VAO, VBO, EBO);

    /* load card */
    Card spadesA("../cards/SpadesA.jpg", shader);
    Card spades2("../cards/Spades2.jpg", shader);

    while(!glfwWindowShouldClose(window))
    {
        /* input */
        processInput(window);
        
        /* rendering commands here... */
        glClearColor(0.666f, 0.770f, 0.950f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader->use();

        spadesA.move(glm::vec2(0.5f, 0.5f));
        std::cout << "spadesA " << spadesA.position.x << " " << spadesA.position.y << " " << spadesA.angle << std::endl;
        spadesA.show();
        std::cout << "spadesA " << spadesA.position.x << " " << spadesA.position.y << " " << spadesA.angle << std::endl;

        /* first card */
        glBindTexture(GL_TEXTURE_2D, spadesA.texture);
        glBindVertexArray(VAO);
        
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        /* second card */
        spades2.rotate((float) glfwGetTime());
        std::cout << "spades2 " << spades2.position.x << " " << spades2.position.y << " " << spades2.angle << std::endl;
        spades2.show();
        std::cout << "spades2 " << spades2.position.x << " " << spades2.position.y << " " << spades2.angle << std::endl;
        glBindTexture(GL_TEXTURE_2D, spades2.texture);
        
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        
        /* check and call events and swap the buffers */
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &EBO);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}