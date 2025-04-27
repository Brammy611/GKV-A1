#include <math.h>
#include <GL/glut.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Variabel posisi dan arah kamera
float angle = 0.0f, deltaAngle = 0.0f;
float pitch = 0.0f, deltaPitch = 0.0f;
float x = 0.0f, y = 1.75f, z = 15.0f;  
// vektor arah kamera (dihitung dari angle dan pitch)
float lx = 0.0f, ly = 0.0f, lz = -1.0f; 

int  h, w; 
int bitmapHeight = 12;

// Variabel pergerakan posisi
float deltaMove = 0.0f;   // maju/mundur (W/S)
float strafeMove = 0.0f;    // geser kiri/kanan (A/D)
float verticalMove = 0.0f;  // naik/turun posisi (Space/V)

float ratio;

// Fungsi untuk menghitung ulang arah kamera berdasarkan angle dan pitch
void orientMe() {
    lx = cos(pitch) * sin(angle);
    ly = sin(pitch);
    lz = -cos(pitch) * cos(angle);
    glLoadIdentity();
    gluLookAt(x, y, z,
              x + lx, y + ly, z + lz,
              0.0f, 1.0f, 0.0f);
}

// Fungsi Reshape untuk mengatur viewport dan perspektif
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
    orientMe();
}

// Fungsi untuk pergerakan maju/mundur (menggerakkan posisi ke depan/belakang)
void moveMeFlat(float i) {
    x += i * lx * 0.01;
    z += i * lz * 0.01;
    orientMe();
}

// Fungsi untuk geser kiri/kanan (perubahan posisi)
void strafeCamera(float i) {
    x += i * (-lz) * 0.01;
    z += i * lx * 0.01;
    orientMe();
}

// Fungsi untuk mengubah posisi vertikal (naik/turun posisi, seperti terbang)
void moveVertical(float i) {
    y += i * 0.01;
    orientMe();
}

