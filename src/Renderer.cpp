#include <windows.h>
#include <gl/GL.h>
#include <GL/glut.h>

#include "Application.h"
#include "Renderer.h"

struct RendererPrivate
{
	RendererPrivate(Application &app, Game &game)
		: app(app), game(game) {}

    Application &app;
    Game &game;
	double camRotationX = 0;
	double camRotationY = 0;
};

namespace {

void drawTile(int i, int j, Tile number)
{

}

}

Renderer::Renderer(Application &app, Game &game)
    : d(new RendererPrivate(app, game))
{
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 50.0 };
	GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
}

Renderer::~Renderer()
{
	delete d;
	d = nullptr;
}

void Renderer::display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotated(d->camRotationX, 1.0, 0.0, 0.0);
	glRotated(d->camRotationY, 0.0, 1.0, 0.0);

	glTranslated(-0.75, 0.75, 0);
	for (int i = 0; i < 4; ++i) {	
		for (int j = 0; j < 4; ++j) {
			auto c = ((i + 1) * (j + 1) + 10) / 32.;
			glColor3f(c, c, c);
			glutSolidCube(0.5);
			glTranslated(0.5, 0, 0);
		}
		glTranslated(-2, -0.5, 0);
	}
	glutSwapBuffers();
}

void Renderer::onGameChanged()
{
    d->app.repaint();
}

void Renderer::rotareCameraX(double angle)
{
	d->camRotationX += angle;
	d->app.repaint();
}

void Renderer::rotareCameraY(double angle)
{
	d->camRotationY += angle;
	d->app.repaint();
}