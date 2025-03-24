#include <math.h>
#include <GL/glut.h>
#include <stdio.h>
#include "stb_image.h"
#include <string.h>
#include <stdlib.h>
GLuint texture;

// Camera movement variables
float angle = 0.0, deltaAngle = 0.0, ratio;
float x = 0.0f, y = 1.75f, z = 15.0f;  // Initial camera position
float lx = 0.0f, ly = 0.0f, lz = -1.0f; // Camera direction
int deltaMove = 0, h, w; // Camera movement state
int bitmapHeight = 12;

// Reshape function to handle window resizing
void Reshape(int w1, int h1) {
    if (h1 == 0) h1 = 1;
    w = w1;
    h = h1;
    ratio = 1.0f * w / h;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, w, h);
    gluPerspective(45, ratio, 0.1, 1000);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(x, y, z, x + lx, y + ly, z + lz, 0.0f, 1.0f, 0.0f);
}

// Function to rotate the camera direction
void orientMe(float ang) {
    lx = sin(ang);
    lz = -cos(ang);
    glLoadIdentity();
    gluLookAt(x, y, z, x + lx, y + ly, z + lz, 0.0f, 1.0f, 0.0f);
}

// Function to move the camera forward/backward
void moveMeFlat(int i) {
    x += i * (lx) * 0.01;
    z += i * (lz) * 0.01;
    glLoadIdentity();
    gluLookAt(x, y, z, x + lx, y + ly, z + lz, 0.0f, 1.0f, 0.0f);
}

// Function to draw a grid on the ground
void Grid() {
    double i;
    const float Z_MIN = -50, Z_MAX = 50;
    const float X_MIN = -50, X_MAX = 50;
    const float gap = 1.5;
    
    glColor3f(0.5, 0.5, 0.5);
    glBegin(GL_LINES);
    for (i = Z_MIN; i < Z_MAX; i += gap) {
        glVertex3f(i, 0, Z_MIN);
        glVertex3f(i, 0, Z_MAX);
    }
    for (i = X_MIN; i < X_MAX; i += gap) {
        glVertex3f(X_MIN, 0, i);
        glVertex3f(X_MAX, 0, i);
    }
    glEnd();
}

// Function to draw a wooden box
void KotakKayu() {
	glColor3f(1.0f, 0.0f, 0.0f);
    // Front face
    glPushMatrix();
    glTranslatef(0, 0, 3);
    glBegin(GL_QUADS);
    glVertex3f(-3.0f, -3.0f, 0.0f);
    glVertex3f(3.0f, -3.0f, 0.0f);
    glVertex3f(3.0f, 3.0f, 0.0f);
    glVertex3f(-3.0f, 3.0f, 0.0f);
    glEnd();
    glPopMatrix();

    // Top face
    glPushMatrix();
    glRotated(-90, 1, 0, 0);
    glTranslatef(0, 0, 3);
    glBegin(GL_QUADS);
    glVertex3f(-3.0f, -3.0f, 0.0f);
    glVertex3f(3.0f, -3.0f, 0.0f);
    glVertex3f(3.0f, 3.0f, 0.0f);
    glVertex3f(-3.0f, 3.0f, 0.0f);
    glEnd();
    glPopMatrix();

    // Back face
    glPushMatrix();
    glRotated(-180, 1, 0, 0);
    glTranslatef(0, 0, 3);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); // Bottom left
    glVertex3f(-3.0f, -3.0f, 0.0f);
    glTexCoord2f(1.0f, 0.0f); // Bottom right
    glVertex3f(3.0f, -3.0f, 0.0f);
    glTexCoord2f(1.0f, 1.0f); // Top right
    glVertex3f(3.0f, 3.0f, 0.0f);
    glTexCoord2f(0.0f, 1.0f); // Top left
    glVertex3f(-3.0f, 3.0f, 0.0f);
    glEnd();
    glPopMatrix();

    // Bottom face
    glPushMatrix();
    glRotated(90, 1, 0, 0);
    glTranslatef(0, 0, 3);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); // Bottom left
    glVertex3f(-3.0f, -3.0f, 0.0f);
    glTexCoord2f(1.0f, 0.0f); // Bottom right
    glVertex3f(3.0f, -3.0f, 0.0f);
    glTexCoord2f(1.0f, 1.0f); // Top right
    glVertex3f(3.0f, 3.0f, 0.0f);
    glTexCoord2f(0.0f, 1.0f); // Top left
    glVertex3f(-3.0f, 3.0f, 0.0f);
    glEnd();
    glPopMatrix();

    // Left face
    glPushMatrix();
    glRotated(-90, 0, 1, 0);
    glTranslatef(0, 0, 3);
    glBegin(GL_QUADS);
    glVertex3f(-3.0f, -3.0f, 0.0f);
    glVertex3f(3.0f, -3.0f, 0.0f);
    glVertex3f(3.0f, 3.0f, 0.0f);
    glVertex3f(-3.0f, 3.0f, 0.0f);
    glEnd();
    glPopMatrix();

    // Right face
    glPushMatrix();
    glRotated(90, 0, 1, 0);
    glTranslatef(0, 0, 3);
    glBegin(GL_QUADS);
    glVertex3f(-3.0f, -3.0f, 0.0f);
    glVertex3f(3.0f, -3.0f, 0.0f);
    glVertex3f(3.0f, 3.0f, 0.0f);
    glVertex3f(-3.0f, 3.0f, 0.0f);
    glEnd();
    glPopMatrix();
}

