#pragma once

#include <functional>

class Menu;

/**
 * @class Приложение
 *
 * Этот класс управляет основными ресурсами приложения,
 * такими как основной цикл приложения, окно и т.д.
 * Возможно существование только одного экземпляра этого
 * класса в программе.
 */
class Application
{
public:
	typedef std::function<void()> DisplayCallback;
	typedef std::function<void(unsigned char key, int mouseX, int mouseY)> KeyboardCallback;
	typedef std::function<void(int key, int modifiers, int mouseX, int mouseY)> SpecialCallback;

	Application(int argc, char *argv[]);
	~Application();

	/**
	 * @brief Этот метод инициализирует окно GLUT,
	 *        открывая его по центру экрана.
	 *        Ширина и высота устанавливаются равними
	 *        640 на 480 пикселей соответственно.
	 */
	void show(const char *windowTitle);
	/**
	 * @brief Этот метод инициализирует окно GLUT,
	 *        открывая его в полноэкранном режиме.
	 */
	void showFullScreen(const char *windowTitle);
	/**
	 * @brief Этот метод запускает основной цикл библиотеки GLUT.
	 */
	void exec();
	/**
	 * @brief Этот метод устанавливает функцию обратного
	 *        вызова на событие перерисовки экрана.
	 */
	void setDisplayFunction(DisplayCallback display);
	/**
	 * @brief Этот метод устанавливает функцию обратного
	 *        вызова на событие нажатия на клавишу клавиатуры.
	 */
	void setKeyBoardFunction(KeyboardCallback keyboard);
	/**
	* @brief Этот метод устанавливает функцию обратного
	*        вызова на событие нажатия на специальные клавиши клавиатуры.
	*/
	void setSpecialFunction(SpecialCallback special);
	/**
	 * @brief Этот метод добавляет в очереть событие перерисовки окна.
	 */
	void repaint();
	/**
	 * @brief Этот метод устанавливает меню
	 */
	void setMenu(Menu &&menu);
	/**
	 * @brief Этот метод посылает сигнал завершения приложения
	 */
	void exit();
	/**
	 * @brief Этот метод запускает обработку накопившихся сообщений
	 */
	void processEvents();
	
private:
	bool m_isWork = true;
};
