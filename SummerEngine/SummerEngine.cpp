#include <iostream>
#include <GL/glew.h>
#include <GL/GL.h>
#include <SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ShaderUtils.h"
#include "stb_image.h"

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

void display_max_vertex_attrs()
{
    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::cout << "Maximum nr of vertex attributes supported is " << nrAttributes << std::endl;
}

void render(SDL_Window* window,
    unsigned int shaderProgram,
    unsigned int VAO,
    unsigned int texture,
    unsigned int texture1,
    float timeUnits,
    float gamma,
    glm::vec3 cubePositions[],
    int screenWidth,
    int screenHeight)
{

    glClearColor(0.2f, 0.3f, 0.3f, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 view;
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(45.0f), (float)(screenWidth / screenHeight), 0.1f, 100.0f);

    glUseProgram(shaderProgram);
    //set up uniforms
    glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 0);
    glUniform1i(glGetUniformLocation(shaderProgram, "texture2"), 1);
    glUniform1f(glGetUniformLocation(shaderProgram, "gamma"), gamma);
    //unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
    //glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture1);

    glBindVertexArray(VAO);
    for (int i = 0; i < 10; i++) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, cubePositions[i]);

        model = glm::rotate(model, glm::radians(20.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        if (i % 3 == 0) {
            model = glm::rotate(model, glm::radians(timeUnits), glm::vec3(1.0f, 0.3f, 0.5f));
        }
        else if (i % 3 == 1) {
            model = glm::rotate(model, -glm::radians(timeUnits), glm::vec3(0.7f, 0.6f, 0.2f));
        }
        else {
            model = glm::rotate(model, glm::radians(timeUnits), glm::vec3(0.0f, 0.5f, 0.8f));
        }
        unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }
    glBindVertexArray(0);

    SDL_GL_SwapWindow(window);
}

