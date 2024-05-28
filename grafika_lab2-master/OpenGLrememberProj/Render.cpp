#include "Render.h"

#include <sstream>
#include <iostream>

#include <windows.h>
#include <GL\GL.h>
#include <GL\GLU.h>

#include "MyOGL.h"

#include "Camera.h"
#include "Light.h"
#include "Primitives.h"

#include "GUItextRectangle.h"

bool textureMode = true;
bool lightMode = true;

double a[] = { 0, 0, 0 };
double b[] = { -7, -1, 0 };
double c[] = { -3, 2, 0 };
double d[] = { -4, 7, 0 };
double e[] = { 0, 2, 0 };
double f[] = { 7, 7, 0 };
double g[] = { 7, -4, 0 };
double h[] = { 3, -6, 0 };

double a2[] = { 0, 0, 2 };
double b2[] = { -7, -1, 2 };
double c2[] = { -3, 2, 2 };
double d2[] = { -4, 7, 2 };
double e2[] = { 0, 2, 2 };
double f2[] = { 7, 7, 2 };
double g2[] = { 7, -4, 2 };
double h2[] = { 3, -6, 2 };

double N_VectorX(double A[], double B[], double C[]) {
	//Счиатем А и В по входным точкам
	double Vector_AB[3] = { B[0] - A[0], B[1] - A[1], B[2] - A[2] };
	double Vector_AC[3] = { C[0] - A[0], C[1] - A[1], C[2] - A[2] };

	//Создаем вектор N
	double N_X = Vector_AB[1] * Vector_AC[2] - Vector_AC[1] * Vector_AB[2];
	double N_Y = -Vector_AB[0] * Vector_AC[2] + Vector_AC[0] * Vector_AB[2];
	double N_Z = Vector_AB[0] * Vector_AC[1] - Vector_AC[0] * Vector_AB[1];

	double N_Vector[] = { N_X,N_Y,N_Z };
	double Abs_Vector = sqrt(N_X * N_X + N_Y * N_Y + N_Z * N_Z);

	N_Vector[0] = N_X / Abs_Vector;
	return N_Vector[0];
}
double N_VectorY(double A[], double B[], double C[]) {
	//Счиатем А и В по входным точкам
	double Vector_AB[3] = { B[0] - A[0], B[1] - A[1], B[2] - A[2] };
	double Vector_AC[3] = { C[0] - A[0], C[1] - A[1], C[2] - A[2] };

	//Создаем вектор N
	double N_X = Vector_AB[1] * Vector_AC[2] - Vector_AC[1] * Vector_AB[2];
	double N_Y = -Vector_AB[0] * Vector_AC[2] + Vector_AC[0] * Vector_AB[2];
	double N_Z = Vector_AB[0] * Vector_AC[1] - Vector_AC[0] * Vector_AB[1];

	double N_Vector[] = { N_X,N_Y,N_Z };
	double Abs_Vector = sqrt(N_X * N_X + N_Y * N_Y + N_Z * N_Z);

	N_Vector[1] = N_Y / Abs_Vector;
	return N_Vector[1];
}
double N_VectorZ(double A[], double B[], double C[]) {
	//Счиатем А и В по входным точкам
	double Vector_AB[3] = { B[0] - A[0], B[1] - A[1], B[2] - A[2] };
	double Vector_AC[3] = { C[0] - A[0], C[1] - A[1], C[2] - A[2] };

	//Создаем вектор N
	double N_X = Vector_AB[1] * Vector_AC[2] - Vector_AC[1] * Vector_AB[2];
	double N_Y = -Vector_AB[0] * Vector_AC[2] + Vector_AC[0] * Vector_AB[2];
	double N_Z = Vector_AB[0] * Vector_AC[1] - Vector_AC[0] * Vector_AB[1];

	double N_Vector[] = { N_X,N_Y,N_Z };
	double Abs_Vector = sqrt(N_X * N_X + N_Y * N_Y + N_Z * N_Z);

	N_Vector[2] = N_Z / Abs_Vector;
	return N_Vector[2];
}


