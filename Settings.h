//���� � ����������� ���������

#pragma once
#include <SFML\Graphics.hpp>

int WIDTH = 800, HEIGHT = 600; //������, ������ ����
int MAX_VERTICES = 100; //������������ ���-�� ������
int VERTEX_RADIUS = 10; //������ ������� 
 
sf::Color vertexColor(100, 200, 150); //���� �������

sf::Color backg(234, 239, 243); //���� ����

sf::RenderWindow app(sf::VideoMode(WIDTH, HEIGHT), "Graph wizard"); //���� ����������