// Fungsi menggambar grid sebagai alas
void Grid() {
    double i;
    const float Z_MIN = -50, Z_MAX = 50;
    const float X_MIN = -50, X_MAX = 50;
    const float gap = 1.5;
    
    glColor3f(0.5f, 0.5f, 0.5f);
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


void drawBrick(float width, float height, float depth) {
    glPushMatrix();
        glScalef(width, height, depth);
        glutSolidCube(1.0f);
    glPopMatrix();
}


void rumah() {
    { // Blok untuk lantai bawah
        glPushMatrix();
            glColor3ub(90, 81, 102); 
            glTranslatef(0.0f, 0.35f, 0.0f);
            float halfWidth  = 5.5;   
            float halfHeight = 0.35f;  
            float halfDepth  = 5.5f;  

            glBegin(GL_QUADS);
                // Face atas
                glVertex3f(-halfWidth,  halfHeight, -halfDepth);
                glVertex3f( halfWidth,  halfHeight, -halfDepth);
                glVertex3f( halfWidth,  halfHeight,  halfDepth);
                glVertex3f(-halfWidth,  halfHeight,  halfDepth);

                // Face bawah
                glVertex3f(-halfWidth, -halfHeight, -halfDepth);
                glVertex3f( halfWidth, -halfHeight, -halfDepth);
                glVertex3f( halfWidth, -halfHeight,  halfDepth);
                glVertex3f(-halfWidth, -halfHeight,  halfDepth);

                // Face depan
                glVertex3f(-halfWidth, -halfHeight,  halfDepth);
                glVertex3f( halfWidth, -halfHeight,  halfDepth);
                glVertex3f( halfWidth,  halfHeight,  halfDepth);
                glVertex3f(-halfWidth,  halfHeight,  halfDepth);

                // Face belakang
                glVertex3f(-halfWidth, -halfHeight, -halfDepth);
                glVertex3f( halfWidth, -halfHeight, -halfDepth);
                glVertex3f( halfWidth,  halfHeight, -halfDepth);
                glVertex3f(-halfWidth,  halfHeight, -halfDepth);

                // Face kiri
                glVertex3f(-halfWidth, -halfHeight, -halfDepth);
                glVertex3f(-halfWidth, -halfHeight,  halfDepth);
                glVertex3f(-halfWidth,  halfHeight,  halfDepth);
                glVertex3f(-halfWidth,  halfHeight, -halfDepth);

                // Face kanan
                glVertex3f( halfWidth, -halfHeight, -halfDepth);
                glVertex3f( halfWidth, -halfHeight,  halfDepth);
                glVertex3f( halfWidth,  halfHeight,  halfDepth);
                glVertex3f( halfWidth,  halfHeight, -halfDepth);
            glEnd();
        glPopMatrix();
    } 

    { // Blok untuk badan rumah 
        glPushMatrix();
            glColor3ub(118, 148, 61); 

            glTranslatef(0.0f, 4.6f, 0.0f);
            float halfWallWidth  = 4.5f;
            float halfWallHeight = 4.0f;
            float halfWallDepth  = 4.5f;

            glBegin(GL_QUADS);
                // Face atas
                glVertex3f(-halfWallWidth,  halfWallHeight, -halfWallDepth);
                glVertex3f( halfWallWidth,  halfWallHeight, -halfWallDepth);
                glVertex3f( halfWallWidth,  halfWallHeight,  halfWallDepth);
                glVertex3f(-halfWallWidth,  halfWallHeight,  halfWallDepth);

                // Face bawah
                glVertex3f(-halfWallWidth, -halfWallHeight, -halfWallDepth);
                glVertex3f( halfWallWidth, -halfWallHeight, -halfWallDepth);
                glVertex3f( halfWallWidth, -halfWallHeight,  halfWallDepth);
                glVertex3f(-halfWallWidth, -halfWallHeight,  halfWallDepth);

                // Face depan
                glVertex3f(-halfWallWidth, -halfWallHeight,  halfWallDepth);
                glVertex3f( halfWallWidth, -halfWallHeight,  halfWallDepth);
                glVertex3f( halfWallWidth,  halfWallHeight,  halfWallDepth);
                glVertex3f(-halfWallWidth,  halfWallHeight,  halfWallDepth);

                // Face belakang
                glVertex3f(-halfWallWidth, -halfWallHeight, -halfWallDepth);
                glVertex3f( halfWallWidth, -halfWallHeight, -halfWallDepth);
                glVertex3f( halfWallWidth,  halfWallHeight, -halfWallDepth);
                glVertex3f(-halfWallWidth,  halfWallHeight, -halfWallDepth);

                // Face kiri
                glVertex3f(-halfWallWidth, -halfWallHeight, -halfWallDepth);
                glVertex3f(-halfWallWidth, -halfWallHeight,  halfWallDepth);
                glVertex3f(-halfWallWidth,  halfWallHeight,  halfWallDepth);
                glVertex3f(-halfWallWidth,  halfWallHeight, -halfWallDepth);

                // Face kanan
                glVertex3f( halfWallWidth, -halfWallHeight, -halfWallDepth);
                glVertex3f( halfWallWidth, -halfWallHeight,  halfWallDepth);
                glVertex3f( halfWallWidth,  halfWallHeight,  halfWallDepth);
                glVertex3f( halfWallWidth,  halfWallHeight, -halfWallDepth);
            glEnd();
        glPopMatrix();
    } 

    { // Blok untuk lantai tengah
        glPushMatrix();
            glColor3ub(90, 81, 102); 
            glTranslatef(0.0f, 9.0f, 0.0f);
            float halfWidth  = 5.0f;  
            float halfHeight = 0.3f; 
            float halfDepth  = 5.0f;  

            glBegin(GL_QUADS);
                // Face atas 
                glVertex3f(-halfWidth,  halfHeight, -halfDepth);
                glVertex3f( halfWidth,  halfHeight, -halfDepth);
                glVertex3f( halfWidth,  halfHeight,  halfDepth);
                glVertex3f(-halfWidth,  halfHeight,  halfDepth);

                // Face bawah
                glVertex3f(-halfWidth, -halfHeight, -halfDepth);
                glVertex3f( halfWidth, -halfHeight, -halfDepth);
                glVertex3f( halfWidth, -halfHeight,  halfDepth);
                glVertex3f(-halfWidth, -halfHeight,  halfDepth);

                // Face depan
                glVertex3f(-halfWidth, -halfHeight,  halfDepth);
                glVertex3f( halfWidth, -halfHeight,  halfDepth);
                glVertex3f( halfWidth,  halfHeight,  halfDepth);
                glVertex3f(-halfWidth,  halfHeight,  halfDepth);

                // Face belakang
                glVertex3f(-halfWidth, -halfHeight, -halfDepth);
                glVertex3f( halfWidth, -halfHeight, -halfDepth);
                glVertex3f( halfWidth,  halfHeight, -halfDepth);
                glVertex3f(-halfWidth,  halfHeight, -halfDepth);

                // Face kiri
                glVertex3f(-halfWidth, -halfHeight, -halfDepth);
                glVertex3f(-halfWidth, -halfHeight,  halfDepth);
                glVertex3f(-halfWidth,  halfHeight,  halfDepth);
                glVertex3f(-halfWidth,  halfHeight, -halfDepth);

                // Face kanan
                glVertex3f( halfWidth, -halfHeight, -halfDepth);
                glVertex3f( halfWidth, -halfHeight,  halfDepth);
                glVertex3f( halfWidth,  halfHeight,  halfDepth);
                glVertex3f( halfWidth,  halfHeight, -halfDepth);
            glEnd();
        glPopMatrix();
    } 

    { // Blok untuk badan rumah 2
        glPushMatrix();
            glColor3ub(118, 148, 61); 
            glTranslatef(0.0f, 12.4f, 0.0f);
            float halfWallWidth  = 4.5f;
            float halfWallHeight = 4.0f;
            float halfWallDepth  = 4.5f;

            glBegin(GL_QUADS);
                // Face atas
                glVertex3f(-halfWallWidth,  halfWallHeight, -halfWallDepth);
                glVertex3f( halfWallWidth,  halfWallHeight, -halfWallDepth);
                glVertex3f( halfWallWidth,  halfWallHeight,  halfWallDepth);
                glVertex3f(-halfWallWidth,  halfWallHeight,  halfWallDepth);

                // Face bawah
                glVertex3f(-halfWallWidth, -halfWallHeight, -halfWallDepth);
                glVertex3f( halfWallWidth, -halfWallHeight, -halfWallDepth);
                glVertex3f( halfWallWidth, -halfWallHeight,  halfWallDepth);
                glVertex3f(-halfWallWidth, -halfWallHeight,  halfWallDepth);

                // Face depan
                glVertex3f(-halfWallWidth, -halfWallHeight,  halfWallDepth);
                glVertex3f( halfWallWidth, -halfWallHeight,  halfWallDepth);
                glVertex3f( halfWallWidth,  halfWallHeight,  halfWallDepth);
                glVertex3f(-halfWallWidth,  halfWallHeight,  halfWallDepth);

                // Face belakang
                glVertex3f(-halfWallWidth, -halfWallHeight, -halfWallDepth);
                glVertex3f( halfWallWidth, -halfWallHeight, -halfWallDepth);
                glVertex3f( halfWallWidth,  halfWallHeight, -halfWallDepth);
                glVertex3f(-halfWallWidth,  halfWallHeight, -halfWallDepth);

                // Face kiri
                glVertex3f(-halfWallWidth, -halfWallHeight, -halfWallDepth);
                glVertex3f(-halfWallWidth, -halfWallHeight,  halfWallDepth);
                glVertex3f(-halfWallWidth,  halfWallHeight,  halfWallDepth);
                glVertex3f(-halfWallWidth,  halfWallHeight, -halfWallDepth);

                // Face kanan
                glVertex3f( halfWallWidth, -halfWallHeight, -halfWallDepth);
                glVertex3f( halfWallWidth, -halfWallHeight,  halfWallDepth);
                glVertex3f( halfWallWidth,  halfWallHeight,  halfWallDepth);
                glVertex3f( halfWallWidth,  halfWallHeight, -halfWallDepth);
            glEnd();
        glPopMatrix();
    } 
 
    { // atap 1
        glPushMatrix();
            glColor3ub(90, 81, 102); 
            glTranslatef(0.0f, 16.7f, 0.0f);
            float halfWidth  = 5.0f;   
            float halfHeight = 0.3f; 
            float halfDepth  = 5.0f;   

            glBegin(GL_QUADS);
                // Face atas 
                glVertex3f(-halfWidth,  halfHeight, -halfDepth);
                glVertex3f( halfWidth,  halfHeight, -halfDepth);
                glVertex3f( halfWidth,  halfHeight,  halfDepth);
                glVertex3f(-halfWidth,  halfHeight,  halfDepth);

                // Face bawah
                glVertex3f(-halfWidth, -halfHeight, -halfDepth);
                glVertex3f( halfWidth, -halfHeight, -halfDepth);
                glVertex3f( halfWidth, -halfHeight,  halfDepth);
                glVertex3f(-halfWidth, -halfHeight,  halfDepth);

                // Face depan
                glVertex3f(-halfWidth, -halfHeight,  halfDepth);
                glVertex3f( halfWidth, -halfHeight,  halfDepth);
                glVertex3f( halfWidth,  halfHeight,  halfDepth);
                glVertex3f(-halfWidth,  halfHeight,  halfDepth);

                // Face belakang
                glVertex3f(-halfWidth, -halfHeight, -halfDepth);
                glVertex3f( halfWidth, -halfHeight, -halfDepth);
                glVertex3f( halfWidth,  halfHeight, -halfDepth);
                glVertex3f(-halfWidth,  halfHeight, -halfDepth);

                // Face kiri
                glVertex3f(-halfWidth, -halfHeight, -halfDepth);
                glVertex3f(-halfWidth, -halfHeight,  halfDepth);
                glVertex3f(-halfWidth,  halfHeight,  halfDepth);
                glVertex3f(-halfWidth,  halfHeight, -halfDepth);

                // Face kanan
                glVertex3f( halfWidth, -halfHeight, -halfDepth);
                glVertex3f( halfWidth, -halfHeight,  halfDepth);
                glVertex3f( halfWidth,  halfHeight,  halfDepth);
                glVertex3f( halfWidth,  halfHeight, -halfDepth);
            glEnd();
        glPopMatrix();
    } 

    { // atap 2
    glPushMatrix();
        glColor3ub(90, 81, 102);
        glTranslatef(0.0f, 17.3f, 0.0f);
        float halfWidth  = 5.2f;   
        float halfHeight = 0.3f;
        float halfDepth  = 5.2f; 

        glBegin(GL_QUADS);
            // Face atas 
            glVertex3f(-halfWidth,  halfHeight, -halfDepth);
            glVertex3f( halfWidth,  halfHeight, -halfDepth);
            glVertex3f( halfWidth,  halfHeight,  halfDepth);
            glVertex3f(-halfWidth,  halfHeight,  halfDepth);

            // Face bawah
            glVertex3f(-halfWidth, -halfHeight, -halfDepth);
            glVertex3f( halfWidth, -halfHeight, -halfDepth);
            glVertex3f( halfWidth, -halfHeight,  halfDepth);
            glVertex3f(-halfWidth, -halfHeight,  halfDepth);

            // Face depan
            glVertex3f(-halfWidth, -halfHeight,  halfDepth);
            glVertex3f( halfWidth, -halfHeight,  halfDepth);
            glVertex3f( halfWidth,  halfHeight,  halfDepth);
            glVertex3f(-halfWidth,  halfHeight,  halfDepth);

            // Face belakang
            glVertex3f(-halfWidth, -halfHeight, -halfDepth);
            glVertex3f( halfWidth, -halfHeight, -halfDepth);
            glVertex3f( halfWidth,  halfHeight, -halfDepth);
            glVertex3f(-halfWidth,  halfHeight, -halfDepth);

            // Face kiri
            glVertex3f(-halfWidth, -halfHeight, -halfDepth);
            glVertex3f(-halfWidth, -halfHeight,  halfDepth);
            glVertex3f(-halfWidth,  halfHeight,  halfDepth);
            glVertex3f(-halfWidth,  halfHeight, -halfDepth);

            // Face kanan
            glVertex3f( halfWidth, -halfHeight, -halfDepth);
            glVertex3f( halfWidth, -halfHeight,  halfDepth);
            glVertex3f( halfWidth,  halfHeight,  halfDepth);
            glVertex3f( halfWidth,  halfHeight, -halfDepth);
        glEnd();
    glPopMatrix();
 } 
 
    { // pintu dan jendela
    glPushMatrix();

        glTranslatef(0.0f, 0.35f, 4.65f); 
        glColor3ub(133, 101, 14);

        // Ukuran pintu
        float doorWidth     = 2.5f;  
        float doorHeight    = 4.0f;  
        float doorThickness = 0.1f;  
        float archRadius    = 1.25f; 
        int   segments      = 16;    // jumlah segmen lengkungan

        // Variabel pembantu
        float w2 = doorWidth * 0.5f; // setengah lebar
        float t  = doorThickness;
        float h  = doorHeight;

        // Depan
        glBegin(GL_QUADS);
            glVertex3f(-w2, 0.0f,       0.0f);
            glVertex3f( w2, 0.0f,       0.0f);
            glVertex3f( w2, h,          0.0f);
            glVertex3f(-w2, h,          0.0f);
        glEnd();

        // Belakang
        glBegin(GL_QUADS);
            glVertex3f(-w2, 0.0f,      -t);
            glVertex3f( w2, 0.0f,      -t);
            glVertex3f( w2, h,         -t);
            glVertex3f(-w2, h,         -t);
        glEnd();

        // Sisi bawah
        glBegin(GL_QUADS);
            glVertex3f(-w2, 0.0f,  0.0f);
            glVertex3f(-w2, 0.0f, -t);
            glVertex3f( w2, 0.0f, -t);
            glVertex3f( w2, 0.0f,  0.0f);
        glEnd();

        // Sisi atas 
        glBegin(GL_QUADS);
            glVertex3f(-w2, h,  0.0f);
            glVertex3f(-w2, h, -t);
            glVertex3f( w2, h, -t);
            glVertex3f( w2, h,  0.0f);
        glEnd();

        // Sisi kiri
        glBegin(GL_QUADS);
            glVertex3f(-w2, 0.0f,  0.0f);
            glVertex3f(-w2, h,     0.0f);
            glVertex3f(-w2, h,    -t);
            glVertex3f(-w2, 0.0f, -t);
        glEnd();

        // Sisi kanan
        glBegin(GL_QUADS);
            glVertex3f(w2, 0.0f,  0.0f);
            glVertex3f(w2, h,     0.0f);
            glVertex3f(w2, h,    -t);
            glVertex3f(w2, 0.0f, -t);
        glEnd();

    
        // border
        {    float borderThickness = 0.1f;
        glColor3ub(90, 81, 102);

        // border atas
        glPushMatrix();
            glTranslatef(0.0f, h + borderThickness*0.5f, -t*0.5f);
            glScalef(doorWidth + 2*borderThickness, borderThickness, doorThickness + 2*borderThickness);
            glutSolidCube(1.0f);
        glPopMatrix();

        // 2) Border sisi kiri
        glPushMatrix();
            glTranslatef(-w2 - borderThickness*0.5f, h*0.5f, -t*0.5f);
            glScalef(borderThickness, doorHeight, doorThickness + 2*borderThickness);
            glutSolidCube(1.0f);
        glPopMatrix();

        // 3) Border sisi kanan 
        glPushMatrix();
            glTranslatef(w2 + borderThickness*0.5f, h*0.5f, -t*0.5f);
            glScalef(borderThickness, doorHeight, doorThickness + 2*borderThickness);
            glutSolidCube(1.0f);
        glPopMatrix();}



        // 1. Bagian kaca 
        {
            glPushMatrix();
            glTranslatef(0.0f, 0.0f, -0.08f);
            glColor3ub(107, 215, 255);

            float baseY = h + 0.5f;  // pangkal lengkungan
            // Bagian depan (z=0)
            glBegin(GL_TRIANGLE_FAN);
                glVertex3f(0.0f, baseY, 0.0f); // pusat lengkungan
                for(int i = 0; i <= segments; i++) {
                    float theta = 3.14159f * i / segments; 
                    float xPos = archRadius * cosf(theta);
                    float yPos = archRadius * sinf(theta);
                    glVertex3f(xPos, baseY + yPos, 0.0f);
                }
            glEnd();
 
            // Bagian belakang (z = -t)
            glBegin(GL_TRIANGLE_FAN);
                glVertex3f(0.0f, baseY, -t);
                for(int i = 0; i <= segments; i++) {
                    float theta = 3.14159f * i / segments;
                    float xPos = archRadius * cosf(theta);
                    float yPos = archRadius * sinf(theta);
                    glVertex3f(xPos, baseY + yPos, -t);
                }
            glEnd();

            // Sisi lengkungan 
            glBegin(GL_QUAD_STRIP);
                for(int i = 0; i <= segments; i++) {
                    float theta = 3.14159f * i / segments;
                    float xPos = archRadius * cosf(theta);
                    float yPos = archRadius * sinf(theta);
                    glVertex3f(xPos, baseY + yPos, 0.0f);
                    glVertex3f(xPos, baseY + yPos, -t);
                }
            glEnd();

            glPopMatrix();
        }

        // 2. Border 
        {
            glColor3ub(255, 255, 255);

            float borderThickness = 0.1f;
            float outerArchRadius = archRadius + borderThickness;
            float archBaseY = h + 0.5f; 

            // Bagian depan border 
            glBegin(GL_QUAD_STRIP);
            for(int i = 0; i <= segments; i++) {
                float theta = 3.14159f * i / segments;
                float xOuter = outerArchRadius * cosf(theta);
                float yOuter = outerArchRadius * sinf(theta);
                float xInner = archRadius * cosf(theta);
                float yInner = archRadius * sinf(theta);

                glVertex3f(xOuter, archBaseY + yOuter, 0.0f); 
                glVertex3f(xInner, archBaseY + yInner, 0.0f);
            }
            glEnd();

            // Bagian belakang border 
            glBegin(GL_QUAD_STRIP);
            for(int i = 0; i <= segments; i++) {
                float theta = 3.14159f * i / segments;
                float xOuter = outerArchRadius * cosf(theta);
                float yOuter = outerArchRadius * sinf(theta);
                float xInner = archRadius * cosf(theta);
                float yInner = archRadius * sinf(theta);

                glVertex3f(xOuter, archBaseY + yOuter, -t);
                glVertex3f(xInner, archBaseY + yInner, -t);
            }
            glEnd();

            // Sisi lengkungan border
            glBegin(GL_QUAD_STRIP);
            for(int i = 0; i <= segments; i++) {
                float theta = 3.14159f * i / segments;
                float xPos = outerArchRadius * cosf(theta);
                float yPos = outerArchRadius * sinf(theta);

                glVertex3f(xPos, archBaseY + yPos, 0.0f);
                glVertex3f(xPos, archBaseY + yPos, -t);
            }
            glEnd();

            // border atas
            glPushMatrix();
                glTranslatef(0.0f, 0.5 + h + borderThickness*0.5f, -t*0.5f);
                glScalef(doorWidth + 2*borderThickness, borderThickness, doorThickness + 2*borderThickness);
                glutSolidCube(1.0f);
             glPopMatrix();
        }

        // 3. Tralis 
        {
        {
            float tralisThickness = 0.05f;
            float R = archRadius;  // jari-jari lengkungan

            float centerX = 0.0f;
            float centerY = h + 0.5f;  
            float centerZ = 0.0f;   

            for (int i = 0; i < 3; i++) {
                float theta;
                if (i == 0)
                    theta = 3.14159f * 0.5f;   // 90° (vertikal)
                else if (i == 1)
                    theta = 3.14159f * 0.25f;  // 45°
                else
                    theta = 3.14159f * 0.75f;  // 135°

                // Titik start adalah pusat
                float sx = centerX;
                float sy = centerY;
                float sz = centerZ;
                // Titik end dihitung dari pusat + offset berbasis theta
                float ex = centerX + R * cosf(theta);
                float ey = centerY + R * sinf(theta);
                float ez = centerZ;
                
                // Vektor dari start ke end:
                float dx = ex - sx;
                float dy = ey - sy;
                // Panjang vektor (seharusnya sama dengan R)
                float len = sqrt(dx * dx + dy * dy);
                // Vektor normal (perpendicular di bidang XY)
                float nx = -dy / len;
                float ny = dx / len;
                // Offset sebesar setengah ketebalan
                float ox = nx * (tralisThickness * 0.5f);
                float oy = ny * (tralisThickness * 0.5f);

                // Tentukan empat titik quad:
                // v1: start + offset
                float v1x = sx + ox, v1y = sy + oy, v1z = sz;
                // v2: start - offset
                float v2x = sx - ox, v2y = sy - oy, v2z = sz;
                // v3: end - offset
                float v3x = ex - ox, v3y = ey - oy, v3z = ez;
                // v4: end + offset
                float v4x = ex + ox, v4y = ey + oy, v4z = ez;
                
                glColor3ub(255, 255, 255);
                glBegin(GL_QUADS);
                    glVertex3f(v1x, v1y, v1z);
                    glVertex3f(v2x, v2y, v2z);
                    glVertex3f(v3x, v3y, v3z);
                    glVertex3f(v4x, v4y, v4z);
                glEnd();
            }
        }
     }

        // gagang pintu
        glPushMatrix();
             glTranslatef(-w2 + 0.2f, h * 0.5f, 0.0f);
             glColor3ub(80, 80, 80); 
             glutSolidSphere(0.08, 16, 16);
        glPopMatrix();

    glPopMatrix();
    }

    { // Jendela 1
    glPushMatrix();
        glTranslatef(2.0f, 12.85f, 4.65f); 
        float windowWidth  = 2.0f;
        float windowHeight = 3.0f;
        float windowDepth  = 0.1f;
        float frameThickness = 0.2f;
        float crossThickness = 0.1f;

        // Kaca jendela
        glColor3ub(107, 215, 255);
        glBegin(GL_QUADS);
            glVertex3f(-windowWidth * 0.5f, -windowHeight * 0.5f, 0.0f);
            glVertex3f( windowWidth * 0.5f, -windowHeight * 0.5f, 0.0f);
            glVertex3f( windowWidth * 0.5f,  windowHeight * 0.5f, 0.0f);
            glVertex3f(-windowWidth * 0.5f,  windowHeight * 0.5f, 0.0f);
        glEnd();

        // Frame 
        glColor3ub(255, 255, 255);
        // Atas
        glPushMatrix();
            glTranslatef(0.0f, windowHeight * 0.5f + frameThickness * 0.5f, 0.0f);
            glScalef(windowWidth + frameThickness * 2, frameThickness, windowDepth);
            glutSolidCube(1.0f);
        glPopMatrix();

        // Bawah
        glPushMatrix();
            glTranslatef(0.0f, -windowHeight * 0.5f - frameThickness * 0.5f, 0.0f);
            glScalef(windowWidth + frameThickness * 2, frameThickness, windowDepth);
            glutSolidCube(1.0f);
        glPopMatrix();


        // Kiri
        glPushMatrix();
            glTranslatef(-windowWidth * 0.5f - frameThickness * 0.5f, 0.0f, 0.0f);
            glScalef(frameThickness, windowHeight + frameThickness * 2, windowDepth);
            glutSolidCube(1.0f);
        glPopMatrix();

        // Kanan
        glPushMatrix();
            glTranslatef(windowWidth * 0.5f + frameThickness * 0.5f, 0.0f, 0.0f);
            glScalef(frameThickness, windowHeight + frameThickness * 2, windowDepth);
            glutSolidCube(1.0f);
        glPopMatrix();

        
        {glPushMatrix(); //alas
            glColor3ub(90, 81, 102); 

            float sillWidth  = 2.7f;  
            float sillHeight = 0.4f;  
            float sillDepth  = 0.5f; 

            glTranslatef(0.0f, (-windowHeight * 0.5f - frameThickness * 0.5f) - 0.2f, 0.0f);

            glScalef(sillWidth, sillHeight, sillDepth);
            glutSolidCube(1.0f);
        glPopMatrix();}

        // Frame tengah 
        glColor3ub(255, 255, 255);

        // Vertikal
        glPushMatrix();
            glScalef(crossThickness, windowHeight, windowDepth);
            glutSolidCube(1.0f);
        glPopMatrix();

        // Horizontal
        glPushMatrix();
            glScalef(windowWidth, crossThickness, windowDepth);
            glutSolidCube(1.0f);
        glPopMatrix();

    glPopMatrix();    
}

    { // Jendela 2
            glPushMatrix();
            glTranslatef(-2.0f, 12.85f, 4.65f);
            float windowWidth  = 2.0f;
            float windowHeight = 3.0f;
            float windowDepth  = 0.1f;
            float frameThickness = 0.2f;
            float crossThickness = 0.1f;
    
            // Kaca jendela
            glColor3ub(107, 215, 255);
            glBegin(GL_QUADS);
                glVertex3f(-windowWidth * 0.5f, -windowHeight * 0.5f, 0.0f);
                glVertex3f( windowWidth * 0.5f, -windowHeight * 0.5f, 0.0f);
                glVertex3f( windowWidth * 0.5f,  windowHeight * 0.5f, 0.0f);
                glVertex3f(-windowWidth * 0.5f,  windowHeight * 0.5f, 0.0f);
            glEnd();
    
            // Frame 
            glColor3ub(255, 255, 255);
            // Atas
            glPushMatrix();
                glTranslatef(0.0f, windowHeight * 0.5f + frameThickness * 0.5f, 0.0f);
                glScalef(windowWidth + frameThickness * 2, frameThickness, windowDepth);
                glutSolidCube(1.0f);
            glPopMatrix();
    
            // Bawah
            glPushMatrix();
                glTranslatef(0.0f, -windowHeight * 0.5f - frameThickness * 0.5f, 0.0f);
                glScalef(windowWidth + frameThickness * 2, frameThickness, windowDepth);
                glutSolidCube(1.0f);
            glPopMatrix();
    
            // Kiri
            glPushMatrix();
                glTranslatef(-windowWidth * 0.5f - frameThickness * 0.5f, 0.0f, 0.0f);
                glScalef(frameThickness, windowHeight + frameThickness * 2, windowDepth);
                glutSolidCube(1.0f);
            glPopMatrix();
    
            // Kanan
            glPushMatrix();
                glTranslatef(windowWidth * 0.5f + frameThickness * 0.5f, 0.0f, 0.0f);
                glScalef(frameThickness, windowHeight + frameThickness * 2, windowDepth);
                glutSolidCube(1.0f);
            glPopMatrix();
    
            {glPushMatrix(); // alas
                glColor3ub(90, 81, 102);

                float sillWidth  = 2.7f; 
                float sillHeight = 0.4f;  
                float sillDepth  = 0.5f; 

                glTranslatef(0.0f, (-windowHeight * 0.5f - frameThickness * 0.5f) - 0.2f, 0.0f);
    
                glScalef(sillWidth, sillHeight, sillDepth);
                glutSolidCube(1.0f);
            glPopMatrix();}
    
    
            // Frame tengah 
            glColor3ub(255, 255, 255);
    
            // Vertikal
            glPushMatrix();
                glScalef(crossThickness, windowHeight, windowDepth);
                glutSolidCube(1.0f);
            glPopMatrix();
    
            // Horizontal
            glPushMatrix();
                glScalef(windowWidth, crossThickness, windowDepth);
                glutSolidCube(1.0f);
            glPopMatrix();
        glPopMatrix();
    }

    { // brick

        glPushMatrix();
            glTranslatef(0.0f, 4.6f, 0.0f);
            glColor3ub(106, 133, 55);
    
            // 1
            glPushMatrix();
                glTranslatef(3.2f, 2.0f, 4.51f);
                drawBrick(1.25f, 0.75f, 0.2f); 
            glPopMatrix();
    
            // 2
            glPushMatrix();
                glTranslatef(-2.5f, 1.75f, 4.51f);
                drawBrick(1.25f, 0.75f, 0.2f); 
            glPopMatrix();

            // 3
            glPushMatrix();
                glTranslatef(-3.4f, 2.75f, 4.51f);
                drawBrick(1.25f, 0.75f, 0.2f); 
            glPopMatrix();

            // 4
            glPushMatrix();
                glTranslatef(2.4f, -1.3f, 4.51f);
                drawBrick(1.25f, 0.75f, 0.2f); 
            glPopMatrix();


            // 5
            glPushMatrix();
                glTranslatef(0.8f, 5.5f, 4.51f);
                drawBrick(1.25f, 0.75f, 0.2f); 
            glPopMatrix();

            // 6
            glPushMatrix();
                glTranslatef(-2.9f, 10.8f, 4.51f);
                drawBrick(1.25f, 0.75f, 0.2f); 
            glPopMatrix();

        glPopMatrix();
    }
    
}




void rumah2() {
    glPushMatrix();
    glTranslatef(17.0f , 0.0f, 0.0f);

    { // Blok untuk lantai bawah
        glPushMatrix();
            glColor3ub(90, 81, 102); 
            glTranslatef(0.0f, 0.35f, 0.0f);
            float halfWidth  = 5.5;   
            float halfHeight = 0.35f;  
            float halfDepth  = 5.5f;  

            glBegin(GL_QUADS);
                // Face atas
                glVertex3f(-halfWidth,  halfHeight, -halfDepth);
                glVertex3f( halfWidth,  halfHeight, -halfDepth);
                glVertex3f( halfWidth,  halfHeight,  halfDepth);
                glVertex3f(-halfWidth,  halfHeight,  halfDepth);

                // Face bawah
                glVertex3f(-halfWidth, -halfHeight, -halfDepth);
                glVertex3f( halfWidth, -halfHeight, -halfDepth);
                glVertex3f( halfWidth, -halfHeight,  halfDepth);
                glVertex3f(-halfWidth, -halfHeight,  halfDepth);

                // Face depan
                glVertex3f(-halfWidth, -halfHeight,  halfDepth);
                glVertex3f( halfWidth, -halfHeight,  halfDepth);
                glVertex3f( halfWidth,  halfHeight,  halfDepth);
                glVertex3f(-halfWidth,  halfHeight,  halfDepth);

                // Face belakang
                glVertex3f(-halfWidth, -halfHeight, -halfDepth);
                glVertex3f( halfWidth, -halfHeight, -halfDepth);
                glVertex3f( halfWidth,  halfHeight, -halfDepth);
                glVertex3f(-halfWidth,  halfHeight, -halfDepth);

                // Face kiri
                glVertex3f(-halfWidth, -halfHeight, -halfDepth);
                glVertex3f(-halfWidth, -halfHeight,  halfDepth);
                glVertex3f(-halfWidth,  halfHeight,  halfDepth);
                glVertex3f(-halfWidth,  halfHeight, -halfDepth);

                // Face kanan
                glVertex3f( halfWidth, -halfHeight, -halfDepth);
                glVertex3f( halfWidth, -halfHeight,  halfDepth);
                glVertex3f( halfWidth,  halfHeight,  halfDepth);
                glVertex3f( halfWidth,  halfHeight, -halfDepth);
            glEnd();
        glPopMatrix();
    } 

    { // Blok untuk badan rumah 
        glPushMatrix();
            glColor3ub(153, 47, 51); 

            glTranslatef(0.0f, 4.6f, 0.0f);
            float halfWallWidth  = 4.5f;
            float halfWallHeight = 4.0f;
            float halfWallDepth  = 4.5f;

            glBegin(GL_QUADS);
                // Face atas
                glVertex3f(-halfWallWidth,  halfWallHeight, -halfWallDepth);
                glVertex3f( halfWallWidth,  halfWallHeight, -halfWallDepth);
                glVertex3f( halfWallWidth,  halfWallHeight,  halfWallDepth);
                glVertex3f(-halfWallWidth,  halfWallHeight,  halfWallDepth);

                // Face bawah
                glVertex3f(-halfWallWidth, -halfWallHeight, -halfWallDepth);
                glVertex3f( halfWallWidth, -halfWallHeight, -halfWallDepth);
                glVertex3f( halfWallWidth, -halfWallHeight,  halfWallDepth);
                glVertex3f(-halfWallWidth, -halfWallHeight,  halfWallDepth);

                // Face depan
                glVertex3f(-halfWallWidth, -halfWallHeight,  halfWallDepth);
                glVertex3f( halfWallWidth, -halfWallHeight,  halfWallDepth);
                glVertex3f( halfWallWidth,  halfWallHeight,  halfWallDepth);
                glVertex3f(-halfWallWidth,  halfWallHeight,  halfWallDepth);

                // Face belakang
                glVertex3f(-halfWallWidth, -halfWallHeight, -halfWallDepth);
                glVertex3f( halfWallWidth, -halfWallHeight, -halfWallDepth);
                glVertex3f( halfWallWidth,  halfWallHeight, -halfWallDepth);
                glVertex3f(-halfWallWidth,  halfWallHeight, -halfWallDepth);

                // Face kiri
                glVertex3f(-halfWallWidth, -halfWallHeight, -halfWallDepth);
                glVertex3f(-halfWallWidth, -halfWallHeight,  halfWallDepth);
                glVertex3f(-halfWallWidth,  halfWallHeight,  halfWallDepth);
                glVertex3f(-halfWallWidth,  halfWallHeight, -halfWallDepth);

                // Face kanan
                glVertex3f( halfWallWidth, -halfWallHeight, -halfWallDepth);
                glVertex3f( halfWallWidth, -halfWallHeight,  halfWallDepth);
                glVertex3f( halfWallWidth,  halfWallHeight,  halfWallDepth);
                glVertex3f( halfWallWidth,  halfWallHeight, -halfWallDepth);
            glEnd();
        glPopMatrix();
    } 

    { // Blok untuk lantai tengah
        glPushMatrix();
            glColor3ub(90, 81, 102); 
            glTranslatef(0.0f, 9.0f, 0.0f);
            float halfWidth  = 5.0f;  
            float halfHeight = 0.3f; 
            float halfDepth  = 5.0f;  

            glBegin(GL_QUADS);
                // Face atas 
                glVertex3f(-halfWidth,  halfHeight, -halfDepth);
                glVertex3f( halfWidth,  halfHeight, -halfDepth);
                glVertex3f( halfWidth,  halfHeight,  halfDepth);
                glVertex3f(-halfWidth,  halfHeight,  halfDepth);

                // Face bawah
                glVertex3f(-halfWidth, -halfHeight, -halfDepth);
                glVertex3f( halfWidth, -halfHeight, -halfDepth);
                glVertex3f( halfWidth, -halfHeight,  halfDepth);
                glVertex3f(-halfWidth, -halfHeight,  halfDepth);

                // Face depan
                glVertex3f(-halfWidth, -halfHeight,  halfDepth);
                glVertex3f( halfWidth, -halfHeight,  halfDepth);
                glVertex3f( halfWidth,  halfHeight,  halfDepth);
                glVertex3f(-halfWidth,  halfHeight,  halfDepth);

                // Face belakang
                glVertex3f(-halfWidth, -halfHeight, -halfDepth);
                glVertex3f( halfWidth, -halfHeight, -halfDepth);
                glVertex3f( halfWidth,  halfHeight, -halfDepth);
                glVertex3f(-halfWidth,  halfHeight, -halfDepth);

                // Face kiri
                glVertex3f(-halfWidth, -halfHeight, -halfDepth);
                glVertex3f(-halfWidth, -halfHeight,  halfDepth);
                glVertex3f(-halfWidth,  halfHeight,  halfDepth);
                glVertex3f(-halfWidth,  halfHeight, -halfDepth);

                // Face kanan
                glVertex3f( halfWidth, -halfHeight, -halfDepth);
                glVertex3f( halfWidth, -halfHeight,  halfDepth);
                glVertex3f( halfWidth,  halfHeight,  halfDepth);
                glVertex3f( halfWidth,  halfHeight, -halfDepth);
            glEnd();
        glPopMatrix();
    } 

    { // Blok untuk badan rumah 2
        glPushMatrix();
            glColor3ub(153, 47, 51); 
            glTranslatef(0.0f, 12.4f, 0.0f);
            float halfWallWidth  = 4.5f;
            float halfWallHeight = 4.0f;
            float halfWallDepth  = 4.5f;

            glBegin(GL_QUADS);
                // Face atas
                glVertex3f(-halfWallWidth,  halfWallHeight, -halfWallDepth);
                glVertex3f( halfWallWidth,  halfWallHeight, -halfWallDepth);
                glVertex3f( halfWallWidth,  halfWallHeight,  halfWallDepth);
                glVertex3f(-halfWallWidth,  halfWallHeight,  halfWallDepth);

                // Face bawah
                glVertex3f(-halfWallWidth, -halfWallHeight, -halfWallDepth);
                glVertex3f( halfWallWidth, -halfWallHeight, -halfWallDepth);
                glVertex3f( halfWallWidth, -halfWallHeight,  halfWallDepth);
                glVertex3f(-halfWallWidth, -halfWallHeight,  halfWallDepth);

                // Face depan
                glVertex3f(-halfWallWidth, -halfWallHeight,  halfWallDepth);
                glVertex3f( halfWallWidth, -halfWallHeight,  halfWallDepth);
                glVertex3f( halfWallWidth,  halfWallHeight,  halfWallDepth);
                glVertex3f(-halfWallWidth,  halfWallHeight,  halfWallDepth);

                // Face belakang
                glVertex3f(-halfWallWidth, -halfWallHeight, -halfWallDepth);
                glVertex3f( halfWallWidth, -halfWallHeight, -halfWallDepth);
                glVertex3f( halfWallWidth,  halfWallHeight, -halfWallDepth);
                glVertex3f(-halfWallWidth,  halfWallHeight, -halfWallDepth);

                // Face kiri
                glVertex3f(-halfWallWidth, -halfWallHeight, -halfWallDepth);
                glVertex3f(-halfWallWidth, -halfWallHeight,  halfWallDepth);
                glVertex3f(-halfWallWidth,  halfWallHeight,  halfWallDepth);
                glVertex3f(-halfWallWidth,  halfWallHeight, -halfWallDepth);

                // Face kanan
                glVertex3f( halfWallWidth, -halfWallHeight, -halfWallDepth);
                glVertex3f( halfWallWidth, -halfWallHeight,  halfWallDepth);
                glVertex3f( halfWallWidth,  halfWallHeight,  halfWallDepth);
                glVertex3f( halfWallWidth,  halfWallHeight, -halfWallDepth);
            glEnd();
        glPopMatrix();
    } 
 
    { // atap 1
        glPushMatrix();
            glColor3ub(90, 81, 102); 
            glTranslatef(0.0f, 16.7f, 0.0f);
            float halfWidth  = 5.0f;   
            float halfHeight = 0.3f; 
            float halfDepth  = 5.0f;   

            glBegin(GL_QUADS);
                // Face atas 
                glVertex3f(-halfWidth,  halfHeight, -halfDepth);
                glVertex3f( halfWidth,  halfHeight, -halfDepth);
                glVertex3f( halfWidth,  halfHeight,  halfDepth);
                glVertex3f(-halfWidth,  halfHeight,  halfDepth);

                // Face bawah
                glVertex3f(-halfWidth, -halfHeight, -halfDepth);
                glVertex3f( halfWidth, -halfHeight, -halfDepth);
                glVertex3f( halfWidth, -halfHeight,  halfDepth);
                glVertex3f(-halfWidth, -halfHeight,  halfDepth);

                // Face depan
                glVertex3f(-halfWidth, -halfHeight,  halfDepth);
                glVertex3f( halfWidth, -halfHeight,  halfDepth);
                glVertex3f( halfWidth,  halfHeight,  halfDepth);
                glVertex3f(-halfWidth,  halfHeight,  halfDepth);

                // Face belakang
                glVertex3f(-halfWidth, -halfHeight, -halfDepth);
                glVertex3f( halfWidth, -halfHeight, -halfDepth);
                glVertex3f( halfWidth,  halfHeight, -halfDepth);
                glVertex3f(-halfWidth,  halfHeight, -halfDepth);

                // Face kiri
                glVertex3f(-halfWidth, -halfHeight, -halfDepth);
                glVertex3f(-halfWidth, -halfHeight,  halfDepth);
                glVertex3f(-halfWidth,  halfHeight,  halfDepth);
                glVertex3f(-halfWidth,  halfHeight, -halfDepth);

                // Face kanan
                glVertex3f( halfWidth, -halfHeight, -halfDepth);
                glVertex3f( halfWidth, -halfHeight,  halfDepth);
                glVertex3f( halfWidth,  halfHeight,  halfDepth);
                glVertex3f( halfWidth,  halfHeight, -halfDepth);
            glEnd();
        glPopMatrix();
    } 

    { // atap 2
    glPushMatrix();
        glColor3ub(90, 81, 102);
        glTranslatef(0.0f, 17.3f, 0.0f);
        float halfWidth  = 5.2f;   
        float halfHeight = 0.3f;
        float halfDepth  = 5.2f; 

        glBegin(GL_QUADS);
            // Face atas 
            glVertex3f(-halfWidth,  halfHeight, -halfDepth);
            glVertex3f( halfWidth,  halfHeight, -halfDepth);
            glVertex3f( halfWidth,  halfHeight,  halfDepth);
            glVertex3f(-halfWidth,  halfHeight,  halfDepth);

            // Face bawah
            glVertex3f(-halfWidth, -halfHeight, -halfDepth);
            glVertex3f( halfWidth, -halfHeight, -halfDepth);
            glVertex3f( halfWidth, -halfHeight,  halfDepth);
            glVertex3f(-halfWidth, -halfHeight,  halfDepth);

            // Face depan
            glVertex3f(-halfWidth, -halfHeight,  halfDepth);
            glVertex3f( halfWidth, -halfHeight,  halfDepth);
            glVertex3f( halfWidth,  halfHeight,  halfDepth);
            glVertex3f(-halfWidth,  halfHeight,  halfDepth);

            // Face belakang
            glVertex3f(-halfWidth, -halfHeight, -halfDepth);
            glVertex3f( halfWidth, -halfHeight, -halfDepth);
            glVertex3f( halfWidth,  halfHeight, -halfDepth);
            glVertex3f(-halfWidth,  halfHeight, -halfDepth);

            // Face kiri
            glVertex3f(-halfWidth, -halfHeight, -halfDepth);
            glVertex3f(-halfWidth, -halfHeight,  halfDepth);
            glVertex3f(-halfWidth,  halfHeight,  halfDepth);
            glVertex3f(-halfWidth,  halfHeight, -halfDepth);

            // Face kanan
            glVertex3f( halfWidth, -halfHeight, -halfDepth);
            glVertex3f( halfWidth, -halfHeight,  halfDepth);
            glVertex3f( halfWidth,  halfHeight,  halfDepth);
            glVertex3f( halfWidth,  halfHeight, -halfDepth);
        glEnd();
    glPopMatrix();
 } 
 
    { // pintu dan jendela
    glPushMatrix();

        glTranslatef(0.0f, 0.35f, 4.65f); 
        glColor3ub(133, 101, 14);

        // Ukuran pintu
        float doorWidth     = 2.5f;  
        float doorHeight    = 4.0f;  
        float doorThickness = 0.1f;  
        float archRadius    = 1.25f; 
        int   segments      = 16;    // jumlah segmen lengkungan

        // Variabel pembantu
        float w2 = doorWidth * 0.5f; // setengah lebar
        float t  = doorThickness;
        float h  = doorHeight;

        // Depan
        glBegin(GL_QUADS);
            glVertex3f(-w2, 0.0f,       0.0f);
            glVertex3f( w2, 0.0f,       0.0f);
            glVertex3f( w2, h,          0.0f);
            glVertex3f(-w2, h,          0.0f);
        glEnd();

        // Belakang
        glBegin(GL_QUADS);
            glVertex3f(-w2, 0.0f,      -t);
            glVertex3f( w2, 0.0f,      -t);
            glVertex3f( w2, h,         -t);
            glVertex3f(-w2, h,         -t);
        glEnd();

        // Sisi bawah
        glBegin(GL_QUADS);
            glVertex3f(-w2, 0.0f,  0.0f);
            glVertex3f(-w2, 0.0f, -t);
            glVertex3f( w2, 0.0f, -t);
            glVertex3f( w2, 0.0f,  0.0f);
        glEnd();

        // Sisi atas 
        glBegin(GL_QUADS);
            glVertex3f(-w2, h,  0.0f);
            glVertex3f(-w2, h, -t);
            glVertex3f( w2, h, -t);
            glVertex3f( w2, h,  0.0f);
        glEnd();

        // Sisi kiri
        glBegin(GL_QUADS);
            glVertex3f(-w2, 0.0f,  0.0f);
            glVertex3f(-w2, h,     0.0f);
            glVertex3f(-w2, h,    -t);
            glVertex3f(-w2, 0.0f, -t);
        glEnd();

        // Sisi kanan
        glBegin(GL_QUADS);
            glVertex3f(w2, 0.0f,  0.0f);
            glVertex3f(w2, h,     0.0f);
            glVertex3f(w2, h,    -t);
            glVertex3f(w2, 0.0f, -t);
        glEnd();

    
        // border
        {    float borderThickness = 0.1f;
        glColor3ub(90, 81, 102);

        // border atas
        glPushMatrix();
            glTranslatef(0.0f, h + borderThickness*0.5f, -t*0.5f);
            glScalef(doorWidth + 2*borderThickness, borderThickness, doorThickness + 2*borderThickness);
            glutSolidCube(1.0f);
        glPopMatrix();

        // 2) Border sisi kiri
        glPushMatrix();
            glTranslatef(-w2 - borderThickness*0.5f, h*0.5f, -t*0.5f);
            glScalef(borderThickness, doorHeight, doorThickness + 2*borderThickness);
            glutSolidCube(1.0f);
        glPopMatrix();

        // 3) Border sisi kanan 
        glPushMatrix();
            glTranslatef(w2 + borderThickness*0.5f, h*0.5f, -t*0.5f);
            glScalef(borderThickness, doorHeight, doorThickness + 2*borderThickness);
            glutSolidCube(1.0f);
        glPopMatrix();}



        // 1. Bagian kaca 
        {
            glPushMatrix();
            glTranslatef(0.0f, 0.0f, -0.08f);
            glColor3ub(107, 215, 255);

            float baseY = h + 0.5f;  // pangkal lengkungan
            // Bagian depan (z=0)
            glBegin(GL_TRIANGLE_FAN);
                glVertex3f(0.0f, baseY, 0.0f); // pusat lengkungan
                for(int i = 0; i <= segments; i++) {
                    float theta = 3.14159f * i / segments; 
                    float xPos = archRadius * cosf(theta);
                    float yPos = archRadius * sinf(theta);
                    glVertex3f(xPos, baseY + yPos, 0.0f);
                }
            glEnd();
 
            // Bagian belakang (z = -t)
            glBegin(GL_TRIANGLE_FAN);
                glVertex3f(0.0f, baseY, -t);
                for(int i = 0; i <= segments; i++) {
                    float theta = 3.14159f * i / segments;
                    float xPos = archRadius * cosf(theta);
                    float yPos = archRadius * sinf(theta);
                    glVertex3f(xPos, baseY + yPos, -t);
                }
            glEnd();

            // Sisi lengkungan 
            glBegin(GL_QUAD_STRIP);
                for(int i = 0; i <= segments; i++) {
                    float theta = 3.14159f * i / segments;
                    float xPos = archRadius * cosf(theta);
                    float yPos = archRadius * sinf(theta);
                    glVertex3f(xPos, baseY + yPos, 0.0f);
                    glVertex3f(xPos, baseY + yPos, -t);
                }
            glEnd();

            glPopMatrix();
        }

        // 2. Border 
        {
            glColor3ub(255, 255, 255);

            float borderThickness = 0.1f;
            float outerArchRadius = archRadius + borderThickness;
            float archBaseY = h + 0.5f; 

            // Bagian depan border 
            glBegin(GL_QUAD_STRIP);
            for(int i = 0; i <= segments; i++) {
                float theta = 3.14159f * i / segments;
                float xOuter = outerArchRadius * cosf(theta);
                float yOuter = outerArchRadius * sinf(theta);
                float xInner = archRadius * cosf(theta);
                float yInner = archRadius * sinf(theta);

                glVertex3f(xOuter, archBaseY + yOuter, 0.0f); 
                glVertex3f(xInner, archBaseY + yInner, 0.0f);
            }
            glEnd();

            // Bagian belakang border 
            glBegin(GL_QUAD_STRIP);
            for(int i = 0; i <= segments; i++) {
                float theta = 3.14159f * i / segments;
                float xOuter = outerArchRadius * cosf(theta);
                float yOuter = outerArchRadius * sinf(theta);
                float xInner = archRadius * cosf(theta);
                float yInner = archRadius * sinf(theta);

                glVertex3f(xOuter, archBaseY + yOuter, -t);
                glVertex3f(xInner, archBaseY + yInner, -t);
            }
            glEnd();

            // Sisi lengkungan border
            glBegin(GL_QUAD_STRIP);
            for(int i = 0; i <= segments; i++) {
                float theta = 3.14159f * i / segments;
                float xPos = outerArchRadius * cosf(theta);
                float yPos = outerArchRadius * sinf(theta);

                glVertex3f(xPos, archBaseY + yPos, 0.0f);
                glVertex3f(xPos, archBaseY + yPos, -t);
            }
            glEnd();

            // border atas
            glPushMatrix();
                glTranslatef(0.0f, 0.5 + h + borderThickness*0.5f, -t*0.5f);
                glScalef(doorWidth + 2*borderThickness, borderThickness, doorThickness + 2*borderThickness);
                glutSolidCube(1.0f);
             glPopMatrix();
        }

        // 3. Tralis 
        {
        {
            float tralisThickness = 0.05f;
            float R = archRadius;  // jari-jari lengkungan

            float centerX = 0.0f;
            float centerY = h + 0.5f;  
            float centerZ = 0.0f;   

            for (int i = 0; i < 3; i++) {
                float theta;
                if (i == 0)
                    theta = 3.14159f * 0.5f;   // 90° (vertikal)
                else if (i == 1)
                    theta = 3.14159f * 0.25f;  // 45°
                else
                    theta = 3.14159f * 0.75f;  // 135°

                // Titik start adalah pusat
                float sx = centerX;
                float sy = centerY;
                float sz = centerZ;
                // Titik end dihitung dari pusat + offset berbasis theta
                float ex = centerX + R * cosf(theta);
                float ey = centerY + R * sinf(theta);
                float ez = centerZ;
                
                // Vektor dari start ke end:
                float dx = ex - sx;
                float dy = ey - sy;
                // Panjang vektor (seharusnya sama dengan R)
                float len = sqrt(dx * dx + dy * dy);
                // Vektor normal (perpendicular di bidang XY)
                float nx = -dy / len;
                float ny = dx / len;
                // Offset sebesar setengah ketebalan
                float ox = nx * (tralisThickness * 0.5f);
                float oy = ny * (tralisThickness * 0.5f);

                // Tentukan empat titik quad:
                // v1: start + offset
                float v1x = sx + ox, v1y = sy + oy, v1z = sz;
                // v2: start - offset
                float v2x = sx - ox, v2y = sy - oy, v2z = sz;
                // v3: end - offset
                float v3x = ex - ox, v3y = ey - oy, v3z = ez;
                // v4: end + offset
                float v4x = ex + ox, v4y = ey + oy, v4z = ez;
                
                glColor3ub(255, 255, 255);
                glBegin(GL_QUADS);
                    glVertex3f(v1x, v1y, v1z);
                    glVertex3f(v2x, v2y, v2z);
                    glVertex3f(v3x, v3y, v3z);
                    glVertex3f(v4x, v4y, v4z);
                glEnd();
            }
        }
     }

        // gagang pintu
        glPushMatrix();
             glTranslatef(-w2 + 0.2f, h * 0.5f, 0.0f);
             glColor3ub(80, 80, 80); 
             glutSolidSphere(0.08, 16, 16);
        glPopMatrix();

    glPopMatrix();
    }

    { // Jendela 1
    glPushMatrix();
        glTranslatef(2.0f, 12.85f, 4.65f); 
        float windowWidth  = 2.0f;
        float windowHeight = 3.0f;
        float windowDepth  = 0.1f;
        float frameThickness = 0.2f;
        float crossThickness = 0.1f;

        // Kaca jendela
        glColor3ub(107, 215, 255);
        glBegin(GL_QUADS);
            glVertex3f(-windowWidth * 0.5f, -windowHeight * 0.5f, 0.0f);
            glVertex3f( windowWidth * 0.5f, -windowHeight * 0.5f, 0.0f);
            glVertex3f( windowWidth * 0.5f,  windowHeight * 0.5f, 0.0f);
            glVertex3f(-windowWidth * 0.5f,  windowHeight * 0.5f, 0.0f);
        glEnd();

        // Frame 
        glColor3ub(255, 255, 255);
        // Atas
        glPushMatrix();
            glTranslatef(0.0f, windowHeight * 0.5f + frameThickness * 0.5f, 0.0f);
            glScalef(windowWidth + frameThickness * 2, frameThickness, windowDepth);
            glutSolidCube(1.0f);
        glPopMatrix();

        // Bawah
        glPushMatrix();
            glTranslatef(0.0f, -windowHeight * 0.5f - frameThickness * 0.5f, 0.0f);
            glScalef(windowWidth + frameThickness * 2, frameThickness, windowDepth);
            glutSolidCube(1.0f);
        glPopMatrix();


        // Kiri
        glPushMatrix();
            glTranslatef(-windowWidth * 0.5f - frameThickness * 0.5f, 0.0f, 0.0f);
            glScalef(frameThickness, windowHeight + frameThickness * 2, windowDepth);
            glutSolidCube(1.0f);
        glPopMatrix();

        // Kanan
        glPushMatrix();
            glTranslatef(windowWidth * 0.5f + frameThickness * 0.5f, 0.0f, 0.0f);
            glScalef(frameThickness, windowHeight + frameThickness * 2, windowDepth);
            glutSolidCube(1.0f);
        glPopMatrix();

        
        {glPushMatrix(); //alas
            glColor3ub(90, 81, 102); 

            float sillWidth  = 2.7f;  
            float sillHeight = 0.4f;  
            float sillDepth  = 0.5f; 

            glTranslatef(0.0f, (-windowHeight * 0.5f - frameThickness * 0.5f) - 0.2f, 0.0f);

            glScalef(sillWidth, sillHeight, sillDepth);
            glutSolidCube(1.0f);
        glPopMatrix();}

        // Frame tengah 
        glColor3ub(255, 255, 255);

        // Vertikal
        glPushMatrix();
            glScalef(crossThickness, windowHeight, windowDepth);
            glutSolidCube(1.0f);
        glPopMatrix();

        // Horizontal
        glPushMatrix();
            glScalef(windowWidth, crossThickness, windowDepth);
            glutSolidCube(1.0f);
        glPopMatrix();

    glPopMatrix();    
}

    { // Jendela 2
            glPushMatrix();
            glTranslatef(-2.0f, 12.85f, 4.65f);
            float windowWidth  = 2.0f;
            float windowHeight = 3.0f;
            float windowDepth  = 0.1f;
            float frameThickness = 0.2f;
            float crossThickness = 0.1f;
    
            // Kaca jendela
            glColor3ub(107, 215, 255);
            glBegin(GL_QUADS);
                glVertex3f(-windowWidth * 0.5f, -windowHeight * 0.5f, 0.0f);
                glVertex3f( windowWidth * 0.5f, -windowHeight * 0.5f, 0.0f);
                glVertex3f( windowWidth * 0.5f,  windowHeight * 0.5f, 0.0f);
                glVertex3f(-windowWidth * 0.5f,  windowHeight * 0.5f, 0.0f);
            glEnd();
    
            // Frame 
            glColor3ub(255, 255, 255);
            // Atas
            glPushMatrix();
                glTranslatef(0.0f, windowHeight * 0.5f + frameThickness * 0.5f, 0.0f);
                glScalef(windowWidth + frameThickness * 2, frameThickness, windowDepth);
                glutSolidCube(1.0f);
            glPopMatrix();
    
            // Bawah
            glPushMatrix();
                glTranslatef(0.0f, -windowHeight * 0.5f - frameThickness * 0.5f, 0.0f);
                glScalef(windowWidth + frameThickness * 2, frameThickness, windowDepth);
                glutSolidCube(1.0f);
            glPopMatrix();
    
            // Kiri
            glPushMatrix();
                glTranslatef(-windowWidth * 0.5f - frameThickness * 0.5f, 0.0f, 0.0f);
                glScalef(frameThickness, windowHeight + frameThickness * 2, windowDepth);
                glutSolidCube(1.0f);
            glPopMatrix();
    
            // Kanan
            glPushMatrix();
                glTranslatef(windowWidth * 0.5f + frameThickness * 0.5f, 0.0f, 0.0f);
                glScalef(frameThickness, windowHeight + frameThickness * 2, windowDepth);
                glutSolidCube(1.0f);
            glPopMatrix();
    
            {glPushMatrix(); // alas
                glColor3ub(90, 81, 102);

                float sillWidth  = 2.7f; 
                float sillHeight = 0.4f;  
                float sillDepth  = 0.5f; 

                glTranslatef(0.0f, (-windowHeight * 0.5f - frameThickness * 0.5f) - 0.2f, 0.0f);
    
                glScalef(sillWidth, sillHeight, sillDepth);
                glutSolidCube(1.0f);
            glPopMatrix();}
    
    
            // Frame tengah 
            glColor3ub(255, 255, 255);
    
            // Vertikal
            glPushMatrix();
                glScalef(crossThickness, windowHeight, windowDepth);
                glutSolidCube(1.0f);
            glPopMatrix();
    
            // Horizontal
            glPushMatrix();
                glScalef(windowWidth, crossThickness, windowDepth);
                glutSolidCube(1.0f);
            glPopMatrix();
        glPopMatrix();
    }

    { // brick

        glPushMatrix();
            glTranslatef(0.0f, 4.6f, 0.0f);
            glColor3ub(128, 37, 40);
    
            // 1
            glPushMatrix();
                glTranslatef(3.2f, 2.0f, 4.51f);
                drawBrick(1.25f, 0.75f, 0.2f); 
            glPopMatrix();
    
            // 2
            glPushMatrix();
                glTranslatef(-2.5f, 1.75f, 4.51f);
                drawBrick(1.25f, 0.75f, 0.2f); 
            glPopMatrix();

            // 3
            glPushMatrix();
                glTranslatef(-3.4f, 2.75f, 4.51f);
                drawBrick(1.25f, 0.75f, 0.2f); 
            glPopMatrix();

            // 4
            glPushMatrix();
                glTranslatef(2.4f, -1.3f, 4.51f);
                drawBrick(1.25f, 0.75f, 0.2f); 
            glPopMatrix();


            // 5
            glPushMatrix();
                glTranslatef(0.8f, 5.5f, 4.51f);
                drawBrick(1.25f, 0.75f, 0.2f); 
            glPopMatrix();

            // 6
            glPushMatrix();
                glTranslatef(-2.9f, 10.8f, 4.51f);
                drawBrick(1.25f, 0.75f, 0.2f); 
            glPopMatrix();

        glPopMatrix();
    }
    glPopMatrix();
}




void rumah3() {
    glPushMatrix();
    glTranslatef(-17.0f , 0.0f, 0.0f);

    { // Blok untuk lantai bawah
        glPushMatrix();
            glColor3ub(90, 81, 102); 
            glTranslatef(0.0f, 0.35f, 0.0f);
            float halfWidth  = 5.5;   
            float halfHeight = 0.35f;  
            float halfDepth  = 5.5f;  

            glBegin(GL_QUADS);
                // Face atas
                glVertex3f(-halfWidth,  halfHeight, -halfDepth);
                glVertex3f( halfWidth,  halfHeight, -halfDepth);
                glVertex3f( halfWidth,  halfHeight,  halfDepth);
                glVertex3f(-halfWidth,  halfHeight,  halfDepth);

                // Face bawah
                glVertex3f(-halfWidth, -halfHeight, -halfDepth);
                glVertex3f( halfWidth, -halfHeight, -halfDepth);
                glVertex3f( halfWidth, -halfHeight,  halfDepth);
                glVertex3f(-halfWidth, -halfHeight,  halfDepth);

                // Face depan
                glVertex3f(-halfWidth, -halfHeight,  halfDepth);
                glVertex3f( halfWidth, -halfHeight,  halfDepth);
                glVertex3f( halfWidth,  halfHeight,  halfDepth);
                glVertex3f(-halfWidth,  halfHeight,  halfDepth);

                // Face belakang
                glVertex3f(-halfWidth, -halfHeight, -halfDepth);
                glVertex3f( halfWidth, -halfHeight, -halfDepth);
                glVertex3f( halfWidth,  halfHeight, -halfDepth);
                glVertex3f(-halfWidth,  halfHeight, -halfDepth);

                // Face kiri
                glVertex3f(-halfWidth, -halfHeight, -halfDepth);
                glVertex3f(-halfWidth, -halfHeight,  halfDepth);
                glVertex3f(-halfWidth,  halfHeight,  halfDepth);
                glVertex3f(-halfWidth,  halfHeight, -halfDepth);

                // Face kanan
                glVertex3f( halfWidth, -halfHeight, -halfDepth);
                glVertex3f( halfWidth, -halfHeight,  halfDepth);
                glVertex3f( halfWidth,  halfHeight,  halfDepth);
                glVertex3f( halfWidth,  halfHeight, -halfDepth);
            glEnd();
        glPopMatrix();
    } 

    { // Blok untuk badan rumah 
        glPushMatrix();
            glColor3ub(173, 133, 0); 

            glTranslatef(0.0f, 4.6f, 0.0f);
            float halfWallWidth  = 4.5f;
            float halfWallHeight = 4.0f;
            float halfWallDepth  = 4.5f;

            glBegin(GL_QUADS);
                // Face atas
                glVertex3f(-halfWallWidth,  halfWallHeight, -halfWallDepth);
                glVertex3f( halfWallWidth,  halfWallHeight, -halfWallDepth);
                glVertex3f( halfWallWidth,  halfWallHeight,  halfWallDepth);
                glVertex3f(-halfWallWidth,  halfWallHeight,  halfWallDepth);

                // Face bawah
                glVertex3f(-halfWallWidth, -halfWallHeight, -halfWallDepth);
                glVertex3f( halfWallWidth, -halfWallHeight, -halfWallDepth);
                glVertex3f( halfWallWidth, -halfWallHeight,  halfWallDepth);
                glVertex3f(-halfWallWidth, -halfWallHeight,  halfWallDepth);

                // Face depan
                glVertex3f(-halfWallWidth, -halfWallHeight,  halfWallDepth);
                glVertex3f( halfWallWidth, -halfWallHeight,  halfWallDepth);
                glVertex3f( halfWallWidth,  halfWallHeight,  halfWallDepth);
                glVertex3f(-halfWallWidth,  halfWallHeight,  halfWallDepth);

                // Face belakang
                glVertex3f(-halfWallWidth, -halfWallHeight, -halfWallDepth);
                glVertex3f( halfWallWidth, -halfWallHeight, -halfWallDepth);
                glVertex3f( halfWallWidth,  halfWallHeight, -halfWallDepth);
                glVertex3f(-halfWallWidth,  halfWallHeight, -halfWallDepth);

                // Face kiri
                glVertex3f(-halfWallWidth, -halfWallHeight, -halfWallDepth);
                glVertex3f(-halfWallWidth, -halfWallHeight,  halfWallDepth);
                glVertex3f(-halfWallWidth,  halfWallHeight,  halfWallDepth);
                glVertex3f(-halfWallWidth,  halfWallHeight, -halfWallDepth);

                // Face kanan
                glVertex3f( halfWallWidth, -halfWallHeight, -halfWallDepth);
                glVertex3f( halfWallWidth, -halfWallHeight,  halfWallDepth);
                glVertex3f( halfWallWidth,  halfWallHeight,  halfWallDepth);
                glVertex3f( halfWallWidth,  halfWallHeight, -halfWallDepth);
            glEnd();
        glPopMatrix();
    } 

    { // Blok untuk lantai tengah
        glPushMatrix();
            glColor3ub(90, 81, 102); 
            glTranslatef(0.0f, 9.0f, 0.0f);
            float halfWidth  = 5.0f;  
            float halfHeight = 0.3f; 
            float halfDepth  = 5.0f;  

            glBegin(GL_QUADS);
                // Face atas 
                glVertex3f(-halfWidth,  halfHeight, -halfDepth);
                glVertex3f( halfWidth,  halfHeight, -halfDepth);
                glVertex3f( halfWidth,  halfHeight,  halfDepth);
                glVertex3f(-halfWidth,  halfHeight,  halfDepth);

                // Face bawah
                glVertex3f(-halfWidth, -halfHeight, -halfDepth);
                glVertex3f( halfWidth, -halfHeight, -halfDepth);
                glVertex3f( halfWidth, -halfHeight,  halfDepth);
                glVertex3f(-halfWidth, -halfHeight,  halfDepth);

                // Face depan
                glVertex3f(-halfWidth, -halfHeight,  halfDepth);
                glVertex3f( halfWidth, -halfHeight,  halfDepth);
                glVertex3f( halfWidth,  halfHeight,  halfDepth);
                glVertex3f(-halfWidth,  halfHeight,  halfDepth);

                // Face belakang
                glVertex3f(-halfWidth, -halfHeight, -halfDepth);
                glVertex3f( halfWidth, -halfHeight, -halfDepth);
                glVertex3f( halfWidth,  halfHeight, -halfDepth);
                glVertex3f(-halfWidth,  halfHeight, -halfDepth);

                // Face kiri
                glVertex3f(-halfWidth, -halfHeight, -halfDepth);
                glVertex3f(-halfWidth, -halfHeight,  halfDepth);
                glVertex3f(-halfWidth,  halfHeight,  halfDepth);
                glVertex3f(-halfWidth,  halfHeight, -halfDepth);

                // Face kanan
                glVertex3f( halfWidth, -halfHeight, -halfDepth);
                glVertex3f( halfWidth, -halfHeight,  halfDepth);
                glVertex3f( halfWidth,  halfHeight,  halfDepth);
                glVertex3f( halfWidth,  halfHeight, -halfDepth);
            glEnd();
        glPopMatrix();
    } 

    { // Blok untuk badan rumah 2
        glPushMatrix();
            glColor3ub(173, 133, 0); 
            glTranslatef(0.0f, 12.4f, 0.0f);
            float halfWallWidth  = 4.5f;
            float halfWallHeight = 4.0f;
            float halfWallDepth  = 4.5f;

            glBegin(GL_QUADS);
                // Face atas
                glVertex3f(-halfWallWidth,  halfWallHeight, -halfWallDepth);
                glVertex3f( halfWallWidth,  halfWallHeight, -halfWallDepth);
                glVertex3f( halfWallWidth,  halfWallHeight,  halfWallDepth);
                glVertex3f(-halfWallWidth,  halfWallHeight,  halfWallDepth);

                // Face bawah
                glVertex3f(-halfWallWidth, -halfWallHeight, -halfWallDepth);
                glVertex3f( halfWallWidth, -halfWallHeight, -halfWallDepth);
                glVertex3f( halfWallWidth, -halfWallHeight,  halfWallDepth);
                glVertex3f(-halfWallWidth, -halfWallHeight,  halfWallDepth);

                // Face depan
                glVertex3f(-halfWallWidth, -halfWallHeight,  halfWallDepth);
                glVertex3f( halfWallWidth, -halfWallHeight,  halfWallDepth);
                glVertex3f( halfWallWidth,  halfWallHeight,  halfWallDepth);
                glVertex3f(-halfWallWidth,  halfWallHeight,  halfWallDepth);

                // Face belakang
                glVertex3f(-halfWallWidth, -halfWallHeight, -halfWallDepth);
                glVertex3f( halfWallWidth, -halfWallHeight, -halfWallDepth);
                glVertex3f( halfWallWidth,  halfWallHeight, -halfWallDepth);
                glVertex3f(-halfWallWidth,  halfWallHeight, -halfWallDepth);

                // Face kiri
                glVertex3f(-halfWallWidth, -halfWallHeight, -halfWallDepth);
                glVertex3f(-halfWallWidth, -halfWallHeight,  halfWallDepth);
                glVertex3f(-halfWallWidth,  halfWallHeight,  halfWallDepth);
                glVertex3f(-halfWallWidth,  halfWallHeight, -halfWallDepth);

                // Face kanan
                glVertex3f( halfWallWidth, -halfWallHeight, -halfWallDepth);
                glVertex3f( halfWallWidth, -halfWallHeight,  halfWallDepth);
                glVertex3f( halfWallWidth,  halfWallHeight,  halfWallDepth);
                glVertex3f( halfWallWidth,  halfWallHeight, -halfWallDepth);
            glEnd();
        glPopMatrix();
    } 
 
    { // atap 1
        glPushMatrix();
            glColor3ub(90, 81, 102); 
            glTranslatef(0.0f, 16.7f, 0.0f);
            float halfWidth  = 5.0f;   
            float halfHeight = 0.3f; 
            float halfDepth  = 5.0f;   

            glBegin(GL_QUADS);
                // Face atas 
                glVertex3f(-halfWidth,  halfHeight, -halfDepth);
                glVertex3f( halfWidth,  halfHeight, -halfDepth);
                glVertex3f( halfWidth,  halfHeight,  halfDepth);
                glVertex3f(-halfWidth,  halfHeight,  halfDepth);

                // Face bawah
                glVertex3f(-halfWidth, -halfHeight, -halfDepth);
                glVertex3f( halfWidth, -halfHeight, -halfDepth);
                glVertex3f( halfWidth, -halfHeight,  halfDepth);
                glVertex3f(-halfWidth, -halfHeight,  halfDepth);

                // Face depan
                glVertex3f(-halfWidth, -halfHeight,  halfDepth);
                glVertex3f( halfWidth, -halfHeight,  halfDepth);
                glVertex3f( halfWidth,  halfHeight,  halfDepth);
                glVertex3f(-halfWidth,  halfHeight,  halfDepth);

                // Face belakang
                glVertex3f(-halfWidth, -halfHeight, -halfDepth);
                glVertex3f( halfWidth, -halfHeight, -halfDepth);
                glVertex3f( halfWidth,  halfHeight, -halfDepth);
                glVertex3f(-halfWidth,  halfHeight, -halfDepth);

                // Face kiri
                glVertex3f(-halfWidth, -halfHeight, -halfDepth);
                glVertex3f(-halfWidth, -halfHeight,  halfDepth);
                glVertex3f(-halfWidth,  halfHeight,  halfDepth);
                glVertex3f(-halfWidth,  halfHeight, -halfDepth);

                // Face kanan
                glVertex3f( halfWidth, -halfHeight, -halfDepth);
                glVertex3f( halfWidth, -halfHeight,  halfDepth);
                glVertex3f( halfWidth,  halfHeight,  halfDepth);
                glVertex3f( halfWidth,  halfHeight, -halfDepth);
            glEnd();
        glPopMatrix();
    } 

    { // atap 2
    glPushMatrix();
        glColor3ub(90, 81, 102);
        glTranslatef(0.0f, 17.3f, 0.0f);
        float halfWidth  = 5.2f;   
        float halfHeight = 0.3f;
        float halfDepth  = 5.2f; 

        glBegin(GL_QUADS);
            // Face atas 
            glVertex3f(-halfWidth,  halfHeight, -halfDepth);
            glVertex3f( halfWidth,  halfHeight, -halfDepth);
            glVertex3f( halfWidth,  halfHeight,  halfDepth);
            glVertex3f(-halfWidth,  halfHeight,  halfDepth);

            // Face bawah
            glVertex3f(-halfWidth, -halfHeight, -halfDepth);
            glVertex3f( halfWidth, -halfHeight, -halfDepth);
            glVertex3f( halfWidth, -halfHeight,  halfDepth);
            glVertex3f(-halfWidth, -halfHeight,  halfDepth);

            // Face depan
            glVertex3f(-halfWidth, -halfHeight,  halfDepth);
            glVertex3f( halfWidth, -halfHeight,  halfDepth);
            glVertex3f( halfWidth,  halfHeight,  halfDepth);
            glVertex3f(-halfWidth,  halfHeight,  halfDepth);

            // Face belakang
            glVertex3f(-halfWidth, -halfHeight, -halfDepth);
            glVertex3f( halfWidth, -halfHeight, -halfDepth);
            glVertex3f( halfWidth,  halfHeight, -halfDepth);
            glVertex3f(-halfWidth,  halfHeight, -halfDepth);

            // Face kiri
            glVertex3f(-halfWidth, -halfHeight, -halfDepth);
            glVertex3f(-halfWidth, -halfHeight,  halfDepth);
            glVertex3f(-halfWidth,  halfHeight,  halfDepth);
            glVertex3f(-halfWidth,  halfHeight, -halfDepth);

            // Face kanan
            glVertex3f( halfWidth, -halfHeight, -halfDepth);
            glVertex3f( halfWidth, -halfHeight,  halfDepth);
            glVertex3f( halfWidth,  halfHeight,  halfDepth);
            glVertex3f( halfWidth,  halfHeight, -halfDepth);
        glEnd();
    glPopMatrix();
 } 
 
    { // pintu dan jendela
    glPushMatrix();

        glTranslatef(0.0f, 0.35f, 4.65f); 
        glColor3ub(133, 101, 14);

        // Ukuran pintu
        float doorWidth     = 2.5f;  
        float doorHeight    = 4.0f;  
        float doorThickness = 0.1f;  
        float archRadius    = 1.25f; 
        int   segments      = 16;    // jumlah segmen lengkungan

        // Variabel pembantu
        float w2 = doorWidth * 0.5f; // setengah lebar
        float t  = doorThickness;
        float h  = doorHeight;

        // Depan
        glBegin(GL_QUADS);
            glVertex3f(-w2, 0.0f,       0.0f);
            glVertex3f( w2, 0.0f,       0.0f);
            glVertex3f( w2, h,          0.0f);
            glVertex3f(-w2, h,          0.0f);
        glEnd();

        // Belakang
        glBegin(GL_QUADS);
            glVertex3f(-w2, 0.0f,      -t);
            glVertex3f( w2, 0.0f,      -t);
            glVertex3f( w2, h,         -t);
            glVertex3f(-w2, h,         -t);
        glEnd();

        // Sisi bawah
        glBegin(GL_QUADS);
            glVertex3f(-w2, 0.0f,  0.0f);
            glVertex3f(-w2, 0.0f, -t);
            glVertex3f( w2, 0.0f, -t);
            glVertex3f( w2, 0.0f,  0.0f);
        glEnd();

        // Sisi atas 
        glBegin(GL_QUADS);
            glVertex3f(-w2, h,  0.0f);
            glVertex3f(-w2, h, -t);
            glVertex3f( w2, h, -t);
            glVertex3f( w2, h,  0.0f);
        glEnd();

        // Sisi kiri
        glBegin(GL_QUADS);
            glVertex3f(-w2, 0.0f,  0.0f);
            glVertex3f(-w2, h,     0.0f);
            glVertex3f(-w2, h,    -t);
            glVertex3f(-w2, 0.0f, -t);
        glEnd();

        // Sisi kanan
        glBegin(GL_QUADS);
            glVertex3f(w2, 0.0f,  0.0f);
            glVertex3f(w2, h,     0.0f);
            glVertex3f(w2, h,    -t);
            glVertex3f(w2, 0.0f, -t);
        glEnd();

    
        // border
        {    float borderThickness = 0.1f;
        glColor3ub(90, 81, 102);

        // border atas
        glPushMatrix();
            glTranslatef(0.0f, h + borderThickness*0.5f, -t*0.5f);
            glScalef(doorWidth + 2*borderThickness, borderThickness, doorThickness + 2*borderThickness);
            glutSolidCube(1.0f);
        glPopMatrix();

        // 2) Border sisi kiri
        glPushMatrix();
            glTranslatef(-w2 - borderThickness*0.5f, h*0.5f, -t*0.5f);
            glScalef(borderThickness, doorHeight, doorThickness + 2*borderThickness);
            glutSolidCube(1.0f);
        glPopMatrix();

        // 3) Border sisi kanan 
        glPushMatrix();
            glTranslatef(w2 + borderThickness*0.5f, h*0.5f, -t*0.5f);
            glScalef(borderThickness, doorHeight, doorThickness + 2*borderThickness);
            glutSolidCube(1.0f);
        glPopMatrix();}



        // 1. Bagian kaca 
        {
            glPushMatrix();
            glTranslatef(0.0f, 0.0f, -0.08f);
            glColor3ub(107, 215, 255);

            float baseY = h + 0.5f;  // pangkal lengkungan
            // Bagian depan (z=0)
            glBegin(GL_TRIANGLE_FAN);
                glVertex3f(0.0f, baseY, 0.0f); // pusat lengkungan
                for(int i = 0; i <= segments; i++) {
                    float theta = 3.14159f * i / segments; 
                    float xPos = archRadius * cosf(theta);
                    float yPos = archRadius * sinf(theta);
                    glVertex3f(xPos, baseY + yPos, 0.0f);
                }
            glEnd();
 
            // Bagian belakang (z = -t)
            glBegin(GL_TRIANGLE_FAN);
                glVertex3f(0.0f, baseY, -t);
                for(int i = 0; i <= segments; i++) {
                    float theta = 3.14159f * i / segments;
                    float xPos = archRadius * cosf(theta);
                    float yPos = archRadius * sinf(theta);
                    glVertex3f(xPos, baseY + yPos, -t);
                }
            glEnd();

            // Sisi lengkungan 
            glBegin(GL_QUAD_STRIP);
                for(int i = 0; i <= segments; i++) {
                    float theta = 3.14159f * i / segments;
                    float xPos = archRadius * cosf(theta);
                    float yPos = archRadius * sinf(theta);
                    glVertex3f(xPos, baseY + yPos, 0.0f);
                    glVertex3f(xPos, baseY + yPos, -t);
                }
            glEnd();

            glPopMatrix();
        }

        // 2. Border 
        {
            glColor3ub(255, 255, 255);

            float borderThickness = 0.1f;
            float outerArchRadius = archRadius + borderThickness;
            float archBaseY = h + 0.5f; 

            // Bagian depan border 
            glBegin(GL_QUAD_STRIP);
            for(int i = 0; i <= segments; i++) {
                float theta = 3.14159f * i / segments;
                float xOuter = outerArchRadius * cosf(theta);
                float yOuter = outerArchRadius * sinf(theta);
                float xInner = archRadius * cosf(theta);
                float yInner = archRadius * sinf(theta);

                glVertex3f(xOuter, archBaseY + yOuter, 0.0f); 
                glVertex3f(xInner, archBaseY + yInner, 0.0f);
            }
            glEnd();

            // Bagian belakang border 
            glBegin(GL_QUAD_STRIP);
            for(int i = 0; i <= segments; i++) {
                float theta = 3.14159f * i / segments;
                float xOuter = outerArchRadius * cosf(theta);
                float yOuter = outerArchRadius * sinf(theta);
                float xInner = archRadius * cosf(theta);
                float yInner = archRadius * sinf(theta);

                glVertex3f(xOuter, archBaseY + yOuter, -t);
                glVertex3f(xInner, archBaseY + yInner, -t);
            }
            glEnd();

            // Sisi lengkungan border
            glBegin(GL_QUAD_STRIP);
            for(int i = 0; i <= segments; i++) {
                float theta = 3.14159f * i / segments;
                float xPos = outerArchRadius * cosf(theta);
                float yPos = outerArchRadius * sinf(theta);

                glVertex3f(xPos, archBaseY + yPos, 0.0f);
                glVertex3f(xPos, archBaseY + yPos, -t);
            }
            glEnd();

            // border atas
            glPushMatrix();
                glTranslatef(0.0f, 0.5 + h + borderThickness*0.5f, -t*0.5f);
                glScalef(doorWidth + 2*borderThickness, borderThickness, doorThickness + 2*borderThickness);
                glutSolidCube(1.0f);
             glPopMatrix();
        }

        // 3. Tralis 
        {
        {
            float tralisThickness = 0.05f;
            float R = archRadius;  // jari-jari lengkungan

            float centerX = 0.0f;
            float centerY = h + 0.5f;  
            float centerZ = 0.0f;   

            for (int i = 0; i < 3; i++) {
                float theta;
                if (i == 0)
                    theta = 3.14159f * 0.5f;   // 90° (vertikal)
                else if (i == 1)
                    theta = 3.14159f * 0.25f;  // 45°
                else
                    theta = 3.14159f * 0.75f;  // 135°

                // Titik start adalah pusat
                float sx = centerX;
                float sy = centerY;
                float sz = centerZ;
                // Titik end dihitung dari pusat + offset berbasis theta
                float ex = centerX + R * cosf(theta);
                float ey = centerY + R * sinf(theta);
                float ez = centerZ;
                
                // Vektor dari start ke end:
                float dx = ex - sx;
                float dy = ey - sy;
                // Panjang vektor (seharusnya sama dengan R)
                float len = sqrt(dx * dx + dy * dy);
                // Vektor normal (perpendicular di bidang XY)
                float nx = -dy / len;
                float ny = dx / len;
                // Offset sebesar setengah ketebalan
                float ox = nx * (tralisThickness * 0.5f);
                float oy = ny * (tralisThickness * 0.5f);

                // Tentukan empat titik quad:
                // v1: start + offset
                float v1x = sx + ox, v1y = sy + oy, v1z = sz;
                // v2: start - offset
                float v2x = sx - ox, v2y = sy - oy, v2z = sz;
                // v3: end - offset
                float v3x = ex - ox, v3y = ey - oy, v3z = ez;
                // v4: end + offset
                float v4x = ex + ox, v4y = ey + oy, v4z = ez;
                
                glColor3ub(255, 255, 255);
                glBegin(GL_QUADS);
                    glVertex3f(v1x, v1y, v1z);
                    glVertex3f(v2x, v2y, v2z);
                    glVertex3f(v3x, v3y, v3z);
                    glVertex3f(v4x, v4y, v4z);
                glEnd();
            }
        }
     }

        // gagang pintu
        glPushMatrix();
             glTranslatef(-w2 + 0.2f, h * 0.5f, 0.0f);
             glColor3ub(80, 80, 80); 
             glutSolidSphere(0.08, 16, 16);
        glPopMatrix();

    glPopMatrix();
    }

    { // Jendela 1
    glPushMatrix();
        glTranslatef(2.0f, 12.85f, 4.65f); 
        float windowWidth  = 2.0f;
        float windowHeight = 3.0f;
        float windowDepth  = 0.1f;
        float frameThickness = 0.2f;
        float crossThickness = 0.1f;

        // Kaca jendela
        glColor3ub(107, 215, 255);
        glBegin(GL_QUADS);
            glVertex3f(-windowWidth * 0.5f, -windowHeight * 0.5f, 0.0f);
            glVertex3f( windowWidth * 0.5f, -windowHeight * 0.5f, 0.0f);
            glVertex3f( windowWidth * 0.5f,  windowHeight * 0.5f, 0.0f);
            glVertex3f(-windowWidth * 0.5f,  windowHeight * 0.5f, 0.0f);
        glEnd();

        // Frame 
        glColor3ub(255, 255, 255);
        // Atas
        glPushMatrix();
            glTranslatef(0.0f, windowHeight * 0.5f + frameThickness * 0.5f, 0.0f);
            glScalef(windowWidth + frameThickness * 2, frameThickness, windowDepth);
            glutSolidCube(1.0f);
        glPopMatrix();

        // Bawah
        glPushMatrix();
            glTranslatef(0.0f, -windowHeight * 0.5f - frameThickness * 0.5f, 0.0f);
            glScalef(windowWidth + frameThickness * 2, frameThickness, windowDepth);
            glutSolidCube(1.0f);
        glPopMatrix();


        // Kiri
        glPushMatrix();
            glTranslatef(-windowWidth * 0.5f - frameThickness * 0.5f, 0.0f, 0.0f);
            glScalef(frameThickness, windowHeight + frameThickness * 2, windowDepth);
            glutSolidCube(1.0f);
        glPopMatrix();

        // Kanan
        glPushMatrix();
            glTranslatef(windowWidth * 0.5f + frameThickness * 0.5f, 0.0f, 0.0f);
            glScalef(frameThickness, windowHeight + frameThickness * 2, windowDepth);
            glutSolidCube(1.0f);
        glPopMatrix();

        
        {glPushMatrix(); //alas
            glColor3ub(90, 81, 102); 

            float sillWidth  = 2.7f;  
            float sillHeight = 0.4f;  
            float sillDepth  = 0.5f; 

            glTranslatef(0.0f, (-windowHeight * 0.5f - frameThickness * 0.5f) - 0.2f, 0.0f);

            glScalef(sillWidth, sillHeight, sillDepth);
            glutSolidCube(1.0f);
        glPopMatrix();}

        // Frame tengah 
        glColor3ub(255, 255, 255);

        // Vertikal
        glPushMatrix();
            glScalef(crossThickness, windowHeight, windowDepth);
            glutSolidCube(1.0f);
        glPopMatrix();

        // Horizontal
        glPushMatrix();
            glScalef(windowWidth, crossThickness, windowDepth);
            glutSolidCube(1.0f);
        glPopMatrix();

    glPopMatrix();    
}

    { // Jendela 2
            glPushMatrix();
            glTranslatef(-2.0f, 12.85f, 4.65f);
            float windowWidth  = 2.0f;
            float windowHeight = 3.0f;
            float windowDepth  = 0.1f;
            float frameThickness = 0.2f;
            float crossThickness = 0.1f;
    
            // Kaca jendela
            glColor3ub(107, 215, 255);
            glBegin(GL_QUADS);
                glVertex3f(-windowWidth * 0.5f, -windowHeight * 0.5f, 0.0f);
                glVertex3f( windowWidth * 0.5f, -windowHeight * 0.5f, 0.0f);
                glVertex3f( windowWidth * 0.5f,  windowHeight * 0.5f, 0.0f);
                glVertex3f(-windowWidth * 0.5f,  windowHeight * 0.5f, 0.0f);
            glEnd();
    
            // Frame 
            glColor3ub(255, 255, 255);
            // Atas
            glPushMatrix();
                glTranslatef(0.0f, windowHeight * 0.5f + frameThickness * 0.5f, 0.0f);
                glScalef(windowWidth + frameThickness * 2, frameThickness, windowDepth);
                glutSolidCube(1.0f);
            glPopMatrix();
    
            // Bawah
            glPushMatrix();
                glTranslatef(0.0f, -windowHeight * 0.5f - frameThickness * 0.5f, 0.0f);
                glScalef(windowWidth + frameThickness * 2, frameThickness, windowDepth);
                glutSolidCube(1.0f);
            glPopMatrix();
    
            // Kiri
            glPushMatrix();
                glTranslatef(-windowWidth * 0.5f - frameThickness * 0.5f, 0.0f, 0.0f);
                glScalef(frameThickness, windowHeight + frameThickness * 2, windowDepth);
                glutSolidCube(1.0f);
            glPopMatrix();
    
            // Kanan
            glPushMatrix();
                glTranslatef(windowWidth * 0.5f + frameThickness * 0.5f, 0.0f, 0.0f);
                glScalef(frameThickness, windowHeight + frameThickness * 2, windowDepth);
                glutSolidCube(1.0f);
            glPopMatrix();
    
            {glPushMatrix(); // alas
                glColor3ub(90, 81, 102);

                float sillWidth  = 2.7f; 
                float sillHeight = 0.4f;  
                float sillDepth  = 0.5f; 

                glTranslatef(0.0f, (-windowHeight * 0.5f - frameThickness * 0.5f) - 0.2f, 0.0f);
    
                glScalef(sillWidth, sillHeight, sillDepth);
                glutSolidCube(1.0f);
            glPopMatrix();}
    
    
            // Frame tengah 
            glColor3ub(255, 255, 255);
    
            // Vertikal
            glPushMatrix();
                glScalef(crossThickness, windowHeight, windowDepth);
                glutSolidCube(1.0f);
            glPopMatrix();
    
            // Horizontal
            glPushMatrix();
                glScalef(windowWidth, crossThickness, windowDepth);
                glutSolidCube(1.0f);
            glPopMatrix();
        glPopMatrix();
    }

    { // brick

        glPushMatrix();
            glTranslatef(0.0f, 4.6f, 0.0f);
            glColor3ub(145, 111, 0);
    
            // 1
            glPushMatrix();
                glTranslatef(3.2f, 2.0f, 4.51f);
                drawBrick(1.25f, 0.75f, 0.2f); 
            glPopMatrix();
    
            // 2
            glPushMatrix();
                glTranslatef(-2.5f, 1.75f, 4.51f);
                drawBrick(1.25f, 0.75f, 0.2f); 
            glPopMatrix();

            // 3
            glPushMatrix();
                glTranslatef(-3.4f, 2.75f, 4.51f);
                drawBrick(1.25f, 0.75f, 0.2f); 
            glPopMatrix();

            // 4
            glPushMatrix();
                glTranslatef(2.4f, -1.3f, 4.51f);
                drawBrick(1.25f, 0.75f, 0.2f); 
            glPopMatrix();


            // 5
            glPushMatrix();
                glTranslatef(0.8f, 5.5f, 4.51f);
                drawBrick(1.25f, 0.75f, 0.2f); 
            glPopMatrix();

            // 6
            glPushMatrix();
                glTranslatef(-2.9f, 10.8f, 4.51f);
                drawBrick(1.25f, 0.75f, 0.2f); 
            glPopMatrix();

        glPopMatrix();
    }
    glPopMatrix();
}


// Fungsi display: perbarui pergerakan dan gambar scene
void display() {
    // Update pergerakan posisi (jika ada)
    if (deltaMove) moveMeFlat(deltaMove);
    if (strafeMove) strafeCamera(strafeMove);
    if (verticalMove) moveVertical(verticalMove);
    
    // Update rotasi horizontal dan pitch (dari tombol panah)
    angle += deltaAngle;
    pitch += deltaPitch;
    orientMe();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Grid();
    rumah();
    rumah2();
    rumah3();

    glutSwapBuffers();
    glFlush();
}

// --- Input Handling ---

// Special keys: hanya untuk rotasi (menoleh)  
// Panah kiri/kanan: rotasi horizontal  
// Panah atas: mengengok ke atas (meningkatkan pitch)  
// Panah bawah: mengengok ke bawah (menurunkan pitch)
void pressKey(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_LEFT:
            deltaAngle = -0.0005f;
            break;
        case GLUT_KEY_RIGHT:
            deltaAngle = 0.0005f;
            break;
        case GLUT_KEY_UP:
            deltaPitch = 0.0005f;
            break;
        case GLUT_KEY_DOWN:
            deltaPitch = -0.0005f;
            break;
    }
}

