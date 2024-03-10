#include <GLFW/glfw3.h>
#define _USE_MATH_DEFINES
#include <cmath>

float ball_x = 0.0f;
float ball_y = 0.5f; // Start from the middle of the window
float const BALL_RADIUS = 0.1f;

float ball_velocity_y = 0.0f; // Initial vertical velocity
float const GRAVITY = -0.001f; // Gravity acceleration
float const RESTITUTION = 0.8f; // Coefficient of restitution (bounce)

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

void update() {
    // Apply gravity
    ball_velocity_y += GRAVITY;

    // Update ball position
    ball_y += ball_velocity_y;

    // Check for collision with bottom of the window
    if (ball_y - BALL_RADIUS < -1.0f) {
        ball_y = -1.0f + BALL_RADIUS; // Set ball position just above bottom edge
        ball_velocity_y *= -RESTITUTION; // Reverse velocity with restitution
    }
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

        // Draw the ball
        drawBall();

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
