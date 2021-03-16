#include <Windows.h>
#include <glut.h>
#include <math.h>
#include <stdio.h>
#include <time.h>
#define Pi 3.14159265358979323846

GLfloat xAngle, yAngle, zAngle;
GLfloat p1x = (rand()%200-100)/100.0;
GLfloat p1y = (rand()%200-100)/100.0;
GLfloat p1z = (rand()%200-100)/100.0;
GLfloat p2x = (rand()%200-100)/100.0;
GLfloat p2y = (rand()%200-100)/100.0;
GLfloat p2z = (rand()%200-100)/100.0;
GLfloat p3x = (rand()%200-100)/100.0;
GLfloat p3y = (rand()%200-100)/100.0;
GLfloat p3z = (rand()%200-100)/100.0;
GLfloat v1x = (rand()%200-100)/100.0;
GLfloat v1y = (rand()%200-100)/100.0;
GLfloat v1z = (rand()%200-100)/100.0;
GLfloat r1 = (rand()%100)/100.0;
GLfloat r2 = (rand()%100)/100.0;
GLfloat rx = (r1>r2) ? r2 : r1;
GLfloat ry = (r1>r2) ? (r1-r2) : (r2-r1);
GLfloat rz = (r1>r2) ? (1-r1) : (1-r2);
GLfloat v2x = p1x*rx+p2x*ry+p3x*rz;
GLfloat v2y = p1y*rx+p2y*ry+p3y*rz;
GLfloat v2z = p1z*rx+p2z*ry+p3z*rz;


GLboolean bEdge = GL_FALSE;

class VECTOR{
public:
	float Magnitude();
	float InnerProduct(VECTOR v);
	VECTOR CrossProduct(VECTOR v);


	float x;
	float y;
	float z;
};

float VECTOR::Magnitude(){
	return sqrt(x*x+y*y+z*z);
}

float VECTOR::InnerProduct(VECTOR v){
	return (x*v.x + y*v.y + z*v.z);
}

VECTOR VECTOR::CrossProduct(VECTOR v){
	VECTOR result;
	result.x = y*v.z - z*v.y;
	result.y = z*v.x - x*v.z;
	result.z = x*v.y - y*v.x;

	return result;

}

class MATRIX{
public:

    MATRIX  Add(MATRIX m);
    MATRIX  Subtract(MATRIX m);
    MATRIX  Multiply(MATRIX m);
    MATRIX  Transpose();  

    float       ele[4][4];
    float       num_of_rows;
    float       num_of_columns;
};

MATRIX MATRIX::Add(MATRIX m){
    MATRIX result;
    for(int i = 0; i < num_of_rows; i++)
        for(int j = 0; j < num_of_columns; j++)
            result.ele[i][j] = ele[i][j] + m.ele[i][j];

    return result;
}

MATRIX MATRIX::Multiply(MATRIX m){
    int i, j, k;
    MATRIX result;
    for(i = 0; i < num_of_rows; i++){
        for(j = 0; j < num_of_columns; j++){
            result.ele[i][j] = 0.0;
		}
	}

    if(num_of_columns == m.num_of_rows){
        result.num_of_rows = num_of_rows;
        result.num_of_columns = m. num_of_columns;
        for(i = 0; i < num_of_rows; i++){
            for(j = 0; j < m.num_of_columns; j++){
                for(k = 0; k < num_of_columns; k++){
               result.ele[i][j] += ele[i][k] * m.ele[k][j];
				}
			}
		}
	}

	return result;
}



void DoTimer(int value){
	r1 = (rand()%100)/100.0;
	r2 = (rand()%100)/100.0;
	rx = (r1>r2) ? r2 : r1;
	ry = (r1>r2) ? (r1-r2) : (r2-r1);
	rz = (r1>r2) ? (1-r1) : (1-r2);
	v2x = p1x*rx+p2x*ry+p3x*rz;
	v2y = p1y*rx+p2y*ry+p3y*rz;
	v2z = p1z*rx+p2z*ry+p3z*rz;
	glutPostRedisplay();
	glutTimerFunc(30, DoTimer, 1);
}



void DoKeyboard(unsigned char key, int x, int y){
	switch(key) {
	case 'a':yAngle += 2;break;
	case 'd':yAngle -= 2;break;
	case 'w':xAngle += 2;break;
	case 's':xAngle -= 2;break;
	case 'q':zAngle += 2;break;
	case 'e':zAngle -= 2;break;
	case 'z':xAngle = yAngle = zAngle = 0.0;break;
}

     glutPostRedisplay();

}

