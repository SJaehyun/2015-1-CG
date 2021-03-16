#include <iostream>
#include <glut.h>
#include <string>
#include <fstream>
using namespace std;



class Vertex{
private:
	GLfloat x;
	GLfloat y;
	GLfloat z;
public:
	Vertex(){
		x=0;
		y=0;
		z=0;
	}
	void setX(GLfloat x_){
		x = x_;
	}
	void setY(GLfloat y_){
		y = y_;
	}
	void setZ(GLfloat z_){
		z = z_;
	}
	GLfloat getX(){
		return x;
	}
	GLfloat getY(){
		return y;
	}
	GLfloat getZ(){
		return z;
	}
};

class Face{
private:
	GLint x;
	GLint y;
	GLint z;
public:
	Face(){
		x=0;
		y=0;
		z=0;
	}
	void setX(GLint x_){
		x = x_;
	}
	void setY(GLint y_){
		y = y_;
	}
	void setZ(GLint z_){
		z = z_;
	}
	GLint getX(){
		return x;
	}
	GLint getY(){
		return y;
	}
	GLint getZ(){
		return z;
	}
};
	

class Mesh{
private:
	GLfloat maxx;
	GLfloat maxy;
	GLfloat maxz;
	GLfloat minx;
	GLfloat miny;
	GLfloat minz;

	GLfloat max_;
	GLfloat min_;
	GLfloat max;
	
	GLfloat averagex;
	GLfloat averagey;
	GLfloat averagez;
	
	string temp;
	FILE *fp;
	ifstream file;
	Vertex *vertexList;
	Face *faceList;
public:
	Mesh(){
		maxx=0;
		maxy=0;
		maxz=0;
		minx=0;
		miny=0;
		minz=0;

		max_=0;
		min_=0;
		max=0;
	
		averagex=0;
		averagey=0;
		averagez=0;
	}

	int MeshInit(char *filename){
		file.open(filename, ios::in);
		if(!file.is_open()){
			cout<<"fail";
			return 1;
		}
		else{
			GLint ix, iy, iz;
			GLfloat fx, fy, fz;
			GLint t;
			vertexList = new Vertex();
			faceList = new Face();
			while(!file.eof()){
				int i=0;
				int j=0;
				getline(file,  temp);
				if(temp.c_str()[0] == 'v' && temp.c_str[1] == ' '){
					temp[0] = ' ';
					sscanf(temp.c_str(), "%f %f %f", &fx, &fy, &fz);
					vertexList[i].setX(fx);
					vertexList[i].setY(fy);
					vertexList[i].setZ(fz);
					i++;
				}
				else if(temp.c_str()[0] == 'f' && temp.c_str[1] == ' '){
					temp[0] = ' ';
					sscanf(temp.c_str(), "%d/%d/%d %d/%d/%d %d/%d/%d", &ix, &t, &t, &iy, &t, &t, &iz, &t, &t);
					faceList[j].setX(ix);
					faceList[j].setY(iy);
					faceList[j].setZ(iz);
					j++;
				

				}

			}

		}
	}
};

Mesh mesh;
	
void main(){
	if(mesh.MeshInit("alexstrasza.obj")) exit(1);
	mesh.readObj(filename);
}