// Nama : Bramantyo Kunni Nurrisqi
// NIM 	: 24060123130091
// Kelas: A

#include <GL/glut.h>
#include <math.h>

#define PI 3.1415926535898

// Fungsi untuk menggambar lingkaran
void DrawCircle(GLfloat x, GLfloat y, GLfloat radius, int circle_points) {
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y); // Titik pusat lingkaran
    for (int i = 0; i <= circle_points; i++) {
        float angle = 2.0f * PI * i / circle_points;
        glVertex2f(x + radius * cos(angle), y + radius * sin(angle));
    }
    glEnd();
}

void DrawCircleOutline(GLfloat x, GLfloat y, GLfloat radius, int circle_points) {
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i <= circle_points; i++) {
        float angle = 2.0f * PI * i / circle_points;
        glVertex2f(x + radius * cos(angle), y + radius * sin(angle));
    }
    glEnd();
}

// Fungsi untuk menggambar segi empat
void SegiEmpat(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2) {
    glBegin(GL_QUADS);
    glVertex2f(x1, y1);
    glVertex2f(x2, y1);
    glVertex2f(x2, y2);
    glVertex2f(x1, y2);
    glEnd();
}

void SegiEmpatOutline(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2) {
    glBegin(GL_LINE_STRIP);
    glVertex2f(x1, y1);
    glVertex2f(x2, y1);
    glVertex2f(x2, y2);
    glVertex2f(x1, y2);
    glEnd();
}

// Fungsi untuk menggambar segitiga
void SegiTiga(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat x3, GLfloat y3) {
    glBegin(GL_TRIANGLES); 
        glVertex2f(x1, y1);
        glVertex2f(x2, y2);
        glVertex2f(x3, y3);
    glEnd();
}

// Fungsi menggambar roda mobil
void wheels(void){
    glColor3f(0.0f, 0.0f, 0.0f);
    DrawCircle(0.0f, 0.0f, 0.2f, 20); // Ban hitam 

    glColor3f(1.0f, 1.0f, 1.0f);
    DrawCircle(0.0f, 0.0f, 0.15f, 20); // Velg putih 

    glColor3f(0.4f, 0.4f, 0.4f);
    DrawCircle(0.0f, 0.0f, 0.12f, 20); // Velg abu-abu 

    glColor3f(0.8f, 0.8f, 0.8f);
    DrawCircle(0.0f, 0.0f, 0.08f, 20); // Bagian tengah velg 

    glPushMatrix();
    glTranslatef(0.065f, 0.065f, 0.0f);
    glRotated(-45.0f, 0.0f, 0.0f, 1.0f);
    glColor3f(0.0f, 0.0f, 0.0f);
    SegiEmpat(-0.05f, 0.025f, 0.05f, -0.025); // Jari-jari velg 
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, 0.1f, 0.0f);
    glRotated(90.0f, 0.0f, 0.0f, 1.0f);
    glColor3f(0.8f, 0.8f, 0.8f);
    SegiEmpat(-0.04f, 0.02f, 0.04f, -0.02); // Jari-jari velg 
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0.1f, 0.04f, 0.0f);
    glRotated(25.0f, 0.0f, 0.0f, 1.0f);
    glColor3f(0.8f, 0.8f, 0.8f);
    SegiEmpat(-0.045f, 0.02f, 0.04f, -0.02); // Jari-jari velg 
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(-0.1f, 0.04f, 0.0f);
    glRotated(-25.0f, 0.0f, 0.0f, 1.0f);
    glColor3f(0.8f, 0.8f, 0.8f);
    SegiEmpat(-0.04f, 0.02f, 0.045f, -0.02); // Jari-jari velg 
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0.07f, -0.06f, 0.0f);
    glRotated(315.0f, 0.0f, 0.0f, 1.0f);
    glColor3f(0.8f, 0.8f, 0.8f);
    SegiEmpat(-0.03f, 0.02f, 0.05f, -0.02); // Jari-jari velg 
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(-0.07f, -0.06f, 0.0f);
    glRotated(-315.0f, 0.0f, 0.0f, 1.0f);
    glColor3f(0.8f, 0.8f, 0.8f);
    SegiEmpat(-0.05f, 0.02f, 0.03f, -0.02); 
    glPopMatrix();
    
    glColor3f(0.2f, 0.2f, 0.2f);
    DrawCircle(0.0f, 0.0f, 0.05f, 20);
}

