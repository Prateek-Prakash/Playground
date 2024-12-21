//
//  Main.cpp
//  Playground
//
//  Created by Prateek Prakash on 5/11/24.
//

#include <iostream>
using namespace std;

#include <fstream>
#include <sstream>

#include <GL/glew.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <OpenGL/glu.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>

double xScreen = -1.0;
double yScreen = -1.0;

const float TRANSLATION_STEP = 0.02f;
float xTranslate = 0.0f;
float yTranslate = 0.0f;
float zTranslate = 0.0f;

float xAngle = 0.0f;
float yAngle = 0.0f;

float zoomFactor = 1.0f;

bool fillMode = true;
bool mouseDown = false;

double prevX = 0.0f;
double prevY = 0.0f;

static void handleError(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}

static void handleKey(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    if (key == GLFW_KEY_RIGHT && (action == GLFW_REPEAT || action == GLFW_PRESS)) {
        xTranslate += TRANSLATION_STEP;
    }
    if (key == GLFW_KEY_LEFT && (action == GLFW_REPEAT || action == GLFW_PRESS)) {
        xTranslate -= TRANSLATION_STEP;
    }
    if (key == GLFW_KEY_UP && (action == GLFW_REPEAT || action == GLFW_PRESS)) {
        yTranslate += TRANSLATION_STEP;
    }
    if (key == GLFW_KEY_DOWN && (action == GLFW_REPEAT || action == GLFW_PRESS)) {
        yTranslate -= TRANSLATION_STEP;
    }
    if (key == GLFW_KEY_F && action == GLFW_PRESS) {
        fillMode = !fillMode;
        cout << "fillMode: " << fillMode << endl;
    }
}

static void handleMouseButton(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        mouseDown = true;
        glfwGetCursorPos(window, &prevX, &prevY);
    }
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        mouseDown = false;
    }
}

static void handleScroll(GLFWwindow* window, double xoffset, double yoffset) {
    cout << "Scroll Offset: " << yoffset << endl;
    zoomFactor += yoffset;
}

static void handleCursorPosition(GLFWwindow* window, double xpos, double ypos) {
    if (mouseDown) {
        cout << "Cursor: (" << xpos << ", " << ypos << ")" << endl;
        
        xAngle += (prevY - ypos) / 2;
        prevY = ypos;
        
        yAngle += (prevX - xpos) / 2;
        prevX = xpos;
    }
}

int main(int argc, const char * argv[]) {
    glfwSetErrorCallback(handleError);
    
    // GLFW Initialization
    if (!glfwInit()) {
        cerr << "GLFW Initialization: Failure" << endl;
        exit(EXIT_FAILURE);
    }
    cout << "GLFW Initialization: Success" << endl;
    
    // Window Creation
    GLFWwindow* glfwWin = glfwCreateWindow(1024, 1024, "OpenGL Playground", NULL, NULL);
    if (!glfwWin) {
        cerr << "Failed Creating GLFW Window" << endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    
    // GLFW Stuff
    glfwSetKeyCallback(glfwWin, handleKey);
    glfwSetMouseButtonCallback(glfwWin, handleMouseButton);
    glfwSetScrollCallback(glfwWin, handleScroll);
    glfwSetCursorPosCallback(glfwWin, handleCursorPosition);
    glfwMakeContextCurrent(glfwWin);
    glfwSwapInterval(1);
    
    // GLEW Initialization
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        cerr << "GLEW Initialization: Failure" << endl;
        exit(EXIT_FAILURE);
    }
    cout << "GLEW Initialization: Success" << endl;
    
    // MAIN LOOP
    while (!glfwWindowShouldClose(glfwWin)) {
        int winWidth, winHeight;
        glfwGetWindowSize(glfwWin, &winWidth, &winHeight);
        int winRatio = winWidth / winHeight;
        
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClearDepth(1.0f);
        //Anti-Aliasing
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_LINE_SMOOTH);
        glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
        glEnable(GL_DEPTH_TEST);
        
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glFrustum(-(winWidth / 2) * winRatio, (winWidth / 2) * winRatio, (winHeight / 2), -(winHeight / 2), 0.0f, 1000.f);
        
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        if (fillMode == true)
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        
        glTranslatef(xTranslate, yTranslate, zTranslate);
        glRotatef(xAngle, 1.0, 0.0, 0.0);
        glRotatef(yAngle, 0.0, 1.0, 0.0);
        glRotatef(0.0f, 0.0, 0.0, 1.0);
        
        glBegin(GL_POLYGON);
            glColor3f(1.0f, 0.0f, 0.0f);
            glVertex3f(-0.5f, -0.5f, -0.5f);
            glColor3f(0.0f, 1.0f, 0.0f);
            glVertex3f(0.5f, -0.5f, -0.5f);
            glColor3f(0.0f, 0.0f, 1.0f);
            glVertex3f(0.5f, 0.5f, -0.5f);
            glColor3f(1.0f, 1.0f, 1.0f);
            glVertex3f(-0.5f, 0.5f, -0.5f);
        glEnd();
        
        glBegin(GL_POLYGON);
            glColor3f(1.0f, 0.0f, 0.0f);
            glVertex3f(-0.5f, -0.5f, 0.5f);
            glColor3f(0.0f, 1.0f, 0.0f);
            glVertex3f(0.5f, -0.5f, 0.5f);
            glColor3f(0.0f, 0.0f, 1.0f);
            glVertex3f(0.5f, 0.5f, 0.5f);
            glColor3f(1.0f, 1.0f, 1.0f);
            glVertex3f(-0.5f, 0.5f, 0.5f);
        glEnd();
        
        // Buffer & Poll
        glfwSwapBuffers(glfwWin);
        glfwPollEvents();
    }
    
    // Termination
    cout << "Terminating GLFW..." << endl;
    glfwDestroyWindow(glfwWin);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