void releaseKey(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_LEFT:
        case GLUT_KEY_RIGHT:
            deltaAngle = 0.0f;
            break;
        case GLUT_KEY_UP:
        case GLUT_KEY_DOWN:
            deltaPitch = 0.0f;
            break;
    }
}

// Keyboard input untuk karakter:
// A/D: geser kiri/kanan  
// W/S: maju/mundur  
// Space: naik (fly up)  
// c: turun (fly down)
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'a': case 'A':
            strafeMove = -0.4f;
            break;
        case 'd': case 'D':
            strafeMove = 0.4f;
            break;
        case 'w': case 'W':
            deltaMove = 0.4f;
            break;
        case 's': case 'S':
            deltaMove = -0.4f;
            break;
        case ' ':  // Space untuk naik (fly up)
            verticalMove = 0.4f;
            break;
        case 'c': case 'C':  //c untuk turun (fly down)
            verticalMove = -0.3f;
            break;
    }
}

void keyboardUp(unsigned char key, int x, int y) {
    switch (key) {
        case 'a': case 'A': case 'd': case 'D':
            strafeMove = 0.0f;
            break;
        case 'w': case 'W': case 's': case 'S':
            deltaMove = 0.0f;
            break;
        case ' ':
            verticalMove = 0.0f;
            break;
        case 'c': case 'C':
            verticalMove = 0.0f;
            break;
    }
}

// --- Pencahayaan ---
// Pencahayaan yang lebih natural
// Lighting setup function




void lighting() {

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

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);

    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
}


void init(void) {
    glEnable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}



int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(640, 480);
    glutCreateWindow("Agathan Khairy - 24060123140129");

    glutIgnoreKeyRepeat(1);
    glutSpecialFunc(pressKey);
    glutSpecialUpFunc(releaseKey);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardUp);
    glutDisplayFunc(display);
    glutIdleFunc(display);
    glutReshapeFunc(Reshape);

    lighting();
    init();

    glutMainLoop();
    return 0;
}
