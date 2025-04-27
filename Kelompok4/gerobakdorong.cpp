#include <math.h>
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>

// Struktur untuk menyimpan koordinat titik (vertex)
struct Vertex {
    float x, y, z;
};

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

// Properti pencahayaan dan material
const GLfloat light_ambient[]  = { 0.3f, 0.3f, 0.3f, 1.0f };
const GLfloat light_diffuse[]  = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 0.0f, 20.0f, 10.0f, 1.0f };

const GLfloat mat_ambient[]    = { 0.2f, 0.2f, 0.2f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.6f, 0.3f, 0.3f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 50.0f };

// Fungsi untuk menangani perubahan ukuran window
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

// Fungsi untuk merotasi arah pandang kamera berdasarkan yaw (angle) dan pitch (pitchAngle)
void orientMe(float yaw, float pitch) {
    lx = cos(pitch) * sin(yaw);
    ly = sin(pitch);
    lz = -cos(pitch) * cos(yaw);
    glLoadIdentity();
    gluLookAt(x, y, z, x + lx, y + ly, z + lz, 0.0f, 1.0f, 0.0f);
}

// Fungsi untuk menggerakkan kamera maju/mundur (pergerakan horizontal)
void moveMeFlat(int i) {
    float horizontalLX = sin(angle);
    float horizontalLZ = -cos(angle);
    x += i * horizontalLX * movementSpeed;
    z += i * horizontalLZ * movementSpeed;
    glLoadIdentity();
    gluLookAt(x, y, z, x + lx, y + ly, z + lz, 0.0f, 1.0f, 0.0f);
}

// Fungsi untuk menggerakkan kamera secara vertikal (naik/turun)
void moveVertical(int i) {
    y += i * movementSpeed;
    glLoadIdentity();
    gluLookAt(x, y, z, x + lx, y + ly, z + lz, 0.0f, 1.0f, 0.0f);
}

// Fungsi untuk menggambar grid di tanah
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

// Fungsi untuk menggambar sebuah balok dengan vertex yang dikustomisasi
// Normal disisipkan agar pencahayaan dapat dihitung secara tepat
void drawBlockCustom(struct Vertex v[8], GLubyte r, GLubyte g, GLubyte b) {
    glColor3ub(r, g, b);
    glBegin(GL_QUADS);
        // Front face (depan) dengan normal (0, 0, 1)
        glNormal3f(0.0f, 0.0f, 1.0f);
        glVertex3f(v[4].x, v[4].y, v[4].z);
        glVertex3f(v[5].x, v[5].y, v[5].z);
        glVertex3f(v[6].x, v[6].y, v[6].z);
        glVertex3f(v[7].x, v[7].y, v[7].z);
        
        // Back face (belakang) dengan normal (0, 0, -1)
        glNormal3f(0.0f, 0.0f, -1.0f);
        glVertex3f(v[0].x, v[0].y, v[0].z);
        glVertex3f(v[1].x, v[1].y, v[1].z);
        glVertex3f(v[2].x, v[2].y, v[2].z);
        glVertex3f(v[3].x, v[3].y, v[3].z);
        
        // Left face (kiri) dengan normal (-1, 0, 0)
        glNormal3f(-1.0f, 0.0f, 0.0f);
        glVertex3f(v[0].x, v[0].y, v[0].z);
        glVertex3f(v[3].x, v[3].y, v[3].z);
        glVertex3f(v[7].x, v[7].y, v[7].z);
        glVertex3f(v[4].x, v[4].y, v[4].z);
        
        // Right face (kanan) dengan normal (1, 0, 0)
        glNormal3f(1.0f, 0.0f, 0.0f);
        glVertex3f(v[1].x, v[1].y, v[1].z);
        glVertex3f(v[2].x, v[2].y, v[2].z);
        glVertex3f(v[6].x, v[6].y, v[6].z);
        glVertex3f(v[5].x, v[5].y, v[5].z);
        
        // Top face (atas) dengan normal (0, 1, 0)
        glNormal3f(0.0f, 1.0f, 0.0f);
        glVertex3f(v[3].x, v[3].y, v[3].z);
        glVertex3f(v[2].x, v[2].y, v[2].z);
        glVertex3f(v[6].x, v[6].y, v[6].z);
        glVertex3f(v[7].x, v[7].y, v[7].z);
        
        // Bottom face (bawah) dengan normal (0, -1, 0)
        glNormal3f(0.0f, -1.0f, 0.0f);
        glVertex3f(v[0].x, v[0].y, v[0].z);
        glVertex3f(v[1].x, v[1].y, v[1].z);
        glVertex3f(v[5].x, v[5].y, v[5].z);
        glVertex3f(v[4].x, v[4].y, v[4].z);
    glEnd();
}

