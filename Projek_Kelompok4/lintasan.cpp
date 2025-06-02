#include <GL/glut.h>
#include <math.h>
#include <iostream>
#include <string>
#include <iomanip>
#include <vector>
#include <sstream>
#include <cstdlib> 
#include <ctime>   
#include "imageloader.h" 

float _angle = 0;             
GLuint _textureId;
// Variabel untuk tracking jalan
float roadStartZ = 10.0f;     // Titik awal jalan (legacy - akan diganti)
float roadEndZ = -350.0f;     // Titik akhir jalan (legacy - akan diganti)


float l[]={-4.0,8.0,-3.0,1.0};    // Light position
float e[]={0.0,0.0,0.0,1.0};      // Eye position
float n[]={0.0,1.0,0.0,0.0};      // Plane equation
float rx=0.0,ry=0.0;   

// Variabel untuk road segments
const int NUM_ROAD_SEGMENTS = 12;
struct RoadSegment {
    float startZ;
    float endZ;
    bool active;
};
RoadSegment roadSegments[NUM_ROAD_SEGMENTS];
float roadSegmentLength = 40.0f; // Panjang tiap segmen jalan 
float roadWidth = 6.0f;    // Lebar jalan (diperkecil dari 8.0f menjadi 6.0f)
float groundLevel = 0.0f;  // Ketinggian dasar ground

// Konstanta untuk ruas jalan
const int NUM_LANES = 2; // Jumlah ruas jalan (diubah dari 3 menjadi 2)
float laneWidth; // Akan dihitung sebagai roadWidth / NUM_LANES

// Variabel untuk tracking jalan infinite
float visibleRoadStartZ = 10.0f;     // Awal jalan terlihat (menggantikan roadStartZ)
float visibleRoadEndZ = -350.0f;     // Akhir jalan terlihat (menggantikan roadEndZ)
float roadGenerationDistance = 100.0f; // Jarak di mana segmen baru dibuat

// Fungsi untuk render text
void renderText(float x, float y, const std::string& text, void* font = GLUT_BITMAP_HELVETICA_18) {
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(x, y);
    for (size_t i = 0; i < text.length(); i++) {  // Traditional C++ for loop
        glutBitmapCharacter(font, text[i]);
    }
}

// Struktur Vertex untuk drawBlockCustom
struct Vertex {
    float x, y, z;
};

// Forward declarations
void repositionBarrier(int index);

// Variabel untuk gerobak
float carX = 0.0f;
float targetCarX = 0.0f;
float steeringSpeed = 0.15f; // Ditingkatkan agar lebih responsif
float carZ = 10.0f;
float carY = 0.0f;
float carYOffset = 0.3f; // Offset untuk menaikkan gerobak dari permukaan jalan
float carAngle = 0.0f; // Rotasi Y utama gerobak (jika diperlukan nanti)
float carTiltAngle = 0.0f; // Untuk animasi miring saat belok (sumbu Z lokal)

float wheelRotation = 0.0f; // Rotasi roda untuk animasi

float carSpeed = 0.0f;  // Mulai dari diam (0)
float initialCarSpeed = 0.02f; // Lebih lambat dari sebelumnya (0.05f)
float maxSpeed = 0.5f;  // Lebih lambat dari sebelumnya (0.8f)
float gravityFactor = 0.00002f; // Mengurangi efek gravitasi

// Variabel untuk kontrol
bool keys[256] = {false};
bool specialKeys[256] = {false};

// Skor
long long score = 0;
float lastScoreUpdateZ = 10.0f;

const float GLOBAL_SLOPE_FACTOR = 0.0f; // Set to zero to make road horizontal

// Data untuk barrier
const int MAX_BARRIERS = 8; // Disesuaikan untuk 2 jalur
struct BarrierData {
    float x;         // Posisi X
    float z;         // Posisi Z
    float scale;     // Skala barrier
    bool active;     // Apakah barrier aktif
    int lane;        // Ruas jalan (0: kiri, 1: kanan)
};
BarrierData barriers[MAX_BARRIERS];

// Struktur untuk awan
const int NUM_CLOUDS = 20;
struct CloudData {
    float x, y, z;     // Posisi awan
    float scale;       // Skala awan
    float speed;       // Kecepatan pergerakan awan (opsional)
};
CloudData clouds[NUM_CLOUDS];

// Status untuk pesan tabrakan
bool showCollisionMessage = false;
clock_t collisionTime;
const int COLLISION_MESSAGE_DURATION = 1500; // 1.5 detik


GLuint loadTexture(Image* image) {
    GLuint textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    
    // Set texture parameters untuk kualitas yang lebih baik
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    // Upload texture data
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 
                 0, GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
    
    return textureId;
}

GLuint createDefaultTexture() {
    GLuint textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    
    // Create simple green checkerboard pattern
    unsigned char defaultTexture[64][64][3];
    for (int i = 0; i < 64; i++) {
        for (int j = 0; j < 64; j++) {
            if ((i/8 + j/8) % 2 == 0) {
                defaultTexture[i][j][0] = 34;  // R - Dark green
                defaultTexture[i][j][1] = 139; // G
                defaultTexture[i][j][2] = 34;  // B
            } else {
                defaultTexture[i][j][0] = 50;  // R - Light green
                defaultTexture[i][j][1] = 205; // G
                defaultTexture[i][j][2] = 50;  // B
            }
        }
    }
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 64, 64, 0, GL_RGB, GL_UNSIGNED_BYTE, defaultTexture);
    
    return textureId;
}

// 2. PERBAIKI initRendering - TAMBAHKAN ERROR CHECKING
void initRendering() { 
    glEnable(GL_DEPTH_TEST); 
    glEnable(GL_LIGHTING); 
    glEnable(GL_LIGHT0); 
    glEnable(GL_NORMALIZE); 
    glEnable(GL_COLOR_MATERIAL); 
    
    // Load texture dengan error checking
    Image* image = loadBMP("rumput.bmp"); 
    if (image ) {
        _textureId = loadTexture(image); 
        delete image;
        printf("Texture loaded successfully! ID: %d\n", _textureId);
    } else {
        printf("ERROR: Failed to load rumput.bmp!\n");
        // Create default texture jika gagal load
        _textureId = createDefaultTexture();
    }
} 


void gambarAwan(float x, float y, float z) {
    glColor3d(1.0, 1.0, 1.0); // set warna awan putih
    glPushMatrix();
    glTranslatef(x, y, z);
    glutSolidSphere(1.0, 50, 50);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(x + 1.0, y + 0.5, z);
    glutSolidSphere(1.5, 50, 50);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(x - 1.0, y + 0.5, z);
    glutSolidSphere(2, 50, 50);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(x + 0.5, y + 1.0, z);
    glutSolidSphere(1.0, 50, 50);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(x - 0.5, y + 1.0, z);
    glutSolidSphere(1.0, 50, 50);
    glPopMatrix();
}

void initClouds() {
    // Inisialisasi awan dengan posisi acak
    for (int i = 0; i < NUM_CLOUDS; i++) {
        clouds[i].x = (rand() % 200) - 100.0f;  // Posisi X: -100 sampai +100
        clouds[i].y = 20.0f + (rand() % 10);    // Ketinggian awan: 20-30 unit di langit
        
        // Posisikan awan dari awal jalan hingga jauh ke depan
        float startZ = carZ + 50.0f;
        float endZ = startZ - 400.0f;
        clouds[i].z = startZ - (rand() % (int)(startZ - endZ));
        
        // Variasi skala awan
        clouds[i].scale = 1.5f + ((float)(rand() % 100) / 100.0f);
        
        // Awan bergerak dengan kecepatan yang berbeda-beda (opsional)
        clouds[i].speed = 0.005f + ((float)(rand() % 10) / 1000.0f);
    }
}

void drawClouds() {
    glDisable(GL_LIGHTING);  // Matikan pencahayaan agar awan terlihat lebih cerah
    
    for (int i = 0; i < NUM_CLOUDS; i++) {
        glPushMatrix();
            glTranslatef(clouds[i].x, clouds[i].y, clouds[i].z);
            glScalef(clouds[i].scale, clouds[i].scale * 0.7f, clouds[i].scale);
            gambarAwan(0, 0, 0); // Gambar awan pada posisi yang ditentukan
        glPopMatrix();
    }
    
     
    
    glEnable(GL_LIGHTING);  // Aktifkan kembali lighting untuk objek lain
}
 
void updateClouds() {
    // Perbarui posisi awan
    for (int i = 0; i < NUM_CLOUDS; i++) {
        // Opsional: Gerakan awan secara perlahan pada sumbu X
        clouds[i].x += clouds[i].speed;
        
        // Recycle awan yang sudah terlalu jauh dari pemain
        if (clouds[i].z > carZ + 70.0f) {
            // Pindahkan awan ke depan pemain dengan posisi baru
            clouds[i].x = (rand() % 200) - 100.0f;
            clouds[i].y = 20.0f + (rand() % 10);
            clouds[i].z = carZ - 250.0f - (rand() % 100);
            clouds[i].scale = 0.8f + ((float)(rand() % 60) / 100.0f);
        }
        
        // Opsional: Recycle awan yang bergerak terlalu jauh ke sisi kanan
        if (clouds[i].x > 100.0f) {
            clouds[i].x = -100.0f;
        }
    }
}

// Fungsi untuk menggambar batu bata
void drawBrick(float width, float height, float depth) {
    glBegin(GL_QUADS);
        // Face atas
        glVertex3f(-width/2, height/2, -depth/2);
        glVertex3f(width/2, height/2, -depth/2);
        glVertex3f(width/2, height/2, depth/2);
        glVertex3f(-width/2, height/2, depth/2);

        // Face bawah
        glVertex3f(-width/2, -height/2, -depth/2);
        glVertex3f(width/2, -height/2, -depth/2);
        glVertex3f(width/2, -height/2, depth/2);
        glVertex3f(-width/2, -height/2, depth/2);

        // Face depan
        glVertex3f(-width/2, -height/2, depth/2);
        glVertex3f(width/2, -height/2, depth/2);
        glVertex3f(width/2, height/2, depth/2);
        glVertex3f(-width/2, height/2, depth/2);

        // Face belakang
        glVertex3f(-width/2, -height/2, -depth/2);
        glVertex3f(width/2, -height/2, -depth/2);
        glVertex3f(width/2, height/2, -depth/2);
        glVertex3f(-width/2, height/2, -depth/2);

        // Face kiri
        glVertex3f(-width/2, -height/2, -depth/2);
        glVertex3f(-width/2, -height/2, depth/2);
        glVertex3f(-width/2, height/2, depth/2);
        glVertex3f(-width/2, height/2, -depth/2);

        // Face kanan
        glVertex3f(width/2, -height/2, -depth/2);
        glVertex3f(width/2, -height/2, depth/2);
        glVertex3f(width/2, height/2, depth/2);
        glVertex3f(width/2, height/2, -depth/2);
    glEnd();
}

// Fungsi untuk menggambar rumah
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
                    theta = 3.14159f * 0.5f;   // 90? (vertikal)
                else if (i == 1)
                    theta = 3.14159f * 0.25f;  // 45?
                else
                    theta = 3.14159f * 0.75f;  // 135?

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

// Inisialisasi laneWidth
void initLaneWidth() {
    laneWidth = roadWidth / NUM_LANES;
}

void init() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glShadeModel(GL_SMOOTH); // Untuk gradasi warna yang lebih baik pada model

    GLfloat lightPos[] = {0.0f, 30.0f, 20.0f, 1.0f};
    GLfloat lightAmbient[] = {0.3f, 0.3f, 0.3f, 1.0f};
    GLfloat lightDiffuse[] = {0.9f, 0.9f, 0.9f, 1.0f};
    GLfloat lightSpecular[] = {1.0f, 1.0f, 1.0f, 1.0f};

    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);

    glClearColor(0.6f, 0.8f, 1.0f, 1.0f);
    
    // Inisialisasi lebar ruas jalan
    initLaneWidth();
}

void drawGround() {
    glDisable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, _textureId);
    glColor3f(1.0f, 1.0f, 1.0f);

    // Posisikan ground pada level yang konsisten
    float groundDepth = -0.2f; // Ground berada di bawah jalan 
    float groundWidth = 200.0f; // Perlebar ground lebih lagi karena jalan juga lebih lebar
    
    // Perpanjang ground di depan dan belakang gerobak
    float frontZ = carZ - 300.0f; // Perpanjang jauh ke depan
    float backZ = carZ + 100.0f; // Perpanjang ke belakang
    
    // Pastikan ground selalu datar (flat)
    float groundY = groundDepth;
    
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-groundWidth, groundY, frontZ);
        
        glTexCoord2f(10.0f, 0.0f); // Repeat 10x di X
        glVertex3f(groundWidth, groundY, frontZ);
        
        glTexCoord2f(10.0f, 20.0f); // Repeat 20x di Z
        glVertex3f(groundWidth, groundY, backZ);
        
        glTexCoord2f(0.0f, 20.0f);
        glVertex3f(-groundWidth, groundY, backZ);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
    
}

void drawFencePost(float x, float y, float z) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glScalef(0.15f, 0.8f, 0.15f);
    glColor3f(0.4f, 0.25f, 0.15f);
    glutSolidCube(1.0f);
    glPopMatrix();
}

void drawFenceRail(float x1, float y1, float z1, float x2, float y2, float z2, float thickness) {
    glPushMatrix();
    float dx = x2 - x1;
    float dy = y2 - y1;
    float dz = z2 - z1;
    float length = sqrt(dx*dx + dy*dy + dz*dz);

    glTranslatef(x1 + dx/2.0f, y1 + dy/2.0f, z1 + dz/2.0f);

    // Hitung sudut rotasi agar rail menghadap benar
    // Rotasi terhadap sumbu Y untuk orientasi XZ
    float angleY = atan2(dx, dz) * 180.0 / M_PI;
    glRotatef(angleY, 0, 1, 0);

    glScalef(thickness, thickness, length);
    glColor3f(0.45f, 0.3f, 0.2f);
    glutSolidCube(1.0f);
    glPopMatrix();
}

void drawInfiniteRoad() {
    float renderSegmentLength = 0.5f; // Segmen kecil untuk rendering garis putus-putus
    float fenceOffsetFromRoad = 0.3f;
    float fencePostActualHeight = 0.8f;
    float fenceRailThickness = 0.08f;
    float fenceTopRailYOffset = 0.55f;
    float fenceBottomRailYOffset = 0.2f;
    float roadYOffset = 0.03f; // Sedikit offset untuk menghindari z-fighting dengan ground

    // Render semua segmen jalan aktif
    for (int segIdx = 0; segIdx < NUM_ROAD_SEGMENTS; segIdx++) {
        if (!roadSegments[segIdx].active) continue;
        
        // Untuk setiap segmen jalan, buat sub-segmen kecil untuk detail
        float segmentZ1 = roadSegments[segIdx].startZ;
        float segmentZ2 = roadSegments[segIdx].endZ;
        int numRenderSegments = (int)((segmentZ1 - segmentZ2) / renderSegmentLength);
        
        for(int i = 0; i < numRenderSegments; i++) {
            float z1_abs = segmentZ1 - i * renderSegmentLength;
            float z2_abs = segmentZ1 - (i + 1) * renderSegmentLength;
            // Jangan render di luar segmen
            if (z2_abs < segmentZ2) z2_abs = segmentZ2;

            // Use constant height for flat road
            float y1 = groundLevel + roadYOffset;
            float y2 = groundLevel + roadYOffset;        
            
            glEnable(GL_LIGHTING);
            glColor3f(0.3f, 0.3f, 0.3f);
            glBegin(GL_QUADS);
                glNormal3f(0.0f, 1.0f, 0.0f);
                glVertex3f(-roadWidth/2, y1, z1_abs); // Jalan statis tidak menggunakan carX
                glVertex3f(roadWidth/2, y1, z1_abs); 
                glVertex3f(roadWidth/2, y2, z2_abs); 
                glVertex3f(-roadWidth/2, y2, z2_abs); 
            glEnd();        
            
            glDisable(GL_LIGHTING);
            
            // Gambar garis pembatas tengah antar jalur (untuk 2 ruas jalan)
            glColor3f(1.0f, 1.0f, 1.0f); // Warna putih untuk garis tengah
            glLineWidth(3.0f); // Garis pemisah tengah lebih tebal
            
            // Hanya satu garis tengah karena hanya 2 ruas
            glBegin(GL_LINES);
                glVertex3f(0.0f, y1 + 0.01f, z1_abs); // Garis tengah pada X = 0
                glVertex3f(0.0f, y2 + 0.01f, z2_abs);
            glEnd();
            
            // Garis putus-putus untuk tiap ruas
            glColor3f(1.0f, 1.0f, 0.0f); // Warna kuning untuk garis tengah jalur
            glLineWidth(2.0f);
            
            if (i % 2 == 0) { // Garis putus-putus
                // Garis tengah di ruas kiri
                float laneCenter1 = -roadWidth/4;
                glBegin(GL_LINES);
                    glVertex3f(laneCenter1, y1 + 0.01f, z1_abs);
                    glVertex3f(laneCenter1, y2 + 0.01f, z2_abs);
                glEnd();
                
                // Garis tengah di ruas kanan
                float laneCenter2 = roadWidth/4;
                glBegin(GL_LINES);
                    glVertex3f(laneCenter2, y1 + 0.01f, z1_abs);
                    glVertex3f(laneCenter2, y2 + 0.01f, z2_abs);
                glEnd();
            }
            
            // Garis tepi jalan
            glColor3f(1.0f, 1.0f, 1.0f); // Warna putih
            glLineWidth(2.0f);
            glBegin(GL_LINES);
                glVertex3f(-roadWidth/2 + 0.1f, y1 + 0.01f, z1_abs); // Garis tepi kiri
                glVertex3f(-roadWidth/2 + 0.1f, y2 + 0.01f, z2_abs);
                glVertex3f(roadWidth/2 - 0.1f, y1 + 0.01f, z1_abs);  // Garis tepi kanan
                glVertex3f(roadWidth/2 - 0.1f, y2 + 0.01f, z2_abs);
            glEnd();

            glEnable(GL_LIGHTING);
            for (int side = -1; side <= 1; side += 2) {
                float fence_base_x_z1 = side * (roadWidth/2 + fenceOffsetFromRoad);
                float fence_base_x_z2 = side * (roadWidth/2 + fenceOffsetFromRoad);

                drawFencePost(fence_base_x_z1, y1 + (fencePostActualHeight * 0.5f) - 0.05f, z1_abs);

                drawFenceRail(fence_base_x_z1, y1 + fenceTopRailYOffset, z1_abs,
                              fence_base_x_z2, y2 + fenceTopRailYOffset, z2_abs,
                              fenceRailThickness);
                drawFenceRail(fence_base_x_z1, y1 + fenceBottomRailYOffset, z1_abs,
                              fence_base_x_z2, y2 + fenceBottomRailYOffset, z2_abs,
                              fenceRailThickness);

                // Draw fence post at the end of each segment
                if (i == numRenderSegments - 1) {
                    drawFencePost(fence_base_x_z2, y2 + (fencePostActualHeight * 0.5f) - 0.05f, z2_abs);
                }
            }
        }
    }
}

