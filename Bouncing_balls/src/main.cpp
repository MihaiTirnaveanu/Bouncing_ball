#include <GLFW/glfw3.h>
#define _USE_MATH_DEFINES
#include <cmath>

float ball_x = 1.5f;
float ball_y = 1.5f; // Start from the middle of the window
float const BALL_RADIUS = 0.1f;

float ball_velocity_y = 0.0f; // Initial vertical velocity
float ball_velocity_x = 0.0f; // Initial horizontal velocity
float const GRAVITY = -0.001f; // Gravity acceleration
float const RESTITUTION = 0.8f; // Coefficient of restitution (bounce)

float const WINDOW_LEFT = -2.0f;
float const WINDOW_RIGHT = 2.0f;
float const WINDOW_BOTTOM = -2.0f;
float const WINDOW_TOP = 2.0f;

bool isFallingVertically = true; // Flag to track vertical falling state
bool hasBounced = false; // Flag to track if the ball has bounced once
int numberOfBounces = 0;
bool ballTouchedBox = false;

void drawBall() {
    glBegin(GL_TRIANGLE_FAN);
    glColor3f(1.0f, 0.0f, 0.0f); // Red color
    glVertex2f(ball_x, ball_y); // Center of the circle

    int num_segments = 100;
    for (int i = 0; i <= num_segments; i++) {
        float angle = 2.0f * M_PI * i / num_segments;
        float x = ball_x + BALL_RADIUS * cos(angle);
        float y = ball_y + BALL_RADIUS * sin(angle);
        glVertex2f(x, y);
    }
    glEnd();
}

void drawBox() {
    glColor3f(0.0f, 0.0f, 1.0f); // Blue color
    glBegin(GL_QUADS);
    glVertex2f(1.5f, -2.0f); // Bottom left
    glVertex2f(2.5f, -2.0f); // Bottom right
    glVertex2f(2.5f, -1.5f); // Top right
    glVertex2f(1.5f, -1.5f); // Top left
    glEnd();
}

void update() {
    // Apply gravity
    ball_velocity_y += GRAVITY;

    // Update vertical position
    ball_y += ball_velocity_y;

    // Check for collision with bottom of the window
    float floorLevel;
    if (ballTouchedBox == false) {
        floorLevel = -1.5f;
    }
    else {
        floorLevel = -2.0f;
    }

    if (ball_y - BALL_RADIUS < floorLevel) {
        ballTouchedBox = true;
        ball_y = floorLevel + BALL_RADIUS; // Set ball position just above bottom edge
        ball_velocity_y *= -RESTITUTION; // Reverse vertical velocity with restitution
        hasBounced = true; // Set flag to indicate vertical bounce
        if (hasBounced) {
            ball_velocity_x = -0.01f; // Set initial horizontal velocity
        }
    }

    if (numberOfBounces > 7) {
        return;
    }
    // Update horizontal position
    ball_x += ball_velocity_x;

    // Check for collision with window edges
    if (ball_x - BALL_RADIUS < WINDOW_LEFT) {
        ball_x = WINDOW_LEFT + BALL_RADIUS; // Set ball position just to the right of the left edge
        ball_velocity_x *= RESTITUTION; // Reverse horizontal velocity with restitution
        numberOfBounces++;
    }
    else if (ball_x + BALL_RADIUS > WINDOW_RIGHT) {
        ball_x = WINDOW_RIGHT - BALL_RADIUS; // Set ball position just to the left of the right edge
        ball_velocity_x *= -RESTITUTION; // Reverse horizontal velocity with restitution
    }

    // Update vertical position
    ball_y += ball_velocity_y;
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit()) {
        return -1;
    }

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1200, 1200, "Falling Ball", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(WINDOW_LEFT, WINDOW_RIGHT, WINDOW_BOTTOM, WINDOW_TOP, -1.0f, 1.0f);
        glMatrixMode(GL_MODELVIEW);

        // Draw the ball
        drawBall();

        // Draw the box
        drawBox();

        // Update ball position
        update();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
