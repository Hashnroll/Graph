//���� � ����������� ���������

#pragma once
#include <SFML\Graphics.hpp>

int WIDTH = 800, HEIGHT = 600; //������, ������ ����
int MAX_VERTICES = 100; //������������ ���-�� ������
int VERTEX_RADIUS = 30; //������ ������� 
int VERTEX_NAME_SIZE = VERTEX_RADIUS * 50 / 51;
 
sf::Color vertexColor(100, 200, 150); //���� �������
sf::Color vertexColorSelected(200, 100, 0);

sf::Color backg(234, 239, 243); //���� ����

sf::Font vertexNameFont; //����� ��� ������� ����� �������
bool vertexNameFontLoaded = false; //�������� �� ����� ������� ��� �������?

sf::RenderWindow app(sf::VideoMode(WIDTH, HEIGHT), "Graph wizard"); //���� ����������