// Fungsi-fungsi untuk membuat orang
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

void BuatKakiMenekuk(bool kiri) {
    glPushMatrix();
    float posisiX = (kiri ? -1.2f : 1.2f);
    
    // Posisi awal kaki
    glTranslatef(posisiX, -3.0f, 0.0f);
    
    // Rotasi kaki untuk tekukan
    glRotatef(80.0f, 1.0f, 0.0f, 0.0f);
    
    // Paha
    glPushMatrix();
        glColor3f(1.0f, 0.0f, 0.0f);
        glScalef(1.1f, 2.0f, 1.1f);
        glTranslatef(0.0f, -1.0f, 0.0f);
        glutSolidCube(2.0f);
    glPopMatrix();
    
    // Betis (ditekuk)
    glPushMatrix();
        glTranslatef(0.0f, -4.0f, -2.0f);
        glRotatef(-70.0f, 1.0f, 0.0f, 0.0f); // Tekukan di lutut
        
        glColor3f(0.0f, 1.0f, 0.0f);
        glScalef(1.0f, 2.0f, 1.0f);
        glTranslatef(0.0f, -1.5f, 0.0f);
        glutSolidCube(2.0f);
        
        // Telapak kaki
        glPushMatrix();
            glTranslatef(0.0f, -2.5f, -0.7f);
            glRotatef(20.0f, 1.0f, 0.0f, 0.0f); // Sedikit menekuk telapak
            
            glColor3f(0.0f, 0.0f, 1.0f);
            glScalef(1.0f, 0.7f, 1.7f);
            glutSolidCube(2.0f);
        glPopMatrix();
    glPopMatrix();

    glPopMatrix();
}

void buatOrang() {
    glPushMatrix();
    glTranslatef(0.0f, 2.0f, 0.0f);
    glScalef(0.3f, 0.3f, 0.3f);
    
    Badan();
    BuatTangan(true);
    BuatTangan(false);
    BuatKakiMenekuk(true);   // Kaki kiri menekuk
    BuatKakiMenekuk(false);  // Kaki kanan menekuk
    BuatKepala();
    BuatTopi();
    BuatLeher();
    
    glPopMatrix();
}

// --- FUNGSI GAMBAR GEROBAK DAN KOMPONENNYA ---
void drawBlockCustom(struct Vertex v[8], GLubyte r, GLubyte g, GLubyte b) {
    glColor3ub(r, g, b);
    glBegin(GL_QUADS);
        glNormal3f(0.0f, 0.0f, 1.0f);
        glVertex3f(v[4].x, v[4].y, v[4].z); glVertex3f(v[5].x, v[5].y, v[5].z);
        glVertex3f(v[6].x, v[6].y, v[6].z); glVertex3f(v[7].x, v[7].y, v[7].z);
        glNormal3f(0.0f, 0.0f, -1.0f);
        glVertex3f(v[0].x, v[0].y, v[0].z); glVertex3f(v[1].x, v[1].y, v[1].z);
        glVertex3f(v[2].x, v[2].y, v[2].z); glVertex3f(v[3].x, v[3].y, v[3].z);
        glNormal3f(-1.0f, 0.0f, 0.0f);
        glVertex3f(v[0].x, v[0].y, v[0].z); glVertex3f(v[3].x, v[3].y, v[3].z);
        glVertex3f(v[7].x, v[7].y, v[7].z); glVertex3f(v[4].x, v[4].y, v[4].z);
        glNormal3f(1.0f, 0.0f, 0.0f);
        glVertex3f(v[1].x, v[1].y, v[1].z); glVertex3f(v[2].x, v[2].y, v[2].z);
        glVertex3f(v[6].x, v[6].y, v[6].z); glVertex3f(v[5].x, v[5].y, v[5].z);
        glNormal3f(0.0f, 1.0f, 0.0f);
        glVertex3f(v[3].x, v[3].y, v[3].z); glVertex3f(v[2].x, v[2].y, v[2].z);
        glVertex3f(v[6].x, v[6].y, v[6].z); glVertex3f(v[7].x, v[7].y, v[7].z);
        glNormal3f(0.0f, -1.0f, 0.0f);
        glVertex3f(v[0].x, v[0].y, v[0].z); glVertex3f(v[1].x, v[1].y, v[1].z);
        glVertex3f(v[5].x, v[5].y, v[5].z); glVertex3f(v[4].x, v[4].y, v[4].z);
    glEnd();
}

void drawVelg() {
    glColor3f(0.8f, 0.8f, 0.8f);
    GLUquadric* quad = gluNewQuadric();
    gluDisk(quad, 0.0, 0.25, 20, 1);
    gluDeleteQuadric(quad);
}

void drawWheel1() {
    glPushMatrix();
        // Rotate wheel based on current speed
        glRotatef(wheelRotation, 0, 0, 1);
        
        glColor3f(0.2f, 0.2f, 0.2f);
        glutSolidTorus(0.3, 0.6, 20, 20);
        glPushMatrix();
            glRotatef(90, 0,1,0);
            drawVelg();
        glPopMatrix();
    glPopMatrix();
}

void drawWheel2() {
    glPushMatrix();
        // Rotate wheel based on current speed
        glRotatef(wheelRotation, 0, 0, 1);
        
        glColor3f(0.2f, 0.2f, 0.2f);
        glutSolidTorus(0.2, 0.4, 20, 20);
        glPushMatrix();
            glRotatef(90,0,1,0);
            drawVelg();
        glPopMatrix();
    glPopMatrix();
}

void drawCylinder(float baseRadius, float topRadius, float height, int slices, int stacks) {
    GLUquadric* quad = gluNewQuadric();
    glPushMatrix();
        glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
        gluCylinder(quad, baseRadius, topRadius, height, slices, stacks);
    glPopMatrix();
    gluDeleteQuadric(quad);
}

