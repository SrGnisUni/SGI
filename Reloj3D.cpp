/***************************************************
ISGI::Reloj·D
Genis Martinez Sanchis
Reloj analogico en 3D
Dependencias:
+freeglut
***************************************************/
#define PROYECTO "Reloj3D"
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>	
#include <sstream>
#include <Utilidades.h>
#include <ctime>

using namespace std;

#define tasaFPS 60
GLuint marca, rueda, manillaH, manillaS, manillaM;

//Variable dependiente del timpo
static float alfa = 0;
static float alfaS = 0;
static float alfaM = 0;
static float alfaH = 0;

void init()
// Inicializaciones
{
	cout << "Iniciando " << PROYECTO << endl;
	cout << "GL version " << glGetString(GL_VERSION) << endl;

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	// Configurar el motor de render
	glEnable(GL_DEPTH_TEST);

	//Marca
	marca = glGenLists(1);
	glNewList(marca, GL_COMPILE);

	glColor3f(0, 0, 0);
	glPushMatrix();
	glScalef(0.5, 1, 0.5);
	glutSolidSphere(0.1, 20, 20);
	glPopMatrix();

	glEndList();

	//Rueda de marcas
	rueda = glGenLists(1);
	glNewList(rueda, GL_COMPILE);

	for (auto i = 0; i < 12; i++) {
		glPushMatrix();

		glRotatef(i*30.0, 0, 0, 1);
		glTranslatef(0, 0.9, 0);
		glCallList(marca);

		glPopMatrix();
	}

	glEndList();

	//ManillaH
	manillaH = glGenLists(1);
	glNewList(manillaH, GL_COMPILE);

	glPushMatrix();
	glColor3f(0, 0, 0);
	glRotatef(-90, 1, 0, 0);
	glTranslatef(0, 0, 0.1);
	glutSolidCone(0.05, 0.4, 5, 5);
	glPopMatrix();

	glEndList();

	//ManillaS
	manillaS = glGenLists(1);
	glNewList(manillaS, GL_COMPILE);

	glPushMatrix();
	glColor3f(1, 0, 0);
	glRotatef(-90, 1, 0, 0);
	glTranslatef(0, 0, 0.1);
	glutWireCone(0.05, 0.7, 10, 10);
	glPopMatrix();

	glEndList();

	//ManillaM
	manillaM = glGenLists(1);
	glNewList(manillaM, GL_COMPILE);

	glPushMatrix();
	glColor3f(0, 1, 0);
	glRotatef(-90, 1, 0, 0);
	glTranslatef(0, 0, 0.1);
	glutWireCone(0.05, 0.6, 5, 5);
	glPopMatrix();

	glEndList();

	time_t curr_time;
	curr_time = time(NULL);
	tm *tm_local = localtime(&curr_time);
	alfaH = tm_local->tm_hour;
	alfaM = tm_local->tm_min;
	alfaS = tm_local->tm_sec;
}

void FPS() {
	//Muestra los fpas en la barra de titulo
	int ahora, tiempoT;
	static int antes = glutGet(GLUT_ELAPSED_TIME);
	static int fotogramas = 0;

	fotogramas++;

	ahora = glutGet(GLUT_ELAPSED_TIME);
	tiempoT = ahora - antes;

	if (tiempoT > 1000) {
		//modificar el titulo
		stringstream titulo;
		titulo << "FPS =" << fotogramas;
		glutSetWindowTitle(titulo.str().c_str());

		// reiniciamos la cuenta
		fotogramas = 0;
		antes = ahora;
	}
}

void display()
// Funcion de atencion al dibujo
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Seleccion de la matriz modelo-vista
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Situamos y orientamos la camara
	gluLookAt(0, 0, -3, 0, 0, 0, 0, 1, 0);

	//rueda
	glPushMatrix();
	glCallList(rueda);
	glPopMatrix();

	//esfera interior
	glPushMatrix();
	glColor3f(0.2, 0.3, 0.5);
	glRotatef(alfa / 10, 0, 1, 0);
	glutWireSphere(0.1, 4, 25);
	glPopMatrix();

	//esfera exterior
	glPushMatrix();
	glColor3f(0.2, 0.3, 0.5);
	glRotatef(-alfa / 10, 0, 1, 0);
	glutWireSphere(1, 25, 25);
	glPopMatrix();

	//manillaS
	glPushMatrix();
	glRotatef(6*alfaS, 0, 0, 1);
	glCallList(manillaS);
	glPopMatrix();

	//manillaM
	glPushMatrix();
	glRotatef(6 * alfaM, 0, 0, 1);
	glRotatef(alfa / 10, 0, 1, 0);
	glCallList(manillaM);
	glPopMatrix();

	//manillaH
	glPushMatrix();
	glRotatef(30 * alfaH, 0, 0, 1);
	glCallList(manillaH);
	glPopMatrix();

	glutSwapBuffers();

}

void reshape(GLint w, GLint h)
// Funcion de atencion al redimensionamiento
{
	float ra = (float)w / h;

	// Fijamos el marco dentro de la ventana de dibujo
	glViewport(0, 0, w, h);

	// Seleccionar la camara
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Camara perspectiva
	gluPerspective(40, ra, 0.1, 100);
}

void update()
{

	//Control del timepo transcurrido
	static int antes = glutGet(GLUT_ELAPSED_TIME);
	int ahora = glutGet(GLUT_ELAPSED_TIME);
	float tiempotranscurrido = float(ahora - antes) / 1000;

	//Velocidad angular contante
	static const float omega = 1;//Vueltas por segundo

	alfa += omega * 360 * tiempotranscurrido;

	//Actualizar la hora
	antes = ahora;

	//encolar un elemento de display
	glutPostRedisplay();
}

void onTimer(int tiempo)
{
	//Encolar un nuevo temporizador
	glutTimerFunc(tiempo, onTimer, tiempo);

	//llamar alupdate
	update();

}

void onTimerS(int tiempo)
{
	//Encolar un nuevo temporizador
	glutTimerFunc(tiempo, onTimerS, tiempo);

	alfaS += 1;
	if (alfaS >= 60) {
		alfaS = 0;
		alfaM += 1;
		if (alfaM >= 60) {
			alfaM = 0;
			alfaH += 1;
		}
	}

	glutPostRedisplay();

}

void main(int argc, char** argv)
// Programa principal
{
	// Inicializaciones
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(600, 600);
	glutCreateWindow(PROYECTO);
	init();

	// Registro de callbacks	
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutTimerFunc(1000 / tasaFPS, onTimer, 1000 / tasaFPS);
	glutTimerFunc(1000, onTimerS, 1000);

	// Bucle de atencion a eventos
	glutMainLoop();
}
