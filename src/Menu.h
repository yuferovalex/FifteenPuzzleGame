#pragma once

#include <functional>
#include <map>
#include <vector>

/**
 * @brief Меню
 *
 * Класс-оболочка над вспомогательным контекстным меню из
 * библиотеки GLUT. Из-за сервисной архитектуры GLUT 
 * работа с объектами данного класса должна происходить 
 * поочередно, т.е. при создании нового экземпляра, предыдущий
 * экземпляр становится объектом только для чтения. При попытке 
 * добавить подменю или вхождение в старые объекты будет брошено
 * исключение. 
 */
class Menu
{
public:
    typedef std::function<void ()> MenuCallback;
    
    Menu();
    /**
     * @brief Идентификатор меню, полученый от GLUT
     */
    int id() const { return m_id; }
    /**
     * @brief Добавить вхождение 
     */
    Menu &addMenuEntry(std::string title, MenuCallback callback);
    /**
     * @brief Добавить подменю
     */
    Menu &addSubMenu(std::string title, Menu &&menu);

private:
    static void emitCallback(int);

    static std::map<int, MenuCallback> m_callbacks;
    static int m_lastCallbackId;
    static int m_lastMenuChanged; 

    int m_id;
};
