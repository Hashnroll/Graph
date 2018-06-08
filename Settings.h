//файл с настройками программы

#pragma once
#include <SFML\Graphics.hpp>

int WIDTH = 800, HEIGHT = 600; //ширина, высота окна
int MAX_VERTICES = 100; //максимальное кол-во вершин
int VERTEX_RADIUS = 25; //радиус вершины 
int VERTEX_NAME_SIZE = VERTEX_RADIUS * 50 / 51; //размер надписи на вершине
int MOUSE_TRAVEL_THRESHOLD = 100; //если мышь прошла расстояние, меньшее этого, то не регистировать это, как пройденное расстояние

int EDGE_BOUND_VALUE = 20; //коэф. для определения попадания точки в ребро(максим. расстояние от ребра до границы области, где еще регистрируется попадание в ребро)
int EDGE_TEXT_SIZE = 30;
 
sf::Color vertexColor(100, 200, 150); //цвет вершины
sf::Color vertexColorSelected(200, 100, 0); //цвет выделенной вершины
sf::Color edgeColor = sf::Color::Black; //цвет ребра
sf::Color edgeColorSelected = sf::Color::Red; //цвет выделенного ребра

sf::Color backg(234, 239, 243); //цвет фона

sf::Font font; //шрифт для надписи имени вершины
bool fontLoaded = false; //загружен ли шрифт надписи для вершины?

sf::Texture arrowTexture;
sf::Sprite arrowTemplate; //спрайт для стрелочки(которая на дуге между вершинами)


sf::RenderWindow app(sf::VideoMode(WIDTH, HEIGHT), "Graph wizard"); //окно приложения
sf::View view;
