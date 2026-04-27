// GROUP 6
// Parsley Njoroge - Sct211-0007/2023
// Patrick Leon - Sct211-0003/2023

#include <GL/glut.h>
#include <cmath>
#include <cstdio>

GLuint textureID;
float timeValue = 0.0f;
float fabricOffsetY = 1.75f;

float camX = 0.0f;
float camY = 0.0f;
float camZ = 5.0f;

// ================= TEXTURE LOADING =================
void loadTexture() {
    FILE* file;
    fopen_s(&file, "banner.bmp", "rb");

    if (!file) {
        printf("Failed to load texture\n");
        return;
    }

    unsigned char header[54];
    fread(header, sizeof(unsigned char), 54, file);

    int width = *(int*)&header[18];
    int height = *(int*)&header[22];

    // Correct row size (with padding)
    int row_padded = (width * 3 + 3) & (~3);

    // Allocate full padded data
    unsigned char* data = new unsigned char[row_padded * height];

    // Read full image
    fread(data, sizeof(unsigned char), row_padded * height, file);
    fclose(file);

    // Convert BGR → RGB safely
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int i = y * row_padded + x * 3;

            unsigned char temp = data[i];
            data[i] = data[i + 2];
            data[i + 2] = temp;
        }
    }

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
        width, height, 0,
        GL_RGB, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    delete[] data;
}

void drawPole(float x) {
    glPushMatrix();

    glTranslatef(x, 0.0f, 0.0f);
    glRotatef(-90, 1, 0, 0);

    GLUquadric* quad = gluNewQuadric();
    glColor3f(0.6f, 0.6f, 0.6f);

    gluCylinder(quad, 0.05, 0.05, 2.0, 20, 20);

    gluDeleteQuadric(quad);
    glPopMatrix();
}

void display() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    gluLookAt(camX, camY, camZ,
        0.0, 0.0, 0.0,
        0.0, 1.0, 0.0);

    // Background
    glClearColor(0.6f, 0.3f, 0.2f, 1.0f);

    // Poles
    drawPole(-0.99f);
    drawPole(0.99f);

    // Banner parameters
    int segments = 50;
    float width = 2.0f;
    float height = 0.6f;

    glBindTexture(GL_TEXTURE_2D, textureID);

    for (int i = 0; i < segments; i++) {

        float x1 = -1.0f + (width / segments) * i;
        float x2 = -1.0f + (width / segments) * (i + 1);

        float t = (x1 + 1.0f) / 2.0f;
        float edgeDamping = sin(3.14159f * t);

        float wave1 = 0.1f * edgeDamping * sin(5 * x1 - timeValue + t * 2.0f);
        float wave2 = 0.1f * edgeDamping * sin(5 * x2 - timeValue + t * 2.0f);

        float sag = 0.05f * (1 - edgeDamping);

        float u1 = (float)i / segments;
        float u2 = (float)(i + 1) / segments;

        glBegin(GL_QUADS);
        
        glTexCoord2f(u1, 0.0f);

        glVertex3f(x1, -height / 2 + wave1 - sag + fabricOffsetY, 0.0f);

        glTexCoord2f(u2, 0.0f);
        glVertex3f(x2, -height / 2 + wave2 - sag + fabricOffsetY, 0.0f);

        glTexCoord2f(u2, 1.0f);
        glVertex3f(x2, height / 2 + wave2 - sag + fabricOffsetY, 0.0f);

        glTexCoord2f(u1, 1.0f);
        glVertex3f(x1, height / 2 + wave1 - sag + fabricOffsetY, 0.0f);

        glEnd();
    }

    glFlush();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (float)w / h, 1.0, 20.0);

    glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y) {

    if (key == 'w') camZ -= 0.2f;
    else if (key == 's') camZ += 0.2f;
    else if (key == 'a') camX -= 0.2f;
    else if (key == 'd') camX += 0.2f;
    else if (key == 'q') camY += 0.2f;
    else if (key == 'e') camY -= 0.2f;

    glutPostRedisplay();
}

void init() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);

    loadTexture();
}
void update(int value) {
    timeValue += 0.05f;

    glutPostRedisplay();
    glutTimerFunc(24, update, 0);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(800, 400);
    glutCreateWindow("JKUAT Banner");

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(0, update, 0);

    glutMainLoop();
    return 0;
}