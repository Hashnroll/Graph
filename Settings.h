//���� � ����������� ���������

#pragma once
#include <TGUI/TGUI.hpp>

extern unsigned int WIDTH, HEIGHT; //������, ������ ����
extern unsigned int MAX_VERTICES; //������������ ���-�� ������
extern int VERTEX_RADIUS; //������ ������� 
extern int VERTEX_NAME_SIZE; //������ ������� �� �������
extern int MOUSE_TRAVEL_THRESHOLD; //���� ���� ������ ����������, ������� �����, �� �� ������������� ���, ��� ���������� ����������

extern int EDGE_BOUND_VALUE; //����. ��� ����������� ��������� ����� � �����(������. ���������� �� ����� �� ������� �������, ��� ��� �������������� ��������� � �����)
extern int EDGE_TEXT_SIZE;
 
extern sf::Color vertexColor; //���� �������
extern sf::Color vertexColorSelected; //���� ���������� �������
extern sf::Color edgeColor; //���� �����
extern sf::Color edgeColorSelected; //���� ����������� �����

extern sf::Color backg; //���� ����

extern sf::Font font; //����� ��� ������� ����� �������
extern bool fontLoaded; //�������� �� ����� ������� ��� �������?

extern sf::Texture arrowTexture;
extern sf::Sprite arrowTemplate; //������ ��� ���������(������� �� ���� ����� ���������)

extern tgui::TextBox::Ptr inputWeightTextBox; //���� ���� ����
extern tgui::Layout2d inputWeightTextBoxSize; //������� textbox ��� ����� �����
extern bool inputWeightInProcess;

extern tgui::ComboBox::Ptr verticesComboBox; //����� ������� �� ����������� ������, �� ������� ����� �������� �������� �����-��������

extern tgui::Tab::Ptr tabs; //�������

extern sf::RenderWindow app; //���� ����������
extern sf::View view;

extern sf::RenderWindow infoApp; //����, ���������� ��� ������ ������ ���������, ������� ��������� � �.�.

extern tgui::Gui gui; //gui ��� ��������
extern tgui::Gui infoGui; //gui ��� ������ ����������

void onTabSelected(tgui::Gui& gui, std::string selectedTab);
