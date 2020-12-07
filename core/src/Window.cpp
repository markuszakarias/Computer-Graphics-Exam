#include "Window.h"

/**
*  Window class that handles context creation and window management/input.
*  This class can create a user defined size, if that is not given general size is 800x600.
*  Mouse management is also done through this class because that is window specific I/O.
* 
*  Things specific to OpenGL are also enabled here in the window creation, such as anti-
*  aliasing, face-culling and depth-testing.
* 
*  @name GLWindow.cpp
*  @author Markus Zakarias Faljord Strømseth, NTNU
*/

void GLAPIENTRY MessageCallback(GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar* message,
    const void* userParam);


/**
*   Constructor for window with default size.
*
*/
Window::Window() 
	: bufferWidth(0), bufferHeight(0), initialPos(0),
	lastPosX(0), lastPosY(0), mainWindow(nullptr)
{

	width = 800;
	height = 600;

	for (size_t i = 0; i < 1024; i++) {
		keys[i] = 0;
	}

	changeX = 0.0f;
	changeY = 0.0f; // VERY IMPORTANT

}

/**
*   Constructor with user defined window size.
*
*   @param     wWidth  - Width size for window.
*   @param     wHeight - Height size for window.
*/
Window::Window(GLint wWidth, GLint wHeight)
{
    width = wWidth;
    height = wHeight;

    for (size_t i = 0; i < 1024; i++) 
    {
        keys[i] = 0;
    }

    changeX = 0.0f;
    changeY = 0.0f; // VERY IMPORTANT
}

/**
*   Initialize all things for OpenGL and the OpenGL context through GLFW.
*
*   @return int - whether it was successful or not.
*/
int Window::initialise() 
{
    // Initialization of GLFW
    if (!glfwInit()) 
    {
        std::cerr << "GLFW initialization failed." << '\n'; // Using error stream in these cases.
        std::cin.get();
        return EXIT_FAILURE;
    }

    // Setting window hints
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    mainWindow = glfwCreateWindow(width, height, "Autumn Exam 2020", nullptr, nullptr);
    if (mainWindow == nullptr) 
    {
        std::cerr << "GLFW failed on window creation." << '\n';
        std::cin.get();
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

    
    glfwMakeContextCurrent(mainWindow); // Setting the OpenGL context.
    

    callback(); // handling input
    glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // locking mouse to screen, it will become invisible in effect 
                                                                     // but it's still there, similar to modern games.
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) 
    {
        std::cerr << "GLEW failed to start!." << '\n';
        std::cin.get();
        return EXIT_FAILURE;
    }

    // Enable capture of debug output.
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(MessageCallback, 0);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST); // HAVE TO HAVE THIS FOR 3D
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_MULTISAMPLE); // Enable MSSA buffers

    glViewport(0, 0, bufferWidth, bufferHeight); 

    glfwSetWindowUserPointer(mainWindow, this); // "this" class is the user.
}



/**
*   This function prints out the frames per second of the window context.
*   Mostly used for development purposes.
*/
void Window::updateFPS() 
{
    static float previousSeconds = glfwGetTime();
    static int frameCount;
    float currentSeconds = glfwGetTime();
    float elapsedSeconds = currentSeconds - previousSeconds;
    
    if (elapsedSeconds < 0.25f) 
    {
        previousSeconds = currentSeconds;
        float FPS = (float)frameCount / elapsedSeconds; // can not use dynamic or static cast for this.
        std::cout << "FPS IS : " << 1000/(float)FPS << std::endl; // should lie between 1500-2000.
        frameCount = 0;
    }
    frameCount++;
}

/**
*   I/O handler for the keyboard.
*
*   @param     window - Relevant window to retrieve input from.
*   @param     key    - What key is currently pressed.
*   @param     code   - Ascii code for the key
*   @param     action - released or pressed
*   @param     mode   - modifier bit.
*/
void Window::inputHandler(GLFWwindow* window, int key, int code, int action, int mode) 
{
    Window* myWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
    
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) // robust check for key and action
    {
        glfwSetWindowShouldClose(window, GL_TRUE); // close on escape!
    }

    if (key != 0 && key < 1024) // extended ascii character size.
    { 
        if (action == GLFW_PRESS) {
            myWindow->keys[key] = true;
        } else if (action == GLFW_RELEASE) {
           myWindow->keys[key] = false;
        }
    }
}

/**
*   Used for bypassing vector error when calling glfwTerminate().
*/
void Window::closeWindow() {
    glfwSetWindowShouldClose(mainWindow, GL_TRUE);
}

/**
*   I/O handler for mouse input form user to window.
*
*   @param     window - Window to retrieve i/o actions from.
*   @param     xPos   - mouse position in x direction
*   @param     yPos   - mouse position in y direction
*/
void Window::mouseHandler(GLFWwindow* window, double xPos, double yPos) 
{
    Window* myWindow = static_cast<Window*>(glfwGetWindowUserPointer(window)); // give access to window

    if (myWindow->initialPos) 
    {
        myWindow->lastPosX = xPos;
        myWindow->lastPosY = yPos;
        myWindow->initialPos = false;
    }

    myWindow->changeX = xPos - myWindow->lastPosX;
    myWindow->changeY = myWindow->lastPosY - yPos; // duplicate line like above would mean that everything is inverted, avoiding this.

    myWindow->lastPosX = xPos;
    myWindow->lastPosY = yPos;
}

/**
*   Notifies the game engine on PRESS, RELEASE, or continual press.
*/
void Window::callback() 
{
    glfwSetKeyCallback(mainWindow, inputHandler); // when a key is pressed handle the input on that window it was pressed to.
    glfwSetCursorPosCallback(mainWindow, mouseHandler);
}

/**
*   Retrieve change in X for mouse.
*
*   @see GLfloat - value of the change.
*/
GLfloat Window::getChangeX() 
{
    GLfloat theChange = changeX;
    changeX = 0.0f;
    return theChange;
}

/**
*   Retrieve change in Y for mouse.
*
*   @see GLfloat - value of the change.
*/
GLfloat Window::getChangeY() 
{
    GLfloat theChange = changeY;
    changeY = 0.0f;
    return theChange;
}

/**
*   Destroying the window when the object is destroyed. This is a form of cleanup.
*/
Window::~Window() 
{
    glfwDestroyWindow(mainWindow);
    glfwTerminate();
}

/*
// ----------------------------------------
// MessageCallback (for debugging purposes)
// ----------------------------------------
*/
void GLAPIENTRY
MessageCallback(GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar* message,
    const void* userParam)
{
    std::cerr << "GL CALLBACK:" << (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "") <<
        "type = 0x" << type <<
        ", severity = 0x" << severity <<
        ", message =" << message << "\n";
}
