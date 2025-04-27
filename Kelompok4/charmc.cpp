#include <math.h>
#include <GL/glut.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Camera variables
float cameraX = 0.0f, cameraY = 3.0f, cameraZ = 15.0f;
float lookX = 0.0f, lookY = 0.0f, lookZ = 0.0f;
float upX = 0.0f, upY = 1.0f, upZ = 0.0f;

// Movement variables
float cameraSpeed = 0.5f;
float rotationSpeed = 2.0f;
float angle = 0.0f;

// Projection modes
int projectionMode = 1; // 0 = Ortho, 1 = Perspective
int orthoView = 0;      // Orthographic view mode
int perspectiveView = 1; // Perspective view mode

// Window dimensions
int windowWidth = 800;
int windowHeight = 600;

void initLighting() {
    GLfloat lightAmbient[] = {0.5f, 0.5f, 0.5f, 1.0f};
    GLfloat lightDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat lightSpecular[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat lightPosition[] = {5.0f, 10.0f, 5.0f, 1.0f};
    
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
}

void setupPerspective() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (float)windowWidth/windowHeight, 0.1f, 100.0f);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    switch(perspectiveView) {
        case 1: // 1-point perspective
            gluLookAt(0.0f, 3.0f, 15.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
            break;
        case 2: // 2-point perspective
            gluLookAt(10.0f, 5.0f, 10.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
            break;
        case 3: // 3-point perspective
            gluLookAt(10.0f, 10.0f, 10.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
            break;
    }
}

void setupOrthographic() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    float aspect = (float)windowWidth/windowHeight;
    if (windowWidth >= windowHeight) {
        glOrtho(-10.0*aspect, 10.0*aspect, -10.0, 10.0, -50.0, 50.0);
    } else {
        glOrtho(-10.0, 10.0, -10.0/aspect, 10.0/aspect, -50.0, 50.0);
    }
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    switch(orthoView) {
        case 0: // Front view
            gluLookAt(0.0, 0.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
            break;
        case 1: // Top view
            gluLookAt(0.0, 10.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -1.0);
            break;
        case 2: // Side view
            gluLookAt(10.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
            break;
        case 3: // Bottom view
            gluLookAt(0.0, -10.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);
            break;
        case 4: // Isometric
            gluLookAt(10.0, 10.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
            break;
        case 5: // Dimetric
            gluLookAt(10.0, 7.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
            break;
        case 6: // Trimetric
            gluLookAt(10.0, 6.0, 4.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
            break;
    }
}

void reshape(int w, int h) {
    windowWidth = w;
    windowHeight = h;
    glViewport(0, 0, w, h);
}

void drawGrid() {
    glColor3f(0.5f, 0.5f, 0.5f);
    glBegin(GL_LINES);
    for (int i = -20; i <= 20; i++) {
        glVertex3f(i, 0.0f, -20.0f);
        glVertex3f(i, 0.0f, 20.0f);
        glVertex3f(-20.0f, 0.0f, i);
        glVertex3f(20.0f, 0.0f, i);
    }
    glEnd();
}

void BuatKepala() {
    glPushMatrix();
    glTranslatef(0.0f, 6.0f, 0.0f);
    glColor3f(1.0f, 0.8f, 0.6f);
    glutSolidSphere(2.0f, 50, 50);
    glPopMatrix();
}

void BuatTopi() {
    glPushMatrix();
    glTranslatef(1.0f, 7.5f, 0.0f);
    glColor3f(0.0f, 0.0f, 1.0f);
    glScalef(3.0f, 0.3f, 2.2f);
    glutSolidCube(2.0f);
    glPopMatrix();
}

void BuatLeher() {
    glPushMatrix();
    glTranslatef(0.0f, 4.0f, 0.0f);
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    glColor3f(1.0f, 0.8f, 0.6f);
    GLUquadricObj *quadratic = gluNewQuadric();
    gluCylinder(quadratic, 0.5f, 0.5f, 1.0f, 32, 32);
    gluDeleteQuadric(quadratic);
    glPopMatrix();
}

void Badan() {
    glPushMatrix();
    glScalef(0.8f, 1.1f, 0.5f);
    
    // Depan
    glBegin(GL_QUADS);  
        glVertex3f(-3.0f, -3.0f, 3.0f);  
        glVertex3f(3.0f, -3.0f, 3.0f);  
        glVertex3f(3.0f, 3.0f, 3.0f);  
        glVertex3f(-3.0f, 3.0f, 3.0f);  
    glEnd();  

    // Belakang
    glBegin(GL_QUADS);  
        glVertex3f(-3.0f, -3.0f, -3.0f);  
        glVertex3f(-3.0f, 3.0f, -3.0f);  
        glVertex3f(3.0f, 3.0f, -3.0f);  
        glVertex3f(3.0f, -3.0f, -3.0f);  
    glEnd();  

    // Atas
    glBegin(GL_QUADS);  
        glVertex3f(-3.0f, 3.0f, -3.0f);  
        glVertex3f(-3.0f, 3.0f, 3.0f);  
        glVertex3f(3.0f, 3.0f, 3.0f);  
        glVertex3f(3.0f, 3.0f, -3.0f);  
    glEnd();  

    // Bawah
    glBegin(GL_QUADS);  
        glVertex3f(-3.0f, -3.0f, -3.0f);  
        glVertex3f(3.0f, -3.0f, -3.0f);  
        glVertex3f(3.0f, -3.0f, 3.0f);  
        glVertex3f(-3.0f, -3.0f, 3.0f);  
    glEnd();  

    // Kiri
    glBegin(GL_QUADS);  
        glVertex3f(-3.0f, -3.0f, -3.0f);  
        glVertex3f(-3.0f, -3.0f, 3.0f);  
        glVertex3f(-3.0f, 3.0f, 3.0f);  
        glVertex3f(-3.0f, 3.0f, -3.0f);  
    glEnd();  

    // Kanan
    glBegin(GL_QUADS);  
        glVertex3f(3.0f, -3.0f, -3.0f);  
        glVertex3f(3.0f, 3.0f, -3.0f);  
        glVertex3f(3.0f, 3.0f, 3.0f);  
        glVertex3f(3.0f, -3.0f, 3.0f);  
    glEnd();  

    glPopMatrix();
}

void BuatTangan(bool kiri) {
    glPushMatrix();
    float posisiX = (kiri ? -3.2f : 3.2f);
    glTranslatef(posisiX, 3.0f, 0.0f);

    // Lengan Atas
    glPushMatrix();
    glColor3f(1.0f, 0.0f, 0.0f);
    glScalef(0.7f, 1.6f, 0.7f);
    glTranslatef(0.0f, -1.0f, 0.0f);
    glutSolidCube(2.0f);
    glPopMatrix();

    // Lengan Bawah
    glPushMatrix();
    glColor3f(0.0f, 1.0f, 0.0f);
    glScalef(0.6f, 1.5f, 0.6f);
    glTranslatef(0.0f, -3.0f, 0.0f);
    glutSolidCube(2.0f);
    glPopMatrix();

    // Telapak Tangan
    glPushMatrix();
    glColor3f(0.0f, 0.0f, 1.0f);
    glScalef(0.5f, 0.6f, 0.5f);
    glTranslatef(0.0f, -10.1f, 0.0f);
    glutSolidCube(2.0f);
    glPopMatrix();

    glPopMatrix();
}

void BuatKaki(bool kiri) {
    glPushMatrix();
    float posisiX = (kiri ? -1.2f : 1.2f);
    glTranslatef(posisiX, -3.0f, 0.0f);

    // Kaki Atas
    glPushMatrix();
    glColor3f(1.0f, 0.0f, 0.0f);
    glScalef(1.1f, 2.0f, 1.1f);
    glTranslatef(0.0f, -1.0f, 0.0f);
    glutSolidCube(2.0f);
    glPopMatrix();

    // Kaki Bawah
    glPushMatrix();
    glColor3f(0.0f, 1.0f, 0.0f);
    glScalef(1.0f, 2.0f, 1.0f);
    glTranslatef(0.0f, -3.0f, 0.0f);
    glutSolidCube(2.0f);
    glPopMatrix();

    // Telapak Kaki
    glPushMatrix();
    glColor3f(0.0f, 0.0f, 1.0f);
    glScalef(1.0f, 0.7f, 1.7f);
    glTranslatef(0.0f, -12.1f, 0.35f);
    glutSolidCube(2.0f);
    glPopMatrix();

    glPopMatrix();
}

void buatOrang() {
    glPushMatrix();
    glTranslatef(0.0f, 3.5f, 0.0f);
    glScalef(0.3f, 0.3f, 0.3f);
    Badan();
    BuatTangan(true);
    BuatTangan(false);
    BuatKaki(true);
    BuatKaki(false);
    BuatKepala();
    BuatTopi();
    BuatLeher();
    glPopMatrix();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    if (projectionMode == 0) {
        setupOrthographic();
    } else {
        setupPerspective();
    }
    
    drawGrid();
    buatOrang();
    
    glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y) {
    switch(key) {
        // Projection mode
        case 'o': case 'O': 
            projectionMode = 0; 
            break;
        case 'p': case 'P': 
            projectionMode = 1; 
            break;
            
        // Orthographic views
        case '4': case 'F': orthoView = 0; break; // Front
        case '5': case 'T': orthoView = 1; break; // Top
        case '6': case 'S': orthoView = 2; break; // Side
        case '7': case 'B': orthoView = 3; break; // Bottom
        case '8': case 'I': orthoView = 4; break; // Isometric
        case '9': case 'D': orthoView = 5; break; // Dimetric
        case '0': case 'M': orthoView = 6; break; // Trimetric
            
        // Perspective views
        case '1': perspectiveView = 1; break; // 1-point
        case '2': perspectiveView = 2; break; // 2-point
        case '3': perspectiveView = 3; break; // 3-point
            
        // Camera movement
        case 'w': case 'W': cameraZ -= cameraSpeed; break;
        case 'l': case 'L': cameraZ += cameraSpeed; break;
        case 'a': case 'A': cameraX -= cameraSpeed; break;
        case 'j': case 'J': cameraX += cameraSpeed; break;
        case 'q': case 'Q': cameraY -= cameraSpeed; break;
        case 'e': case 'E': cameraY += cameraSpeed; break;
            
        case 27: exit(0); break; // ESC key
    }
    
    glutPostRedisplay();
}

void specialKeys(int key, int x, int y) {
    switch(key) {
        case GLUT_KEY_LEFT: angle -= rotationSpeed; break;
        case GLUT_KEY_RIGHT: angle += rotationSpeed; break;
        case GLUT_KEY_UP: cameraSpeed += 0.1f; break;
        case GLUT_KEY_DOWN: cameraSpeed = (cameraSpeed > 0.1f) ? cameraSpeed - 0.1f : 0.1f; break;
    }
    
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("Orang 3D");
    
    initLighting();
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    
    glutMainLoop();
    return 0;
}