void drawBackground() {
    glDisable(GL_LIGHTING); 
    glBegin(GL_QUADS);
        glColor3f(0.0f, 0.0f, 0.5f); glVertex3f(-50.0f, -10.0f, -50.0f);
        glColor3f(0.0f, 0.0f, 0.8f); glVertex3f(50.0f, -10.0f, -50.0f);
        glColor3f(0.2f, 0.2f, 1.0f); glVertex3f(50.0f, 50.0f, -100.0f);
        glColor3f(0.1f, 0.1f, 0.7f); glVertex3f(-50.0f, 50.0f, -100.0f);
    glEnd();
    glEnable(GL_LIGHTING); 
}

// Display function to render the scene
void display() {
    if (deltaMove)
        moveMeFlat(deltaMove);
    
    if (deltaAngle) {
        angle += deltaAngle;
        orientMe(angle);
    }
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw the grid and object
    Grid();
    KotakKayu();
	drawBackground();
    glutSwapBuffers();
    glFlush();
}

// Function to handle key press events for camera movement
void pressKey(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_LEFT:
            deltaAngle = -0.001f;
            break;
        case GLUT_KEY_RIGHT:
            deltaAngle = 0.001f;
            break;
        case GLUT_KEY_UP:
            deltaMove = 1;
            break;
        case GLUT_KEY_DOWN:
            deltaMove = -1;
            break;
    }
}

// Function to handle key release events
void releaseKey(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_LEFT:
            if (deltaAngle < 0.0f)
                deltaAngle = 0.0f;
            break;
        case GLUT_KEY_RIGHT:
            if (deltaAngle > 0.0f)
                deltaAngle = 0.0f;
            break;
        case GLUT_KEY_UP:
            if (deltaMove > 0)
                deltaMove = 0;
            break;
        case GLUT_KEY_DOWN:
            if (deltaMove < 0)
                deltaMove = 0;
            break;
    }
}

// Lighting properties
const GLfloat light_ambient[] = { 0.5f, 0.5f, 0.5f, 1.0f };  // Ambient light
const GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };  // Diffuse light
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // Specular light
const GLfloat light_position[] = { 0.0f, 20.0f, 10.0f, 1.0f }; // Light position (x, y, z, w)

// Material properties
const GLfloat mat_ambient[] = { 0.7f, 0.7f, 0.7f, 1.0f };    // Material ambient color
const GLfloat mat_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };    // Material diffuse color
const GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };   // Material specular color
const GLfloat high_shininess[] = { 100.0f };                   // Material shininess


// Lighting setup function
void lighting() {
	glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);

    // Cahaya utama
    glEnable(GL_LIGHT0);
    GLfloat light0_position[] = { 0.0f, 10.0f, 5.0f, 1.0f };
    GLfloat light0_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
    glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.0f);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.05f);
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.02f);
    
    // Spotlight
    glEnable(GL_LIGHT1);
    GLfloat light1_position[] = { 10.0f, 15.0f, 10.0f, 1.0f };
    GLfloat light1_diffuse[] = { 1.0f, 1.0f, 0.8f, 1.0f };
    GLfloat spot_direction[] = { -1.0f, -1.0f, -1.0f };
    glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction);
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 25.0f);
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 2.0f);
}

// OpenGL initialization function
void init(void) {
    glEnable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}



int main(int argc, char **argv) {
    printf("stb_image.h berhasil dimuat!\n");
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(640, 480);
    glutCreateWindow("3D Lighting");

    glutIgnoreKeyRepeat(1);  
    glutSpecialFunc(pressKey);
    glutSpecialUpFunc(releaseKey);
    glutDisplayFunc(display);
    glutIdleFunc(display);  
    glutReshapeFunc(Reshape);

    lighting();
    init();

    glutMainLoop();
    return 0;
}
