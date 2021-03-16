#include <Windows.h>
#include <glut.h>
#include <math.h>
#include <stdio.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <gl/GL.h>
#include <gl/glext.h>

using namespace std;
#define Pi 3.14159265358979323846


GLint MAX_REFLECT;;
GLint scene = 0;
GLfloat xAngle = 0;
GLfloat yAngle = 0;
GLfloat zAngle = 0;
GLint resX, resY;
GLint vertexNum = 0;
GLint faceNum = 0;
GLint textureNum = 0;
GLint mtlNum = 0;
float zoom = 0.10;


class VECTOR{
public:
	VECTOR(void);
	VECTOR(float x_, float y_, float z_);
	float Magnitude();
	float InnerProduct(VECTOR v);
	VECTOR CrossProduct(VECTOR v);
	void RotateY(GLfloat x);

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

void VECTOR::RotateY(GLfloat w){
	float tempx;
	float tempy;
	float tempz;
	tempx = cos(w)*x + sin(w)*z;
	tempy = y;
	tempz = (-1)*sin(w)*x + cos(w)*z;
	x = tempx;
	y = tempy;
	z = tempz;
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
	VECTOR result(v1.x*v2.x, v1.y*v2.y, v1.z*v2.z);
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
	VECTOR normal;
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
	void setN(VECTOR n){
		normal = n;
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
	VECTOR getNormal(){
		return normal;
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

	
class Quad{
private:
	VECTOR V1;
	VECTOR V2;
	VECTOR V3;
	VECTOR V4;
	VECTOR N;
public:
	Quad(){
	}
	Quad(VECTOR v1, VECTOR v2, VECTOR v3, VECTOR v4){
		V1 = v1;
		V2 = v2;
		V3 = v3;
		V4 = v4;
		N =((const VECTOR)(V2-V1)).CrossProduct(V3-V1);
		N = N / N.Magnitude();
	}
	VECTOR getV1(){
		return V1;
	}
	VECTOR getV2(){
		return V2;
	}
	VECTOR getV3(){
		return V3;
	}
	VECTOR getV4(){
		return V4;
	}
	VECTOR getNormal(){
		return N;
	}
};
class Cube{
private:
	VECTOR V1;
	VECTOR V2;
	VECTOR V3;
	VECTOR V4;
	VECTOR V5;
	VECTOR V6;
	VECTOR V7;
	VECTOR V8;
public:
	Cube(){
	}
	Cube(VECTOR v1, VECTOR v2, VECTOR v3, VECTOR v4, VECTOR v5, VECTOR v6, VECTOR v7, VECTOR v8){
		V1 = v1;
		V2 = v2;
		V3 = v3;
		V4 = v4;
		V5 = v5;
		V6 = v6;
		V7 = v7;
		V8 = v8;
	}
	Quad getF1(){
		return Quad(V1, V4, V3, V2);
	}
	Quad getF2(){
		return Quad(V1, V2, V6, V5);
	}
	Quad getF3(){
		return Quad(V2, V3, V7, V6);
	}
	Quad getF4(){
		return Quad(V3, V4, V8, V7);
	}
	Quad getF5(){
		return Quad(V1, V5, V8, V4);
	}
	Quad getF6(){
		return Quad(V5, V6, V7, V8);
	}
	VECTOR getMin(){
		return V1;
	}
	VECTOR getMax(){
		return V7;
	}
};

class Plain{
private:
	Quad Q;
	VECTOR D;
	VECTOR S;
public:
	Plain(){
	}
	Plain(Quad q, VECTOR d, VECTOR s){
		Q = q;
		D = d;
		S = s;
	}
	Quad getQ(){
		return Q;
	}
	VECTOR getD(){
		return D;
	}
	VECTOR getS(){
		return S;
	}
};

Plain plain;
Plain wall1;
Plain wall2;
Plain mirror1;
Plain mirror2;
Plain celling;
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
					if(fx < minx){
						minx = fx;
					}
					if(fx > maxx){
						maxx = fx;
					}
					if(fy < miny){
						miny = fy;
					}
					if(fy > maxy){
						maxy = fy;
					}
					if(fz < minz){
						minz = fz;
					}
					if(fz > maxz){
						maxz = fz;
					}





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
			for(int i=0; i<faceNum; i++){
				VECTOR v1;
				VECTOR v2;
				VECTOR v3;
				VECTOR normal;
				v1 = vertexList[faceList[i].getV1()-1].getV();
				v2 = vertexList[faceList[i].getV2()-1].getV();
				v3 = vertexList[faceList[i].getV3()-1].getV();
				normal = ((const VECTOR)(v2-v1)).CrossProduct(v3-v1);
				normal = normal / normal.Magnitude();
				faceList[i].setN(normal);
			}
			for(int i=0; i<vertexNum; i++){
				int normalNum = 0;
				int list[5000];
				VECTOR vn;
				VECTOR normal;
				for(int j=0; j<faceNum; j++){
					if(faceList[j].getV1() == (i+1) || faceList[j].getV2() == (i+1) || faceList[j].getV3() == (i+1) || faceList[j].getV4() == (i+1)){
						list[normalNum] = j;
						normalNum+=1;
					}
				}
				for(int k=0; k<normalNum; k++){
					normal = faceList[list[k]].getNormal();
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
	Cube getCube(){
		VECTOR V1 = VECTOR(minx, miny, minz);
		VECTOR V2 = VECTOR(maxx, miny, minz);
		VECTOR V3 = VECTOR(maxx, maxy, minz);
		VECTOR V4 = VECTOR(minx, maxy, minz);
		VECTOR V5 = VECTOR(minx, miny, maxz);
		VECTOR V6 = VECTOR(maxx, miny, maxz);
		VECTOR V7 = VECTOR(maxx, maxy, maxz);
		VECTOR V8 = VECTOR(minx, maxy, maxz);
		return Cube(V1, V2, V3, V4, V5, V6, V7, V8);
	}
	
	GLfloat getMinx(){
		return minx;
	}
	GLfloat getMiny(){
		return miny;
	}
	GLfloat getMinz(){
		return minz;
	}
	GLfloat getMaxx(){
		return maxx;
	}
	GLfloat getMaxy(){
		return maxy;
	}
	GLfloat getMaxz(){
		return maxz;
	}

};

Mesh mesh;

class Sphere{
private:
	VECTOR location;
	GLfloat R;
	VECTOR D;
	VECTOR S;
	GLfloat refraction;
public:
	Sphere(){
	}
	Sphere(VECTOR l, GLfloat r, VECTOR d, VECTOR s, GLfloat ref){
		location = l;
		R = r;
		D = d;
		S = s;
		refraction = ref;
	}
	VECTOR getLocation(){
		return location;
	}
	GLfloat getR(){
		return R;
	}
	VECTOR getD(){
		return D;
	}
	VECTOR getS(){
		return S;
	}
	GLfloat getRefraction(){
		return refraction;
	}
	Cube getCube(){
		GLfloat minx, miny, minz, maxx, maxy, maxz;
		minx = location.x-R;
		maxx = location.x+R;
		miny = location.y-R;
		maxy = location.y+R;
		minz = location.z-R;
		maxz = location.z+R;
		VECTOR V1 = VECTOR(minx, miny, minz);
		VECTOR V2 = VECTOR(maxx, miny, minz);
		VECTOR V3 = VECTOR(maxx, maxy, minz);
		VECTOR V4 = VECTOR(minx, maxy, minz);
		VECTOR V5 = VECTOR(minx, miny, maxz);
		VECTOR V6 = VECTOR(maxx, miny, maxz);
		VECTOR V7 = VECTOR(maxx, maxy, maxz);
		VECTOR V8 = VECTOR(minx, maxy, maxz);
		return Cube(V1, V2, V3, V4, V5, V6, V7, V8);
	}
};

Sphere sphere1;
Sphere sphere2;
Sphere sphere3;

class Light{
private:
	VECTOR direction;
	VECTOR intensity;
public:
	Light(){
	}
	Light(VECTOR d, VECTOR i){
		direction = d;
		direction = direction / direction.Magnitude();
		intensity = i;
	}
	void setDirection(VECTOR d){
		direction = d;
	}
	void setIntensity(VECTOR i){
		intensity = i;
	}
	VECTOR getDirection(){
		return direction;
	}
	VECTOR getIntensity(){
		return intensity;
	}
	VECTOR reflected(VECTOR n){
		VECTOR R;
		R = ( 2 * (-1) * n.InnerProduct(direction) ) * n  + direction;
	}
	VECTOR getColor(VECTOR color, VECTOR V, VECTOR N, VECTOR D, VECTOR S){
		VECTOR R;
		float NL;
		float VR;
		V = (-1)*(V / V.Magnitude());
		if(N.InnerProduct(direction) < 0){

			R = ( 2 * (-1) * N.InnerProduct(direction) ) * N  + direction;
		}
		else{
			R.x = 0;
			R.y = 0;
			R.z = 0;
		}
		NL = N.InnerProduct((-1) * direction) ;
		VR = (-1)*V.InnerProduct(R);
		if(NL < 0){
		}
		else{
			color = color + (D * ( NL * intensity ));
		}
		if(VR > 0){
		}
		else{
			color = color + (S * ( pow((-1)*VR,1) * intensity));
		}
		return color;
	}
};
class Spotlight{
private:
	VECTOR direction;
	VECTOR intensity;
	GLfloat cutoff;
	VECTOR locate;
	GLfloat K0;
	GLfloat K1;
	GLfloat K2;
public:
	Spotlight(){
		Light();
	}
	Spotlight(VECTOR d, VECTOR i, GLfloat c, VECTOR l, GLfloat k0, GLfloat k1, GLfloat k2){
		cutoff = c;
		locate = l;
		K0 = k0;
		K1 = k1;
		K2 = k2;
		direction = d;
		direction = direction / direction.Magnitude();
		intensity = i;
	}
	void setDirection(VECTOR d){
		direction = d;
	}
	void setIntensity(VECTOR i){
		intensity = i;
	}
	void setLocate(VECTOR l){
		locate = l;
	}
	void setCutoff(GLfloat c){
		cutoff = c;
	}
	VECTOR getDirection(){
		return direction;
	}
	VECTOR getIntensity(){
		return intensity;
	}
	VECTOR getLocate(){
		return locate;
	}
	GLfloat getCutoff(){
		return cutoff;
	}
	VECTOR getColor(VECTOR color, VECTOR V, VECTOR N, VECTOR D, VECTOR S, VECTOR P){
		VECTOR newI;
		VECTOR R;
		GLfloat d;
		GLfloat NL;
		GLfloat VR;
		VECTOR newd;
		d = ((const VECTOR)(P - locate)).Magnitude();
		newd = (P - locate) / d;
		
		newI = intensity / (K0 + K1*d + K2*pow(d,2));

		if(newd.InnerProduct(direction) < cos(cutoff)){
			return color;
		}
		
		if( cutoff < Pi/2 ) {
			newI = pow(newd.InnerProduct(direction), 10) * newI;
		}
		V = V / V.Magnitude();
		if(N.InnerProduct(newd) < 0){

			R = ( 2 * (-1) * N.InnerProduct(newd) ) * N  + newd;
		}
		else{
			R.x = 0;
			R.y = 0;
			R.z = 0;
		}
		NL = N.InnerProduct((-1) * newd) ;
		VR = (-1)*V.InnerProduct(R);
		if(NL < 0){
		}
		else{
			color = color + (D * ( NL * newI ));
		}
		if(VR > 0){
		}
		else{
			color = color + (S * ( pow((-1)*VR,1) * newI));
		}
		return color;
	}

};


class Ray{
private:
	VECTOR start;
	VECTOR direction;
public:
	Ray(){
	}
	Ray(VECTOR s, VECTOR d){
		start = s;
		direction = d;
	}
	VECTOR getStart(){
		return start;
	}
	VECTOR getDirection(){
		return direction;
	}
};

class Camera{
private:
	VECTOR position;
	VECTOR toward;
	GLfloat povX;
	GLfloat povY;
	GLfloat distance;
	VECTOR P00;
	VECTOR P01;
	VECTOR P10;
	VECTOR P11;
public:
	Camera(){
	}
	Camera(VECTOR p, VECTOR t, GLfloat x, GLfloat y, GLfloat d){
		position = p;
		toward = t;
		povX = x;
		povY = y;
		distance = d;
		VECTOR P;
		P = position + distance * (toward / toward.Magnitude());
		P00 = VECTOR(P.x - distance * tan(povX), P.y - distance * tan(povY), P.z);
		P01 = VECTOR(P.x - distance * tan(povX), P.y + distance * tan(povY), P.z);
		P10 = VECTOR(P.x + distance * tan(povX), P.y - distance * tan(povY), P.z);
		P11 = VECTOR(P.x + distance * tan(povX), P.y + distance * tan(povY), P.z);
	}
	Ray getRay(GLfloat x, GLfloat y, GLfloat resX, GLfloat resY){
		VECTOR direction;
		VECTOR P;
		P = P00 + (x/resX) * (P10-P00) + (y/resY) * (P01 - P00);
		return Ray(position, P - position);
	
	}
	void rotate(GLfloat x){
		position.RotateY(x);
		toward.RotateY((-1)*x);
		P00.RotateY(x);
		P01.RotateY(x);
		P10.RotateY(x);
		P11.RotateY(x);

	}


};
/*
class Trace{
private:
	VECTOR P;
	int f;
	VECTOR color; 
public:
	Trace(){
	}
	Trace(VECTOR p, int f_, VECTOR c){
		P = p;
		f = f_;
		color = c;
	}
	VECTOR getP(){
		return P;
	}
	int getF(){
		return f;
	}
	VECTOR getColor(){
		return color;
	}
};
*/

Light light;
Spotlight point;
Spotlight spot;
Camera camera;
VECTOR* viewPlane;
/*
void DoTimer(int value){
	glutPostRedisplay();
	glutTimerFunc(30, DoTimer, 1);
}
*/



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

boolean inside(VECTOR t1, VECTOR t2, VECTOR t3, VECTOR p){
	//GLfloat a, b, c, d, e, f, g, h, i;
	//GLfloat M;
	GLfloat alpha, beta, gamma;
	VECTOR u, v, n, w;
	u = t2 - t1;
	v = t3 - t1;
	n = u.CrossProduct(v);
	w = p - t1;
	gamma = ((u.CrossProduct(w).InnerProduct(n))/pow(n.Magnitude(), 2));
	beta = ((w.CrossProduct(v).InnerProduct(n))/pow(n.Magnitude(), 2));
	alpha = 1-gamma-beta;
	if( alpha <0 || alpha >1 || beta <0 || beta >1 || gamma <0 || gamma >1){
		return false;
	}
	else{
		return true;
	}
	/*
	if( (p.x > t1.x+0.1 && p.x > t2.x+0.1 && p.x > t3.x+0.1) || (p.y > t1.y+0.1 && p.y > t2.y+0.1 && p.y > t3.y+0.1) || (p.z > t1.z+0.1 && p.z > t2.z+0.1 && p.z > t3.z+0.1) || (p.x < t1.x-0.1 && p.x < t2.x-0.1 && p.x < t3.x-0.1) || (p.y < t1.y-0.1 && p.y < t2.y-0.1 && p.y < t3.y-0.1) || (p.z < t1.z-0.1 && p.z < t2.z-0.1 && p.z < t3.z-0.1)){
		return false;
	}
	a = t2.x - t1.x;
	b = t3.x - t1.x;
	c = t1.x - p.x+1;
	d = t2.y - t1.y;
	e = t3.y - t1.y;
	f = t1.y - p.y+1;
	g = t2.z - t1.z;
	h = t3.z - t1.z;
	i = t1.z - p.z+1;
	M = a*e*i - a*f*h - b*d*i + b*f*g + c*d*h - c*e*g;
	if( a==0 && b==0 ){
		alpha = ( (p.y - t1.y) * (t3.z - t1.z) - (p.z - t1.z) * (t3.y - t1.y) ) / ( (t2.y - t1.y) * (t3.z - t1.z) - (t2.z - t1.z) * (t3.y - t1.y) );
		if( (t3.y - t1.y) != 0){
			beta = ( (p.y - t1.y) - alpha * (t2.y - t1.y) ) / (t3.y - t1.y);
		}
		else if( (t3.z - t1.z) != 0){
			beta = ( (p.z - t1.z) - alpha * (t2.z - t1.z) ) / (t3.z - t1.z);
		}
		else{
			return false;
		}
	}
	else if( d==0 && e==0 ){
		alpha = ( (p.z - t1.z) * (t3.x - t1.x) - (p.x - t1.x) * (t3.z - t1.z) ) / ( (t2.z - t1.z) * (t3.x - t1.x) - (t2.x - t1.x) * (t3.z - t1.z) );
		if( (t3.x - t1.x) != 0){
			beta = ( (p.x - t1.x) - alpha * (t2.x - t1.x) ) / (t3.x - t1.x);
		}
		else if( (t3.z - t1.z) != 0){
			beta = ( (p.z - t1.z) - alpha * (t2.z - t1.z) ) / (t3.z - t1.z);
		}
		else{
			return false;
		}
	}
	else if( g==0 && h==0 ){
		alpha = ( (p.x - t1.x) * (t3.y - t1.y) - (p.y - t1.y) * (t3.x - t1.x) ) / ( (t2.x - t1.x) * (t3.y - t1.y) - (t2.y - t1.y) * (t3.x - t1.x) );
		if( (t3.x - t1.x) != 0){
			beta = ( (p.x - t1.x) - alpha * (t2.x - t1.x) ) / (t3.x - t1.x);
		}
		else if( (t3.y - t1.y) != 0){
			beta = ( (p.y - t1.y) - alpha * (t2.y - t1.y) ) / (t3.y - t1.y);
		}
		else{
			return false;
		}
	}
	else if( M != 0 ){
		alpha = ((e*i - f*h) + (c*h - b*i) + (b*f - c*e)) / M;
		beta = ((f*g - d*i) + (a*i - c*g) + (c*d - a*f)) / M;
	}
	else{
		return false;
	}
	if( alpha<0 || alpha>1 || beta<0 || beta>1 || (alpha+beta) > 1){
		return false;
	}
	else{
		return true;
	}
	*/
}
boolean inCube(Ray r, Cube c){
	VECTOR N;
	VECTOR P;
	VECTOR t1;
	VECTOR t2;
	VECTOR t3;
	VECTOR t4;
	GLfloat* t;
	t = new GLfloat[6];
	VECTOR V;
	Quad F;
	GLfloat d;
	V = r.getDirection() / r.getDirection().Magnitude();
	/*
	if(r.getStart().x <= c.getMax().x && r.getStart().y <= c.getMax().y && r.getStart().z <= c.getMax().z && r.getStart().x >= c.getMin().x && r.getStart().y >= c.getMin().y && r.getStart().z >= c.getMin().z){
		delete[] t;
		return true;
	}*/
	for(int i=0; i<6; i++){
		switch(i){
		case 0:
			F = c.getF1();
			break;
		case 1:
			F = c.getF2();
			break;
		case 2:
			F = c.getF3();
			break;
		case 3:
			F = c.getF4();
			break;
		case 4:
			F = c.getF5();
			break;
		case 5:
			F = c.getF6();
			break;
		}
		N = F.getNormal();
		t1 = F.getV1();
		t2 = F.getV2();
		t3 = F.getV3();
		t4 = F.getV4();
		//if(V.InnerProduct(N) <0){
			d = (-1) * t1.InnerProduct(N);
			t[i] = (-1) * (r.getStart().InnerProduct(N) + d) / (V.InnerProduct(N));
			P = r.getStart() + t[i]*V;
			if( (inside(t1, t2, t3, P) || inside(t3, t4, t1, P) && t[i] > 0 )){
				delete[] t;
				return true;
			}
		//}
	}
	delete[] t;

	return false;

}


Light traceLight(Ray r, GLfloat tl, Light l){
	Face f;
	GLfloat T;
	GLfloat t;
	int n=0;
	GLfloat d;
	VECTOR V;
	VECTOR N;
	VECTOR P;
	VECTOR R;
	VECTOR t1, t2, t3, t4;
	VECTOR color;
	GLfloat b, c;
	V = r.getDirection() / r.getDirection() .Magnitude();
	T = 0;
	if( inCube(r, mesh.getCube())){
		for(int i=0; i<faceNum; i++){
			N = mesh.getFace(i).getNormal();
			t1 = mesh.getVertex(mesh.getFace(i).getV1()-1).getV();
			t2 = mesh.getVertex(mesh.getFace(i).getV2()-1).getV();
			t3 = mesh.getVertex(mesh.getFace(i).getV3()-1).getV();
			if(V.InnerProduct(N) <0){
				d = (-1) * t1.InnerProduct(N);
				t = (-1) * (r.getStart().InnerProduct(N) + d) / (V.InnerProduct(N));
				P = r.getStart() + t*V;
				if(mesh.getFace(i).getV4()==0){
					if( inside( t1, t2, t3, P) && t< tl && t>0){
						l.setIntensity(VECTOR(0,0,0));
						return l;
					}
				}
				else{
					t4 = mesh.getVertex(mesh.getFace(i).getV4()-1).getV();
					if( (inside(t1, t2, t3, P) || inside(t3, t4, t1, P))  && t< tl && t>0){
						l.setIntensity(VECTOR(0,0,0));
						return l;
					}
				}
			}
			else{
				N = (-1) * N;
				d = (-1) * t1.InnerProduct(N);
				t = (-1) * (r.getStart().InnerProduct(N) + d) / (V.InnerProduct(N));
				P = r.getStart() + t*V;
				if(mesh.getFace(i).getV4()==0){
					if( inside( t1, t2, t3, P) && t< tl && t>0){
						l.setIntensity(VECTOR(0,0,0));
						return l;
					}
				}
				else{
					t4 = mesh.getVertex(mesh.getFace(i).getV4()-1).getV();
					if( (inside(t1, t2, t3, P) || inside(t3, t4, t1, P))  && t< tl && t>0){
						l.setIntensity(VECTOR(0,0,0));
						return l;
					}
				}
			}
		}
	}
	b = 2 * V.InnerProduct(r.getStart() - sphere1.getLocation());
	c = pow(((const VECTOR)(r.getStart() - sphere1.getLocation())).Magnitude(),2) - sphere1.getR();

	if( (pow(b,2) - 4*c) < 0 ){
		t = 0;
	}
	else{
		t = ((-1)*b - pow((pow(b,2) - 4*c), 0.5))/2;
	}

	if(  t< tl && t>0 ){
		l.setIntensity(VECTOR(0,0,0));
		return l;
	}
		
	b = 2 * V.InnerProduct(r.getStart() - sphere2.getLocation());
	c = pow(((const VECTOR)(r.getStart() - sphere2.getLocation())).Magnitude(),2) - sphere2.getR();

	if( (pow(b,2) - 4*c) < 0 ){
		t = 0;
	}
	else{
		t = ((-1)*b - pow((pow(b,2) - 4*c), 0.5))/2;
	}

	if(  t< tl && t>0 ){
		l.setIntensity(VECTOR(0,0,0));
		return l;
	}

	
	b = 2 * V.InnerProduct(r.getStart() - sphere3.getLocation());
	c = pow(((const VECTOR)(r.getStart() - sphere3.getLocation())).Magnitude(),2) - sphere3.getR();

	if( (pow(b,2) - 4*c) < 0 ){
		t = 0;
	}
	else{
		t = ((-1)*b - pow((pow(b,2) - 4*c), 0.5))/2;
	}
	if( t< tl && t < 0 && (((-1)*b + pow((pow(b,2) - 4*c), 0.5))/2) > 0){
		t = ((-1)*b + pow((pow(b,2) - 4*c), 0.5))/2;
		P = r.getStart() + t*V;
		N = (P - sphere3.getLocation()) / sphere3.getR();
		if( V.CrossProduct((-1)*N).Magnitude() <= 1/sphere3.getRefraction()){
			R = ( ( (sphere3.getRefraction())*((-1)*N.InnerProduct(V)) - pow(pow((sphere3.getRefraction()), 2)*(1-pow((-1)*N.InnerProduct(V),2)), 0.5)) * (-1)*N + (sphere3.getRefraction())*V);
			//l.setIntensity(pow((1/sphere3.getRefraction()), 2)*(1-pow(N.InnerProduct(V),2))*traceLight(Ray(P+0.1*R, R), 100, l).getIntensity());
			l.setIntensity(traceLight(Ray(P+0.1*R, R), 100, l).getIntensity());
			return l;
		}
		else{
			l.setIntensity(VECTOR(0,0,0));
			return l;
		}
	}
	else if( t< tl && t > 0){
		P = r.getStart() + t*V;
		N = (P - sphere3.getLocation()) / sphere3.getR();
		R = ( ( (1/sphere3.getRefraction())*(N.InnerProduct(V)) - pow(pow((1/sphere3.getRefraction()), 2)*(1-pow(N.InnerProduct(V),2)), 0.5)) * N + (1/sphere3.getRefraction())*V);
		l.setIntensity(pow((1/sphere3.getRefraction()), 2)*(1-pow(N.InnerProduct(V),2))*traceLight(Ray(P+0.1*R, R), 100, l).getIntensity());
		return l;
	}
	return l;
}
Spotlight traceSpot(Ray r, GLfloat tl, Spotlight l){
	Face f;
	GLfloat T;
	GLfloat t;
	int n=0;
	GLfloat d;
	VECTOR V;
	VECTOR N;
	VECTOR P;
	VECTOR R;
	VECTOR t1, t2, t3, t4;
	VECTOR color;
	GLfloat b, c;
	V = r.getDirection() / r.getDirection() .Magnitude();
	T = 0;
	if( inCube(r, mesh.getCube())){
		for(int i=0; i<faceNum; i++){
			N = mesh.getFace(i).getNormal();
			t1 = mesh.getVertex(mesh.getFace(i).getV1()-1).getV();
			t2 = mesh.getVertex(mesh.getFace(i).getV2()-1).getV();
			t3 = mesh.getVertex(mesh.getFace(i).getV3()-1).getV();
			//if(V.InnerProduct(N) <0){
				d = (-1) * t1.InnerProduct(N);
				t = (-1) * (r.getStart().InnerProduct(N) + d) / (V.InnerProduct(N));
				P = r.getStart() + t*V;
				if(mesh.getFace(i).getV4()==0){
					if( inside( t1, t2, t3, P) && t< tl && t>0){
						l.setIntensity(VECTOR(0,0,0));
						return l;
					}
				}
				else{
					t4 = mesh.getVertex(mesh.getFace(i).getV4()-1).getV();
					if( (inside(t1, t2, t3, P) || inside(t3, t4, t1, P))  && t< tl && t>0){
						l.setIntensity(VECTOR(0,0,0));
						return l;
					}
				}
			//}
		}
	}
	b = 2 * V.InnerProduct(r.getStart() - sphere1.getLocation());
	c = pow(((const VECTOR)(r.getStart() - sphere1.getLocation())).Magnitude(),2) - sphere1.getR();

	if( (pow(b,2) - 4*c) < 0 ){
		t = 0;
	}
	else{
		t = ((-1)*b - pow((pow(b,2) - 4*c), 0.5))/2;
	}

	if(  t< tl && t>0 ){
		l.setIntensity(VECTOR(0,0,0));
		return l;
	}
		
	b = 2 * V.InnerProduct(r.getStart() - sphere2.getLocation());
	c = pow(((const VECTOR)(r.getStart() - sphere2.getLocation())).Magnitude(),2) - sphere2.getR();

	if( (pow(b,2) - 4*c) < 0 ){
		t = 0;
	}
	else{
		t = ((-1)*b - pow((pow(b,2) - 4*c), 0.5))/2;
	}

	if(  t< tl && t>0 ){
		l.setIntensity(VECTOR(0,0,0));
		return l;
	}

	
	b = 2 * V.InnerProduct(r.getStart() - sphere3.getLocation());
	c = pow(((const VECTOR)(r.getStart() - sphere3.getLocation())).Magnitude(),2) - sphere3.getR();

	if( (pow(b,2) - 4*c) < 0 ){
		t = 0;
	}
	else{
		t = ((-1)*b - pow((pow(b,2) - 4*c), 0.5))/2;
	}
	if( t< tl && t < 0  && (((-1)*b + pow((pow(b,2) - 4*c), 0.5))/2) > 0){
		t = ((-1)*b + pow((pow(b,2) - 4*c), 0.5))/2;
		P = r.getStart() + t*V;
		N = (P - sphere3.getLocation()) / sphere3.getR();
		if( V.CrossProduct((-1)*N).Magnitude() <= 1/sphere3.getRefraction()){
			R = ( ( (sphere3.getRefraction())*((-1)*N.InnerProduct(V)) - pow(pow((sphere3.getRefraction()), 2)*(1-pow((-1)*N.InnerProduct(V),2)), 0.5)) * (-1)*N + (sphere3.getRefraction())*V);
			//l.setIntensity(pow((1/sphere3.getRefraction()), 2)*(1-pow(N.InnerProduct(V),2))*traceSpot(Ray(P + 0.1*R, R), 100, l).getIntensity());
			l.setIntensity(traceSpot(Ray(P + 0.1*R, R), 100, l).getIntensity());
			return l;
		}
		else{
			l.setIntensity(VECTOR(0,0,0));
			return l;
		}
	}
	else if( t< tl && t > 0){
		P = r.getStart() + t*V;
		N = (P - sphere3.getLocation()) / sphere3.getR();
		R = ( ( (1/sphere3.getRefraction())*(N.InnerProduct(V)) - pow(pow((1/sphere3.getRefraction()), 2)*(1-pow(N.InnerProduct(V),2)), 0.5)) * N + (1/sphere3.getRefraction())*V);
		l.setIntensity(pow((1/sphere3.getRefraction()), 2)*(1-pow(N.InnerProduct(V),2))*traceSpot(Ray(P + 0.1*R, R), 100, l).getIntensity());
		return l;
	}

	return l;
}


VECTOR trace(Ray r, int tracedNum){
	Light temp;
	Spotlight tempS;
	Face f;
	GLfloat* t;
	t = new GLfloat[faceNum+4];
	GLfloat T;
	int n=0;
	GLfloat d;
	VECTOR V;
	VECTOR N;
	VECTOR P;
	VECTOR R;
	VECTOR t1, t2, t3, t4;
	VECTOR color;
	GLfloat b, c;
	V = r.getDirection() / r.getDirection() .Magnitude();
	T = 0;
	if(tracedNum > MAX_REFLECT){
		delete[] t;
		return VECTOR(0,0,0);
	}
	if( inCube(r, mesh.getCube()) ){
		for(int i=0; i<faceNum; i++){
			N = mesh.getFace(i).getNormal();
			t1 = mesh.getVertex(mesh.getFace(i).getV1()-1).getV();
			t2 = mesh.getVertex(mesh.getFace(i).getV2()-1).getV();
			t3 = mesh.getVertex(mesh.getFace(i).getV3()-1).getV();
			if(V.InnerProduct(N) <0){
				d = (-1) * t1.InnerProduct(N);
				t[i] = (-1) * (r.getStart().InnerProduct(N) + d) / (V.InnerProduct(N));
				P = r.getStart() + t[i]*V;
				if(mesh.getFace(i).getV4()==0){
					if( !inside( t1, t2, t3, P) ){
						t[i] = 0;
					}
				}
				else{
					t4 = mesh.getVertex(mesh.getFace(i).getV4()-1).getV();
					if( !(inside(t1, t2, t3, P) || inside(t3, t4, t1, P)) ){
						t[i] = 0;
					}
				}
			}
		}
	}
	
	b = 2 * V.InnerProduct(r.getStart() - sphere1.getLocation());
	c = pow(((const VECTOR)(r.getStart() - sphere1.getLocation())).Magnitude(),2) - sphere1.getR();

	if( (pow(b,2) - 4*c) < 0 ){
		t[faceNum] = 0;
	}
	else{
		t[faceNum] = ((-1)*b - pow((pow(b,2) - 4*c), 0.5))/2;
	}

		
	b = 2 * V.InnerProduct(r.getStart() - sphere2.getLocation());
	c = pow(((const VECTOR)(r.getStart() - sphere2.getLocation())).Magnitude(),2) - sphere2.getR();

	if( (pow(b,2) - 4*c) < 0 ){
		t[faceNum+1] = 0;
	}
	else{
		t[faceNum+1] = ((-1)*b - pow((pow(b,2) - 4*c), 0.5))/2;
	}

	
	b = 2 * V.InnerProduct(r.getStart() - sphere3.getLocation());
	c = pow(((const VECTOR)(r.getStart() - sphere3.getLocation())).Magnitude(),2) - sphere3.getR();

	if( (pow(b,2) - 4*c) < 0 ){
		t[faceNum+2] = 0;
	}
	else{
		t[faceNum+3] = ((-1)*b + pow((pow(b,2) - 4*c), 0.5))/2;
		t[faceNum+2] = ((-1)*b - pow((pow(b,2) - 4*c), 0.5))/2;
		if(t[faceNum+2] < 0.1){
			t[faceNum+2] = 0;
		}
		if(t[faceNum+3] < 0.1){
			t[faceNum+3] = 0;
		}
	}

	for(int i=0; i<faceNum+4; i++){
		if( t[i] > 0 ){
			if( T == 0 ){
				T = t[i];
				n = i;
			}
			else{
				if( t[i] < T ){
					T = t[i];
					n = i;
				}
			}

		}

	}
	P = r.getStart() + t[n]*V;
	delete[] t;
	
	if( T > 0 ){
		if(n == faceNum){
			N = (P - sphere1.getLocation()) / sphere1.getR();
			//if(!intersection(Ray(P, (-1)*light.getDirection()), 100)){
			temp = traceLight(Ray(P, (-1)*light.getDirection()), 100, light);
				color = color + temp.getColor(VECTOR(0,0,0), V, N, sphere1.getD(), sphere1.getS());
			//}
			//if(!intersection(Ray(P, point.getLocate()-P), ((const VECTOR)(point.getLocate() - P)).Magnitude())){
			tempS = traceSpot(Ray(P, point.getLocate()-P), ((const VECTOR)(point.getLocate() - P)).Magnitude(), point);
				color = color + tempS.getColor(VECTOR(0,0,0), V, N, sphere1.getD(), sphere1.getS(), P);
			//}
			//if(!intersection(Ray(P, spot.getLocate()-P), ((const VECTOR)(spot.getLocate() - P)).Magnitude())){
			tempS = traceSpot(Ray(P, spot.getLocate()-P), ((const VECTOR)(spot.getLocate() - P)).Magnitude(), spot);
				color = color + tempS.getColor(VECTOR(0,0,0), V, N, sphere1.getD(), sphere1.getS(), P);
			//}
			R = ( 2 * (-1) * N.InnerProduct(V) ) * N  + V;
			temp = Light((-1)*R, trace(Ray(P, R), tracedNum+1));
			color = color + 0.5 * temp.getColor(VECTOR(0, 0, 0), V, N, sphere1.getD(), sphere1.getS());
			//color = color + 0.1*trace(Ray(P, R), tracedNum+1).getColor();
			return color;
		}
		else if(n == faceNum+1){
			N = (P - sphere2.getLocation()) / sphere2.getR();
			//if(!intersection(Ray(P, (-1)*light.getDirection()), 100)){
			temp = traceLight(Ray(P, (-1)*light.getDirection()), 100, light);
				color = color + temp.getColor(VECTOR(0,0,0), V, N, sphere2.getD(), sphere2.getS());
			//}
			//if(!intersection(Ray(P, point.getLocate()-P), ((const VECTOR)(point.getLocate() - P)).Magnitude())){
			tempS = traceSpot(Ray(P, point.getLocate()-P), ((const VECTOR)(point.getLocate() - P)).Magnitude(), point);
				color = color + tempS.getColor(VECTOR(0,0,0), V, N, sphere2.getD(), sphere2.getS(), P);
			//}
			//if(!intersection(Ray(P, spot.getLocate()-P), ((const VECTOR)(spot.getLocate() - P)).Magnitude())){
			tempS = traceSpot(Ray(P, spot.getLocate()-P), ((const VECTOR)(spot.getLocate() - P)).Magnitude(), spot);
				color = color + tempS.getColor(VECTOR(0,0,0), V, N, sphere2.getD(), sphere2.getS(), P);
			//}
			R = ( 2 * (-1) * N.InnerProduct(V) ) * N  + V;
			temp = Light((-1)*R, trace(Ray(P, R), tracedNum+1));
			color = color + 0.5 * temp.getColor(VECTOR(0, 0, 0), V, N, sphere2.getD(), sphere2.getS());
			//color = color + 0.1*trace(Ray(P, R), tracedNum+1).getColor();
			return color;
		}
		
		else if(n == faceNum+2){
			N = (P - sphere3.getLocation()) / sphere3.getR();
			//if(!intersection(Ray(P, (-1)*light.getDirection()), 100)){
			temp = traceLight(Ray(P + (-0.1)*light.getDirection(), (-1)*light.getDirection()), 100, light);
				color = color + temp.getColor(VECTOR(0,0,0), V, N, sphere3.getD(), sphere3.getS());
			//}
			//if(!intersection(Ray(P, point.getLocate()-P), ((const VECTOR)(point.getLocate() - P)).Magnitude())){
			tempS = traceSpot(Ray(P + 0.1*(point.getLocate()-P), point.getLocate()-P), ((const VECTOR)(point.getLocate() - P)).Magnitude(), point);
				color = color + tempS.getColor(VECTOR(0,0,0), V, N, sphere3.getD(), sphere3.getS(), P);
			//}
			//if(!intersection(Ray(P, spot.getLocate()-P), ((const VECTOR)(spot.getLocate() - P)).Magnitude())){
			tempS = traceSpot(Ray(P + 0.1*(spot.getLocate()-P), spot.getLocate()-P), ((const VECTOR)(spot.getLocate() - P)).Magnitude(), spot);
				color = color + tempS.getColor(VECTOR(0,0,0), V, N, sphere3.getD(), sphere3.getS(), P);
			//}
			
			
			R = ( ( (1/sphere3.getRefraction())*(N.InnerProduct(V)) - pow(pow((1/sphere3.getRefraction()), 2)*(1-pow(N.InnerProduct(V),2)), 0.5)) * N + (1/sphere3.getRefraction())*V);
			
			color = color + 0.7 * trace(Ray(P, R), tracedNum);
			
			R = ( 2 * (-1) * N.InnerProduct(V) ) * N  + V;
			temp = Light((-1)*R, trace(Ray(P, R), tracedNum+1));
			color = color + 0.15 * temp.getColor(VECTOR(0, 0, 0), V, N, sphere3.getD(), sphere3.getS());
			//color = color + 0.1*trace(Ray(P, R), tracedNum+1).getColor();
			return color;
		}
		else if(n == faceNum+3){
			N = (P - sphere3.getLocation()) / sphere3.getR();
			if( V.CrossProduct((-1)*N).Magnitude() <= 1/sphere3.getRefraction()){
				R = ( ( (sphere3.getRefraction())*((-1)*N.InnerProduct(V)) - pow(pow((sphere3.getRefraction()), 2)*(1-pow((-1)*N.InnerProduct(V),2)), 0.5)) * (-1)*N + (sphere3.getRefraction())*V);
				color = color + 0.9 * trace(Ray(P, R), tracedNum+1);
			}
			else{
				R = (-1) * ( 2 * N.InnerProduct(V) ) * N  + V;
				color = color + trace(Ray(P, R), tracedNum+1);
			}
			//color = color + 0.1*trace(Ray(P, R), tracedNum+1).getColor();
			return color;
		}
		else{
			f = mesh.getFace(n);
			for(int i=0; i<mtlNum; i++){
				if(n >= mesh.getMtl(i).getX() && n<= mesh.getMtl(i).getY()){
					N = f.getNormal();
					//if(!intersection(Ray(P, (-1)*light.getDirection()), 100)){
						temp = traceLight(Ray(P + (-0.1)*light.getDirection(), (-1)*light.getDirection()), 100, light);
						color = color + temp.getColor(VECTOR(0,0,0), V, N, mesh.getMtl(i).getD(), mesh.getMtl(i).getS());
					//}
					//if(!intersection(Ray(P, point.getLocate()-P), ((const VECTOR)(point.getLocate() - P)).Magnitude())){
						tempS = traceSpot(Ray(P + (0.1)*(point.getLocate()-P), point.getLocate()-P), ((const VECTOR)(point.getLocate() - P)).Magnitude(), point);
						color = color + tempS.getColor(VECTOR(0,0,0), V, N, mesh.getMtl(i).getD(), mesh.getMtl(i).getS(), P);
					//}
					//if(!intersection(Ray(P, spot.getLocate()-P), ((const VECTOR)(spot.getLocate() - P)).Magnitude())){
						tempS = traceSpot(Ray(P + (0.1)*(spot.getLocate()-P), spot.getLocate()-P), ((const VECTOR)(spot.getLocate() - P)).Magnitude(), spot);
						color = color + tempS.getColor(VECTOR(0,0,0), V, N, mesh.getMtl(i).getD(), mesh.getMtl(i).getS(), P);
					//}
					R = ( 2 * (-1) * N.InnerProduct(V) ) * N  + V;
					temp = Light((-1)*R, trace(Ray(P, R), tracedNum+1));
					color = color + 0.1 * temp.getColor(VECTOR(0, 0, 0), V, N, mesh.getMtl(i).getD(), mesh.getMtl(i).getS());
					//color = color + 0.1*trace(Ray(P, R), tracedNum+1).getColor();
					return color;
				}
			}
		}
	}
		
	N = plain.getQ().getNormal();
	t1 = plain.getQ().getV1();
	t2 = plain.getQ().getV2();
	t3 = plain.getQ().getV3();
	t4 = plain.getQ().getV4();
	if(V.InnerProduct(N) <0){
		d = (-1) * t1.InnerProduct(N);
		T = (-1) * (r.getStart().InnerProduct(N) + d) / (V.InnerProduct(N));
		P = r.getStart() + T*V;
		if( (inside(t1, t2, t3, P) || inside(t3, t4, t1, P) && T > 0 )){
			//if(!intersection(Ray(P, (-1)*light.getDirection()), 100)){
				temp = traceLight(Ray(P, (-1)*light.getDirection()), 100, light);
				color = color + temp.getColor(VECTOR(0,0,0), V, N, plain.getD(), plain.getS());
			//}
			//if(!intersection(Ray(P, point.getLocate()-P), ((const VECTOR)(point.getLocate() - P)).Magnitude())){
				tempS = traceSpot(Ray(P, point.getLocate()-P), ((const VECTOR)(point.getLocate() - P)).Magnitude(), point);
				color = color + tempS.getColor(VECTOR(0,0,0), V, N, plain.getD(), plain.getS(), P);
			//}
			//if(!intersection(Ray(P, spot.getLocate()-P), ((const VECTOR)(spot.getLocate() - P)).Magnitude())){
				tempS = traceSpot(Ray(P, spot.getLocate()-P), ((const VECTOR)(spot.getLocate() - P)).Magnitude(), spot);
				color = color + tempS.getColor(VECTOR(0,0,0), V, N, plain.getD(), plain.getS(), P);
			//}
			R = ( 2 * (-1) * N.InnerProduct(V) ) * N  + V;
			temp = Light((-1)*R, trace(Ray(P, R), tracedNum+1));
			color = color + 0.05 * temp.getColor(VECTOR(0, 0, 0), V, N, plain.getD(), plain.getS());
			//color = color + 0.01*trace(Ray(P, R), tracedNum+1).getColor();
			return color;

		}
	}
		
	N = wall1.getQ().getNormal();
	t1 = wall1.getQ().getV1();
	t2 = wall1.getQ().getV2();
	t3 = wall1.getQ().getV3();
	t4 = wall1.getQ().getV4();
	if(V.InnerProduct(N) <0){
		d = (-1) * t1.InnerProduct(N);
		T = (-1) * (r.getStart().InnerProduct(N) + d) / (V.InnerProduct(N));
		P = r.getStart() + T*V;
		if( (inside(t1, t2, t3, P) || inside(t3, t4, t1, P) && T > 0 )){
			//if(!intersection(Ray(P, (-1)*light.getDirection()), 100)){
				temp = traceLight(Ray(P, (-1)*light.getDirection()), 100, light);
				color = color + temp.getColor(VECTOR(0,0,0), V, N, wall1.getD(), wall1.getS());
			//}
			//if(!intersection(Ray(P, point.getLocate()-P), ((const VECTOR)(point.getLocate() - P)).Magnitude())){
				tempS = traceSpot(Ray(P, point.getLocate()-P), ((const VECTOR)(point.getLocate() - P)).Magnitude(), point);
				color = color + tempS.getColor(VECTOR(0,0,0), V, N, wall1.getD(), wall1.getS(), P);
			//}
			//if(!intersection(Ray(P, spot.getLocate()-P), ((const VECTOR)(spot.getLocate() - P)).Magnitude())){
				tempS = traceSpot(Ray(P, spot.getLocate()-P), ((const VECTOR)(spot.getLocate() - P)).Magnitude(), spot);
				color = color + tempS.getColor(VECTOR(0,0,0), V, N, wall1.getD(), wall1.getS(), P);
			//}
			R = ( 2 * (-1) * N.InnerProduct(V) ) * N  + V;
			temp = Light((-1)*R, trace(Ray(P, R), tracedNum+1));
			color = color + 0.05 * temp.getColor(VECTOR(0, 0, 0), V, N, wall1.getD(), wall1.getS());
			//color = color + 0.01*trace(Ray(P, R), tracedNum+1).getColor();
			return color;
		}
	}
			
	N = wall2.getQ().getNormal();
	t1 = wall2.getQ().getV1();
	t2 = wall2.getQ().getV2();
	t3 = wall2.getQ().getV3();
	t4 = wall2.getQ().getV4();
	if(V.InnerProduct(N) <0){
		d = (-1) * t1.InnerProduct(N);
		T = (-1) * (r.getStart().InnerProduct(N) + d) / (V.InnerProduct(N));
		P = r.getStart() + T*V;
		if( (inside(t1, t2, t3, P) || inside(t3, t4, t1, P) && T > 0 )){
			//if(!intersection(Ray(P, (-1)*light.getDirection()), 100)){
				temp = traceLight(Ray(P, (-1)*light.getDirection()), 100, light);
				color = color + temp.getColor(VECTOR(0,0,0), V, N, wall2.getD(), wall2.getS());
			//}
			//if(!intersection(Ray(P, point.getLocate()-P), ((const VECTOR)(point.getLocate() - P)).Magnitude())){
				tempS = traceSpot(Ray(P, point.getLocate()-P), ((const VECTOR)(point.getLocate() - P)).Magnitude(), point);
				color = color + tempS.getColor(VECTOR(0,0,0), V, N, wall2.getD(), wall2.getS(), P);
			//}
			//if(!intersection(Ray(P, spot.getLocate()-P), ((const VECTOR)(spot.getLocate() - P)).Magnitude())){
				tempS = traceSpot(Ray(P, spot.getLocate()-P), ((const VECTOR)(spot.getLocate() - P)).Magnitude(), spot);
				color = color + tempS.getColor(VECTOR(0,0,0), V, N, wall2.getD(), wall2.getS(), P);
			//}
			R = ( 2 * (-1) * N.InnerProduct(V) ) * N  + V;
			temp = Light((-1)*R, trace(Ray(P, R), tracedNum+1));
			color = color + 0.05 * temp.getColor(VECTOR(0, 0, 0), V, N, wall2.getD(), wall2.getS());
			//color = color + 0.01*trace(Ray(P, R), tracedNum+1).getColor();
			return color;
		}
	}

			
	N = mirror1.getQ().getNormal();
	t1 = mirror1.getQ().getV1();
	t2 = mirror1.getQ().getV2();
	t3 = mirror1.getQ().getV3();
	t4 = mirror1.getQ().getV4();
	if(V.InnerProduct(N) <0){
		d = (-1) * t1.InnerProduct(N);
		T = (-1) * (r.getStart().InnerProduct(N) + d) / (V.InnerProduct(N));
		P = r.getStart() + T*V;
		if( (inside(t1, t2, t3, P) || inside(t3, t4, t1, P) && T > 0 )){
			//color = color + getColor(VECTOR(0,0,0), r.getDirection(), N, plain.getD(), plain.getS());
			
			//if(!intersection(Ray(P, point.getLocate()-P), ((const VECTOR)(point.getLocate() - P)).Magnitude())){
				tempS = traceSpot(Ray(P, point.getLocate()-P), ((const VECTOR)(point.getLocate() - P)).Magnitude(), point);
				color = color + tempS.getColor(VECTOR(0,0,0), V, N, mirror1.getD(), mirror1.getS(), P);
			//}
			//if(!intersection(Ray(P, spot.getLocate()-P), ((const VECTOR)(spot.getLocate() - P)).Magnitude())){
				tempS = traceSpot(Ray(P, spot.getLocate()-P), ((const VECTOR)(spot.getLocate() - P)).Magnitude(), spot);
				color = color + tempS.getColor(VECTOR(0,0,0), V, N, mirror1.getD(), mirror1.getS(), P);
			//}
			R = ( 2 * (-1) * N.InnerProduct(V) ) * N  + V;
			temp = Light((-1)*R, trace(Ray(P, R), tracedNum+1));
			color = color + temp.getColor(VECTOR(0, 0, 0), V, N, mirror1.getD(), mirror1.getS());
			return color;
		}
	}

			
			
	N = mirror2.getQ().getNormal();
	t1 = mirror2.getQ().getV1();
	t2 = mirror2.getQ().getV2();
	t3 = mirror2.getQ().getV3();
	t4 = mirror2.getQ().getV4();
	if(V.InnerProduct(N) <0){
		d = (-1) * t1.InnerProduct(N);
		T = (-1) * (r.getStart().InnerProduct(N) + d) / (V.InnerProduct(N));
		P = r.getStart() + T*V;
		if( (inside(t1, t2, t3, P) || inside(t3, t4, t1, P) && T > 0 )){
			//color = color + getColor(VECTOR(0,0,0), r.getDirection(), N, plain.getD(), plain.getS());
			
			//if(!intersection(Ray(P, point.getLocate()-P), ((const VECTOR)(point.getLocate() - P)).Magnitude())){
				tempS = traceSpot(Ray(P, point.getLocate()-P), ((const VECTOR)(point.getLocate() - P)).Magnitude(), point);
				color = color + tempS.getColor(VECTOR(0,0,0), V, N, mirror2.getD(), mirror2.getS(), P);
			//}
			//if(!intersection(Ray(P, spot.getLocate()-P), ((const VECTOR)(spot.getLocate() - P)).Magnitude())){
				tempS = traceSpot(Ray(P, spot.getLocate()-P), ((const VECTOR)(spot.getLocate() - P)).Magnitude(), spot);
				color = color + tempS.getColor(VECTOR(0,0,0), V, N, mirror2.getD(), mirror2.getS(), P);
			//}
			R = ( 2 * (-1) * N.InnerProduct(V) ) * N  + V;
			temp = Light((-1)*R, trace(Ray(P, R), tracedNum+1));
			color = color + temp.getColor(VECTOR(0, 0, 0), V, N, mirror2.getD(), mirror2.getS());
			return color;
		}
	}
	
	N = celling.getQ().getNormal();
	t1 = celling.getQ().getV1();
	t2 = celling.getQ().getV2();
	t3 = celling.getQ().getV3();
	t4 = celling.getQ().getV4();
	if(V.InnerProduct(N) <0){
		d = (-1) * t1.InnerProduct(N);
		T = (-1) * (r.getStart().InnerProduct(N) + d) / (V.InnerProduct(N));
		P = r.getStart() + T*V;
		if( (inside(t1, t2, t3, P) || inside(t3, t4, t1, P) && T > 0 )){
			//if(!intersection(Ray(P, (-1)*light.getDirection()), 100)){
				temp = traceLight(Ray(P, (-1)*light.getDirection()), 100, light);
				color = color + temp.getColor(VECTOR(0,0,0), V, N, celling.getD(), celling.getS());
			//}
			//if(!intersection(Ray(P, point.getLocate()-P), ((const VECTOR)(point.getLocate() - P)).Magnitude())){
				tempS = traceSpot(Ray(P, point.getLocate()-P), ((const VECTOR)(point.getLocate() - P)).Magnitude(), point);
				color = color + tempS.getColor(VECTOR(0,0,0), V, N, celling.getD(), celling.getS(), P);
			//}
			//if(!intersection(Ray(P, spot.getLocate()-P), ((const VECTOR)(spot.getLocate() - P)).Magnitude())){
				tempS = traceSpot(Ray(P, spot.getLocate()-P), ((const VECTOR)(spot.getLocate() - P)).Magnitude(), spot);
				color = color + tempS.getColor(VECTOR(0,0,0), V, N, celling.getD(), celling.getS(), P);
			//}
			R = ( 2 * (-1) * N.InnerProduct(V) ) * N  + V;
			temp = Light((-1)*R, trace(Ray(P, R), tracedNum+1));
			color = color + 0.05 * temp.getColor(VECTOR(0, 0, 0), V, N, celling.getD(), celling.getS());
			//color = color + 0.01*trace(Ray(P, R), tracedNum+1).getColor();
			return color;
		}
	}




	return VECTOR(0,0,0);
	
	/*
	if(T>0){
		return n;
	}
	else{
		return 0;
	}*/

	
}
void ScreenCapture( const char *strFilePath )
{
    //비트맵 파일 처리를 위한 헤더 구조체
    BITMAPFILEHEADER    BMFH;
    BITMAPINFOHEADER    BMIH;
 
    int nWidth = 0;
    int nHeight = 0;
    unsigned long dwQuadrupleWidth = 0;     //LJH 추가, 가로 사이즈가 4의 배수가 아니라면 4의 배수로 만들어서 저장
 
    GLbyte *pPixelData = NULL;              //front buffer의 픽셀 값들을 얻어 오기 위한 버퍼의 포인터
 
#ifdef WIN32
    //윈도우의 클라이언트 영역 좌표
    /*
	RECT ImageRect;
	HWND* m_hWndCopy;
    GetClientRect( *m_hWndCopy, &ImageRect );           
 
    //이미지 영역 좌표를 이용하여 실제 이미지의 사이즈를 계산
    nWidth  = ImageRect.right - ImageRect.left;     //윈도우 버전의 경우 사이즈 변경이 되므로 그때그때 조사
    nHeight = ImageRect.bottom - ImageRect.top;
 */
	nWidth  = 1000;     //윈도우 버전의 경우 사이즈 변경이 되므로 그때그때 조사
    nHeight = 1000;
#else
    nWidth  = 1000;     //(나의 경우)리눅스에서의 경우 해상도 고정이므로 그 값을 입력
    nHeight = 1000;
 
#endif
 
    //4의 배수인지 아닌지 확인해서 4의 배수가 아니라면 4의 배수로 맞춰준다.
    dwQuadrupleWidth = ( nWidth % 4 ) ? ( ( nWidth ) + ( 4 - ( nWidth % 4 ) ) ) : ( nWidth );
 
    //비트맵 파일 헤더 처리
    BMFH.bfType  = 0x4D42;      //B(42)와 M(4D)에 해당하는 ASCII 값을 넣어준다.
    //바이트 단위로 전체파일 크기
    BMFH.bfSize  = sizeof( BITMAPFILEHEADER ) + sizeof( BITMAPINFOHEADER ) + ( dwQuadrupleWidth * 3 * nHeight );
    //영상 데이터 위치까지의 거리
    BMFH.bfOffBits = sizeof( BITMAPFILEHEADER ) + sizeof( BITMAPINFOHEADER );
 
    //비트맵 인포 헤더 처리
    BMIH.biSize             = sizeof( BITMAPINFOHEADER );       //이 구조체의 크기
    BMIH.biWidth            = nWidth;                           //픽셀 단위로 영상의 폭
    BMIH.biHeight           = nHeight;                          //영상의 높이
    BMIH.biPlanes           = 1;                                //비트 플레인 수(항상 1)
    BMIH.biBitCount         = 24;                               //픽셀당 비트수(컬러, 흑백 구별)
    BMIH.biCompression      = BI_RGB;                           //압축 유무
    BMIH.biSizeImage        = dwQuadrupleWidth * 3 * nHeight;   //영상의 크기
    BMIH.biXPelsPerMeter    = 0;                                //가로 해상도
    BMIH.biYPelsPerMeter    = 0;                                //세로 해상도
    BMIH.biClrUsed          = 0;                                //실제 사용 색상수
    BMIH.biClrImportant     = 0;                                //중요한 색상 인덱스
 
    pPixelData = new GLbyte[ dwQuadrupleWidth * 3 * nHeight ];  //LJH 수정
 
    //프런트 버퍼로 부터 픽셀 정보들을 얻어온다.
    glReadPixels(
        0, 0,                   //캡처할 영역의 좌측상단 좌표
        nWidth, nHeight,        //캡처할 영역의 크기
        GL_BGR,                 //캡처한 이미지의 픽셀 포맷
        GL_UNSIGNED_BYTE,       //캡처한 이미지의 데이터 포맷
        pPixelData              //캡처한 이미지의 정보를 담아둘 버퍼 포인터
        );
 
    {//저장 부분
        FILE *outFile = fopen( strFilePath, "wb" );
        if( outFile == NULL )
        {
            //에러 처리
            //printf( "에러" );
            //fclose( outFile );
        }
 
        fwrite( &BMFH, sizeof( char ), sizeof(BITMAPFILEHEADER), outFile );         //파일 헤더 쓰기
        fwrite( &BMIH, sizeof( char ), sizeof(BITMAPINFOHEADER), outFile );         //인포 헤더 쓰기
        fwrite( pPixelData, sizeof( unsigned char ), BMIH.biSizeImage, outFile );   //glReadPixels로 읽은 데이터 쓰기
 
        fclose( outFile );  //파일 닫기
    }
 
    if ( pPixelData != NULL )
    {
        delete pPixelData;
    }
}
 
void DoInit(){
	if(mesh.MeshInit("LEGO_Man.obj")) exit(1);
	mesh.readObj("LEGO_Man.obj");
	mesh.readMtl("LEGO_Man.mtl");
	MAX_REFLECT = 2;
	resX = 500;
	resY = 500;
	light = Light(VECTOR(1, -1, -1), VECTOR(0.5, 0.5, 0.5));
	point = Spotlight(VECTOR(0, -1, 0), VECTOR(5, 5, 5), Pi, VECTOR(4, 5, 0), 1, 0.5, 0.1);
	spot = Spotlight(VECTOR(0, -1, -1), VECTOR(20, 20, 20), Pi/4, VECTOR(0, 4, 4), 1, 2, 0.05);
	//plain = Plain(Quad(VECTOR(5, mesh.getMiny(), 5), VECTOR(5, mesh.getMiny(), -5), VECTOR(-5, mesh.getMiny(), -5), VECTOR(-5, mesh.getMiny(), 5)), VECTOR(0.5, 0, 0), VECTOR(1, 0, 0));
	plain = Plain(Quad(VECTOR(10, 0, 10), VECTOR(10, 0, -10), VECTOR(-10, 0, -10), VECTOR(-10, 0, 10)), VECTOR(0, 0.5, 0), VECTOR(0, 0.1, 0));
	wall1 = Plain(Quad(VECTOR(10, 10, 10), VECTOR(10, 10, -10), VECTOR(10, 0, -10), VECTOR(10, 0, 10)), VECTOR(0.7, 0.7, 0.7), VECTOR(0, 0, 0));
	wall2 = Plain(Quad(VECTOR(-10, 10, 10), VECTOR(-10, 0, 10), VECTOR(-10, 0, -10),  VECTOR(-10, 10, -10)), VECTOR(0.7, 0.7, 0.7), VECTOR(0, 0, 0));
	mirror1 = Plain(Quad(VECTOR(10, 10, -10), VECTOR(-10, 10, -10), VECTOR(-10, 0, -10), VECTOR(10, 0, -10)), VECTOR(0.1, 0.1, 0.2), VECTOR(0.5, 0.5, 0.5));
	mirror2 = Plain(Quad(VECTOR(10, 10, 10),VECTOR(10, 0, 10) , VECTOR(-10, 0, 10),VECTOR(-10, 10, 10) ), VECTOR(0.1, 0.1, 0.2), VECTOR(0.5, 0.5, 0.5));
	celling = Plain(Quad(VECTOR(10, 10, 10), VECTOR(-10, 10, 10), VECTOR(-10, 10, -10),  VECTOR(10, 10, -10)), VECTOR(0.7, 0.7, 0.7), VECTOR(0, 0, 0));
	sphere1 = Sphere(VECTOR(3, 1, 3), 1, VECTOR(0.5, 0, 0), VECTOR(0.6, 0.1, 0.1), 1);
	sphere2 = Sphere(VECTOR(-3, 1, 3), 1, VECTOR(0, 0, 0.5), VECTOR(0.1, 0.1, 0.6), 1);
	sphere3 = Sphere(VECTOR(0, 1, 3), 1, VECTOR(0, 0, 0), VECTOR(0.4, 0.4, 0.4), 1.5);

	camera = Camera(VECTOR(0, 5, 9), VECTOR(0, 0, -1), Pi/4, Pi/4, 1);
	
	

	//faces[i*resX+j] = trace(r, 0);
	//cout<<"("<<i<<", "<<j<<") => "<<trace(camera.getRay(i, j, resX, resY), 0)<<endl;

}

void DoDraw(){
	string str;
	WORD sec;
	SYSTEMTIME time1, time2;
	Ray r;
	GetSystemTime(&time1);
	viewPlane = new VECTOR[resX*resY];
	cout<<"Drawing... wait please."<<endl;
#pragma omp parallel for
	for(int i =0; i<resX; i++){
		for(int j = 0; j<resY; j++){
			r = camera.getRay(i, j, resX, resY);
			viewPlane[i*resX+j] = trace(r, 0);
			//cout<<"("<<i<<", "<<j<<") => ("<<viewPlane[i*resX+j].getColor().x<<", "<<viewPlane[i*resX+j].getColor().y<<", "<<viewPlane[i*resX+j].getColor().z<<")"<<endl;
			//faces[i*resX+j] = trace(r, 0);
			//cout<<"("<<i<<", "<<j<<") => "<<trace(camera.getRay(i, j, resX, resY), 0)<<endl;
		}
	}
	
	
	
	/*
	r = camera.getRay(533, 395, resX, resY);
	viewPlane[533*resX+395] = trace(r, 0);
	
	
	r = camera.getRay(850, 120, resX, resY);
	viewPlane[850*resX+120] = trace(r, 0);
	
	*/

	GetSystemTime(&time2);
	sec = ((time2.wHour * 3600) + (time2.wMinute * 60) + (time2.wSecond)) - ((time1.wHour * 3600) + (time1.wMinute * 60) + (time1.wSecond)) ; 
	str.append(to_string(sec)).append(" sec");
	cout<<str<<endl;
}


void DoMouse(int button, int state, int x, int y){
	y = resY - y - 1;
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		cout<<"("<<x<<", "<<y<<")"<<endl;
		//cout<<mesh.getMiny()<<endl;
		ScreenCapture("test.bmp");
        glutPostRedisplay();
    }
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
		
		camera.rotate((-1)*Pi/12);
		delete[] viewPlane;
		DoDraw();
		
		if(scene > 0){
			scene -=1;
		}
		else{
			scene = 179;
		}
		break;
	case GLUT_KEY_RIGHT:
		
		camera.rotate(Pi/12);
		delete[] viewPlane;
		DoDraw();
		

		if(scene >= 179){
			scene = 0 ;
		}
		else{
			scene += 1;
		}
		break;
	}
	glutPostRedisplay();
}

 
void DoMenu(int value){
	switch(value) {
	case 11:
		resX = 500;
		resY = 500;
		delete[] viewPlane;
		glutReshapeWindow(resX, resY);
		DoDraw();
		break;
	case 12:
		resX = 700;
		resY = 700;
		delete[] viewPlane;
		glutReshapeWindow(resX, resY);
		DoDraw();
		break;
	case 13:
		resX = 1000;
		resY = 1000;
		delete[] viewPlane;
		glutReshapeWindow(resX, resY);
		DoDraw();
		break;
	case 21:
		MAX_REFLECT = 1;
		delete[] viewPlane;
		DoDraw();
		break;
	case 22:
		MAX_REFLECT = 2;
		delete[] viewPlane;
		DoDraw();
		break;
	case 23:
		MAX_REFLECT = 3;
		delete[] viewPlane;
		DoDraw();
		break;
	case 24:
		MAX_REFLECT = 4;
		delete[] viewPlane;
		DoDraw();
		break;
	case 25:
		MAX_REFLECT = 5;
		delete[] viewPlane;
		DoDraw();
		break;
     }
     glutPostRedisplay();
}


void DoTimer(int value){
	cout<<scene<<endl;
	if(scene < 360){
		string str("screen_");
		str.append(to_string(scene)).append(".bmp");
		ScreenCapture(str.c_str());
		camera.rotate(Pi/180);
		delete[] viewPlane;
		DoDraw();
		scene++;
	}
	glutPostRedisplay();
	glutTimerFunc(30, DoTimer, 1);
}


void reshape(int w, int h){
	glLoadIdentity();
	glViewport(0, 0, w, h);
	glutReshapeWindow(resX, resY);
	gluOrtho2D(0.0, resX, 0.0, resY);
}
void display(void){
    glClear(GL_COLOR_BUFFER_BIT);
	
	
	glBegin(GL_POINTS);
	glPointSize(1);
	for(int i =0; i<resX; i++){
		for(int j = 0; j<resY; j++){
			//glColor3f(viewPlane[scene*resX*resY+i*resX+j].x, viewPlane[scene*resX*resY+i*resX+j].y, viewPlane[scene*resX*resY+i*resX+j].z);
			glColor3f(viewPlane[i*resX+j].x, viewPlane[i*resX+j].y, viewPlane[i*resX+j].z);
			glVertex2i(i, j);
		}
	}
	glEnd();
	glutSwapBuffers();
}

void main(int argc, char **argv){
	
	
	DoInit();
	DoDraw();

	glutInit(&argc, argv);
	glutInitWindowSize(resX, resY);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutCreateWindow("2012210011_4");
	
	GLint SubMenu1 = glutCreateMenu(DoMenu);
	glutAddMenuEntry("500x500", 11);
	glutAddMenuEntry("700x700", 12);
	glutAddMenuEntry("1000x1000", 13);

	GLint SubMenu2 = glutCreateMenu(DoMenu);
	glutAddMenuEntry("1", 21);
	glutAddMenuEntry("2", 22);
	glutAddMenuEntry("3", 23);
	glutAddMenuEntry("4", 24);
	glutAddMenuEntry("5", 25);

	glutCreateMenu(DoMenu);
	glutAddSubMenu("Resolution", SubMenu1);
	glutAddSubMenu("Max reflection", SubMenu2);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	//glutKeyboardFunc(DoKeyboard);
    glutSpecialFunc(DoSpecial);
	//glutMouseFunc(DoMouse);
	//glutTimerFunc(30, DoTimer, 1);
	
	

	glutMainLoop();
}