// Fungsi untuk menggambar detail velg (rim) menggunakan gluDisk
void drawVelg() {
    glColor3f(0.8f, 0.8f, 0.8f);
    GLUquadric* quad = gluNewQuadric();
    gluDisk(quad, 0.0, 0.25, 20, 1);
    gluDeleteQuadric(quad);
}

// Fungsi untuk menggambar roda 3D (samping kanan/kiri) dengan detail velg
void drawWheel1() {
    glColor3f(0.2f, 0.2f, 0.2f);
    glutSolidTorus(0.3, 0.6, 30, 30); // glutSolidTorus(innerRadius, outerRadius, slices, stacks);
    glPushMatrix();
        drawVelg();
    glPopMatrix();
}

// Fungsi untuk menggambar roda 3D (bagian belakang) dengan detail velg
void drawWheel2() {
    glColor3f(0.2f, 0.2f, 0.2f);
    glutSolidTorus(0.2, 0.4, 30, 30); // glutSolidTorus(innerRadius, outerRadius, slices, stacks);
    glPushMatrix();
        drawVelg();
    glPopMatrix();
}

// Fungsi untuk menggambar tabung 3D dengan rotasi 90 derajat searah jarum jam terhadap sumbu X
void drawCylinder(float baseRadius, float topRadius, float height, int slices, int stacks) {
    GLUquadric* quad = gluNewQuadric();
    glPushMatrix();
        glRotatef(-90, 0.0f, 0.0f, 1.0f);
        gluCylinder(quad, baseRadius, topRadius, height, slices, stacks);
    glPopMatrix();
    gluDeleteQuadric(quad);
}