void DoSpecial(int key, int x, int y){
	switch(key) {
     case GLUT_KEY_LEFT:
		 
		p1x = (rand()%200-100)/100.0;
		p1y = (rand()%200-100)/100.0;
		p1z = (rand()%200-100)/100.0;
		p2x = (rand()%200-100)/100.0;
		p2y = (rand()%200-100)/100.0;
		p2z = (rand()%200-100)/100.0;
		p3x = (rand()%200-100)/100.0;
		p3y = (rand()%200-100)/100.0;
		p3z = (rand()%200-100)/100.0;
		break;
	 case GLUT_KEY_RIGHT:
		v1x = (rand()%200-100)/100.0;
		v1y = (rand()%200-100)/100.0;
		v1z = (rand()%200-100)/100.0;
		break;
	}
	glutPostRedisplay();
}


 
void display(void){
	glClear(GL_COLOR_BUFFER_BIT);
	VECTOR v1, v2, nv, vv;
	
	v1.x = p2x-p1x;
	v1.y = p2y-p1y;
	v1.z = p2z-p1z;
	v2.x = p3x-p2x;
	v2.y = p3y-p2y;
	v2.z = p3z-p2z;

	nv = v1.CrossProduct(v2);

	vv.x = v2x-v1x;
	vv.y = v2y-v1y;
	vv.z = v2z-v1z;

	glShadeModel(GL_FLAT);
	glPushMatrix();
	glRotatef(xAngle, 1.0f, 0.0f, 0.0f);
	glRotatef(yAngle, 0.0f, 1.0f, 0.0f);
	glRotatef(zAngle, 0.0f, 0.0f, 1.0f);

	glColor3f(0.5, 0.5, 0.5);
    glBegin(GL_LINES);
		glVertex3f(0, 0, -1);
		glVertex3f(0, 0, 1);
		glVertex3f(0, -1, 0);
		glVertex3f(0, 1, 0);
		glVertex3f(-1, 0, 0);
		glVertex3f(1, 0, 0);
	glEnd();

	
	glPopMatrix();

	
	glPushMatrix();
	glRotatef(xAngle, 1.0f, 0.0f, 0.0f);
	glRotatef(yAngle, 0.0f, 1.0f, 0.0f);
	glRotatef(zAngle, 0.0f, 0.0f, 1.0f);


	
	
	
	if(nv.InnerProduct(vv)<0){
		glColor3f(1,0,0);
	}
	else{
		glColor3f(0,1,0);
	}
    glBegin(GL_TRIANGLES);	
		glVertex3f(p1x, p1y, p1z);
		glVertex3f(p2x, p2y, p2z);
		glVertex3f(p3x, p3y, p3z);
    glEnd();
	



	
	glColor3f(0,0,1);
	
	glPointSize(5.0);
	glBegin(GL_POINTS);
		glVertex3f(v2x, v2y, v2z);
	glEnd();
	
	glColor3f(1.0, 1.0, 0);
	glBegin(GL_LINES);
		glVertex3f((p1x+p2x+p3x)/3, (p1y+p2y+p3y)/3, (p1z+p2z+p3z)/3);
		glVertex3f((p1x+p2x+p3x)/3+nv.x, (p1y+p2y+p3y)/3+nv.y, (p1z+p2z+p3z)/3+nv.z);
	glEnd();

	glColor3f(0, 0, 1);
	glBegin(GL_LINES);
		glVertex3f(p2x, p2y, p2z);
		glVertex3f(p1x, p1y, p1z);
		glVertex3f(p3x, p3y, p3z);
		glVertex3f(p2x, p2y, p2z);
		
	glEnd();

	glColor3f(0,1,0);
	glBegin(GL_POINTS);
		glVertex3f(v1x, v1y, v1z);
	glEnd();
	glBegin(GL_LINES);
		glVertex3f(v2x, v2y, v2z);
		glVertex3f(v1x, v1y, v1z);
	glEnd();
	


 
	glPopMatrix();


	glutSwapBuffers();
}

void main(int argc, char **argv){
		
	srand((unsigned)time(NULL));
	glutInit(&argc, argv);
	glutInitWindowSize(800, 800);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutCreateWindow("2012210011_2");
	glutDisplayFunc(display);
	glutKeyboardFunc(DoKeyboard);
    glutSpecialFunc(DoSpecial);
	glutTimerFunc(30, DoTimer, 1);
	
	

	glutMainLoop();
}