#include <Windows.h>
#include <gl/GL.h>
#include <GL/glut.h>
#include <SOIL.h>
#include <stdexcept>

#include "Application.h"
#include "Renderer.h"

struct RendererPrivate
{
    RendererPrivate(Application &app, Game &game);

    Application &app;
    Game &game;
    double camRotationX = 0;
    double camRotationY = 0;
    GLuint tileTexture = SOIL_CREATE_NEW_ID;

    void init();
    void drawTile(Tile number);
};

RendererPrivate::RendererPrivate(Application &app, Game &game)
    : app(app), game(game)
{}

void RendererPrivate::init()
{
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_TEXTURE_2D);
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = { 100.0 };
    GLfloat light_position[] = { 1.0, 1.0, 2.0, 0.0 };
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_SMOOTH);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);    
}

void RendererPrivate::drawTile(Tile number)
{
    // Zero tile is empty 
    if (number == 0) {
        return ;
    }
    // Size of cube
    constexpr static const float size = 0.5;
    // Texture quads size
    constexpr const float tex_size = 0.25;    
    // Face with number
    const float tex_base_v = (int(number) / 4) * tex_size;
    const float tex_base_u = (int(number) % 4) * tex_size;
    // Base texture coords
    const GLfloat tex_base[4][2]
    {
        { tex_base_u + tex_size, tex_base_v },
        { tex_base_u + tex_size, tex_base_v + tex_size },
        { tex_base_u, tex_base_v + tex_size },
        { tex_base_u, tex_base_v }
    };
    // Other sides
    constexpr const float tex_other_v = 0.0;
    constexpr const float tex_other_u = 0.0;
    // Other texture coords
    constexpr const static GLfloat tex_other[4][2]
    {
        { tex_other_v + tex_size, tex_other_u },
        { tex_other_v, tex_other_u },
        { tex_other_v, tex_other_u + tex_size },
        { tex_other_v + tex_size, tex_other_u + tex_size }
    };
    constexpr const static GLfloat n[6][3] =
    {
        { -1.0, 0.0, 0.0 },
        { 0.0, 1.0, 0.0 },
        { 1.0, 0.0, 0.0 },
        { 0.0, -1.0, 0.0 },
        { 0.0, 0.0, 1.0 },
        { 0.0, 0.0, -1.0 }
    };
    constexpr const static GLint faces[6][4] =
    {
        { 0, 1, 2, 3 },
        { 3, 2, 6, 7 },
        { 7, 6, 5, 4 },
        { 4, 5, 1, 0 },
        { 5, 6, 2, 1 },
        { 7, 4, 0, 3 }
    };
    GLfloat v[8][3];
    GLint i;

    v[0][0] = v[1][0] = v[2][0] = v[3][0] = -size / 2;
    v[4][0] = v[5][0] = v[6][0] = v[7][0] = size / 2;
    v[0][1] = v[1][1] = v[4][1] = v[5][1] = -size / 2;
    v[2][1] = v[3][1] = v[6][1] = v[7][1] = size / 2;
    v[0][2] = v[3][2] = v[4][2] = v[7][2] = -size / 2;
    v[1][2] = v[2][2] = v[5][2] = v[6][2] = size / 2;

    auto tex = tex_base;
    for (i = 5; i >= 0; i--) {
        tex = i == 4 ? tex_base : tex_other;
        glBegin(GL_QUADS);
        glNormal3fv(&n[i][0]);
        glTexCoord2fv(&tex[0][0]);
        glVertex3fv(&v[faces[i][0]][0]);
        glTexCoord2fv(&tex[1][0]);
        glVertex3fv(&v[faces[i][1]][0]);
        glTexCoord2fv(&tex[2][0]);
        glVertex3fv(&v[faces[i][2]][0]);
        glTexCoord2fv(&tex[3][0]);
        glVertex3fv(&v[faces[i][3]][0]);
        glEnd();
    }
}

Renderer::Renderer(Application &app, Game &game)
    : d(new RendererPrivate(app, game))
{
    d->init();
    game.addObserver(this);
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
    glBindTexture(GL_TEXTURE_2D, d->tileTexture);
    for (auto row : d->game.board().tiles()) {
        for (auto tile : row ) {
            d->drawTile(tile);
            glTranslated(0.5, 0, 0);
        }
        glTranslated(-2, -0.5, 0);
    }
    glBindTexture(GL_TEXTURE_2D, 0);
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

void Renderer::setTexturePath(fs::path path)
{
    d->tileTexture = SOIL_load_OGL_texture
    (    
        path.string().c_str(), 
        SOIL_LOAD_RGB, 
        d->tileTexture, 
        SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y
    );
    if (d->tileTexture == 0) {
        throw std::runtime_error(SOIL_last_result());
    }
    glBindTexture(GL_TEXTURE_2D, d->tileTexture);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D, 0);
    d->app.repaint();
}
