//файл с настройками программы

#pragma once
#include <SFML\Graphics.hpp>

int WIDTH = 800, HEIGHT = 600; //ширина, высота окна
int MAX_VERTICES = 100; //максимальное кол-во вершин
int VERTEX_RADIUS = 10; //радиус вершины 
 
sf::Color vertexColor(100, 200, 150); //цвет вершины

sf::Color backg(234, 239, 243); //цвет фона

sf::RenderWindow app(sf::VideoMode(WIDTH, HEIGHT), "Graph wizard"); //окно приложения