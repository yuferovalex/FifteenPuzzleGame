#include <stdexcept>
#include <GL/glut.h>

#include "Menu.h"
#include "Application.h"

namespace {

static Application *app = nullptr;
static Application::DisplayCallback  s_display;
static Application::KeyboardCallback s_keyboard;
static Application::SpecialCallback  s_special;

static void displayCallback()
{
    if (s_display) s_display();
}

static void keyboardCallback(unsigned char key, int x, int y)
{
    if (s_keyboard) s_keyboard(key, x, y);
}

static void specialCallback(int key, int x, int y)
{
	int modifiers = glutGetModifiers();
	if (s_special) s_special(key, modifiers, x, y);
}

static void reshapeCallback(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (w <= h) {
		glOrtho(-1.5, 1.5, -1.5*(GLfloat)h / (GLfloat)w, 1.5*(GLfloat)h / (GLfloat)w, -10.0, 10.0);
	}
	else {
		glOrtho(-1.5*(GLfloat)w / (GLfloat)h, 1.5*(GLfloat)w / (GLfloat)h, -1.5, 1.5, -10.0, 10.0); 
	}
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

} // namespace

Application::Application(int argc, char *argv[])
{
    if (app != nullptr) {
        throw std::logic_error("Application already exists");
    }
    app = this;
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	const int width = 640;
	const int height = 480;
	const int x = (glutGet(GLUT_SCREEN_WIDTH) - width) / 2;
	const int y = (glutGet(GLUT_SCREEN_HEIGHT) - height) / 2;
	glutInitWindowPosition(x, y);
	glutInitWindowSize(width, height);
}

Application::~Application()
{
    app = nullptr;
    s_display = nullptr;
    s_keyboard = nullptr;
}

void Application::show(const char *title)
{
    glutCreateWindow(title);
    glutKeyboardFunc(&::keyboardCallback);
    glutDisplayFunc(&::displayCallback);
    glutReshapeFunc(&::reshapeCallback);
	glutSpecialFunc(&::specialCallback);
}

void Application::showFullScreen(const char *title)
{
    show(title);
    glutFullScreen();
}

void Application::exec()
{
    glutMainLoop();
}

void Application::setDisplayFunction(Application::DisplayCallback display)
{
    s_display = display;
}

void Application::setKeyBoardFunction(Application::KeyboardCallback keyboard)
{
    s_keyboard = keyboard;
}

void Application::setSpecialFunction(SpecialCallback special)
{
	s_special = special;
}

void Application::repaint()
{
    glutPostRedisplay();
}

void Application::setMenu(Menu &&menu)
{
	glutSetMenu(menu.id());
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}
