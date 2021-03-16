#include <Windows.h>
#include <glut.h>
#include <math.h>
#include <stdio.h>
#define Pi 3.14159265358979323846

GLfloat phase = 0.0;
GLfloat state = 0.0;
GLfloat line1 = 0.0;
GLfloat line2 = 12.5;
int blank1 = 1;
int blank2 = 4;
static int is_live = true;

void DoTimer(int value){
	if(is_live)
	{
		phase+=Pi/40;
		line1+=0.1;
		line2+=0.1;
		if(phase>= 2.0 * Pi){
			phase = 0;
		}
		if(line1>=25){
			line1=0;
			blank1 = rand()%6;
		}
		if(line2>=25){
			line2=0;
			blank2 = rand()%6;
		}
	}
	glutPostRedisplay();
	glutTimerFunc(30, DoTimer, 1);
}
void retry()
{
	is_live = true;
	line1 = 0;
	line2 = 12.5;
	blank1 = rand()%6;
	blank2 = rand()%6;
}

void DoSpecial(int key, int x, int y){
	switch(key) {
     case GLUT_KEY_LEFT:
		 if(is_live)
		 state+=Pi/30;
		 while(state >= 2*Pi) state -= 2*Pi;
		 break;
	 case GLUT_KEY_RIGHT:
		 if(is_live)
		 state-=Pi/30;
		 while(state < 0) state += 2*Pi;
		 break;
	 case GLUT_KEY_UP:
		 retry();
		 break;
	}
	glutPostRedisplay();
}

void reshape(int w, int h){
	glLoadIdentity();
	glViewport(0,0,w,h);
	gluOrtho2D(0.0, 100.0, 0.0, 100.0);
}
 
void display(void){
	GLfloat angle;
	double left, right;

	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(sin(phase)*sin(phase), sin(phase+1.0*Pi/3.0)*sin(phase+1.0*Pi/3.0), sin(phase+2.0*Pi/3.0)*sin(phase+2.0*Pi/3.0));
//	glRectf(0,0,100,100);
//	glColor3f(sin(phase+Pi/2)*sin(phase+Pi/2), sin(phase+5.0*Pi/6.0)*sin(phase+5.0*Pi/6.0), sin(phase+7.0*Pi/6.0)*sin(phase+7.0*Pi/6.0));

	//triangle
	glBegin(GL_POLYGON);
	for(angle = 0.0; angle <= 2.0 * Pi; angle += 2.0 * Pi / 3.0){
			glVertex3f(25 * cos(angle+phase) + 25, 25 * sin(angle+phase) + 75, 0.0);

	}
	glEnd();

	//pentagon
	glBegin(GL_POLYGON);
	for(angle = 0.0; angle <= 2.0 * Pi; angle += 2.0 * Pi / 5.0){
			glVertex3f(25 * cos(angle-phase) + 75, 25 * sin(angle-phase) + 75, 0.0);
	}
	glEnd();
	 
	//hexagon
	glBegin(GL_POLYGON);
	for(angle = 0.0; angle <= 2.0 * Pi; angle += 2.0 * Pi / 6.0){
			glVertex3f(25 * cos(angle-phase) + 25, 25 * sin(angle-phase) + 25, 0.0);
	}
	glEnd();

	//dodecagon
	glBegin(GL_POLYGON);
	for(angle = 0.0; angle <= 2.0 * Pi; angle += 2.0 * Pi / 12.0){
			glVertex3f(25 * cos(angle+phase) + 75, 25 * sin(angle+phase) + 25, 0.0);
	}
	glEnd();
	
	//super_hexagon
	glColor3f(0,0,0);
	glBegin(GL_POLYGON);
	for(angle = 0.0; angle <= 2.0 * Pi; angle += 2.0 * Pi / 3.0){
			glVertex3f(25 + 18 * cos(state-phase+Pi/6) + 2 * cos(angle - phase+state - Pi/6), 25 + 18 * sin(state-phase+Pi/6) + 2 * sin(angle - phase+state - Pi/6), 0.0);
	}
	glEnd();

	//line
	glLineWidth(3.0);
	glColor3f(0.5,0.5,0.5);
	glBegin(GL_LINES);
	for(angle = 0.0; angle < 3.0 * Pi / 2.0; angle += Pi / 3.0){
		glVertex3f(25 + (line1+2*sin(2*phase)) * cos(blank1 * Pi/3 - phase + angle), 25 + (line1+2*sin(2*phase)) * sin(blank1 * Pi/3 - phase + angle), 0.0);
		glVertex3f(25 + (line1+2*sin(2*phase)) * cos(blank1 * Pi/3 - phase + angle + Pi/3), 25 + (line1+2*sin(2*phase)) * sin(blank1 * Pi/3 - phase + angle + Pi/3), 0.0);

		glVertex3f(25 + (line2+2*sin(2*phase)) * cos(blank2 * Pi/3 - phase + angle), 25 + (line2+2*sin(2*phase)) * sin(blank2 * Pi/3 - phase + angle), 0.0);
		glVertex3f(25 + (line2+2*sin(2*phase)) * cos(blank2 * Pi/3 - phase + angle + Pi/3), 25 + (line2+2*sin(2*phase)) * sin(blank2 * Pi/3 - phase + angle + Pi/3), 0.0);
	}
	glEnd();

	left = blank1 * Pi/3 - Pi/2;
	right = blank1 * Pi/3 - Pi/6;
	while(left < 0) left += 2*Pi;
	while(right < 0) right += 2*Pi;
	if((line1 > 17.5 && line1 < 18.5) && !((left < right && left <= state && state <= right) ||(left > right && left <= state && state <= right + 2*Pi) || (left > right && left <= state + 2*Pi && state <= right))) is_live = false;

	left = blank2 * Pi/3 - Pi/2;
	right = blank2 * Pi/3 - Pi/6;
	while(left < 0) left += 2*Pi;
	while(right < 0) right += 2*Pi;
	if((line2 > 17.5 && line2 < 18.5) && !((left < right && left <= state && state <= right) ||(left > right && left <= state && state <= right + 2*Pi) || (left > right && left <= state + 2*Pi && state <= right))) is_live = false;
	glutSwapBuffers();
}

void main(int argc, char **argv){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutCreateWindow("2012210011_1");
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
    glutSpecialFunc(DoSpecial);
	glutTimerFunc(30, DoTimer, 1);
	glutMainLoop();
}