int main(int argc, char** argv) {

    const int screenWidth = 1200;
    const int screenHeight = 800;

    SDL_Init(SDL_INIT_VIDEO);

    SDL_GLContext context;
    SDL_Window* window = SDL_CreateWindow
    ("opengl-engine", // window's title
        10, 25, // coordinates on the screen, in pixels, of the window's upper left corner
        screenWidth, screenHeight,
        SDL_WINDOW_OPENGL);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    context = SDL_GL_CreateContext(window);
    if (context == NULL) {
        std::cerr << "SDL_GL_CreateContext ERROR:" << SDL_GetError() << std::endl;
        return 1;
    }

    glewExperimental = GL_TRUE;
    GLenum err = glewInit();

    if (err != GLEW_OK) {
        std::cerr << err << std::endl;
        return 1;
    }

    glViewport(0, 0, screenWidth, screenHeight);
    //glMatrixMode(GL_PROJECTION);
    //glOrtho(-1.0*screenWidth/screenHeight, 1.0*screenWidth/screenHeight, -1.0, 1.0, -1.0, 1.0);

    //glMatrixMode(GL_MODELVIEW);
    //glLoadIdentity();

    glEnable(GL_DEPTH_TEST);
    //display_max_vertex_attrs()

    std::string vertexShaderFileName = std::string(ROOT_PATH) + "/VertexShader.glsl";
    std::string fragmentShaderFileName = std::string(ROOT_PATH) + "/FragmentShader.glsl";
    ShaderProgram shaderProgram(vertexShaderFileName, fragmentShaderFileName);

    if (shaderProgram.hasError()) {
        std::cerr << shaderProgram.getError() << std::endl;
        return 1;
    }

    float vertices[] = {
                -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
                0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
                0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,

                -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
                0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
                0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
                -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,

                -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

                0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

                -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
                0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
                -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

                -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
                0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    };

    unsigned int indices[] = {  // note that we start from 0!
                  0, 1, 3,   // first triangle
                  1, 2, 3,    // second triangle

                  4, 5, 7,
                  5, 6, 7,

                  8, 9, 11,
                  9, 10, 11,

                  12, 13, 15,
                  13, 14, 15,

                  16, 17, 19,
                  17, 18, 19,

                  20, 21, 23,
                  21, 22, 23
    };

    glm::vec3 cubePositions[] = {
                     glm::vec3(0.0f,  0.0f,  0.0f),
                     glm::vec3(2.0f,  5.0f, -15.0f),
                     glm::vec3(-1.5f, -2.2f, -2.5f),
                     glm::vec3(-3.8f, -2.0f, -12.3f),
                     glm::vec3(2.4f, -0.4f, -3.5f),
                     glm::vec3(-1.7f,  3.0f, -7.5f),
                     glm::vec3(1.3f, -2.0f, -2.5f),
                     glm::vec3(1.5f,  2.0f, -2.5f),
                     glm::vec3(1.5f,  0.2f, -1.5f),
                     glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    unsigned int texture, texture1;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;

    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(ROOT_PATH"/wood_container.jpg", &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cerr << "Failed to load the image" << std::endl;
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);

    glGenTextures(1, &texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture1);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    data = stbi_load(ROOT_PATH"/awesomeimage.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cerr << "Failed to load the second image" << std::endl;
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);

    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    SDL_GL_SetSwapInterval(1);
    bool isRunning = true;
    bool wireframeMode = false;
    float gamma = 0;
    float timeUnits = 0;
    float cameraSpeed = 0.1f;
    int mousePosX, mousePosY;
    int prevMousePosX, prevMousePosY;
    float xoffset, yoffset;
    float yaw = -90.0f, pitch = 0.0f;
    Uint32 frameStart, frameDuration;
    Uint32 startTime = SDL_GetTicks();
    Uint32 currentTime;
    const Uint32 FRAME_TIME = 1000 / 30;
    srand(SDL_GetTicks());
    const Uint8* keyboardState = SDL_GetKeyboardState(NULL);

    // init mouse to center of window
    SDL_WarpMouseInWindow(window, screenWidth / 2, screenHeight / 2);

    // init prevMouse positions
    SDL_PumpEvents();
    SDL_GetMouseState(&mousePosX, &mousePosY);
    prevMousePosX = mousePosX;
    prevMousePosY = mousePosY;


    while (isRunning) {
        frameStart = SDL_GetTicks();
        currentTime = frameStart;
        if (currentTime - startTime >= 10) {
            timeUnits++;
            startTime = currentTime;
        }

        // TODO add delay for inputs

        // camera motion should occur without mouse click
        SDL_PumpEvents();
        if (SDL_GetMouseState(&mousePosX, &mousePosY)) {
            // camera movement code
            xoffset = mousePosX - prevMousePosX;
            yoffset = mousePosY - prevMousePosY;
            prevMousePosX = mousePosX;
            prevMousePosY = mousePosY;

            float sensitivity = 0.05f;
            xoffset *= sensitivity;
            yoffset *= sensitivity;

            yaw += xoffset;
            pitch += yoffset;

            if (pitch > 89.0f)
                pitch = 89.0f;
            if (pitch < -89.0f)
                pitch = -89.0f;

            glm::vec3 front;
            front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
            front.y = sin(glm::radians(pitch));
            front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
            cameraFront = glm::normalize(front);
            // ^^^ camera movement code ^^^

        }
        else {
            prevMousePosX = mousePosX;
            prevMousePosY = mousePosY;
        }

        if (keyboardState[SDL_SCANCODE_ESCAPE]) {
            isRunning = false;
        }

        if (keyboardState[SDL_SCANCODE_G]) {
            gamma += 0.01f;
        }

        if (keyboardState[SDL_SCANCODE_H]) {
            gamma -= 0.01f;
        }

        if (keyboardState[SDL_SCANCODE_W]) {
            cameraPos += cameraSpeed * cameraFront;
        }

        if (keyboardState[SDL_SCANCODE_S]) {
            cameraPos -= cameraSpeed * cameraFront;
        }

        if (keyboardState[SDL_SCANCODE_A]) {
            cameraPos -= cameraSpeed * glm::normalize(glm::cross(cameraFront, cameraUp));
        }

        if (keyboardState[SDL_SCANCODE_D]) {
            cameraPos += cameraSpeed * glm::normalize(glm::cross(cameraFront, cameraUp));
        }

        if (keyboardState[SDL_SCANCODE_Z]) {
            wireframeMode = !wireframeMode;
            glPolygonMode(GL_FRONT_AND_BACK, wireframeMode ? GL_LINE : GL_FILL);
        }

        render(window, shaderProgram.program(), VAO, texture, texture1, timeUnits, gamma, cubePositions, screenWidth, screenHeight);
        frameDuration = SDL_GetTicks() - frameStart;
        if (frameDuration < FRAME_TIME) {
            SDL_Delay(FRAME_TIME - frameDuration);
        }
    }
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}