//класс для настройки камеры
class CustomCamera : public Camera
{
public:
	//дистанция камеры
	double camDist;
	//углы поворота камеры
	double fi1, fi2;

	
	//значния масеры по умолчанию
	CustomCamera()
	{
		camDist = 15;
		fi1 = 1;
		fi2 = 1;
	}

	
	//считает позицию камеры, исходя из углов поворота, вызывается движком
	void SetUpCamera()
	{
		//отвечает за поворот камеры мышкой
		lookPoint.setCoords(0, 0, 0);

		pos.setCoords(camDist*cos(fi2)*cos(fi1),
			camDist*cos(fi2)*sin(fi1),
			camDist*sin(fi2));

		if (cos(fi2) <= 0)
			normal.setCoords(0, 0, -1);
		else
			normal.setCoords(0, 0, 1);

		LookAt();
	}

	void CustomCamera::LookAt()
	{
		//функция настройки камеры
		gluLookAt(pos.X(), pos.Y(), pos.Z(), lookPoint.X(), lookPoint.Y(), lookPoint.Z(), normal.X(), normal.Y(), normal.Z());
	}



}  camera;   //создаем объект камеры


//Класс для настройки света
class CustomLight : public Light
{
public:
	CustomLight()
	{
		//начальная позиция света
		pos = Vector3(1, 1, 3);
	}

	
	//рисует сферу и линии под источником света, вызывается движком
	void  DrawLightGhismo()
	{
		glDisable(GL_LIGHTING);

		
		glColor3d(0.9, 0.8, 0);
		Sphere s;
		s.pos = pos;
		s.scale = s.scale*0.08;
		s.Show();
		
		if (OpenGL::isKeyPressed('G'))
		{
			glColor3d(0, 0, 0);
			//линия от источника света до окружности
			glBegin(GL_LINES);
			glVertex3d(pos.X(), pos.Y(), pos.Z());
			glVertex3d(pos.X(), pos.Y(), 0);
			glEnd();

			//рисуем окруность
			Circle c;
			c.pos.setCoords(pos.X(), pos.Y(), 0);
			c.scale = c.scale*1.5;
			c.Show();
		}

	}

	void SetUpLight()
	{
		GLfloat amb[] = { 0.2, 0.2, 0.2, 0 };
		GLfloat dif[] = { 1.0, 1.0, 1.0, 0 };
		GLfloat spec[] = { .7, .7, .7, 0 };
		GLfloat position[] = { pos.X(), pos.Y(), pos.Z(), 1. };

		// параметры источника света
		glLightfv(GL_LIGHT0, GL_POSITION, position);
		// характеристики излучаемого света
		// фоновое освещение (рассеянный свет)
		glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
		// диффузная составляющая света
		glLightfv(GL_LIGHT0, GL_DIFFUSE, dif);
		// зеркально отражаемая составляющая света
		glLightfv(GL_LIGHT0, GL_SPECULAR, spec);

		glEnable(GL_LIGHT0);
	}


} light;  //создаем источник света




//старые координаты мыши
int mouseX = 0, mouseY = 0;

void mouseEvent(OpenGL *ogl, int mX, int mY)
{
	int dx = mouseX - mX;
	int dy = mouseY - mY;
	mouseX = mX;
	mouseY = mY;

	//меняем углы камеры при нажатой левой кнопке мыши
	if (OpenGL::isKeyPressed(VK_RBUTTON))
	{
		camera.fi1 += 0.01*dx;
		camera.fi2 += -0.01*dy;
	}

	
	//двигаем свет по плоскости, в точку где мышь
	if (OpenGL::isKeyPressed('G') && !OpenGL::isKeyPressed(VK_LBUTTON))
	{
		LPPOINT POINT = new tagPOINT();
		GetCursorPos(POINT);
		ScreenToClient(ogl->getHwnd(), POINT);
		POINT->y = ogl->getHeight() - POINT->y;

		Ray r = camera.getLookRay(POINT->x, POINT->y);

		double z = light.pos.Z();

		double k = 0, x = 0, y = 0;
		if (r.direction.Z() == 0)
			k = 0;
		else
			k = (z - r.origin.Z()) / r.direction.Z();

		x = k*r.direction.X() + r.origin.X();
		y = k*r.direction.Y() + r.origin.Y();

		light.pos = Vector3(x, y, z);
	}

	if (OpenGL::isKeyPressed('G') && OpenGL::isKeyPressed(VK_LBUTTON))
	{
		light.pos = light.pos + Vector3(0, 0, 0.02*dy);
	}

	
}

void mouseWheelEvent(OpenGL *ogl, int delta)
{

	if (delta < 0 && camera.camDist <= 1)
		return;
	if (delta > 0 && camera.camDist >= 100)
		return;

	camera.camDist += 0.01*delta;

}

void keyDownEvent(OpenGL *ogl, int key)
{
	if (key == 'L')
	{
		lightMode = !lightMode;
	}

	if (key == 'T')
	{
		textureMode = !textureMode;
	}

	if (key == 'R')
	{
		camera.fi1 = 1;
		camera.fi2 = 1;
		camera.camDist = 15;

		light.pos = Vector3(1, 1, 3);
	}

	if (key == 'F')
	{
		light.pos = camera.pos;
	}
}

void keyUpEvent(OpenGL *ogl, int key)
{
	
}



GLuint texId;

//выполняется перед первым рендером
void initRender(OpenGL *ogl)
{
	//настройка текстур

	//4 байта на хранение пикселя
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	//настройка режима наложения текстур
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	//включаем текстуры
	glEnable(GL_TEXTURE_2D);
	

	//массив трехбайтных элементов  (R G B)
	RGBTRIPLE *texarray;

	//массив символов, (высота*ширина*4      4, потомучто   выше, мы указали использовать по 4 байта на пиксель текстуры - R G B A)
	char *texCharArray;
	int texW, texH;
	OpenGL::LoadBMP("texture.bmp", &texW, &texH, &texarray);
	OpenGL::RGBtoChar(texarray, texW, texH, &texCharArray);

	
	
	//генерируем ИД для текстуры
	glGenTextures(1, &texId);
	//биндим айдишник, все что будет происходить с текстурой, будте происходить по этому ИД
	glBindTexture(GL_TEXTURE_2D, texId);

	//загружаем текстуру в видеопямять, в оперативке нам больше  она не нужна
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texW, texH, 0, GL_RGBA, GL_UNSIGNED_BYTE, texCharArray);

	//отчистка памяти
	free(texCharArray);
	free(texarray);

	//наводим шмон
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);


	//камеру и свет привязываем к "движку"
	ogl->mainCamera = &camera;
	ogl->mainLight = &light;

	// нормализация нормалей : их длины будет равна 1
	glEnable(GL_NORMALIZE);

	// устранение ступенчатости для линий
	glEnable(GL_LINE_SMOOTH); 


	//   задать параметры освещения
	//  параметр GL_LIGHT_MODEL_TWO_SIDE - 
	//                0 -  лицевые и изнаночные рисуются одинаково(по умолчанию), 
	//                1 - лицевые и изнаночные обрабатываются разными режимами       
	//                соответственно лицевым и изнаночным свойствам материалов.    
	//  параметр GL_LIGHT_MODEL_AMBIENT - задать фоновое освещение, 
	//                не зависящее от сточников
	// по умолчанию (0.2, 0.2, 0.2, 1.0)

	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 0);

	camera.fi1 = -1.3;
	camera.fi2 = 0.8;
}


double i[] = { 1, 0, 0 };
double j[] = { 0, 0, 0 };
double k[] = { 0, 0, 1 };

double vector(double x1, double y1, double z1, double x2, double y2, double z2, double x3, double y3, double z3) {
	double A[] = { x2 - x1, y2 - y1, z2 - z1 };
	double B[] = { x3 - x1, y3 - y1, z3 - z1 };
	double n_x = A[2] * B[3] - B[2] * A[3];
	double n_y = -1 * A[1] * B[3] + B[1] * A[3];
	double n_z = A[1] * B[2] - B[1] * A[2];

	double normal[] = { n_x, n_y, n_z };
	double len = sqrt(n_x * n_x + n_y * n_y + n_z * n_z);
	normal[0] = n_x / len;
	normal[1] = n_y / len;
	normal[2] = n_z / len;

	glBegin(GL_TRIANGLES);
	glNormal3f(normal[0], normal[1], normal[2]); // Set the normal vector
	glVertex3f(0.0f, 1.0f, 0.0f); // Vertex 1
	glVertex3f(-1.0f, -1.0f, 0.0f); // Vertex 2
	glVertex3f(1.0f, -1.0f, 0.0f); // Vertex 3
	glEnd();

	return 0;
}
//
void circle() {
	glBindTexture(GL_TEXTURE_2D, texId);
	glBegin(GL_POLYGON);
	for (double i = 0; i <= 2; i += 0.01)
	{
		double x = 9 * cos(i * 3.141593);
		double y = 9 * sin(i * 3.141593);

		double tx = cos(i * 3.141593) * 0.5 + 0.5;
		double ty = sin(i * 3.141593) * 0.5 + 0.5;

		glColor3d(0.5f, 0.5f, 0.5f);
		glNormal3d(0, 0, 1);
		glTexCoord2d(tx, ty);
		glVertex3d(x, y, 0);

	}
	glEnd();
}

