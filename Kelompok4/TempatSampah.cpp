#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>


// Variabel pergerakan kamera
float angle = 0.0, deltaAngle = 0.0, ratio;
float pitchAngle = 0.0, deltaPitch = 0.0;  // Variabel untuk rotasi atas dan bawah (pitch)
float x = 0.0f, y = 1.75f, z = 15.0f;        // Posisi awal kamera

// Arah pandang awal (yaw dan pitch)
float lx = 0.0f, ly = 0.0f, lz = -1.0f;
int deltaMove = 0;    // Gerakan maju/mundur
int deltaUp = 0;      // Gerakan naik/turun
int w, h;             // Lebar dan tinggi window
const float movementSpeed = 0.005f;      // Faktor kecepatan gerakan

// Fungsi untuk merotasi arah pandang kamera berdasarkan yaw (angle) dan pitch (pitchAngle)
void orientMe(float ang, float pitchAng) {
    lx = sin(ang) * cos(pitchAng);
    ly = sin(pitchAng);
    lz = -cos(ang) * cos(pitchAng);
    glLoadIdentity();
    gluLookAt(x, y, z, x + lx, y + ly, z + lz, 0.0f, 1.0f, 0.0f);
}

// Fungsi untuk menggerakkan kamera maju/mundur (pergerakan horizontal)
  void moveMeFlat(int i) {
     x += i * (lx) * movementSpeed;
     z += i * (lz) * movementSpeed;
     glLoadIdentity();
     gluLookAt(x, y, z, x + lx, y + ly, z + lz, 0.0f, 1.0f, 0.0f);
 }
// Fungsi untuk menggambar grid di tanah
void Grid() {
    double i;
    const float Z_MIN = -50, Z_MAX = 50;
    const float X_MIN = -50, X_MAX = 50;
    const float gap = 1.5;
    
    glColor3f(0.3, 0.3, 0.3);
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
void drawTrashBin() {    
    // Body depan
    glPushMatrix();
    glColor3f(0.3f, 0.3f, 0.3f); // Dark gray color
    glScalef(1.0f, 1.5f, 1.0f);
    glutSolidCube(1.0);
    glPopMatrix();
    
	// Body belakang (warna hijau)
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, -0.5f);
    glScalef(1.0f, 1.5f, 0.1f);
    glutSolidCube(1.0);
    glPopMatrix();
    
    // Body kiri (warna hijau)
    glPushMatrix();
    glTranslatef(-0.5f, 0.0f, -0.25f);
    glScalef(0.1f, 1.5f, 1.0f);
    glutSolidCube(1.0);
    glPopMatrix();
    
    // Body kanan (warna hijau)
    glPushMatrix();
    glTranslatef(0.5f, 0.0f, -0.25f);
    glScalef(0.1f, 1.5f, 1.0f);
    glutSolidCube(1.0);
    glPopMatrix();
    
    
    // Roda
    glPushMatrix();
    glColor3f(0.1f, 0.1f, 0.1f);
    glTranslatef(0.4f, -0.75f, 0.5f);
    glutSolidSphere(0.1, 20, 20);
    
    glTranslatef(-0.8f, 0.0f, 0.0f);
    glutSolidSphere(0.1, 20, 20);
    
    glTranslatef(0.8f, 0.0f, -1.0f);
    glutSolidSphere(0.1, 20, 20);
    
    glTranslatef(-0.8f, 0.0f, 0.0f);
    glutSolidSphere(0.1, 20, 20);
    glPopMatrix();
    
    // tutup atas
    glPushMatrix();
    glColor3f(0.2f, 0.2f, 0.2f);
    glTranslatef(0.0f, 0.75f, 0.0f);
    glScalef(1.2f, 0.1f, 1.2f);
    glutSolidCube(1.0);
    glPopMatrix();
}

void display() {
    if (deltaMove)
        moveMeFlat(deltaMove);

    if (deltaAngle || deltaPitch) {
        angle += deltaAngle;
        pitchAngle += deltaPitch;
        orientMe(angle, pitchAngle);
    }
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -5.0f);
    // Gambar grid
    glPushMatrix();
    glTranslatef(0.0, -1.0, 0.0); // Turunkan grid agar ada di bawah tempat sampah
	Grid();
	
    glPopMatrix();
    drawTrashBin();
    
    glutSwapBuffers();
    glFlush();
}
// Fungsi untuk menangani input keyboard
void pressKey(unsigned char key, int x, int y) {
    switch (key) {
        case 'w':
            deltaMove = 1;
            break;
        case 's':
            deltaMove = -1;
            break;
        case 'a':
            deltaAngle = -0.0005f;
            break;
        case 'd':
            deltaAngle = 0.0005f;
            break;
        case 'q':
            deltaPitch = 0.0005f;
            break;
        case 'e':
            deltaPitch = -0.0005f;
            break;
    }
}
void releaseKey(unsigned char key, int x, int y) {
    switch (key) {
        case 'w':
        case 's':
            deltaMove = 0;
            break;
        case 'a':
        case 'd':
            deltaAngle = 0.0f;
            break;
        case 'q':
        case 'e':
            deltaPitch = 0.0f;
            break;
    }
}

// Fungsi untuk menangani input special key (Page Up/Down untuk vertikal)
void specialPressKey(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_PAGE_UP:
            deltaUp = 1;
            break;
        case GLUT_KEY_PAGE_DOWN:
            deltaUp = -1;
            break;
    }
}

void specialReleaseKey(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_PAGE_UP:
        case GLUT_KEY_PAGE_DOWN:
            deltaUp = 0;
            break;
    }
}

//// Fungsi untuk menangani tombol arah
// void pressKey(int key, int x, int y) {
//     switch (key) {
//         case GLUT_KEY_LEFT:
//             // Perubahan: kecepatan rotasi lebih lambat
//             deltaAngle = -0.0005f;
//             break;
//         case GLUT_KEY_RIGHT:
//             deltaAngle = 0.0005f;
//             break;
//         case GLUT_KEY_UP:
//             deltaMove = 1;
//             break;
//         case GLUT_KEY_DOWN:
//             deltaMove = -1;
//             break;
//     }
// }
//
//// Fungsi untuk menangani pelepasan tombol
// void releaseKey(int key, int x, int y) {
//     switch (key) {
//         case GLUT_KEY_LEFT:
//             if (deltaAngle < 0.0f)
//                 deltaAngle = 0.0f;
//             break;
//         case GLUT_KEY_RIGHT:
//             if (deltaAngle > 0.0f)
//                 deltaAngle = 0.0f;
//             break;
//         case GLUT_KEY_UP:
//             if (deltaMove > 0)
//                 deltaMove = 0;
//             break;
//         case GLUT_KEY_DOWN:
//             if (deltaMove < 0)
//                 deltaMove = 0;
//             break;
//     }
// }

void init() {
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 1.0, 1.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("3D Trash Bin");
    
//    glutIgnoreKeyRepeat(1);
//    glutSpecialFunc(pressKey);
//    glutSpecialUpFunc(releaseKey);
  	glutIgnoreKeyRepeat(1);
    glutKeyboardFunc(pressKey);
    glutKeyboardUpFunc(releaseKey);
    glutSpecialFunc(specialPressKey);
    glutSpecialUpFunc(specialReleaseKey);
    glutDisplayFunc(display);
    init();
    
    glutMainLoop();
    return 0;
}