void drawGerobak() {
    glPushMatrix();
    glScalef(0.3f, 0.3f, 0.3f);
    glTranslatef(-9.0f, 1.5f + (carYOffset / 0.3f), -8.9f); // Menaikkan posisi Y untuk menghindari tumpang tindih
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f); // Orientasi utama gerobak

    // Terapkan kemiringan saat belok (pada sumbu Z LOKAL gerobak)
    // Sumbu Z lokal gerobak adalah sumbu maju/mundurnya setelah rotasi orientasi
    glRotatef(carTiltAngle, 0.0f, 1.0f, 0.0f);
    Vertex block1[8] = {
        { -1.5, 0.6, 9.85 }, {  1.5, 0.6, 9.85 }, {  2.0, 3.0, 10.3 }, { -2.0, 3.0, 10.3 },
        { -1.5, 0.6, 10.05 }, {  1.5, 0.6, 10.05 }, {  2.0, 3.0, 10.5 }, { -2.0, 3.0, 10.5 }
    }; drawBlockCustom(block1, 216, 64, 64);
    Vertex block2[8] = {
        {  1.3, 0.6, 8.05 }, {  1.5, 0.6, 8.05 }, {  2.0, 3.0, 7.6 }, {  1.8, 3.0, 7.6 },
        {  1.3, 0.6, 9.85 }, {  1.5, 0.6, 9.85 }, {  2.0, 3.0, 10.3 }, {  1.8, 3.0, 10.3 }
    }; drawBlockCustom(block2, 216, 64, 64);
    Vertex block3[8] = {
        { -1.5, 0.6, 8.05 }, { -1.45, 0.6, 8.05 }, { -1.75, 3.0, 7.6 }, { -2.0, 3.0, 7.6  },
        { -1.5, 0.6, 9.85 }, { -1.45, 0.6, 9.85 }, { -1.75, 3.0, 10.3 },{ -2.0, 3.0, 10.3 }
    }; drawBlockCustom(block3, 216, 64, 64);
    Vertex block4[8] = {
        { -1.5, 0.4, 7.85 }, {  1.5, 0.4, 7.85 }, {  1.5, 0.6, 7.85 }, { -1.5, 0.6, 7.85 },
        { -1.5, 0.4, 10.05 }, {  1.5, 0.4, 10.05 }, {  1.5, 0.6, 10.05 }, { -1.5, 0.6, 10.05 }
    }; drawBlockCustom(block4, 163, 29, 29);
    Vertex block5[8] = {
        { -1.5, 0.6, 7.85 }, {  1.5, 0.6, 7.85 }, {  2.0, 3.0, 7.4 }, { -2.0, 3.0, 7.4 },
        { -1.5, 0.6, 8.05 }, {  1.5, 0.6, 8.05 }, {  2.0, 3.0, 7.6 }, { -2.0, 3.0, 7.6 }
    }; drawBlockCustom(block5, 216, 64, 64);
    Vertex block6[8] = {
        { -2.5, 2.8, 10.3 }, { -1.9, 2.5, 10.2 }, { -2.0, 3.0, 10.3 }, { -2.5, 3.0, 10.3 },
        { -2.5, 2.8, 10.5 }, { -1.9, 2.5, 10.4 }, { -2.0, 3.0, 10.5 }, { -2.5, 3.0, 10.5 }
    }; drawBlockCustom(block6, 255, 255, 0);
    Vertex block7[8] = {
        { -2.5, 2.8, 7.4 }, { -1.9, 2.5, 7.5 }, { -2.0, 3.0, 7.4 }, { -2.5, 3.0, 7.4 },
        { -2.5, 2.8, 7.6 }, { -1.9, 2.5, 7.7 }, { -2.0, 3.0, 7.6 }, { -2.5, 3.0, 7.6 }
    }; drawBlockCustom(block7, 255, 255, 0);
    Vertex block8[8] = {
        { -2.5, 2.8, 7.6 }, { -2.3, 2.8, 7.6 }, { -2.3, 3.0, 7.6 }, { -2.5, 3.0, 7.6 },
        { -2.5, 2.8, 10.3 }, { -2.3, 2.8, 10.3 }, { -2.3, 3.0, 10.3 }, { -2.5, 3.0, 10.3 }
    }; drawBlockCustom(block8, 255, 255, 0);

    glPushMatrix(); glTranslatef(0.5, 0.3, 7.4); drawWheel1(); glPopMatrix();
    glPushMatrix(); glTranslatef(0.5, 0.3, 10.5); drawWheel1(); glPopMatrix();
    glPushMatrix(); glTranslatef(-2.0, 0.0, 8.95); drawWheel2(); glPopMatrix();

    glPushMatrix();
        glTranslatef(0.0, 0.3, 10.5);
        glColor3f(0.8f, 0.8f, 0.8f);
    	glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
        drawCylinder(0.1, 0.1, 3.1, 20, 1);
    glPopMatrix();

    Vertex block9[8] = {
        { -2.15, -0.05, 9.2 }, { -1.85, -0.05, 9.2 }, {  -1.2, 0.4, 9.2 }, { -1.5, 0.4, 9.2 },
        { -2.15, -0.05, 9.3 }, { -1.85, -0.05, 9.3 }, { -1.2, 0.4, 9.3 }, { -1.5, 0.4, 9.3 }
    }; drawBlockCustom(block9, 163, 29, 29);
    Vertex block10[8] = {
        { -2.15, -0.05, 8.6 }, { -1.85, -0.05, 8.6 }, {  -1.2, 0.4, 8.6 }, { -1.5, 0.4, 8.6 },
        { -2.15, -0.05, 8.7 }, { -1.85, -0.05, 8.7 }, { -1.2, 0.4, 8.7 }, { -1.5, 0.4, 8.7 }
    }; drawBlockCustom(block10, 163, 29, 29);

    glPushMatrix();
        glTranslatef(-2.0, 0.0, 8.7);
        glColor3f(0.8f, 0.8f, 0.8f);
        drawCylinder(0.05, 0.05, 0.5, 20, 1);
    glPopMatrix();

	glPushMatrix();
	    // Penyesuaian posisi agar orang duduk pas di dalam gerobak
	    glTranslatef(0.0f, 1.8f, 9.0f); // Posisi lebih rendah di gerobak
	    glRotatef(-90.0f, 0.0f, 1.0f, 0.0f); // Putar 90? berlawanan arah jarum jam (sumbu Y)
	    glScalef(0.8f, 0.8f, 0.8f); // Sesuaikan skala orang agar tidak terlalu besar
	    
	    // Buat animasi untuk orang bergerak saat gerobak berbelok
	    float personTiltFactor = -0.3f; // Arah berlawanan dengan kemiringan gerobak
	    glRotatef(carTiltAngle * personTiltFactor, 0.0f, 0.0f, 1.0f);
	    
	    buatOrang(); // Gambar orangnya
	glPopMatrix();
    
    glPopMatrix();
}

void barrier(float posX, float posY, float posZ, float scale) {
    glPushMatrix();
    glTranslatef(posX, posY, posZ);
    glScalef(scale, scale, scale);
    
    // Warna orange untuk barrier (seperti pada gambar)
    float mainColor[3] = {0.95f, 0.35f, 0.1f}; // Warna orange
    float darkOrange[3] = {0.85f, 0.25f, 0.05f}; // Warna orange lebih gelap
    float yellowStripe[3] = {0.95f, 0.85f, 0.1f}; // Warna kuning 
    float blackStripe[3] = {0.1f, 0.1f, 0.1f}; // Warna hitam 
    
    // Dimensi barrier
    float width = 3.0f;
    float height = 1.0f;
    float depth = 1.0f;
    
    // Bagian bawah 
    glColor3f(mainColor[0], mainColor[1], mainColor[2]);
    glBegin(GL_QUADS);
    // Bagian bawah - Atas
    glVertex3f(-width/2, 0.0f, -depth/2);
    glVertex3f(-width/2, 0.0f, depth/2);
    glVertex3f(width/2, 0.0f, depth/2);
    glVertex3f(width/2, 0.0f, -depth/2);
    
    // Bagian bawah - Depan
    glVertex3f(-width/2, 0.0f, depth/2);
    glVertex3f(-width/2, height, depth/2);
    glVertex3f(width/2, height, depth/2);
    glVertex3f(width/2, 0.0f, depth/2);
    
    // Bagian bawah - Belakang
    glVertex3f(-width/2, 0.0f, -depth/2);
    glVertex3f(width/2, 0.0f, -depth/2);
    glVertex3f(width/2, height, -depth/2);
    glVertex3f(-width/2, height, -depth/2);
    
    // Bagian bawah - Sisi kiri
    glVertex3f(-width/2, 0.0f, -depth/2);
    glVertex3f(-width/2, height, -depth/2);
    glVertex3f(-width/2, height, depth/2);
    glVertex3f(-width/2, 0.0f, depth/2);
    
    // Bagian bawah - Sisi kanan
    glVertex3f(width/2, 0.0f, -depth/2);
    glVertex3f(width/2, 0.0f, depth/2);
    glVertex3f(width/2, height, depth/2);
    glVertex3f(width/2, height, -depth/2);
    glEnd();
    
    // Bagian atas (trapezoid shape)
    float topWidth = width * 0.8f;
    float topHeight = height * 0.5f;
    
    glColor3f(darkOrange[0], darkOrange[1], darkOrange[2]);
    glBegin(GL_QUADS);
    // Bagian atas - Atas
    glVertex3f(-topWidth/2, height + topHeight, -depth/2);
    glVertex3f(-topWidth/2, height + topHeight, depth/2);
    glVertex3f(topWidth/2, height + topHeight, depth/2);
    glVertex3f(topWidth/2, height + topHeight, -depth/2);
    
    // Bagian atas - Depan (miring)
    glVertex3f(-width/2, height, depth/2);
    glVertex3f(-topWidth/2, height + topHeight, depth/2);
    glVertex3f(topWidth/2, height + topHeight, depth/2);
    glVertex3f(width/2, height, depth/2);
    
    // Bagian atas - Belakang (miring)
    glVertex3f(-width/2, height, -depth/2);
    glVertex3f(width/2, height, -depth/2);
    glVertex3f(topWidth/2, height + topHeight, -depth/2);
    glVertex3f(-topWidth/2, height + topHeight, -depth/2);
    
    // Bagian atas - Sisi kiri (miring)
    glVertex3f(-width/2, height, -depth/2);
    glVertex3f(-topWidth/2, height + topHeight, -depth/2);
    glVertex3f(-topWidth/2, height + topHeight, depth/2);
    glVertex3f(-width/2, height, depth/2);
    
    // Bagian atas - Sisi kanan (miring)
    glVertex3f(width/2, height, -depth/2);
    glVertex3f(width/2, height, depth/2);
    glVertex3f(topWidth/2, height + topHeight, depth/2);
    glVertex3f(topWidth/2, height + topHeight, -depth/2);
    glEnd();
      // Membuat reflector strip hitam dan kuning depan 
    float stripWidth = topWidth * 0.7f;
    float stripHeight = topHeight * 0.5f;
    float stripThickness = 0.05f;
    float stripY = height + topHeight/2;
    float stripZ = depth/2 + stripThickness/2;
    
    glPushMatrix();
    glTranslatef(0.0f, stripY, stripZ);
    
    // Background untuk strip
    glColor3f(blackStripe[0], blackStripe[1], blackStripe[2]);
    glBegin(GL_QUADS);
    glVertex3f(-stripWidth/2, -stripHeight/2, 0.0f);
    glVertex3f(-stripWidth/2, stripHeight/2, 0.0f);
    glVertex3f(stripWidth/2, stripHeight/2, 0.0f);
    glVertex3f(stripWidth/2, -stripHeight/2, 0.0f);
    glEnd();      
    int numStripes = 7; 
    float stripeWidth = stripWidth / numStripes;
    
    glBegin(GL_QUADS);
    for (int i = 0; i < numStripes; i++) {
        if (i % 2 == 0) {
            glColor3f(yellowStripe[0], yellowStripe[1], yellowStripe[2]); 
        } else {
            glColor3f(blackStripe[0], blackStripe[1], blackStripe[2]); 
        }
        
        float xLeft = -stripWidth/2 + i * stripeWidth;
        float xRight = xLeft + stripeWidth;
        
        // Gambar strip vertikal
        glVertex3f(xLeft, -stripHeight/2, stripThickness/2);
        glVertex3f(xLeft, stripHeight/2, stripThickness/2);
        glVertex3f(xRight, stripHeight/2, stripThickness/2);
        glVertex3f(xRight, -stripHeight/2, stripThickness/2);
    }
    
    glEnd();
    glPopMatrix();
    
    glPopMatrix();
}