//
void Render(OpenGL *ogl)
{



	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);

	glEnable(GL_DEPTH_TEST);
	if (textureMode)
		glEnable(GL_TEXTURE_2D);

	if (lightMode)
		glEnable(GL_LIGHTING);


	//альфаналожение
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	//настройка материала
	GLfloat amb[] = { 0.2, 0.2, 0.1, 1. };
	GLfloat dif[] = { 0.4, 0.65, 0.5, 1. };
	GLfloat spec[] = { 0.9, 0.8, 0.3, 1. };
	GLfloat sh = 0.1f * 256;


	//фоновая
	glMaterialfv(GL_FRONT, GL_AMBIENT, amb);
	//дифузная
	glMaterialfv(GL_FRONT, GL_DIFFUSE, dif);
	//зеркальная
	glMaterialfv(GL_FRONT, GL_SPECULAR, spec); \
		//размер блика
		glMaterialf(GL_FRONT, GL_SHININESS, sh);

	//чтоб было красиво, без квадратиков (сглаживание освещения)
	glShadeModel(GL_SMOOTH);
	//===================================
	//Прогать тут  
	glBindTexture(GL_TEXTURE_2D, texId);
	glBegin(GL_QUADS);
	glVertex3dv(a);
	glVertex3dv(b);
	glVertex3dv(c);
	glVertex3dv(e);

	glVertex3dv(c);
	glVertex3dv(d);
	glVertex3dv(e);
	glVertex3dv(a);

	glVertex3dv(h);
	glVertex3dv(g);
	glVertex3dv(e);
	glVertex3dv(a);

	glBindTexture(GL_TEXTURE_2D, texId);
	glBegin(GL_QUADS);

	glColor4f(0.5, 0.8, 0.3, 0.7);
	glNormal3f(0, 0, -1);
	/*glVertex3dv(f);
	glVertex3dv(g);
	glVertex3dv(a);
	glVertex3dv(e);*/

	/*glVertex3dv(a);
	glVertex3dv(f);
	glVertex3dv(g);
	glVertex3dv(h);*/
	glNormal3f(0, 0, 1);
	glVertex3dv(a2);
	glVertex3dv(b2);
	glVertex3dv(c2);
	glVertex3dv(e2);

	glVertex3dv(c2);
	glVertex3dv(d2);
	glVertex3dv(e2);
	glVertex3dv(a2);

	/*glVertex3dv(f2);
	glVertex3dv(g2);
	glVertex3dv(a2);
	glVertex3dv(e2);*/

	glVertex3dv(h2);
	glVertex3dv(g2);
	glVertex3dv(e2);
	glVertex3dv(a2);
	glEnd();

	//glColor4f(0.5, 0.8, 0.3, 0.7);
	//glNormal3f(0, 0, -1);
	//glVertex3dv(a);
	//glVertex3dv(b);
	//glVertex3dv(c);
	//glVertex3dv(e);

	//glVertex3dv(c);
	//glVertex3dv(d);
	//glVertex3dv(e);
	//glVertex3dv(a);

	///*glVertex3dv(f);
	//glVertex3dv(g);
	//glVertex3dv(a);
	//glVertex3dv(e);*/

	//glVertex3dv(h);
	//glVertex3dv(g);
	//glVertex3dv(e);
	//glVertex3dv(a);

	///*glVertex3dv(a);
	//glVertex3dv(f);
	//glVertex3dv(g);
	//glVertex3dv(h);*/
	//glNormal3f(0, 0, 1);
	//glVertex3dv(a2);
	//glVertex3dv(b2);
	//glVertex3dv(c2);
	//glVertex3dv(e2);

	//glVertex3dv(c2);
	//glVertex3dv(d2);
	//glVertex3dv(e2);
	//glVertex3dv(a2);

	///*glVertex3dv(f2);
	//glVertex3dv(g2);
	//glVertex3dv(a2);
	//glVertex3dv(e2);*/

	//glVertex3dv(h2);
	//glVertex3dv(g2);
	//glVertex3dv(e2);
	//glVertex3dv(a2);
	glEnd();

	//glVertex3dv(a2);
	//glVertex3dv(f2);
	//glVertex3dv(g2);
	//glVertex3dv(h2);
	glBegin(GL_QUADS);

	double n_X = N_VectorX(a, a2, b);
	double n_Y = N_VectorX(a, a2, b);
	double n_Z = N_VectorX(a, a2, b);


	glColor4f(0.5, 0.8, 0.3, 1.0);
	glNormal3f(-n_X, -n_Y, -n_Z);
	glTexCoord2d(0, 0);
	glVertex3dv(a);
	glTexCoord2d(1, 0);
	glVertex3dv(b);
	glTexCoord2d(1, 1);
	glVertex3dv(b2);
	glTexCoord2d(0, 1);
	glVertex3dv(a2);


	n_X = N_VectorX(c, c2, b);
	n_Y = N_VectorX(c, c2, b);
	n_Z = N_VectorX(c, c2, b);

	glNormal3f(-n_X, -n_Y, -n_Z);
	glTexCoord2d(0, 0);
	glVertex3dv(b);
	glTexCoord2d(1, 0);
	glVertex3dv(c);
	glTexCoord2d(1, 1);
	glVertex3dv(c2);
	glTexCoord2d(0, 1);
	glVertex3dv(b2);

	n_X = N_VectorX(d, d2, c);
	n_Y = N_VectorX(d, d2, c);
	n_Z = N_VectorX(d, d2, c);

	glNormal3f(-n_X, -n_Y, -n_Z);
	glTexCoord2d(0, 0);
	glVertex3dv(c);
	glTexCoord2d(1, 0);
	glVertex3dv(d);
	glTexCoord2d(1, 1);
	glVertex3dv(d2);
	glTexCoord2d(0, 1);
	glVertex3dv(c2);

	n_X = N_VectorX(e, e2, d);
	n_Y = N_VectorX(e, e2, d);
	n_Z = N_VectorX(e, e2, d);

	glNormal3f(n_X, n_Y, n_Z);
	glTexCoord2d(0, 0);
	glVertex3dv(d);
	glTexCoord2d(1, 0);
	glVertex3dv(e);
	glTexCoord2d(1, 1);
	glVertex3dv(e2);
	glTexCoord2d(0, 1);
	glVertex3dv(d2);

	n_X = N_VectorX(f, f2, e);
	n_Y = N_VectorX(f, f2, e);
	n_Z = N_VectorX(f, f2, e);

	glNormal3f(-n_X, -n_Y, -n_Z);
	glTexCoord2d(0, 0);
	glVertex3dv(e);
	glTexCoord2d(1, 0);
	glVertex3dv(f);
	glTexCoord2d(1, 1);
	glVertex3dv(f2);
	glTexCoord2d(0, 1);
	glVertex3dv(e2);

	/*glVertex3dv(f);
	glVertex3dv(g);
	glVertex3dv(g2);
	glVertex3dv(f2);*/

	/*glVertex3dv(h);
	glVertex3dv(g);
	glVertex3dv(g2);
	glVertex3dv(h2);*/

	//glVertex3dv(g);
	//glVertex3dv(h);
	//glVertex3dv(h2);
	//glVertex3dv(g2);

	n_X = N_VectorX(a, a2, h);
	n_Y = N_VectorX(a, a2, h);
	n_Z = N_VectorX(a, a2, h);

	glNormal3f(-n_X, -n_Y, -n_Z);
	glTexCoord2d(0, 0);
	glVertex3dv(h);
	glTexCoord2d(1, 0);
	glVertex3dv(a);
	glTexCoord2d(1, 1);
	glVertex3dv(a2);
	glTexCoord2d(0, 1);
	glVertex3dv(h2);
	glEnd();

	glBegin(GL_QUADS);

	n_X = N_VectorX(h, h2, g);
	n_Y = N_VectorX(h, h2, g);
	n_Z = N_VectorX(h, h2, g);

	glNormal3f(n_X, n_Y, n_Z);
	glColor4f(0.5, 0.8, 0.3, 1.0);
	glTexCoord2d(0, 0);
	glVertex3dv(h);
	glTexCoord2d(1, 0);
	glVertex3dv(g);
	glTexCoord2d(1, 1);
	glVertex3dv(g2);
	glTexCoord2d(0, 1);
	glVertex3dv(h2);
	glEnd();

	circle();

	//Выпуклость
	double x, y, m1[1000][3], m2[1000][3];
	int k = 0;
	double r = sqrt(20) / 2;
	for (double i = 0; i <= 360; i += 0.5) {
		x = r * cos(i * PI / 180) + 5;
		y = r * sin(i * PI / 180) - 5;
		if (y <= 0.5 * x - 7.5) {
			m1[k][0] = x;
			m1[k][1] = y;
			m1[k][2] = 0;

			m2[k][0] = x;
			m2[k][1] = y;
			m2[k][2] = 2;
			k++;
		}
	}


	for (int i = 0; i < k; i++) {

		glBegin(GL_QUADS);
		glColor4f(0.5, 0.8, 0.3, 1.0);
		glVertex3dv(m1[i]);
		glVertex3dv(m1[i + 1]);
		glVertex3dv(m2[i + 1]);
		glVertex3dv(m2[i]);
		glEnd();
	}

	//верх и низ выпуклости
	for (int i = 0; i < k; i++) {
		glBegin(GL_QUADS);
		glColor4f(0.5, 0.8, 0.3, 1.0);
		glNormal3f(0, 0, -1);

		glVertex3dv(m1[i]);
		n_X = N_VectorX(g, e, a);
		n_Y = N_VectorX(g, e, a);
		n_Z = N_VectorX(g, e, a);

		glNormal3f(n_X, n_Y, n_Z);
		glVertex3dv(g);
		glVertex3dv(e);
		glVertex3dv(a);
		glEnd();
	}

	for (int i = 0; i < k; i++) {
		glBegin(GL_QUADS);
		glColor4f(0.5, 0.8, 0.3, 1.0);
		glNormal3f(0, 0, 1);
		glVertex3dv(m2[i]);
		n_X = N_VectorX(g2, e2, a2);
		n_Y = N_VectorX(g2, e2, a2);
		n_Z = N_VectorX(g2, e2, a2);

		glNormal3f(-n_X, -n_Y, -n_Z);
		glVertex3dv(g2);
		glVertex3dv(e2);
		glVertex3dv(a2);
		glEnd();
	}


	//Вогнутость
	double xg, yg, m3[1000][3], m4[1000][3];
	int kg = 0;
	double rg = 13.65 / 2;
	for (double i = 0; i <= 360; i += 0.5) {
		xg = rg * cos(i * PI / 180) + 11;
		yg = rg * sin(i * PI / 180) + 1.5;
		if (xg <= 7) {
			m3[kg][0] = xg;
			m3[kg][1] = yg;
			m3[kg][2] = 0;

			m4[kg][0] = xg;
			m4[kg][1] = yg;
			m4[kg][2] = 2;
			kg++;
		}
	}

	for (int i = 0; i < kg; i++) {
		n_X = N_VectorX(m3[i], m3[i + 1], m4[i]);
		n_Y = N_VectorX(m3[i], m3[i + 1], m4[i]);
		n_Z = N_VectorX(m3[i], m3[i + 1], m4[i]);

		glBegin(GL_QUADS);
		glColor4f(0.5, 0.8, 0.3, 1.0);
		glNormal3f(-n_X, -n_Y, -n_Z);
		glVertex3dv(m3[i]);
		glVertex3dv(m3[i + 1]);
		glVertex3dv(m4[i + 1]);
		glVertex3dv(m4[i]);
		glEnd();

	}

	//верх
	glBegin(GL_POLYGON);
	glColor4f(0.5, 0.8, 0.3, 1.0);
	glNormal3f(0, 0, 1);
	glVertex3dv(f);
	for (int i = 0; i < kg; i++) {
		if (m3[i][0] <= 0) {
			glVertex3dv(m3[i]);
		}
	}
	glVertex3dv(a);
	glEnd();

	glBegin(GL_POLYGON);
	glColor4f(0.5, 0.8, 0.3, 1.0);
	glNormal3f(0, 0, -1);
	glVertex3dv(e);
	for (int i = 0; i < kg; i++) {
		if (m3[i][0] >= 0) {
			glVertex3dv(m3[i]);
		}
	}
	glVertex3dv(a);
	glEnd();

	//низ
	glBegin(GL_POLYGON);
	glColor4f(0.5, 0.8, 0.3, 1.0);
	glNormal3f(0, 0, -1);
	glVertex3dv(e2);
	for (int i = 0; i < kg; i++) {
		if (m4[i][0] <= 0) {
			glVertex3dv(m4[i]);
		}
	}
	glVertex3dv(a2);
	glEnd();

	glBegin(GL_POLYGON);
	glColor4f(0.5, 0.8, 0.3, 1.0);
	glNormal3f(0, 0, 1);
	glVertex3dv(e2);
	for (int i = 0; i < kg; i++) {
		if (m4[i][0] >= 0) {
			glNormal3f(0, 0, 1);
			glVertex3dv(m4[i]);
		}
	}
	glVertex3dv(a2);
	glEnd();

	glEnable(GL_BLEND);
	glBindTexture(GL_TEXTURE_2D, texId);
	glBegin(GL_QUADS);

	//glColor4f(0.5, 0.8, 0.3, 0.7);
	glColor4f(0.5f, 0.0f, 0.5f, 0.7f);
	glNormal3f(0, 0, -1);
	/*glVertex3dv(f);
	glVertex3dv(g);
	glVertex3dv(a);
	glVertex3dv(e);*/

	/*glVertex3dv(a);
	glVertex3dv(f);
	glVertex3dv(g);
	glVertex3dv(h);*/
	glNormal3f(0, 0, 1);
	glVertex3dv(a2);
	glVertex3dv(b2);
	glVertex3dv(c2);
	glVertex3dv(e2);

	glVertex3dv(c2);
	glVertex3dv(d2);
	glVertex3dv(e2);
	glVertex3dv(a2);

	/*glVertex3dv(f2);
	glVertex3dv(g2);
	glVertex3dv(a2);
	glVertex3dv(e2);*/

	glVertex3dv(h2);
	glVertex3dv(g2);
	glVertex3dv(e2);
	glVertex3dv(a2);
	glEnd();

	glBegin(GL_POLYGON);
	//glColor4f(0.5, 0.8, 0.3, 1.0);
	glColor4f(0.5f, 0.0f, 0.5f, 0.7f);
	glNormal3f(0, 0, 1);
	glVertex3dv(f);
	for (int i = 0; i < kg; i++) {
		if (m3[i][0] <= 0) {
			glVertex3dv(m3[i]);
		}
	}
	glVertex3dv(a);
	glEnd();

	glBegin(GL_POLYGON);
	glColor4f(0.5f, 0.0f, 0.5f, 0.7f);
	glNormal3f(0, 0, -1);
	glVertex3dv(e);
	for (int i = 0; i < kg; i++) {
		if (m3[i][0] >= 0) {
			glVertex3dv(m3[i]);
		}
	}
	glVertex3dv(a);

	for (int i = 0; i < k; i++) {
		glBegin(GL_QUADS);
		glColor4f(0.5f, 0.0f, 0.5f, 0.7f);
		glNormal3f(0, 0, 1);
		glVertex3dv(m2[i]);
		n_X = N_VectorX(g2, e2, a2);
		n_Y = N_VectorX(g2, e2, a2);
		n_Z = N_VectorX(g2, e2, a2);

		glNormal3f(-n_X, -n_Y, -n_Z);
		glVertex3dv(g2);
		glVertex3dv(e2);
		glVertex3dv(a2);
		glEnd();
	}
	glEnd();
	circle();
	//конец рисования квадратика станкина


   //Сообщение вверху экрана

	
	glMatrixMode(GL_PROJECTION);	//Делаем активной матрицу проекций. 
	                                //(всек матричные операции, будут ее видоизменять.)
	glPushMatrix();   //сохраняем текущую матрицу проецирования (которая описывает перспективную проекцию) в стек 				    
	glLoadIdentity();	  //Загружаем единичную матрицу
	glOrtho(0, ogl->getWidth(), 0, ogl->getHeight(), 0, 1);	 //врубаем режим ортогональной проекции

	glMatrixMode(GL_MODELVIEW);		//переключаемся на модел-вью матрицу
	glPushMatrix();			  //сохраняем текущую матрицу в стек (положение камеры, фактически)
	glLoadIdentity();		  //сбрасываем ее в дефолт

	glDisable(GL_LIGHTING);



	GuiTextRectangle rec;		   //классик моего авторства для удобной работы с рендером текста.
	rec.setSize(300, 150);
	rec.setPosition(10, ogl->getHeight() - 150 - 10);


	std::stringstream ss;
	ss << "T - вкл/выкл текстур" << std::endl;
	ss << "L - вкл/выкл освещение" << std::endl;
	ss << "F - Свет из камеры" << std::endl;
	ss << "G - двигать свет по горизонтали" << std::endl;
	ss << "G+ЛКМ двигать свет по вертекали" << std::endl;
	ss << "Коорд. света: (" << light.pos.X() << ", " << light.pos.Y() << ", " << light.pos.Z() << ")" << std::endl;
	ss << "Коорд. камеры: (" << camera.pos.X() << ", " << camera.pos.Y() << ", " << camera.pos.Z() << ")" << std::endl;
	ss << "Параметры камеры: R="  << camera.camDist << ", fi1=" << camera.fi1 << ", fi2=" << camera.fi2 << std::endl;
	
	rec.setText(ss.str().c_str());
	rec.Draw();

	glMatrixMode(GL_PROJECTION);	  //восстанавливаем матрицы проекции и модел-вью обратьно из стека.
	glPopMatrix();


	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

}