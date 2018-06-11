//���� � ����������� ���������

#pragma once
#include <TGUI/TGUI.hpp>

unsigned int WIDTH = 1200, HEIGHT = 780; //������, ������ ����
unsigned int MAX_VERTICES = 100; //������������ ���-�� ������
unsigned int MAX_EDGES = 500;
int VERTEX_RADIUS = 25; //������ ������� 
int VERTEX_NAME_SIZE = VERTEX_RADIUS * 50 / 51; //������ ������� �� �������
int MOUSE_TRAVEL_THRESHOLD = 100; //���� ���� ������ ����������, ������� �����, �� �� ������������� ���, ��� ���������� ����������

int EDGE_BOUND_VALUE = 20; //����. ��� ����������� ��������� ����� � �����(������. ���������� �� ����� �� ������� �������, ��� ��� �������������� ��������� � �����)
int EDGE_TEXT_SIZE = 30;
 
sf::Color vertexColor(100, 200, 150); //���� �������
sf::Color vertexColorSelected(200, 100, 0); //���� ���������� �������
sf::Color edgeColor = sf::Color::Black; //���� �����
sf::Color edgeColorSelected = sf::Color::Red; //���� ����������� �����

sf::Color backg(234, 239, 243); //���� ����

sf::Font font; //����� ��� ������� ����� �������
bool fontLoaded = false; //�������� �� ����� ������� ��� �������?

sf::Texture arrowTexture;
sf::Sprite arrowTemplate; //������ ��� ���������(������� �� ���� ����� ���������)

tgui::TextBox::Ptr inputWeightTextBox;
tgui::Layout2d inputWeightTextBoxSize(50,20); //������� textbox ��� ����� �����
bool inputWeightInProcess;

tgui::Tab::Ptr tabs; //�������

sf::RenderWindow app(sf::VideoMode(WIDTH,HEIGHT), "Graph wizard"); //���� ����������
sf::View view;

tgui::Gui gui(app); //gui ��� ��������
