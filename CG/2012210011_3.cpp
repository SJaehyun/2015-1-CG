#include <Windows.h>
#include <glut.h>
#include <math.h>
#include <stdio.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

using namespace std;
#define Pi 3.14159265358979323846

GLfloat xAngle, yAngle, zAngle;
GLfloat xl=0, yl=7, zl=0;
GLint vertexNum = 0;
GLint faceNum = 0;
GLint textureNum = 0;
GLint mtlNum = 0;
float zoom = 0.10;
GLint EnvMode = GL_REPLACE;
GLint TexFilter = GL_LINEAR;
GLint lighttype = 1;
boolean vnshow = FALSE;
boolean wireshow = FALSE;
boolean modelshow = FALSE;

GLboolean bEdge = GL_FALSE;

class VECTOR{
public:
	VECTOR(void);
	VECTOR(float x_, float y_, float z_);
	float Magnitude();
	float InnerProduct(VECTOR v);
	VECTOR CrossProduct(VECTOR v);


	float x;
	float y;
	float z;
};

VECTOR::VECTOR(void){
	x = 0;
	y = 0;
	z = 0;
}
VECTOR::VECTOR(float x_, float y_, float z_){
	x = x_;
	y = y_;
	z = z_;
}
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

const VECTOR operator +(const VECTOR &v1, const VECTOR &v2){
	VECTOR result(v1.x+v2.x, v1.y+v2.y, v1.z+v2.z);
	return result;
}
const VECTOR operator *(float n, const VECTOR &v){
	VECTOR result(n*v.x, n*v.y, n*v.z);
	return result;
}
const VECTOR operator *(const VECTOR &v1, const VECTOR &v2){
	VECTOR result(v1.x*v2.x, v1.x*v2.y, v1.z*v2.z);
	return result;
}
const VECTOR operator -(const VECTOR &v1, const VECTOR &v2){
	VECTOR result(v1.x-v2.x, v1.y-v2.y, v1.z-v2.z);
	return result;
}
const VECTOR operator /(const VECTOR &v, float n){
	VECTOR result(v.x/n, v.y/n, v.z/n);
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

class Vertex{
private:
	VECTOR v;
public:
	Vertex(){
		v.x=0;
		v.y=0;
		v.z=0;
	}
	void setV(GLfloat x, GLfloat y, GLfloat z){
		v.x = x;
		v.y = y;
		v.z = z;
	}
	VECTOR getV(){
		return v;
	}
	void Display(){
		cout <<"{"<<v.x<<","<<v.y<<","<<v.z<<"}"<<endl;
	}
};

class Normal{
private:
	VECTOR v;
public:
	Normal(){
		v.x=0;
		v.y=0;
		v.z=0;
	}
	void setV(VECTOR v_){
		v = v_;
	}
	VECTOR getV(){
		return v;
	}
	void Display(){
		cout <<"{"<<v.x<<","<<v.y<<","<<v.z<<"}"<<endl;
	}
};


class Face{
private:
	int v1, v2, v3, v4;
public:
	Face(){
		v1=0;
		v2=0;
		v3=0;
		v4=0;
	}
	void setV(GLint s1,GLint s2,GLint s3, GLint s4){
		v1 = s1;
		v2 = s2;
		v3 = s3;
		v4 = s4;
	}
	int getV1(){
		return v1;
	}
	int getV2(){
		return v2;
	}
	int getV3(){
		return v3;
	}
	int getV4(){
		return v4;
	}
	void Display(){
		cout<<v1<<", "<<v2<<", "<<v3<<", "<<v4<<endl;
	}
};

class Texture{
private:
	GLfloat x;
	GLfloat y;
public:
	Texture(){
		x=0;
		y=0;
	}
	void setX(GLfloat x_){
		x = x_;
	}
	void setY(GLfloat y_){
		y = y_;
	}
	GLfloat getX(){
		return x;
	}
	GLfloat getY(){
		return y;
	}
	void Display(){
		cout <<"{"<<x<<","<<y<<"}"<<endl;
	}
};

class Mtl{
private:
	GLint x;
	GLint y;
	VECTOR d;
	VECTOR s;
	int i;
	char* name;
public:
	Mtl(){
		x=0;
		y=0;
	}
	void setX(GLint x_){
		x = x_;
	}
	void setY(GLint y_){
		y = y_;
	}
	void setD(GLfloat x, GLfloat y, GLfloat z){
		d.x = x;
		d.y = y;
		d.z = z;
	}
	void setS(GLfloat x, GLfloat y, GLfloat z){
		s.x = x;
		s.y = y;
		s.z = z;
	}
	void setName(const char* name_){
		name = new char[80];
		strcpy(name, name_);
	}
	void setI(int i_){
		i = i_;
	}
	GLint getX(){
		return x;
	}
	GLint getY(){
		return y;
	}
	VECTOR getD(){
		return d;
	}
	VECTOR getS(){
		return s;
	}
	
	const char* getName(){
		return name;
	}
	void Display(){
		cout<<i<<name<<endl;
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
	Texture *textureList;
	Normal *normalList;
	Mtl *mtlList;
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
			while(!file.eof()){
				getline(file,  temp);
				if(temp.c_str()[0] == 'v' && temp.c_str()[1] == ' '){
					vertexNum++;
				}
				else if(temp.c_str()[0] == 'v' && temp.c_str()[1] == 't'){
					textureNum++;
				}
				else if(temp.c_str()[0] == 'u' && temp.c_str()[1] == 's'){
					mtlNum++;
				}
				else if(temp.c_str()[0] == 'f' && temp.c_str()[1] == ' '){
					faceNum++;
				}

			}
			vertexList = (Vertex*) malloc(sizeof(Vertex)*vertexNum);
			faceList = (Face*) malloc(sizeof(Face)*faceNum);
			textureList = (Texture*) malloc(sizeof(Texture)*textureNum);
			mtlList = (Mtl*) malloc(sizeof(Mtl)*mtlNum);
			normalList = (Normal*) malloc(sizeof(Normal)*vertexNum);
			file.close();
			return 0;

		}
	}
	void readObj(char *filename){
		file.open(filename, ios::in);
		if(!file.is_open()){
			cout<<"fail";
		}
		else{
			GLint ix, iy, iz, iw;
			GLfloat fx, fy, fz;
			GLint tx, ty, tz;
			GLint t;
			int i=0;
			int j=0;
			int k=0;
			int l=0;
			while(!file.eof()){
				getline(file,  temp);
				if(temp.c_str()[0] == 'v' && temp.c_str()[1] == ' '){
					temp[0] = ' ';
					sscanf(temp.c_str(), "%f %f %f", &fx, &fy, &fz);
					vertexList[i].setV(fx, fy, fz);
					i++;
				}
				else if(temp.c_str()[0] == 'v' && temp.c_str()[1] == 't'){
					temp[0] = ' ';
					temp[1] = ' ';
					sscanf(temp.c_str(), "%f %f", &fx, &fy);
					
					textureList[k].setX(fx);
					textureList[k].setY(fy);
					k++;
				}
				else if(temp.c_str()[0] == 'u' && temp.c_str()[1] == 's'){
					temp.erase(temp.begin(), temp.begin()+7);

					mtlList[l].setName(temp.c_str());
					if(l==0){
						mtlList[l].setX(j);
					}
					else if(l==mtlNum-1){
						mtlList[l].setX(j+1);
						mtlList[l].setY(faceNum-1);
						mtlList[l-1].setY(j);
					}
					else{
						mtlList[l].setX(j+1);
						mtlList[l-1].setY(j);
					}
					l++;
				}
				else if(temp.c_str()[0] == 'f' && temp.c_str()[1] == ' '){
					temp[0] = ' ';
					int check = sscanf(temp.c_str(), "%d/%d %d/%d %d/%d %d/%d", &ix, &t, &iy, &t, &iz, &t, &iw, &t);
					if(check == 6){
						faceList[j].setV(ix, iy, iz, 0);
					}
					else{
						faceList[j].setV(ix, iy, iz, iw);
					}
					j++;
				

				}

			}
			for(int i=0; i<vertexNum; i++){
				int normalNum = 0;
				int list[5000];
				VECTOR vn;
				VECTOR normal;
				VECTOR v1;
				VECTOR v2;
				VECTOR v3;
				for(int j=0; j<faceNum; j++){
					if(faceList[j].getV1() == (i+1) || faceList[j].getV2() == (i+1) || faceList[j].getV3() == (i+1) || faceList[j].getV4() == (i+1)){
						list[normalNum] = j;
						normalNum+=1;
					}
				}
				for(int k=0; k<normalNum; k++){
					v1 = vertexList[faceList[list[k]].getV1()-1].getV();
					v2 = vertexList[faceList[list[k]].getV2()-1].getV();
					v3 = vertexList[faceList[list[k]].getV3()-1].getV();
					normal = ((const VECTOR)(v2-v1)).CrossProduct(v3-v1);
					normal = normal / normal.Magnitude();
					vn = vn + normal;
				}
				normalList[i].setV(vn / vn.Magnitude());
			}

		}
		file.close();
	}
	void readMtl(char* filename){
		file.open(filename, ios::in);
		if(!file.is_open()){
			cout<<"fail";
		}
		else{
			int j=0;
			int k=0;
			float x, y, z;
			while(!file.eof()){
				getline(file,  temp);
				if(temp.c_str()[0] == 'n' && temp.c_str()[1] == 'e'){
					temp.erase(temp.begin(), temp.begin()+7);
					for(int i=0; i<mtlNum; i++){
						if(temp.compare(mtlList[i].getName()) == 0){
							k = i;
						}
					}

				}
				else if(temp.c_str()[0] == 'K' && temp.c_str()[1] == 'd'){
					temp[0] = ' ';
					temp[1] = ' ';
					sscanf(temp.c_str(), "%f %f %f", &x, &y, &z);
					mtlList[k].setD(x, y, z);
				}
				else if(temp.c_str()[0] == 'K' && temp.c_str()[1] == 's'){
					temp[0] = ' ';
					temp[1] = ' ';
					sscanf(temp.c_str(), "%f %f %f", &x, &y, &z);
					mtlList[k].setS(x, y, z);
				}
			}
		}
		file.close();
	}



	Vertex getVertex(int i){
		return vertexList[i];
	}
	Face getFace(int i){
		return faceList[i];
	}
	Mtl getMtl(int i){
		return mtlList[i];
	}
	Normal getNormal(int i){
		return normalList[i];
	}
	Texture getTexture(int i){
		return textureList[i];
	}

};

Mesh mesh;

VECTOR camera;
VECTOR light;
VECTOR intensity;


/*
void DoTimer(int value){
	glutPostRedisplay();
	glutTimerFunc(30, DoTimer, 1);
}
*/


void DoKeyboard(unsigned char key, int x, int y){
	switch(key) {
	case 'a':yAngle += 2;break;
	case 'd':yAngle -= 2;break;
	case 'w':xAngle += 2;break;
	case 's':xAngle -= 2;break;
	case 'q':zAngle += 2;break;
	case 'e':zAngle -= 2;break;
	case 'z':xAngle = yAngle = zAngle = 0.0;break;
	case 'j':xl-=0.2;break;
	case 'i':yl+=0.2;break;
	case 'k':yl-=0.2;break;
	case 'l':xl+=0.2;break;
	case 'u':zl-=0.2;break;
	case 'o':zl+=0.2;break;
	case '1':
		if(modelshow){
			modelshow = FALSE;
		}
		else{
			modelshow = TRUE;
		}
		break;
	case '2':
		if(wireshow){
			wireshow = FALSE;
		}
		else{
			wireshow = TRUE;
		}
		break;
	case '3':
		if(vnshow){
			vnshow = FALSE;
		}
		else{
			vnshow = TRUE;
		}
		break;
	case '8':lighttype=1;break;
	case '9':lighttype=2;break;
	}

     glutPostRedisplay();

}

