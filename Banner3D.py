from OpenGL.GL import *
from OpenGL.GLU import *
from OpenGL.GLUT import *
from PIL import Image
import math

# ---------------- GLOBALS ----------------
texture = None
time_value = 0.0
fabric_offset_y = 1.75

camX, camY, camZ = 0.0, 0.0, 5.0


# ---------------- TEXTURE ----------------
def load_texture(path):
    global texture

    img = Image.open(path)
    img = img.transpose(Image.FLIP_TOP_BOTTOM)
    img_data = img.convert("RGB").tobytes()

    texture = glGenTextures(1)
    glBindTexture(GL_TEXTURE_2D, texture)

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 img.width, img.height,
                 0, GL_RGB, GL_UNSIGNED_BYTE, img_data)

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR)


# ---------------- DRAW POLE ----------------
def draw_pole(x):
    glPushMatrix()

    glTranslatef(x, 0.0, 0.0)
    glRotatef(-90, 1, 0, 0)

    quad = gluNewQuadric()
    glColor3f(0.6, 0.6, 0.6)

    gluCylinder(quad, 0.05, 0.05, 2.0, 20, 20)

    gluDeleteQuadric(quad)
    glPopMatrix()


# ---------------- DRAW BANNER ----------------
def draw_banner():
    global time_value

    segments = 50
    width = 2.0
    height = 0.6

    glBindTexture(GL_TEXTURE_2D, texture)

    for i in range(segments):

        x1 = -1.0 + (width / segments) * i
        x2 = -1.0 + (width / segments) * (i + 1)

        t = (x1 + 1.0) / 2.0
        edge_damping = math.sin(math.pi * t)

        wave1 = 0.1 * edge_damping * math.sin(5 * x1 - time_value + t * 2.0)
        wave2 = 0.1 * edge_damping * math.sin(5 * x2 - time_value + t * 2.0)

        sag = 0.05 * (1 - edge_damping)

        u1 = i / segments
        u2 = (i + 1) / segments

        glBegin(GL_QUADS)

        glTexCoord2f(u1, 0.0)
        glVertex3f(x1, -height/2 + wave1 - sag + fabric_offset_y, 0)

        glTexCoord2f(u2, 0.0)
        glVertex3f(x2, -height/2 + wave2 - sag + fabric_offset_y, 0)

        glTexCoord2f(u2, 1.0)
        glVertex3f(x2, height/2 + wave2 - sag + fabric_offset_y, 0)

        glTexCoord2f(u1, 1.0)
        glVertex3f(x1, height/2 + wave1 - sag + fabric_offset_y, 0)

        glEnd()


# ---------------- DISPLAY ----------------
def display():
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
    glLoadIdentity()

    gluLookAt(camX, camY, camZ,
              0.0, 0.0, 0.0,
              0.0, 1.0, 0.0)

    glClearColor(0.6, 0.3, 0.2, 1.0)

    # Poles
    draw_pole(-0.99)
    draw_pole(0.99)

    # Banner
    draw_banner()

    glutSwapBuffers()


# ---------------- RESHAPE ----------------
def reshape(w, h):
    glViewport(0, 0, w, h)

    glMatrixMode(GL_PROJECTION)
    glLoadIdentity()
    gluPerspective(60.0, w / h if h != 0 else 1, 1.0, 20.0)

    glMatrixMode(GL_MODELVIEW)


# ---------------- KEYBOARD ----------------
def keyboard(key, x, y):
    global camX, camY, camZ

    key = key.decode("utf-8")

    if key == 'w':
        camZ -= 0.2
    elif key == 's':
        camZ += 0.2
    elif key == 'a':
        camX -= 0.2
    elif key == 'd':
        camX += 0.2
    elif key == 'q':
        camY += 0.2
    elif key == 'e':
        camY -= 0.2

    glutPostRedisplay()


# ---------------- UPDATE ----------------
def update(value):
    global time_value
    time_value += 0.05

    glutPostRedisplay()
    glutTimerFunc(24, update, 0)


# ---------------- INIT ----------------
def init():
    glEnable(GL_DEPTH_TEST)
    glEnable(GL_TEXTURE_2D)

    load_texture("banner.bmp")  # make sure image is in same folder


# ---------------- MAIN ----------------
def main():
    glutInit()
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH)
    glutInitWindowSize(800, 400)
    glutCreateWindow(b"JKUAT Banner - Python")

    init()

    glutDisplayFunc(display)
    glutReshapeFunc(reshape)
    glutKeyboardFunc(keyboard)
    glutTimerFunc(0, update, 0)

    glutMainLoop()


main()