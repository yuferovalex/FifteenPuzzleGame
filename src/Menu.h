#pragma once

#include <functional>
#include <map>
#include <vector>

/**
 * @brief ����
 *
 * �����-�������� ��� ��������������� ����������� ���� ��
 * ���������� GLUT. ��-�� ��������� ����������� GLUT 
 * ������ � ��������� ������� ������ ������ ����������� 
 * ����������, �.�. ��� �������� ������ ����������, ����������
 * ��������� ���������� �������� ������ ��� ������. ��� ������� 
 * �������� ������� ��� ��������� � ������ ������� ����� �������
 * ����������. 
 */
class Menu
{
public:
    typedef std::function<void ()> MenuCallback;
    
    Menu();
	/**
	 * @brief ������������� ����, ��������� �� GLUT
	 */
	int id() const { return m_id; }
	/**
	 * @brief �������� ��������� 
	 */
    Menu &addMenuEntry(std::string title, MenuCallback callback);
	/**
	 * @brief �������� �������
	 */
	Menu &addSubMenu(std::string title, Menu &&menu);

private:
	static void emitCallback(int);

	static std::map<int, MenuCallback> m_callbacks;
    static int m_lastCallbackId;
	static int m_lastMenuChanged; 

    int m_id;
};
