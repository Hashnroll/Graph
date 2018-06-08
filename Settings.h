//файл с настройками программы

#pragma once
#include <SFML\Graphics.hpp>

int WIDTH = 800, HEIGHT = 600; //ширина, высота окна
int MAX_VERTICES = 100; //максимальное кол-во вершин
int VERTEX_RADIUS = 30; //радиус вершины 
int VERTEX_NAME_SIZE = VERTEX_RADIUS * 50 / 51;
 
sf::Color vertexColor(100, 200, 150); //цвет вершины
sf::Color vertexColorSelected(200, 100, 0);

sf::Color backg(234, 239, 243); //цвет фона

sf::Font vertexNameFont; //шрифт для надписи имени вершины
bool vertexNameFontLoaded = false; //загружен ли шрифт надписи для вершины?

sf::RenderWindow app(sf::VideoMode(WIDTH, HEIGHT), "Graph wizard"); //окно приложения