void RenderScene(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    
    glPushMatrix();
        glColor3f(0.52f, 0.80f, 0.92f);
        glRectf(-5.0, 0.0, 5.0, 1.0); // Langit
    glPopMatrix();
    
    glPushMatrix();
	    glTranslatef(0.0f, -0.5f, 0.0f);
	    glColor3f(0.6f, 0.6f, 0.6f);
	    glRectf(-5.0, 0.0, 5.0, 0.5); // Jalan raya
	glPopMatrix();
	
	glPushMatrix(); // Gedung
	    // Lantai pertama
	    glTranslatef(0.5f, 0.5f, 0.0f);
	    glColor3f(1.0f, 1.0f, 1.0f); 
	    SegiEmpat(-0.4f, -0.5f, 0.4f, 0.5f); 
	    
	    // Lantai kedua
	    glTranslatef(0.0f, 0.5f, 0.0f); 
	    glColor3f(0.8f, 0.8f, 0.8f); 
	    SegiEmpat(-0.4f, -0.5f, 0.4f, 0.5f); 
	    
	    // Jendela
	    glTranslatef(-0.2f, -0.7f, 0.0f); 
        glColor4f(0.7f, 0.9f, 1.0f, 0.5f);
	    SegiEmpat(-0.1f, -0.1f, 0.1f, 0.1f); 
	    
	    // Jendela
	    glTranslatef(-0.0f, 0.0f, 0.0f); 
        glColor3f(0.0f, 0.0f, 0.0f);
	    SegiEmpatOutline(-0.1f, -0.1f, 0.1f, 0.1f);
	    
	    // Jendela
	    glTranslatef(0.4f, 0.0f, 0.0f); 
        glColor4f(0.7f, 0.9f, 1.0f, 0.5f);
	    SegiEmpat(-0.1f, -0.1f, 0.1f, 0.1f); 
	    
	    // Jendela
	    glTranslatef(0.0f, 0.0f, 0.0f); 
        glColor3f(0.0f, 0.0f, 0.0f);
	    SegiEmpatOutline(-0.1f, -0.1f, 0.1f, 0.1f);
	    
	    // Jendela
	    glTranslatef(0.0f, 0.5f, 0.0f); 
        glColor4f(0.7f, 0.9f, 1.0f, 0.5f);
	    SegiEmpat(-0.1f, -0.1f, 0.1f, 0.1f); 
	    
	    // Jendela
	    glTranslatef(0.0f, 0.0f, 0.0f); 
        glColor3f(0.0f, 0.0f, 0.0f);
	    SegiEmpatOutline(-0.1f, -0.1f, 0.1f, 0.1f);
	    
	    // Jendela
	    glTranslatef(-0.4f, 0.0f, 0.0f); 
        glColor4f(0.7f, 0.9f, 1.0f, 0.5f);
	    SegiEmpat(-0.1f, -0.1f, 0.1f, 0.1f); 
	    
	    // Jendela
	    glTranslatef(0.0f, 0.0f, 0.0f); 
        glColor3f(0.0f, 0.0f, 0.0f);
	    SegiEmpatOutline(-0.1f, -0.1f, 0.1f, 0.1f);
	    
	    glColor3f(1.0f, 1.0f, 0.8f);
        glTranslatef(0.2f, 0.2f, 0.0f); 
        SegiTiga(-0.4f, -0.5f, 0.4f, -0.5f, 0.0f, -0.3f);
    glPopMatrix();
	
	// Rumput di Jalan
	glPushMatrix();
	    glColor3f(0.0f, 1.0f, 0.0f);
        glTranslatef(-1.0f, -0.01f, 0.0f); 
        glScalef(0.3f, 0.3f, 1.0f);
        SegiTiga(0.0f, 0.0f, 0.5f, 0.0f, 0.25f, 0.5f);
    glPopMatrix();   
	glPushMatrix();
	    glColor3f(0.0f, 1.0f, 0.0f);
        glTranslatef(-0.90f, -0.01f, 0.0f); 
        glScalef(0.3f, 0.3f, 1.0f);
        SegiTiga(0.0f, 0.0f, 0.5f, 0.0f, 0.25f, 0.5f);
    glPopMatrix();  
	glPushMatrix();
	    glColor3f(0.0f, 1.0f, 0.0f);
        glTranslatef(-0.80f, -0.01f, 0.0f); 
        glScalef(0.3f, 0.3f, 1.0f);
        SegiTiga(0.0f, 0.0f, 0.5f, 0.0f, 0.25f, 0.5f);
    glPopMatrix();  
    glPushMatrix();
	    glColor3f(0.0f, 1.0f, 0.0f);
        glTranslatef(-0.70f, -0.01f, 0.0f); 
        glScalef(0.3f, 0.3f, 1.0f);
        SegiTiga(0.0f, 0.0f, 0.5f, 0.0f, 0.25f, 0.5f);
    glPopMatrix();
    glPushMatrix();
	    glColor3f(0.0f, 1.0f, 0.0f);
        glTranslatef(-0.60f, -0.01f, 0.0f); 
        glScalef(0.3f, 0.3f, 1.0f);
        SegiTiga(0.0f, 0.0f, 0.5f, 0.0f, 0.25f, 0.5f);
    glPopMatrix();
    glPushMatrix();
	    glColor3f(0.0f, 1.0f, 0.0f);
        glTranslatef(-0.50f, -0.01f, 0.0f); 
        glScalef(0.3f, 0.3f, 1.0f);
        SegiTiga(0.0f, 0.0f, 0.5f, 0.0f, 0.25f, 0.5f);
    glPopMatrix();
    glPushMatrix();
	    glColor3f(0.0f, 1.0f, 0.0f);
        glTranslatef(-0.40f, -0.01f, 0.0f); 
        glScalef(0.3f, 0.3f, 1.0f);
        SegiTiga(0.0f, 0.0f, 0.5f, 0.0f, 0.25f, 0.5f);
    glPopMatrix();
    glPushMatrix();
	    glColor3f(0.0f, 1.0f, 0.0f);
        glTranslatef(-0.30f, -0.01f, 0.0f); 
        glScalef(0.3f, 0.3f, 1.0f);
        SegiTiga(0.0f, 0.0f, 0.5f, 0.0f, 0.25f, 0.5f);
    glPopMatrix();
    glPushMatrix();
	    glColor3f(0.0f, 1.0f, 0.0f);
        glTranslatef(-0.20f, -0.01f, 0.0f); 
        glScalef(0.3f, 0.3f, 1.0f);
        SegiTiga(0.0f, 0.0f, 0.5f, 0.0f, 0.25f, 0.5f);
    glPopMatrix();
    glPushMatrix();
	    glColor3f(0.0f, 1.0f, 0.0f);
        glTranslatef(-0.10f, -0.01f, 0.0f); 
        glScalef(0.3f, 0.3f, 1.0f);
        SegiTiga(0.0f, 0.0f, 0.5f, 0.0f, 0.25f, 0.5f);
    glPopMatrix();
	glPushMatrix();
	    glColor3f(0.0f, 1.0f, 0.0f);
        glTranslatef(0.0f, -0.01f, 0.0f); 
        glScalef(0.3f, 0.3f, 1.0f);
        SegiTiga(0.0f, 0.0f, 0.5f, 0.0f, 0.25f, 0.5f);
    glPopMatrix();  
    glPushMatrix();
	    glColor3f(0.0f, 1.0f, 0.0f);
        glTranslatef(1.0f, -0.01f, 0.0f); 
        glScalef(0.3f, 0.3f, 1.0f);
        SegiTiga(0.0f, 0.0f, 0.5f, 0.0f, 0.25f, 0.5f);
    glPopMatrix();   
	glPushMatrix();
	    glColor3f(0.0f, 1.0f, 0.0f);
        glTranslatef(0.90f, -0.01f, 0.0f); 
        glScalef(0.3f, 0.3f, 1.0f);
        SegiTiga(0.0f, 0.0f, 0.5f, 0.0f, 0.25f, 0.5f);
    glPopMatrix();  
	glPushMatrix();
	    glColor3f(0.0f, 1.0f, 0.0f);
        glTranslatef(0.80f, -0.01f, 0.0f); 
        glScalef(0.3f, 0.3f, 1.0f);
        SegiTiga(0.0f, 0.0f, 0.5f, 0.0f, 0.25f, 0.5f);
    glPopMatrix();  
    glPushMatrix();
	    glColor3f(0.0f, 1.0f, 0.0f);
        glTranslatef(0.70f, -0.01f, 0.0f); 
        glScalef(0.3f, 0.3f, 1.0f);
        SegiTiga(0.0f, 0.0f, 0.5f, 0.0f, 0.25f, 0.5f);
    glPopMatrix();
    glPushMatrix();
	    glColor3f(0.0f, 1.0f, 0.0f);
        glTranslatef(0.60f, -0.01f, 0.0f); 
        glScalef(0.3f, 0.3f, 1.0f);
        SegiTiga(0.0f, 0.0f, 0.5f, 0.0f, 0.25f, 0.5f);
    glPopMatrix();
    glPushMatrix();
	    glColor3f(0.0f, 1.0f, 0.0f);
        glTranslatef(0.50f, -0.01f, 0.0f); 
        glScalef(0.3f, 0.3f, 1.0f);
        SegiTiga(0.0f, 0.0f, 0.5f, 0.0f, 0.25f, 0.5f);
    glPopMatrix();
    glPushMatrix();
	    glColor3f(0.0f, 1.0f, 0.0f);
        glTranslatef(0.40f, -0.01f, 0.0f); 
        glScalef(0.3f, 0.3f, 1.0f);
        SegiTiga(0.0f, 0.0f, 0.5f, 0.0f, 0.25f, 0.5f);
    glPopMatrix();
    glPushMatrix();
	    glColor3f(0.0f, 1.0f, 0.0f);
        glTranslatef(0.30f, -0.01f, 0.0f); 
        glScalef(0.3f, 0.3f, 1.0f);
        SegiTiga(0.0f, 0.0f, 0.5f, 0.0f, 0.25f, 0.5f);
    glPopMatrix();
    glPushMatrix();
	    glColor3f(0.0f, 1.0f, 0.0f);
        glTranslatef(0.20f, -0.01f, 0.0f); 
        glScalef(0.3f, 0.3f, 1.0f);
        SegiTiga(0.0f, 0.0f, 0.5f, 0.0f, 0.25f, 0.5f);
    glPopMatrix();
    glPushMatrix();
	    glColor3f(0.0f, 1.0f, 0.0f);
        glTranslatef(0.10f, -0.01f, 0.0f); 
        glScalef(0.3f, 0.3f, 1.0f);
        SegiTiga(0.0f, 0.0f, 0.5f, 0.0f, 0.25f, 0.5f);
    glPopMatrix();
	
	// Strip jalan
	glPushMatrix();
	    glColor3f(1.0f, 1.0f, 1.0f);
	    glScalef(2.0f, 2.0f, 1.0f);
	    glLineWidth(30.0f); 
	    glBegin(GL_LINES); 
	        glVertex2f(-1.0f, -0.1f);
	        glVertex2f(-0.5f, -0.1f);
	        glVertex2f(0.0f, -0.1f);
	        glVertex2f(0.5f, -0.1f);
	        glVertex2f(0.5f, -0.1f);
	        glVertex2f(1.0f, -0.1f);
	    glEnd();
	    glLineWidth(1.0f); 
	glPopMatrix();

    	
    
    glPushMatrix();
        glColor3f(5.0f, 2.0f, 0.0f);
        glScalef(2.0f, 2.0f, 1.0f);
        glBegin(GL_POLYGON); // Badan mobil
        	glVertex2f(-0.47f,-0.13f);
        	glVertex2f(-0.46f,-0.07f);
        	glVertex2f(-0.41f,-0.03f);
        	glVertex2f(-0.34f,0.0f);
        	glVertex2f(-0.25f,0.01f);
        	glVertex2f(-0.07f,0.08f);
        	glVertex2f(0.09f,0.08f);
        	glVertex2f(0.21f,0.07f);
        	glVertex2f(0.33f,0.05f);
        	glVertex2f(0.44f,0.03f);
        	glVertex2f(0.45f,-0.01f);
        	glVertex2f(0.46f,-0.06f);
        	glVertex2f(0.46f,-0.1f);
        	glVertex2f(0.45f,-0.11f);
        	glVertex2f(0.44f,-0.12f);
        	glVertex2f(0.43f,-0.13f);
        glEnd();
    glPopMatrix();
    
    glPushMatrix();
        glColor3f(0.4f, 0.4f, 0.4f);
        glScalef(2.0f, 2.0f, 1.0f);
        glBegin(GL_POLYGON); // wadah roda
        	glVertex2f(-0.18f,-0.13f);
			glVertex2f(-0.18f,-0.08f);
			glVertex2f(-0.19f,-0.05f);
			glVertex2f(-0.21f,-0.03f);
			glVertex2f(-0.23f,-0.02f);
			glVertex2f(-0.25f,-0.02f);
			glVertex2f(-0.27f,-0.02f);
			glVertex2f(-0.29f,-0.02f);
			glVertex2f(-0.31f,-0.03f);
			glVertex2f(-0.33f,-0.05f);
			glVertex2f(-0.34f,-0.08f);
			glVertex2f(-0.34f,-0.13f);
        glEnd();
    glPopMatrix();
    
    glPushMatrix();
        glColor3f(0.4f, 0.4f, 0.4f);
        glTranslatef(1.17f, 0.0f, 0.0f);
        glScalef(2.0f, 2.0f, 1.0f);
        glBegin(GL_POLYGON); // wadah roda
        	glVertex2f(-0.18f,-0.13f);
			glVertex2f(-0.18f,-0.08f);
			glVertex2f(-0.19f,-0.05f);
			glVertex2f(-0.21f,-0.03f);
			glVertex2f(-0.23f,-0.02f);
			glVertex2f(-0.25f,-0.02f);
			glVertex2f(-0.27f,-0.02f);
			glVertex2f(-0.29f,-0.02f);
			glVertex2f(-0.31f,-0.03f);
			glVertex2f(-0.33f,-0.05f);
			glVertex2f(-0.34f,-0.08f);
			glVertex2f(-0.34f,-0.13f);
        glEnd();
    glPopMatrix();
    
    glPushMatrix();
        glColor3f(0.0f, 0.0f, 0.0f);
        glScalef(2.0f, 2.0f, 1.0f);
        glBegin(GL_POLYGON); // angin depan
        	glVertex2f(-0.45f,-0.08f);
        	glVertex2f(-0.46f,-0.09f);
        	glVertex2f(-0.45f,-0.12f);
        	glVertex2f(-0.42f,-0.11f);
        glEnd();
    glPopMatrix();

	glPushMatrix();
        glColor3f(0.5f, 0.5f, 1.0f);
        glScalef(2.0f, 2.0f, 1.0f);
        glBegin(GL_POLYGON); // kaca mobil
        	glVertex2f(-0.20f,0.02f);
        	glVertex2f(-0.07f,0.07f);
        	glVertex2f(0.09f,0.07f);
        	glVertex2f(0.23f,0.06f);
        	glVertex2f(0.25f,0.05f);
        	glVertex2f(-0.22f,0.01f);
        glEnd();
    glPopMatrix();
    
    glPushMatrix();
        glColor3f(1.0f, 1.0f, 1.0f);
        glScalef(2.0f, 2.0f, 1.0f);
        glBegin(GL_POLYGON); // lampu depan
        	glVertex2f(-0.44f,-0.05f);
        	glVertex2f(-0.43f,-0.05f);
        	glVertex2f(-0.39f,-0.03f);
        	glVertex2f(-0.36f,-0.01f);
        	glVertex2f(-0.39f,-0.02f);
        glEnd();
    glPopMatrix();
    
    glPushMatrix();
        glColor3f(0.8f, 0.8f, 0.0f);
        glScalef(2.0f, 2.0f, 1.0f);
        glBegin(GL_POLYGON); // spion
        	glVertex2f(-0.10f,0.0f);
        	glVertex2f(-0.11f,0.0f);
        	glVertex2f(-0.14f,0.03f);
        	glVertex2f(-0.13f,0.04f);
        	glVertex2f(-0.11f,0.05f);
        	glVertex2f(-0.09f,0.04f);
        	glVertex2f(-0.09f,0.03f);
        	glVertex2f(-0.10f,0.02f);
        	glVertex2f(-0.09f,0.01f);
        	glVertex2f(-0.10f,0.00f);
        glEnd();
    glPopMatrix();
    
    glPushMatrix();
        glColor3f(1.0f, 0.0f, 0.0f);
        glScalef(2.0f, 2.0f, 1.0f);
        glBegin(GL_POLYGON); // lampu belakang
        	glVertex2f(0.44f,0.03f);
        	glVertex2f(0.42f,0.03f);
        	glVertex2f(0.44f,-0.01f);
        	glVertex2f(0.45f,-0.01f);
        glEnd();
    glPopMatrix();
    
    glPushMatrix();
        glColor3f(0.0f, 0.0f, 0.0f);
        glScalef(2.0f, 2.0f, 1.0f);
        glBegin(GL_LINE_STRIP); // pintu
        	glVertex2f(-0.12f,0.01f);
        	glVertex2f(-0.13f,0.0f);
        	glVertex2f(-0.14f,-0.06f);
        	glVertex2f(-0.14f,-0.12f);
        	glVertex2f(0.08f,-0.12f);
        	glVertex2f(0.15f,-0.01f);
        	glVertex2f(0.12f,0.04f);
        glEnd();
    glPopMatrix();
    
    // Roda Belakang    
    glPushMatrix();
        glTranslatef(0.65f, -0.2f, 0.0f); 
        glScalef(0.75f, 0.75f, 1.0f);
        wheels();
    glPopMatrix();

    // Roda Depan
    glPushMatrix();
        glTranslatef(-0.52f, -0.2f, 0.0f); 
        glScalef(0.75f, 0.75f, 1.0f);
        wheels();
    glPopMatrix();
    
    // Rumput di Jalan
	glPushMatrix();
	    glColor3f(0.0f, 1.0f, 0.0f);
        glTranslatef(-1.0f, -0.52f, 0.0f); 
        glScalef(0.3f, 0.3f, 1.0f);
        SegiTiga(0.0f, 0.0f, 0.5f, 0.0f, 0.25f, 0.5f);
    glPopMatrix();   
	glPushMatrix();
	    glColor3f(0.0f, 1.0f, 0.0f);
        glTranslatef(-0.90f, -0.52f, 0.0f); 
        glScalef(0.3f, 0.3f, 1.0f);
        SegiTiga(0.0f, 0.0f, 0.5f, 0.0f, 0.25f, 0.5f);
    glPopMatrix();  
	glPushMatrix();
	    glColor3f(0.0f, 1.0f, 0.0f);
        glTranslatef(-0.80f, -0.52f, 0.0f); 
        glScalef(0.3f, 0.3f, 1.0f);
        SegiTiga(0.0f, 0.0f, 0.5f, 0.0f, 0.25f, 0.5f);
    glPopMatrix();  
    glPushMatrix();
	    glColor3f(0.0f, 1.0f, 0.0f);
        glTranslatef(-0.70f, -0.52f, 0.0f); 
        glScalef(0.3f, 0.3f, 1.0f);
        SegiTiga(0.0f, 0.0f, 0.5f, 0.0f, 0.25f, 0.5f);
    glPopMatrix();
    glPushMatrix();
	    glColor3f(0.0f, 1.0f, 0.0f);
        glTranslatef(-0.60f, -0.52f, 0.0f); 
        glScalef(0.3f, 0.3f, 1.0f);
        SegiTiga(0.0f, 0.0f, 0.5f, 0.0f, 0.25f, 0.5f);
    glPopMatrix();
    glPushMatrix();
	    glColor3f(0.0f, 1.0f, 0.0f);
        glTranslatef(-0.50f, -0.52f, 0.0f); 
        glScalef(0.3f, 0.3f, 1.0f);
        SegiTiga(0.0f, 0.0f, 0.5f, 0.0f, 0.25f, 0.5f);
    glPopMatrix();
    glPushMatrix();
	    glColor3f(0.0f, 1.0f, 0.0f);
        glTranslatef(-0.40f, -0.52f, 0.0f); 
        glScalef(0.3f, 0.3f, 1.0f);
        SegiTiga(0.0f, 0.0f, 0.5f, 0.0f, 0.25f, 0.5f);
    glPopMatrix();
    glPushMatrix();
	    glColor3f(0.0f, 1.0f, 0.0f);
        glTranslatef(-0.30f, -0.52f, 0.0f); 
        glScalef(0.3f, 0.3f, 1.0f);
        SegiTiga(0.0f, 0.0f, 0.5f, 0.0f, 0.25f, 0.5f);
    glPopMatrix();
    glPushMatrix();
	    glColor3f(0.0f, 1.0f, 0.0f);
        glTranslatef(-0.20f, -0.52f, 0.0f); 
        glScalef(0.3f, 0.3f, 1.0f);
        SegiTiga(0.0f, 0.0f, 0.5f, 0.0f, 0.25f, 0.5f);
    glPopMatrix();
    glPushMatrix();
	    glColor3f(0.0f, 1.0f, 0.0f);
        glTranslatef(-0.10f, -0.52f, 0.0f); 
        glScalef(0.3f, 0.3f, 1.0f);
        SegiTiga(0.0f, 0.0f, 0.5f, 0.0f, 0.25f, 0.5f);
    glPopMatrix();
	glPushMatrix();
	    glColor3f(0.0f, 1.0f, 0.0f);
        glTranslatef(0.0f, -0.52f, 0.0f); 
        glScalef(0.3f, 0.3f, 1.0f);
        SegiTiga(0.0f, 0.0f, 0.5f, 0.0f, 0.25f, 0.5f);
    glPopMatrix();  
    glPushMatrix();
	    glColor3f(0.0f, 1.0f, 0.0f);
        glTranslatef(1.0f, -0.52f, 0.0f); 
        glScalef(0.3f, 0.3f, 1.0f);
        SegiTiga(0.0f, 0.0f, 0.5f, 0.0f, 0.25f, 0.5f);
    glPopMatrix();   
	glPushMatrix();
	    glColor3f(0.0f, 1.0f, 0.0f);
        glTranslatef(0.90f, -0.52f, 0.0f); 
        glScalef(0.3f, 0.3f, 1.0f);
        SegiTiga(0.0f, 0.0f, 0.5f, 0.0f, 0.25f, 0.5f);
    glPopMatrix();  
	glPushMatrix();
	    glColor3f(0.0f, 1.0f, 0.0f);
        glTranslatef(0.80f, -0.52f, 0.0f); 
        glScalef(0.3f, 0.3f, 1.0f);
        SegiTiga(0.0f, 0.0f, 0.5f, 0.0f, 0.25f, 0.5f);
    glPopMatrix();  
    glPushMatrix();
	    glColor3f(0.0f, 1.0f, 0.0f);
        glTranslatef(0.70f, -0.52f, 0.0f); 
        glScalef(0.3f, 0.3f, 1.0f);
        SegiTiga(0.0f, 0.0f, 0.5f, 0.0f, 0.25f, 0.5f);
    glPopMatrix();
    glPushMatrix();
	    glColor3f(0.0f, 1.0f, 0.0f);
        glTranslatef(0.60f, -0.52f, 0.0f); 
        glScalef(0.3f, 0.3f, 1.0f);
        SegiTiga(0.0f, 0.0f, 0.5f, 0.0f, 0.25f, 0.5f);
    glPopMatrix();
    glPushMatrix();
	    glColor3f(0.0f, 1.0f, 0.0f);
        glTranslatef(0.50f, -0.52f, 0.0f); 
        glScalef(0.3f, 0.3f, 1.0f);
        SegiTiga(0.0f, 0.0f, 0.5f, 0.0f, 0.25f, 0.5f);
    glPopMatrix();
    glPushMatrix();
	    glColor3f(0.0f, 1.0f, 0.0f);
        glTranslatef(0.40f, -0.52f, 0.0f); 
        glScalef(0.3f, 0.3f, 1.0f);
        SegiTiga(0.0f, 0.0f, 0.5f, 0.0f, 0.25f, 0.5f);
    glPopMatrix();
    glPushMatrix();
	    glColor3f(0.0f, 1.0f, 0.0f);
        glTranslatef(0.30f, -0.52f, 0.0f); 
        glScalef(0.3f, 0.3f, 1.0f);
        SegiTiga(0.0f, 0.0f, 0.5f, 0.0f, 0.25f, 0.5f);
    glPopMatrix();
    glPushMatrix();
	    glColor3f(0.0f, 1.0f, 0.0f);
        glTranslatef(0.20f, -0.52f, 0.0f); 
        glScalef(0.3f, 0.3f, 1.0f);
        SegiTiga(0.0f, 0.0f, 0.5f, 0.0f, 0.25f, 0.5f);
    glPopMatrix();
    glPushMatrix();
	    glColor3f(0.0f, 1.0f, 0.0f);
        glTranslatef(0.10f, -0.52f, 0.0f); 
        glScalef(0.3f, 0.3f, 1.0f);
        SegiTiga(0.0f, 0.0f, 0.5f, 0.0f, 0.25f, 0.5f);
    glPopMatrix();
	    
    glFlush();
}

// Setup awal
void Setup(void) {
    glClearColor(0.196f, 0.90f, 0.196f, 1.0f); 
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
}

void reshape(int width, int height) {
    glViewport(0, 0, width, height);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float aspect = (float)width / (float)height;
    if (width >= height) {
        glOrtho(-aspect, aspect, -1.0, 1.0, -1.0, 1.0);
    } else {
        glOrtho(-1.0, 1.0, -1.0/aspect, 1.0/aspect, -1.0, 1.0);
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// Fungsi utama
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Mobil Sederhana Bramantyo");
    Setup();
    glutReshapeFunc(reshape);
    glutDisplayFunc(RenderScene);
    glutMainLoop();
    return 0;
}

