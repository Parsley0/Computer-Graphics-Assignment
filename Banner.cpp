//GROUP 6
//Parsley Njoroge - Sct211-0007/2023
//Patrick Leon - Sct211-0003/2023

#include <GL/glut.h>

float scale = 0.002f;
// Function to draw scalable stroke text
void renderStrokeText(float x, float y, float scale, const char* string) {
    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(scale, scale, 1);

    for (const char* c = string; *c != '\0'; c++) {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
    }

    glPopMatrix();
}

float getStrokeTextWidth(const char* text) {
    float width = 0.0f;
    for (const char* c = text; *c != '\0'; c++) {
        width += glutStrokeWidth(GLUT_STROKE_ROMAN, *c);
    }
    return width;
}

// Display function
void display() {
    glClearColor(0.6f, 0.3f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);


    const char* text1 = "JKUAT";
    const char* text2 = "ROCKS";

    // Calculate widths
    float width1 = getStrokeTextWidth(text1) * scale;
    float width2 = getStrokeTextWidth(text2) * scale;

    // Total width (with spacing between words)
    float spacing = 0.05f;
    float totalWidth = width1 + width2 + spacing;

    // Starting X (centered)
    float startX = -totalWidth / 2;

    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Draw JKUAT (Green)
    glColor3f(0.0f, 1.0f, 0.0f);
    renderStrokeText(startX, 0.0f, scale, text1);

    // Draw ROCKS (Red)
    glColor3f(1.0f, 0.0f, 0.0f);
    renderStrokeText(startX + width1 + spacing, 0.0f, scale, text2);

    // Draw border
    glColor3f(1.0f, 1.0f, 1.0f);
    glLineWidth(3);

    glBegin(GL_LINE_LOOP);
    glVertex2f(-0.95f, -0.8f);
    glVertex2f(0.95f, -0.8f);
    glVertex2f(0.95f, 0.8f);
    glVertex2f(-0.95f, 0.8f);
    glEnd();

    glFlush();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w <= h)
        gluOrtho2D(-1.0, 1.0, -1.0 * (GLfloat)h / (GLfloat)w, 1.0 * (GLfloat)h / (GLfloat)w);
    else
        gluOrtho2D(-1.0 * (GLfloat)w / (GLfloat)h, 1.0 * (GLfloat)w / (GLfloat)h, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y) {
    if (key == '+') {
        scale += 0.0002f; // Increase size
    }
    else if (key == '-') {
        if (scale > 0.0002f) scale -= 0.0002f; // Decrease size
    }
    glutPostRedisplay(); // Tell OpenGL to redraw the screen
}

// Initialize projection
void init() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1, 1, -1, 1);
}



// Main function
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(800, 400);
    glutCreateWindow("JKUAT Banner");

    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}
