//файл с настройками программы

#pragma once
#include <TGUI/TGUI.hpp>

extern unsigned int WIDTH, HEIGHT; //ширина, высота окна
extern unsigned int MAX_VERTICES; //максимальное кол-во вершин
extern int VERTEX_RADIUS; //радиус вершины 
extern int VERTEX_NAME_SIZE; //размер надписи на вершине
extern int MOUSE_TRAVEL_THRESHOLD; //если мышь прошла расстояние, меньшее этого, то не регистировать это, как пройденное расстояние

extern int EDGE_BOUND_VALUE; //коэф. для определения попадания точки в ребро(максим. расстояние от ребра до границы области, где еще регистрируется попадание в ребро)
extern int EDGE_TEXT_SIZE;
 
extern sf::Color vertexColor; //цвет вершины
extern sf::Color vertexColorSelected; //цвет выделенной вершины
extern sf::Color edgeColor; //цвет ребра
extern sf::Color edgeColorSelected; //цвет выделенного ребра

extern sf::Color backg; //цвет фона

extern sf::Font font; //шрифт для надписи имени вершины
extern bool fontLoaded; //загружен ли шрифт надписи для вершины?

extern sf::Texture arrowTexture;
extern sf::Sprite arrowTemplate; //спрайт для стрелочки(которая на дуге между вершинами)

extern tgui::TextBox::Ptr inputWeightTextBox; //ввод веса дуги
extern tgui::Layout2d inputWeightTextBoxSize; //размеры textbox для ввода ребра
extern bool inputWeightInProcess;

extern tgui::ComboBox::Ptr verticesComboBox; //выбор вершины из выпадающего списка, от которой будет работать алгоритм Форда-Беллмана

extern tgui::Tab::Ptr tabs; //вкладки

extern sf::RenderWindow app; //окно приложения
extern sf::View view;

extern sf::RenderWindow infoApp; //окно, появляется при выводе работы алгоритма, матрицы смежности и т.д.

extern tgui::Gui gui; //gui для виджетов
extern tgui::Gui infoGui; //gui для вывода информации

void onTabSelected(tgui::Gui& gui, std::string selectedTab);