// Mendapatkan posisi X untuk ruas jalan (lane) tertentu
float getLanePosition(int lane) {
    // Offset dari pinggir kiri jalan
    float leftEdge = -roadWidth / 2;
    
    // Hitung posisi X tengah dari jalur yang diminta
    float laneCenter = leftEdge + (lane + 0.5f) * laneWidth;
    
    return laneCenter;
}

// Fungsi untuk inisialisasi barrier
void initBarriers() {
    // Inisialisasi random seed
    srand(time(NULL));
    
    // Distribusikan barrier di sepanjang jalan yang sudah ada
    float minDistance = 45.0f; // Minimal jarak antar barrier
    float barrierStartZ = visibleRoadStartZ - 35.0f; // Mulai barrier setelah titik awal jalan
    
    // Reset semua barriers
    for (int i = 0; i < MAX_BARRIERS; i++) {
        barriers[i].active = false;
    }
    
    // Posisi barrier awal yang digeser agar pemain dapat memulai dengan aman
    float currentZ = barrierStartZ;
    
    for (int i = 0; i < MAX_BARRIERS; i++) {
        // Menggeser Z ke posisi acak yang lebih dalam
        currentZ -= minDistance + (rand() % 10);
        
        // Pastikan barrier hanya ada di area jalan yang aktif
        if (currentZ < visibleRoadEndZ + 30.0f) {
            break;
        }
        
        barriers[i].z = currentZ;
        
        // Tentukan di ruas jalan mana barrier akan ditempatkan
        barriers[i].lane = rand() % NUM_LANES; // 0: kiri, 1: kanan
        barriers[i].x = getLanePosition(barriers[i].lane);
        
        // Scale lebih besar agar barrier terlihat lebih jelas
        barriers[i].scale = 0.25f + (rand() % 15) / 100.0f;
        barriers[i].active = true;
    }
}

// Fungsi untuk mengupdate barrier
void updateBarriers() {
    // Update posisi barrier dan daur ulang jika sudah terlewati
    for (int i = 0; i < MAX_BARRIERS; i++) {
        if (barriers[i].active) {
            // Cek tabrakan dengan gerobak
            float barrierWidth = 2.5f * barriers[i].scale; // Lebar barrier asli * scale (sedikit diperkecil)
            float barrierDepth = 1.0f * barriers[i].scale; // Kedalaman barrier asli * scale
            float cartWidth = 1.0f; // Lebar gerobak (perkiraan)
              // Position barriers on flat road
            float barrierY = groundLevel + 0.05f;
            
            // Cek tabrakan X-Z sederhana
            if (fabs(barriers[i].z - carZ) < barrierDepth/2 + 0.3f && 
                fabs(barriers[i].x - carX) < barrierWidth/2 + cartWidth/2) {
                
                // Kalau tabrakan, gerobak diperlambat signifikan
                carSpeed *= 0.7f; // Kurangi kecepatan 30%
                score -= 5000; // Penalti skor
                
                // Status saat menabrak untuk ditampilkan
                showCollisionMessage = true;
                collisionTime = clock();
                
                // Pindahkan barrier ke posisi baru yang belum dilewati gerobak
                repositionBarrier(i);
            }
            
            // Jika barrier sudah terlewati jauh
            if (barriers[i].z > carZ + 50.0f) {
                // Repositioning barrier di area yang belum dilewati gerobak
                repositionBarrier(i);
            }
            
            // Jika barrier terlalu jauh di depan dari jalan yang tersedia
            if (barriers[i].z < visibleRoadEndZ - 20.0f) {
                // Nonaktifkan barrier yang sudah di luar jalan
                barriers[i].active = false;
            }
        } else {
            // Coba aktivasi barrier baru jika yang lama sudah tidak aktif
            // tapi jangan terlalu sering (misalnya dalam jarak yang cukup jauh)
			if ((rand() % 100) < 3) {  // Ubah dari 5% menjadi 3%
			    float newZ = carZ - 100.0f - (rand() % 40);
			    
			    // Pastikan barrier masih dalam batas jalan yang tersedia
			    if (newZ > visibleRoadEndZ) {
			        barriers[i].z = newZ;
			        
			        // Posisi X berdasarkan ruas jalan (2 jalur)
			        barriers[i].lane = rand() % NUM_LANES;
			        barriers[i].x = getLanePosition(barriers[i].lane);
			        
			        // Scale acak baru 
			        barriers[i].scale = 0.25f + (rand() % 15) / 100.0f;
			        barriers[i].active = true;
			    }
			}
        }
    }
}

// Helper function untuk memposisikan ulang barrier
void repositionBarrier(int index) {
    if (index < 0 || index >= MAX_BARRIERS) return;
    
    // UBAH NILAI INI: Tingkatkan jarak minimal dan maksimal dari gerobak
    float minDistanceFromCar = 120.0f;
    float maxDistanceFromCar = 220.0f;
    
    // Posisi baru jauh di depan gerobak
    float newZ = carZ - minDistanceFromCar - (rand() % (int)(maxDistanceFromCar - minDistanceFromCar));
    
    // Pastikan barrier masih dalam batas jalan
    if (newZ < visibleRoadEndZ + 20.0f) {
        // Jika jalan tidak cukup panjang, nonaktifkan barrier
        barriers[index].active = false;
        return;
    }
    
    barriers[index].z = newZ;
    
    // Posisi X berdasarkan ruas jalan (2 jalur)
    barriers[index].lane = rand() % NUM_LANES; // 0: kiri, 1: kanan
    barriers[index].x = getLanePosition(barriers[index].lane);
    
    // Scale acak baru 
    barriers[index].scale = 0.25f + (rand() % 15) / 100.0f;
    barriers[index].active = true;
}

// Fungsi untuk menggambar semua barrier
void drawBarriers() {
    for (int i = 0; i < MAX_BARRIERS; i++) {
        if (barriers[i].active) {            // Use flat road height
            float barrierY = groundLevel + 0.05f;
            barrier(barriers[i].x, barrierY, barriers[i].z, barriers[i].scale);
        }
    }
}

// Fungsi untuk menggambar pohon dan rumah
void drawTrees() {
    // Parameter pohon dengan jarak yang diperbesar
    float treeOffset = roadWidth/2 + 3.5f;  // Jarak dari tepi jalan, lebih jauh dari pagar
    float treeSpacing = 40.0f;              // Jarak antar pohon diperbesar
    float houseOffset = 8.0f;               // UBAH: Lebih dekat ke jalan (dari 14.0f)
    
    // Gunakan ketinggian konstan untuk mencegah pohon bergerak naik turun
    float treeGroundY = -0.2f;
    float houseGroundY = -0.2f;
    
    // Iterasi melalui setiap segmen jalan aktif, sama seperti pagar
    for (int segIdx = 0; segIdx < NUM_ROAD_SEGMENTS; segIdx++) {
        if (!roadSegments[segIdx].active) continue;
        
        // Gunakan segmen jalan yang sama seperti digunakan untuk pagar
        float segmentZ1 = roadSegments[segIdx].startZ;
        float segmentZ2 = roadSegments[segIdx].endZ;
        float segmentLength = segmentZ1 - segmentZ2;
        
        // Hitung berapa pohon bisa ditampilkan di segmen ini
        int treesPerSegment = (int)(segmentLength / treeSpacing) + 1;
        
        // Iterasi untuk kedua sisi jalan
        for (int side = -1; side <= 1; side += 2) {
            if (side == 0) continue; // Lewati bagian tengah
            
            // Iterasi pohon dalam segmen ini
            for (int i = 0; i < treesPerSegment; i++) {
                // Posisi Z absolut untuk setiap pohon dalam segmen
                float treeZ = segmentZ1 - i * treeSpacing - treeSpacing/2;
                
                // Cek apakah masih dalam segmen ini
                if (treeZ < segmentZ2) continue;
                
                // Generate ID unik untuk pohon ini berdasarkan posisi absolutnya
                int treeId = (int)(treeZ * 100.0f) + (side < 0 ? 100000 : 200000);
                
                // Gunakan treeId untuk konsistensi acak
                float hashValue1 = sin(treeId * 0.3567f) * 0.5f + 0.5f;
                float hashValue2 = sin(treeId * 0.4321f) * 0.5f + 0.5f;
                float hashValue3 = sin(treeId * 0.7123f) * 0.5f + 0.5f;
                
                // Variasi posisi X dengan jarak dari jalan yang konsisten
                float treeXOffset = hashValue1 * 1.2f;
                float treeX = side * (treeOffset + treeXOffset);
                
                // Variasi ukuran pohon
                float trunkHeight = 2.5f + hashValue2 * 1.0f;
                float trunkWidth = 0.25f + hashValue3 * 0.15f;
                float foliageSize = 1.0f + hashValue2 * 0.4f;
                
                // Variasi warna batang
                float trunkR = 0.35f + hashValue1 * 0.1f;
                float trunkG = 0.2f + hashValue3 * 0.05f;
                float trunkB = 0.1f + hashValue2 * 0.05f;
                
                // Variasi warna daun
                float foliageR = 0.1f + hashValue3 * 0.05f;
                float foliageG = 0.45f + hashValue1 * 0.15f;
                float foliageB = 0.1f + hashValue2 * 0.05f;

                // Render batang pohon
                glEnable(GL_LIGHTING);
                glColor3f(trunkR, trunkG, trunkB);
                glPushMatrix();
                    glTranslatef(treeX, treeGroundY + trunkHeight/2, treeZ);
                    glScalef(trunkWidth, trunkHeight, trunkWidth);
                    glutSolidCube(1.0f);
                glPopMatrix();

                // Render daun (3 lapisan)
                glColor3f(foliageR, foliageG, foliageB);
                glPushMatrix();
                    glTranslatef(treeX, treeGroundY + trunkHeight*0.7f, treeZ);
                    glScalef(foliageSize * 1.3f, foliageSize * 0.9f, foliageSize * 1.3f);
                    glutSolidSphere(1.0f, 8, 6);
                glPopMatrix();
                
                // Lapisan daun tengah
                glColor3f(foliageR + 0.05f, foliageG + 0.05f, foliageB);
                glPushMatrix();
                    glTranslatef(treeX, treeGroundY + trunkHeight*0.85f, treeZ);
                    glScalef(foliageSize * 1.1f, foliageSize * 0.8f, foliageSize * 1.1f);
                    glutSolidSphere(1.0f, 8, 6);
                glPopMatrix();
                
                // Lapisan daun atas
                glColor3f(foliageR + 0.08f, foliageG + 0.08f, foliageB);
                glPushMatrix();
                    glTranslatef(treeX, treeGroundY + trunkHeight, treeZ);
                    glScalef(foliageSize * 0.9f, foliageSize * 0.7f, foliageSize * 0.9f);
                    glutSolidSphere(1.0f, 8, 6);
                glPopMatrix();
                
                // Saat menambahkan rumah, ubah skalanya dan kondisi penempatannya:
			    if (i % 2 == 0) {  // Setiap pohon kedua
			        if (side > 0) {  // Sisi kanan jalan
			            // Rumah dengan jarak yang lebih dekat
			            float houseZ = treeZ - 12.0f;  // UBAH: Letakkan secara eksplisit, tidak di tengah pohon
			            float houseX = side * houseOffset;
			            
			            // TAMBAHKAN: Debug output
			            std::cout << "Placing right house at X: " << houseX << " Z: " << houseZ << std::endl;
			            
			            glPushMatrix();
			                // Sesuaikan skala dan rotasi rumah
			                glTranslatef(houseX, houseGroundY, houseZ);
			                glScalef(0.4f, 0.4f, 0.4f);  // UBAH: Perbesar skala (dari 0.2f)
			                glRotatef(-90, 0, 1, 0);
			                // Pastikan lighting diatur dengan benar untuk rumah
			                GLfloat house_ambient[] = {0.6f, 0.6f, 0.6f, 1.0f};
			                glMaterialfv(GL_FRONT, GL_AMBIENT, house_ambient);
			                rumah();
			            glPopMatrix();
			        }
			        else if (i % 4 == 0) {  // Sisi kiri, lebih jarang
			            // Rumah sisi kiri
			            float houseZ = treeZ - 8.0f;  // UBAH: Posisi eksplisit
			            float houseX = side * (houseOffset + 1.0f);  // UBAH: Lebih dekat (dari 2.0f offset tambahan)
			            
			            glPushMatrix();
			                glTranslatef(houseX, houseGroundY, houseZ);
			                glScalef(0.45f, 0.45f, 0.45f);  // UBAH: Perbesar skala (dari 0.25f)
			                glRotatef(90, 0, 1, 0);
			                GLfloat house_ambient[] = {0.6f, 0.6f, 0.6f, 1.0f};
			                glMaterialfv(GL_FRONT, GL_AMBIENT, house_ambient);
			                rumah();
			            glPopMatrix();
			        }
			    }
            }
        }
    }
}