void DoSpecial(int key, int x, int y){
	switch(key) {
     case GLUT_KEY_LEFT:
		 
		break;
	 case GLUT_KEY_RIGHT:

		break;
	 case GLUT_KEY_UP:
		zoom*=1.25;

		break;
	 case GLUT_KEY_DOWN:
		zoom*=0.8;

		break;
	}
	glutPostRedisplay();
}

GLubyte *LoadBmp(const char *Path, int *Width, int *Height)
{
     HANDLE hFile;
     DWORD FileSize, dwRead;
     BITMAPFILEHEADER *fh=NULL;
     BITMAPINFOHEADER *ih;
     BYTE *pRaster;
 
     hFile=CreateFileA(Path,GENERIC_READ,0,NULL,
          OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
     if (hFile==INVALID_HANDLE_VALUE) {
          return NULL;
     }
 
     FileSize=GetFileSize(hFile,NULL);
     fh=(BITMAPFILEHEADER *)malloc(FileSize);
     ReadFile(hFile,fh,FileSize,&dwRead,NULL);
     CloseHandle(hFile);
 
     int len = FileSize - fh->bfOffBits;
     pRaster=(GLubyte *)malloc(len);
     memcpy(pRaster, (BYTE *)fh+fh->bfOffBits, len);
 
     // RGB로 순서를 바꾼다.
     for (BYTE *p=pRaster;p < pRaster + len - 3;p+=3) {
          BYTE b = *p;
          *p = *(p+2);
          *(p+2) = b;
     }
 
     ih=(BITMAPINFOHEADER *)((PBYTE)fh+sizeof(BITMAPFILEHEADER));
     *Width=ih->biWidth;
     *Height=ih->biHeight;
 
     free(fh);
     return pRaster;
}
/*
GLuint LoadTexture( const char * filename )
{

  GLuint texture;

  int width, height;

  unsigned char * data;

  FILE * file;

  file = fopen( filename, "rb" );

  if ( file == NULL ) return 0;
  width = 1024;
  height = 512;
  data = (unsigned char *)malloc( width * height * 3 );
  //int size = fseek(file,);
  fread( data, width * height * 3, 1, file );
  fclose( file );

 for(int i = 0; i < width * height ; ++i)
{
   int index = i*3;
   unsigned char B,R;
   B = data[index];
   R = data[index+2];

   data[index] = R;
   data[index+2] = B;

}


glGenTextures( 1, &texture );
glBindTexture( GL_TEXTURE_2D, texture );
glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,GL_MODULATE );
glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST );


glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR );
glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT );
glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT );
gluBuild2DMipmaps( GL_TEXTURE_2D, 3, width, height,GL_RGB, GL_UNSIGNED_BYTE, data );
//free(data);

return texture;
}*/

VECTOR getColor(VECTOR color, VECTOR V, VECTOR N, VECTOR D, VECTOR S){
	VECTOR R;
	float NL;
	float VR;
	VECTOR LL = VECTOR(xl, yl, zl);
	if(lighttype == 2){
		light = V - LL;
		light = light / light.Magnitude();
	}
	if(N.InnerProduct(light) < 0){

		R = ( 2 * (-1) * N.InnerProduct(light) ) * N  + light;
	}
	else{
		R.x = 0;
		R.y = 0;
		R.z = 0;
	}
	NL = N.InnerProduct((-1) * light) ;
	VR = (-1)*camera.InnerProduct(R);
	if(NL < 0){
	}
	else{
		color = color + (D * ( NL * intensity ));
	}
	if(VR > 0){
	}
	else{
		color = color + (S * ( pow(VR,2) * intensity));
	}
	return color;
}
 
void display(void){
	GLubyte *data;
	GLuint texture;
	int Width, Height;
	VECTOR color;
	VECTOR newcolor;
	VECTOR N;
	VECTOR L;
	VECTOR V;
	VECTOR R;
	VECTOR v1;
	VECTOR v2;
	VECTOR v3;
	VECTOR v4;
	VECTOR v1n;
	VECTOR v2n;
	VECTOR v3n;
	VECTOR v4n;
	VECTOR il;
	float NL;
	float VR;
	
	//glEnable(GL_TEXTURE_2D);
	/*
	data = LoadBmp(mesh.getGroup(0).getName2(), &Width, &Height);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Width, Height,
		0, GL_RGB,  GL_UNSIGNED_BYTE, data);
	free(data);*/
	
    //glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, EnvMode);
	
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, TexFilter);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, TexFilter);
	
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

	color.x=0.5;
	color.y=0.5;
	color.z=0.5;
	camera.x=0;
	camera.y=0;
	camera.z=1;
	light.x=0;
	light.y=0;
	light.z=1;
	intensity.x = 0.5;
	intensity.y = 0.5;
	intensity.z = 0.5;

	glPushMatrix();
	glRotatef(xAngle, 1.0f, 0.0f, 0.0f);
	glRotatef(yAngle, 0.0f, 1.0f, 0.0f);
	glRotatef(zAngle, 0.0f, 0.0f, 1.0f);


	glScalef(zoom, zoom, zoom);
	if(modelshow){
		for(int i=0; i<mtlNum; i++){
			//texture= LoadTexture( mesh.getGroup(i).getName2() );

			for(int j = mesh.getMtl(i).getX(); j<= mesh.getMtl(i).getY();j++){
				color.x=0.3;
				color.y=0.3;
				color.z=0.3;
				v1 = VECTOR(mesh.getVertex(mesh.getFace(j).getV1()-1).getV().x, mesh.getVertex(mesh.getFace(j).getV1()-1).getV().y, mesh.getVertex(mesh.getFace(j).getV1()-1).getV().z);
				v1n = mesh.getNormal(mesh.getFace(j).getV1()-1).getV();
				v2 = VECTOR(mesh.getVertex(mesh.getFace(j).getV2()-1).getV().x, mesh.getVertex(mesh.getFace(j).getV2()-1).getV().y, mesh.getVertex(mesh.getFace(j).getV2()-1).getV().z);
				v2n = mesh.getNormal(mesh.getFace(j).getV2()-1).getV();
				v3 = VECTOR(mesh.getVertex(mesh.getFace(j).getV3()-1).getV().x, mesh.getVertex(mesh.getFace(j).getV3()-1).getV().y, mesh.getVertex(mesh.getFace(j).getV3()-1).getV().z);
				v3n = mesh.getNormal(mesh.getFace(j).getV3()-1).getV();
				/*
				glBegin(GL_LINES);
				glVertex3f(v1.x, v1.y, v1.z);
				glVertex3f(v1.x + v1n.x/5, v1.y + v1n.y/5, v1.z + v1n.z);
				glVertex3f(v2.x, v2.y, v2.z);
				glVertex3f(v2.x + v2n.x/5, v2.y + v2n.y/5, v2.z + v2n.z);
				glVertex3f(v3.x, v3.y, v3.z);
				glVertex3f(v3.x + v3n.x/5, v3.y + v3n.y/5, v3.z + v3n.z);
				*/
				if((mesh.getFace(j).getV4()) != 0){
					v4 = VECTOR(mesh.getVertex(mesh.getFace(j).getV4()-1).getV().x, mesh.getVertex(mesh.getFace(j).getV4()-1).getV().y, mesh.getVertex(mesh.getFace(j).getV4()-1).getV().z);
					v4n = mesh.getNormal(mesh.getFace(j).getV4()-1).getV();
					/*glVertex3f(v4.x, v4.y, v4.z);
					glVertex3f(v4.x + v4n.x/5, v4.y + v4n.y/5, v4.z + v4n.z);*/
				}
				/*glEnd();*/
				
				if((mesh.getFace(j).getV4()) != 0){
					glBegin(GL_QUADS);
						newcolor = getColor(color, v1, v1n, mesh.getMtl(i).getD(), mesh.getMtl(i).getS());
						glColor3f(newcolor.x, newcolor.y, newcolor.z);;
						glVertex3f(v1.x, v1.y, v1.z);
						newcolor = getColor(color, v2, v2n, mesh.getMtl(i).getD(), mesh.getMtl(i).getS());
						glColor3f(newcolor.x, newcolor.y, newcolor.z);;
						glVertex3f(v2.x, v2.y, v2.z);
						newcolor = getColor(color, v3, v3n, mesh.getMtl(i).getD(), mesh.getMtl(i).getS());
						glColor3f(newcolor.x, newcolor.y, newcolor.z);;
						glVertex3f(v3.x, v3.y, v3.z);
						newcolor = getColor(color, v4, v4n, mesh.getMtl(i).getD(), mesh.getMtl(i).getS());
						glColor3f(newcolor.x, newcolor.y, newcolor.z);;
						glVertex3f(v4.x, v4.y, v4.z);
					glEnd();
				}
				else{
				glBegin(GL_TRIANGLES);
						newcolor = getColor(color, v1, v1n, mesh.getMtl(i).getD(), mesh.getMtl(i).getS());
						glColor3f(newcolor.x, newcolor.y, newcolor.z);;
						glVertex3f(v1.x, v1.y, v1.z);
						newcolor = getColor(color, v2, v2n, mesh.getMtl(i).getD(), mesh.getMtl(i).getS());
						glColor3f(newcolor.x, newcolor.y, newcolor.z);;
						glVertex3f(v2.x, v2.y, v2.z);
						newcolor = getColor(color, v3, v3n, mesh.getMtl(i).getD(), mesh.getMtl(i).getS());
						glColor3f(newcolor.x, newcolor.y, newcolor.z);;
						glVertex3f(v3.x, v3.y, v3.z);
				
				glEnd();
				}
				if(lighttype ==2 ){
					glColor3f(1,0,0);
					glPointSize(10);
					glBegin(GL_POINTS);
					glVertex3f(xl, yl, zl);
					glEnd();
				}
			}
		}
	}
	if(wireshow){
		glColor3f(1,1,1);
		glBegin(GL_LINES);
		for(int i=0; i<faceNum; i++){
			v1 = VECTOR(mesh.getVertex(mesh.getFace(i).getV1()-1).getV().x, mesh.getVertex(mesh.getFace(i).getV1()-1).getV().y, mesh.getVertex(mesh.getFace(i).getV1()-1).getV().z);
			v2 = VECTOR(mesh.getVertex(mesh.getFace(i).getV2()-1).getV().x, mesh.getVertex(mesh.getFace(i).getV2()-1).getV().y, mesh.getVertex(mesh.getFace(i).getV2()-1).getV().z);
			v3 = VECTOR(mesh.getVertex(mesh.getFace(i).getV3()-1).getV().x, mesh.getVertex(mesh.getFace(i).getV3()-1).getV().y, mesh.getVertex(mesh.getFace(i).getV3()-1).getV().z);
			if((mesh.getFace(i).getV4()) != 0){
				v4 = VECTOR(mesh.getVertex(mesh.getFace(i).getV4()-1).getV().x, mesh.getVertex(mesh.getFace(i).getV4()-1).getV().y, mesh.getVertex(mesh.getFace(i).getV4()-1).getV().z);
				
					glVertex3f(v1.x, v1.y, v1.z);
					glVertex3f(v2.x, v2.y, v2.z);
					glVertex3f(v2.x, v2.y, v2.z);
					glVertex3f(v3.x, v3.y, v3.z);
					glVertex3f(v3.x, v3.y, v3.z);
					glVertex3f(v4.x, v4.y, v4.z);
					glVertex3f(v4.x, v4.y, v4.z);
					glVertex3f(v1.x, v1.y, v1.z);



			}
			else{
					glVertex3f(v1.x, v1.y, v1.z);
					glVertex3f(v2.x, v2.y, v2.z);
					glVertex3f(v2.x, v2.y, v2.z);
					glVertex3f(v3.x, v3.y, v3.z);
					glVertex3f(v3.x, v3.y, v3.z);
					glVertex3f(v1.x, v1.y, v1.z);

			}
		}
		glEnd();
	}
	if(vnshow){
		glColor3f(1,1,1);
		glBegin(GL_LINES);
		for(int i=0; i<vertexNum; i++){
			v1 = mesh.getVertex(i).getV();
			v1n = mesh.getNormal(i).getV();
			glVertex3f(v1.x, v1.y, v1.z);
			glVertex3f(v1.x+v1n.x/5, v1.y+v1n.y/5, v1.z+v1n.z/5);

		}
		glEnd();

	}



	glPopMatrix();
	

	glutSwapBuffers();
}

void main(int argc, char **argv){
	if(mesh.MeshInit("LEGO_Man.obj")) exit(1);
	mesh.readObj("LEGO_Man.obj");
	mesh.readMtl("LEGO_Man.mtl");
	glutInit(&argc, argv);

	
	glutInitWindowSize(600, 600);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutCreateWindow("2012210011_3");
	glutDisplayFunc(display);
	glutKeyboardFunc(DoKeyboard);
    glutSpecialFunc(DoSpecial);
	//glutTimerFunc(30, DoTimer, 1);
	
	

	glutMainLoop();
}
