#include <GL/glut.h>
#include <stdexcept>
#include "Menu.h"

std::map<int, Menu::MenuCallback> Menu::m_callbacks;
int Menu::m_lastCallbackId = 0;
int Menu::m_lastMenuChanged = 0;

void Menu::emitCallback(int id)
{
    m_callbacks[id]();
}

Menu::Menu()
    : m_id(glutCreateMenu(&Menu::emitCallback))
{
    m_lastMenuChanged = m_id;
}

Menu &Menu::addMenuEntry(std::string title, MenuCallback callback)
{
    if (m_lastMenuChanged != m_id) {
        throw std::logic_error("Can't add menu entry to this menu");
    }
    glutAddMenuEntry(title.c_str(), m_lastCallbackId);
    m_callbacks[m_lastCallbackId] = callback;
    ++m_lastCallbackId;
    return *this;
}

Menu &Menu::addSubMenu(std::string title, Menu &&menu)
{
    if (m_lastMenuChanged != m_id) {
        throw std::logic_error("Can't add menu entry to this menu");
    }
    glutAddSubMenu(title.c_str(), menu.m_id);
    return *this;
}