void drawScore() {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, 800, 0, 600, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);

    // Draw score
    std::stringstream ss;
    ss << "Score: " << score;
    std::string scoreText = ss.str();
    renderText(650, 570, scoreText, GLUT_BITMAP_HELVETICA_18);    // Tampilkan info debug untuk membantu melihat status pergerakan
    std::stringstream posInfo;
    posInfo << std::fixed << std::setprecision(3) 
            << "Speed: " << carSpeed
            << " (Gas: " << (specialKeys[GLUT_KEY_UP] ? "ON" : "OFF") << ")"
            << " (Brake: " << (specialKeys[GLUT_KEY_DOWN] ? "ON" : "OFF") << ")"
            << " (Friction: " << (!specialKeys[GLUT_KEY_UP] ? "ON" : "OFF") << ")";
    renderText(30, 540, posInfo.str(), GLUT_BITMAP_HELVETICA_12);
    
    // Tampilkan pesan tabrakan jika aktif
    if (showCollisionMessage) {
        clock_t currentTime = clock();
        if ((currentTime - collisionTime) < COLLISION_MESSAGE_DURATION) {
            glColor3f(1.0f, 0.0f, 0.0f); // Merah untuk peringatan
            renderText(300, 300, "CRASH! -5000 Points", GLUT_BITMAP_HELVETICA_18);
        } else {
            showCollisionMessage = false;
        }
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

void drawSpeedometer() {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, 800, 0, 600, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);

    glColor4f(0.1f, 0.1f, 0.1f, 0.75f);
    glBegin(GL_QUADS);
        glVertex2f(20, 20);
        glVertex2f(220, 20);
        glVertex2f(220, 80);
        glVertex2f(20, 80);
    glEnd();

    float speedRatio = fabs(carSpeed) / maxSpeed;
    if (speedRatio > 1.0f) speedRatio = 1.0f;
    if (speedRatio < 0.0f) speedRatio = 0.0f;    
    glColor3f(speedRatio, 1.0f - speedRatio, 0.0f);
    float barWidth = 180;
    glBegin(GL_QUADS);
        glVertex2f(30, 30);
        glVertex2f(30 + speedRatio * barWidth, 30);
        glVertex2f(30 + speedRatio * barWidth, 50);
        glVertex2f(30, 50);
    glEnd();

    std::stringstream ss;
    ss << "Speed: " << std::fixed << std::setprecision(0) << (carSpeed / maxSpeed * 100) << "%";
    std::string speedText = ss.str();
    renderText(30, 60, speedText, GLUT_BITMAP_HELVETICA_12);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

void updateCarPhysics() {
    // Jarak dari titik awal jalan
    float distanceFromStart = roadStartZ - carZ;
    
    // Tambahkan akselerasi jika tombol gas ditekan (tanpa batasan jalan)
    if (specialKeys[GLUT_KEY_UP] && carSpeed > 0.001f) {
        // Gunakan akselerasi konstan tanpa pengaruh kemiringan
        float currentGravity = 0.003f; // Fixed acceleration

        // Update kecepatan dan batasi oleh max speed
        if (carSpeed < maxSpeed) {
            carSpeed += currentGravity;
        }
    }
      // Hambatan alami (gesekan) akan mengurangi kecepatan jika tidak ada input gas
    if (!specialKeys[GLUT_KEY_UP]) {
        // Semakin cepat, semakin tinggi gesekannya (pengereman lebih bertahap)
        float frictionFactor;
        
        if (carSpeed > 0.3f) {
            frictionFactor = 0.99f; // Pengurangan lambat untuk kecepatan tinggi
        } else if (carSpeed > 0.1f) {
            frictionFactor = 0.985f; // Pengurangan sedang untuk kecepatan sedang
        } else {
            frictionFactor = 0.97f; // Pengurangan cepat untuk kecepatan rendah
        }
        
        carSpeed *= frictionFactor;
        
        // Jika kecepatan sangat rendah, jadikan nol
        if (carSpeed < 0.001f) {
            carSpeed = 0.0f; 
        }
    }
    
    // Batasi kecepatan
    if(carSpeed > maxSpeed) carSpeed = maxSpeed;
    if(carSpeed < 0.0f) carSpeed = 0.0f;

    // Update Z (gerobak bergerak ke depan)
    carZ -= carSpeed;
    
    // Update Y (flat surface, constant height)
    carY = groundLevel;

    // Update carX menuju targetCarX untuk animasi smooth steering
    float steerLerpFactor = steeringSpeed; // Faktor dasar interpolasi
    // Buat belokan lebih responsif pada kecepatan rendah, dan lebih halus pada kecepatan tinggi
    if (carSpeed > 0.01f) { // Hindari pembagian dengan nol
        steerLerpFactor *= (1.0f - (carSpeed / (maxSpeed * 1.5f))) + 0.1f;
        if (steerLerpFactor < 0.02f) steerLerpFactor = 0.02f; // Batas minimal responsivitas
    }

    // Smooth interpolation untuk steering
    if (fabs(carX - targetCarX) > 0.005f) {
        carX += (targetCarX - carX) * steerLerpFactor;
    } else {
        carX = targetCarX;
    }

    // Update skor berdasarkan jarak dan kecepatan
    if ((lastScoreUpdateZ - carZ) >= 1.0f) {
        score += 10 * (int)((carSpeed / maxSpeed * 5) + 1);
        lastScoreUpdateZ = carZ;
    }

    // Update animasi kemiringan saat belok (tilt angle)
    float tiltDifference = targetCarX - carX;
    float maxTilt = 20.0f; // Maksimum kemiringan dalam derajat
    float tiltSensitivity = 60.0f; // Seberapa cepat mobil miring

    // Miringkan berdasarkan steering input
    if (fabs(tiltDifference) > 0.01f) {
        carTiltAngle = -tiltDifference * tiltSensitivity;
    } else {
        // Perlahan kembalikan ke posisi normal (damping effect)
        carTiltAngle *= 0.85f;
        if (fabs(carTiltAngle) < 0.1f) {
            carTiltAngle = 0.0f;
        }
    }
    
    // Batasi kemiringan maksimum
    if (carTiltAngle > maxTilt) carTiltAngle = maxTilt;
    if (carTiltAngle < -maxTilt) carTiltAngle = -maxTilt;
    
    // Update rotasi roda berdasarkan kecepatan
    wheelRotation -= carSpeed * 15.0f; // Faktor 15 untuk membuat rotasi terlihat jelas
    if (wheelRotation < -360.0f) {
        wheelRotation += 360.0f; // Reset rotasi setelah satu putaran penuh
    }
}

// Metode untuk membantu gerobak pindah ke ruas jalan target
void moveToLane(int targetLane) {
    // Batasi nilai targetLane valid
    if (targetLane < 0) targetLane = 0;
    if (targetLane >= NUM_LANES) targetLane = NUM_LANES - 1;
    
    // Hitung posisi X tengah dari jalur yang diminta
    targetCarX = getLanePosition(targetLane);
}

void handleInput() {
    float roadHalfWidth = roadWidth / 2 - 0.2f;
    float steeringAmount = 0.08f; // Sedikit berkurang agar tidak terlalu tajam

    // Belok kiri/kanan hanya responsif jika gerobak bergerak
    float speedFactor = carSpeed > 0.01f ? 1.0f : 0.3f; 
    
    // Ambil indeks jalur saat ini
    int currentLane = 0;
    if (carX > 0) {
        currentLane = 1; // Untuk 2 ruas jalan (0: kiri, 1: kanan)
    }
    
    // Pindah jalur saat spesifik key ditekan
    static bool leftKeyPressed = false;
    static bool rightKeyPressed = false;
    
    if (specialKeys[GLUT_KEY_LEFT]) {
        if (!leftKeyPressed && currentLane > 0) {
            // Pindah ke jalur kiri
            moveToLane(currentLane - 1);
            leftKeyPressed = true;
        }
    } else {
        leftKeyPressed = false;
    }
    
    if (specialKeys[GLUT_KEY_RIGHT]) {
        if (!rightKeyPressed && currentLane < NUM_LANES - 1) {
            // Pindah ke jalur kanan
            moveToLane(currentLane + 1);
            rightKeyPressed = true;
        }
    } else {
        rightKeyPressed = false;
    }

    // Batasi gerobak agar tidak keluar jalan
    if(targetCarX < -roadHalfWidth) {
        targetCarX = -roadHalfWidth;
        // Kurangi kecepatan jika mencoba keluar jalur
        carSpeed *= 0.95f;
    }
    if(targetCarX > roadHalfWidth) {
        targetCarX = roadHalfWidth;
        // Kurangi kecepatan jika mencoba keluar jalur
        carSpeed *= 0.95f;
    }

    // Percepatan lebih halus dan responsif
    if(specialKeys[GLUT_KEY_UP]) {
        if (carSpeed < maxSpeed) {
             carSpeed += 0.006f; // Lebih lambat untuk kontrol yang lebih halus
        }
        if (carSpeed > maxSpeed) carSpeed = maxSpeed;
    }
    
    // Pengereman yang lebih bertahap berdasarkan kecepatan saat ini
    if(specialKeys[GLUT_KEY_DOWN]) {
        // Semakin cepat, semakin kuat efek pengereman
        float brakeFactor;
        
        if (carSpeed > 0.3f) {
            brakeFactor = 0.03f; // Pengereman kuat untuk kecepatan tinggi
        } else if (carSpeed > 0.1f) {
            brakeFactor = 0.02f; // Pengereman normal untuk kecepatan sedang
        } else {
            brakeFactor = 0.01f; // Pengereman lembut untuk kecepatan rendah
        }
        
        carSpeed -= brakeFactor;
        if(carSpeed < 0.0f) carSpeed = 0.0f;
    }
}

// Fungsi untuk inisialisasi segmen jalan
void initRoadSegments() {
    float startZ = visibleRoadStartZ;
    
    // Inisialisasi semua segmen jalan
    for (int i = 0; i < NUM_ROAD_SEGMENTS; i++) {
        roadSegments[i].startZ = startZ;
        roadSegments[i].endZ = startZ - roadSegmentLength;
        roadSegments[i].active = true;
        startZ -= roadSegmentLength;
    }
    
    // Update batas jalan terlihat
    visibleRoadEndZ = roadSegments[NUM_ROAD_SEGMENTS - 1].endZ;
    
    // Pastikan batas jalan konsisten dengan posisi gerobak
    roadStartZ = visibleRoadStartZ;
    roadEndZ = visibleRoadEndZ;
}

// Fungsi untuk update dan recycle segmen jalan
void updateRoadSegments() {
    // Cari segmen jalan yang terjauh dari posisi gerobak (yang mungkin sudah terlewati)
    int farthestSegmentIndex = -1;
    float farthestZ = -99999.0f;
    
    // Cari segmen paling belakang (paling positif Z)
    for (int i = 0; i < NUM_ROAD_SEGMENTS; i++) {
        if (roadSegments[i].active && roadSegments[i].endZ > farthestZ) {
            farthestZ = roadSegments[i].endZ;
            farthestSegmentIndex = i;
        }
    }
    
    // Cari juga segmen terdepan (z terkecil)
    int frontalSegmentIndex = -1;
    float frontalZ = 99999.0f;
    
    for (int i = 0; i < NUM_ROAD_SEGMENTS; i++) {
        if (roadSegments[i].active && roadSegments[i].endZ < frontalZ) {
            frontalZ = roadSegments[i].endZ;
            frontalSegmentIndex = i;
        }
    }

    // Jika tidak ada segmen valid, ada masalah dengan inisialisasi
    if (farthestSegmentIndex == -1 || frontalSegmentIndex == -1) {
        return;
    }
    
    // Jika gerobak mendekati ujung jalan paling depan, buat segmen baru
    if (carZ - roadGenerationDistance < roadSegments[frontalSegmentIndex].endZ) {
        // Perpanjang jalan ke depan untuk simulasi tak terbatas
        const float ADDITIONAL_SEGMENTS = 4; // Increased number of segments to generate
        
        float lastEndZ = roadSegments[frontalSegmentIndex].endZ;
        
        // Recycling dari segmen terjauh di belakang jika ada yang sudah terlewati
        for (int i = 0; i < ADDITIONAL_SEGMENTS; i++) {
            // Cari segmen paling jauh di belakang pemain
            int recycleIndex = -1;
            float furthestBackZ = -99999.0f;
            
            for (int j = 0; j < NUM_ROAD_SEGMENTS; j++) {
                if (roadSegments[j].active && 
                    roadSegments[j].startZ > carZ + 60.0f && // Increased distance behind cart
                    roadSegments[j].startZ > furthestBackZ) {
                    furthestBackZ = roadSegments[j].startZ;
                    recycleIndex = j;
                }
            }
            
            // Jika ada segmen yang bisa di-recycle
            if (recycleIndex != -1) {
                // Pindahkan segmen terjauh ke depan
                roadSegments[recycleIndex].endZ = lastEndZ - roadSegmentLength;
                roadSegments[recycleIndex].startZ = lastEndZ;
                
                // Update lastEndZ untuk segmen berikutnya
                lastEndZ = roadSegments[recycleIndex].endZ;
                
                // Update batas jalan yang terlihat
                if (roadSegments[recycleIndex].endZ < visibleRoadEndZ) {
                    visibleRoadEndZ = roadSegments[recycleIndex].endZ;
                }
            }
        }
    }
    
    // Update posisi belakang jalan untuk menghindari rendering berlebihan
    visibleRoadStartZ = fmin(visibleRoadStartZ, carZ + 40.0f);
}

void glShadowProjection(float* l, float* e, float* n) {
    float d, c;
    float mat[16];

    d = n[0]*l[0] + n[1]*l[1] + n[2]*l[2];
    c = e[0]*n[0] + e[1]*n[1] + e[2]*n[2] - d;

    mat[0]  = l[0]*n[0]+c; 
    mat[4]  = n[1]*l[0];    
    mat[8]  = n[2]*l[0];    
    mat[12] = -l[0]*c-l[0]*d;

    mat[1]  = n[0]*l[1];    
    mat[5]  = l[1]*n[1]+c;  
    mat[9]  = n[2]*l[1];    
    mat[13] = -l[1]*c-l[1]*d;

    mat[2]  = n[0]*l[2];    
    mat[6]  = n[1]*l[2];    
    mat[10] = l[2]*n[2]+c;  
    mat[14] = -l[2]*c-l[2]*d;

    mat[3]  = n[0];
    mat[7]  = n[1];
    mat[11] = n[2];
    mat[15] = -d;

    glMultMatrixf(mat);
}

void render() {
    glClearColor(0.0, 0.6, 0.9, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLightfv(GL_LIGHT0, GL_POSITION, l);
    
    // Draw ground plane
    glDisable(GL_LIGHTING);
    glColor3f(0.8f, 0.8f, 0.8f);
    glBegin(GL_QUADS);
        glNormal3f(0.0, 1.0, 0.0);
        glVertex3f(-1300.0, e[1]-0.1, 1300.0);
        glVertex3f(1300.0, e[1]-0.1, 1300.0);
        glVertex3f(1300.0, e[1]-0.1, -1300.0);
        glVertex3f(-1300.0, e[1]-0.1, -1300.0);
    glEnd();

    // Draw actual objects
    glEnable(GL_LIGHTING);
    glPushMatrix();
        glRotatef(ry, 0, 1, 0);
        glRotatef(rx, 1, 0, 0);
        drawTrees();
        drawBarriers();
        drawGerobak();
    glPopMatrix();

    // Draw shadows
    glPushMatrix();
        glShadowProjection(l, e, n);
        glRotatef(ry, 0, 1, 0);
        glRotatef(rx, 1, 0, 0);
        glDisable(GL_LIGHTING);
        glColor3f(0.4, 0.4, 0.4);
        drawTrees();
        drawBarriers();
        drawGerobak();
    glPopMatrix();

    glutSwapBuffers();
}

void idle() {
    rx += 0.1f;
    ry += 0.1f;
    glutPostRedisplay();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    float cameraDistance = 7.0f;
    float cameraHeight = 3.5f;
    float lookAtYOffset = 0.8f;

    gluLookAt(carX, carY + cameraHeight, carZ + cameraDistance,
              carX, carY + lookAtYOffset, carZ,
              0.0f, 1.0f, 0.0f);
              
    // Draw dalam urutan yang benar untuk z-ordering
    drawGround();
    drawInfiniteRoad();
    drawClouds();  // TAMBAHKAN INI: Gambar awan di langit
    drawTrees();
    drawBarriers();

    glPushMatrix();
        glTranslatef(carX, carY + carYOffset, carZ);
        drawGerobak();
    glPopMatrix();

    // UI elements should be drawn last
    drawSpeedometer();
    drawScore();

    glutSwapBuffers();
}

void reshape(int w, int h) {
    if (h == 0) h = 1;
    float ratio = 1.0f * w / h;

    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(55.0, ratio, 0.1, 600.0); // FOV standar
    glMatrixMode(GL_MODELVIEW);
}

void resetGame() {
    carX = -1.5f; // Mulai di jalur kiri
    targetCarX = -1.5f;
    carZ = visibleRoadStartZ; // Reset ke awal jalan dengan sistem segmen baru
    carY = groundLevel; // Reset ke level ground
    carAngle = 0.0f;
    carTiltAngle = 0.0f; // Reset kemiringan
    carSpeed = 0.0f;     // Mulai dari diam, bukan initialCarSpeed
    score = 0;
    lastScoreUpdateZ = carZ;
    showCollisionMessage = false;
    
    // Ensure road bounds are far enough
    visibleRoadEndZ = -1000.0f; // Make sure the end of visible road is far away
    
    // Reinisialisasi road segments dan lebar ruas jalan
    initLaneWidth();
    initRoadSegments();
    
    // Inisialisasi barrier
    initBarriers();
}

void keyboard(unsigned char key, int x, int y) {
    keys[key] = true;

    switch(key) {
        case 'r': case 'R':
            resetGame();
            break;
        case 27: // ESC
            exit(0);
            break;
    }
}

void keyboardUp(unsigned char key, int x, int y) {
    keys[key] = false;
}

void specialKeyboard(int key, int x, int y) {
    specialKeys[key] = true;
}

void specialKeyboardUp(int key, int x, int y) {
    specialKeys[key] = false;
}

void timer(int value) {
    handleInput();
    updateCarPhysics();
    updateBarriers(); // Update posisi dan status barrier
    updateRoadSegments(); // Update segmen jalan
    updateClouds();

    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}

void printInstructions() {
    std::cout << "=== KONTROL GEROBAK TURUN LINTASAN ===" << std::endl;
    std::cout << "Panah Atas  : Gas / Dorong Maju" << std::endl;
    std::cout << "Panah Bawah : Rem" << std::endl;
    std::cout << "Panah Kiri  : Pindah ke Jalur Kiri" << std::endl;
    std::cout << "Panah Kanan : Pindah ke Jalur Kanan" << std::endl;
    std::cout << "R           : Reset Game" << std::endl;
    std::cout << "ESC         : Keluar" << std::endl;
    std::cout << "=======================================" << std::endl;
    std::cout << "Semakin jauh dan cepat, semakin tinggi skormu!" << std::endl;
    std::cout << "AWAS: Hindari barrier orange di jalan!" << std::endl;
    std::cout << "Menabrak barrier akan mengurangi skor -50 dan memperlambat gerobak." << std::endl;
    std::cout << "=======================================" << std::endl;
    std::cout << "Selamat Menikmati Permainan!" << std::endl;
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Gerobak Juara!");

    init();
    printInstructions();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardUp);
    glutSpecialFunc(specialKeyboard);
    glutSpecialUpFunc(specialKeyboardUp);
    glutTimerFunc(16, timer, 0);
    
    glutIdleFunc(idle); 
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHT0);
 
  glLoadIdentity(); 
  gluPerspective(60.0f, 1.0, 1.0, 400.0); 

    // Reset game & inisialisasi barrier
    resetGame();
    initRendering();
    initRoadSegments(); // Inisialisasi segmen jalan
    initClouds();

    glutMainLoop();
    return 0;
}