// Fungsi untuk menggambar gerobak
void gerobak() {
    // Balok 1:
    Vertex block1[8] = {
        { -1.5, 0.6, 9.85 },  // v0: kiri-bawah-belakang
        {  1.5, 0.6, 9.85 },  // v1: kanan-bawah-belakang
        {  2.0, 3.0, 10.3 },  // v2: kanan-atas-belakang
        { -2.0, 3.0, 10.3 },  // v3: kiri-atas-belakang
        { -1.5, 0.6, 10.05 }, // v4: kiri-bawah-depan
        {  1.5, 0.6, 10.05 }, // v5: kanan-bawah-depan
        {  2.0, 3.0, 10.5 },  // v6: kanan-atas-depan
        { -2.0, 3.0, 10.5 }   // v7: kiri-atas-depan
    };
    drawBlockCustom(block1, 216, 64, 64);
    
    // Balok 2:
    Vertex block2[8] = {
        {  1.3, 0.6, 8.05 }, // v0
        {  1.5, 0.6, 8.05 }, // v1
        {  2.0, 3.0, 7.6 }, // v2
        {  1.8, 3.0, 7.6 }, // v3
        {  1.3, 0.6, 9.85 }, // v4
        {  1.5, 0.6, 9.85 }, // v5
        {  2.0, 3.0, 10.3 }, // v6
        {  1.8, 3.0, 10.3 }  // v7
    };
    drawBlockCustom(block2, 216, 64, 64);
    
    // Balok 3:
    Vertex block3[8] = {
        { -1.5, 0.6, 8.05 }, // v0
        { -1.45, 0.6, 8.05 }, // v1
        { -1.75, 3.0, 7.6 }, // v2
        { -2.0, 3.0, 7.6  }, // v3
        { -1.5, 0.6, 9.85 }, // v4
        { -1.45, 0.6, 9.85 }, // v5
        { -1.75, 3.0, 10.3 },// v6
        { -2.0, 3.0, 10.3 }  // v7
    };
    drawBlockCustom(block3, 216, 64, 64);
    
    // Balok 4:
    Vertex block4[8] = {
        { -1.5, 0.4, 7.85 }, // v0
        {  1.5, 0.4, 7.85 }, // v1
        {  1.5, 0.6, 7.85 }, // v2
        { -1.5, 0.6, 7.85 }, // v3
        { -1.5, 0.4, 10.05 }, // v4
        {  1.5, 0.4, 10.05 }, // v5
        {  1.5, 0.6, 10.05 }, // v6
        { -1.5, 0.6, 10.05 }  // v7
    };
    drawBlockCustom(block4, 163, 29, 29);
    
    // Balok 5:
    Vertex block5[8] = {
        { -1.5, 0.6, 7.85 }, // v0
        {  1.5, 0.6, 7.85 }, // v1
        {  2.0, 3.0, 7.4 }, // v2
        { -2.0, 3.0, 7.4 }, // v3
        { -1.5, 0.6, 8.05 }, // v4
        {  1.5, 0.6, 8.05 }, // v5
        {  2.0, 3.0, 7.6 }, // v6
        { -2.0, 3.0, 7.6 }  // v7
    };
    drawBlockCustom(block5, 216, 64, 64);
    
    // Pegangan kanan
    Vertex block6[8] = {
        { -2.5, 2.8, 10.3 }, // v0
        { -1.9, 2.5, 10.2 }, // v1
        { -2.0, 3.0, 10.3 }, // v2
        { -2.5, 3.0, 10.3 }, // v3
        { -2.5, 2.8, 10.5 }, // v4
        { -1.9, 2.5, 10.4 }, // v5
        { -2.0, 3.0, 10.5 }, // v6
        { -2.5, 3.0, 10.5 }  // v7
    };
    drawBlockCustom(block6, 255, 255, 0);
    
    // Pegangan kiri
    Vertex block7[8] = {
        { -2.5, 2.8, 7.4 }, // v0
        { -1.9, 2.5, 7.5 }, // v1
        { -2.0, 3.0, 7.4 }, // v2
        { -2.5, 3.0, 7.4 }, // v3
        { -2.5, 2.8, 7.6 }, // v4
        { -1.9, 2.5, 7.7 }, // v5
        { -2.0, 3.0, 7.6 }, // v6
        { -2.5, 3.0, 7.6 }  // v7
    };
    drawBlockCustom(block7, 255, 255, 0);
    
    // Pegangan tengah
    Vertex block8[8] = {
        { -2.5, 2.8, 7.6 }, // v0
        { -2.3, 2.8, 7.6 }, // v1
        { -2.3, 3.0, 7.6 }, // v2
        { -2.5, 3.0, 7.6 }, // v3
        { -2.5, 2.8, 10.3 }, // v4
        { -2.3, 2.8, 10.3 }, // v5
        { -2.3, 3.0, 10.3 }, // v6
        { -2.5, 3.0, 10.3 }  // v7
    };
    drawBlockCustom(block8, 255, 255, 0);
    
    // Roda depan kiri
    glPushMatrix();
        glTranslatef(0.5, 0.3, 7.4);
        drawWheel1();
    glPopMatrix();

    // Roda depan kanan
    glPushMatrix();
        glTranslatef(0.5, 0.3, 10.5);
        drawWheel1();
    glPopMatrix();
    
    // Roda belakang
    glPushMatrix();
        glTranslatef(-2.0, 0.0, 8.95);
        drawWheel2();
    glPopMatrix();

	//detail roda
    glPushMatrix();
        glTranslatef(0.5, 0.3, 7.4);
        glColor3f(0.8f, 0.8f, 0.8f);
        drawCylinder(0.1, 0.1, 3.1, 30, 30);
    glPopMatrix();
    
    Vertex block9[8] = {
        { -2.15, -0.05, 9.2 }, // v0
        { -1.85, -0.05, 9.2 }, // v1
        {  -1.2, 0.4, 9.2 }, // v2
        { -1.5, 0.4, 9.2 }, // v3
        { -2.15, -0.05, 9.3 }, // v4
        { -1.85, -0.05, 9.3 }, // v5
        { -1.2, 0.4, 9.3 }, // v6
        { -1.5, 0.4, 9.3 }  // v7
    };
    drawBlockCustom(block9, 163, 29, 29);

    Vertex block10[8] = {
        { -2.15, -0.05, 8.6 }, // v0
        { -1.85, -0.05, 8.6 }, // v1
        {  -1.2, 0.4, 8.6 }, // v2
        { -1.5, 0.4, 8.6 }, // v3
        { -2.15, -0.05, 8.7 }, // v4
        { -1.85, -0.05, 8.7 }, // v5
        { -1.2, 0.4, 8.7 }, // v6
        { -1.5, 0.4, 8.7 }  // v7
    };
    drawBlockCustom(block10, 163, 29, 29);

    glPushMatrix();
        glTranslatef(-2.0, 0.0, 8.7);
        glColor3f(0.8f, 0.8f, 0.8f);
        drawCylinder(0.05, 0.05, 0.5, 30, 30);
    glPopMatrix();
}

// Fungsi display untuk merender scene
void display() {
    if (deltaMove)
        moveMeFlat(deltaMove);
    if (deltaAngle || deltaPitch) {
        angle += deltaAngle;
        pitchAngle += deltaPitch;
        orientMe(angle, pitchAngle);
    }
    if (deltaUp)
        moveVertical(deltaUp);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    Grid();
    gerobak();

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

// Fungsi untuk mengatur pencahayaan dan material
void lighting() {
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);

    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
}

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
    glutCreateWindow("3D Object : Gerobak Dorong");

    glutIgnoreKeyRepeat(1);
    glutKeyboardFunc(pressKey);
    glutKeyboardUpFunc(releaseKey);
    glutSpecialFunc(specialPressKey);
    glutSpecialUpFunc(specialReleaseKey);
    glutDisplayFunc(display);
    glutIdleFunc(display);
    glutReshapeFunc(Reshape);

    lighting();
    init();

    glutMainLoop();
    return 0;
}
