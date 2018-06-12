#include "Settings.h"
#include "FordBellman.h"

//задание основных параметров
unsigned int WIDTH = 1200, HEIGHT = 780;
unsigned int MAX_VERTICES = 100;

int VERTEX_RADIUS = 25;
int VERTEX_NAME_SIZE = VERTEX_RADIUS * 50 / 51;
int MOUSE_TRAVEL_THRESHOLD = 100;
int EDGE_BOUND_VALUE = 20;
int EDGE_TEXT_SIZE = 30;
sf::Color vertexColor = sf::Color(100, 200, 150);
sf::Color vertexColorSelected = sf::Color(200, 100, 0);
sf::Color edgeColor = sf::Color::Black;
sf::Color edgeColorSelected = sf::Color::Red;
sf::Color backg = sf::Color(234, 239, 243);

tgui::TextBox::Ptr inputWeightTextBox;
tgui::Layout2d inputWeightTextBoxSize = sf::Vector2f(50, 20);
bool inputWeightInProcess;

sf::RenderWindow app(sf::VideoMode(WIDTH, HEIGHT), "Graph wizard");
sf::RenderWindow infoApp;

sf::Font font;
bool fontLoaded;

sf::Texture arrowTexture; 
sf::Sprite arrowTemplate;

sf::View view;

tgui::ComboBox::Ptr verticesComboBox;

tgui::Tab::Ptr tabs;

tgui::Gui gui{app};
tgui::Gui infoGui;

//загрузить шрифт для надписей
void init(Graph* g) {

	if (font.loadFromFile("fonts/arial.ttf"))	fontLoaded = true;
	else fontLoaded = false;

	if (arrowTexture.loadFromFile("sprites/arrow.png")) //загрузить спрайт для стрелки на дуге
		arrowTemplate.setTexture(arrowTexture);

	view.reset(sf::FloatRect(0, 0, WIDTH, HEIGHT));

	//настройка вкладок для 1)отображения графа и 2)меню
	tabs = tgui::Tab::create();
	tabs->add("Graph");
	tabs->add("Menu");
	tabs->setPosition(0, 0);
	tabs->setSize(0, 0);
	gui.add(tabs);
	// Create the first panel
	tgui::Panel::Ptr menuPanel = tgui::Panel::create();
	menuPanel->setSize(WIDTH, HEIGHT);
	menuPanel->setPosition(tabs->getPosition().x, tabs->getPosition().y + tabs->getTabHeight());
	//добавить панель в gui
	gui.add(menuPanel, "MenuPanel");
	//добавить виджеты на панель
	tgui::Label::Ptr labelAlgorithm = tgui::Label::create();
	labelAlgorithm->setText(L"Поиск кратчайшего пути от вершины");
	labelAlgorithm->setPosition(10, 10);
	menuPanel->add(labelAlgorithm);
	tgui::Button::Ptr buttonAlgorithm = tgui::Button::create();
	buttonAlgorithm->setText(L"Форд-Беллман");
	buttonAlgorithm->setSize(200, 50);
	buttonAlgorithm->setPosition(10, 40);
	buttonAlgorithm->connect("pressed", computeFordBellman, g);
	menuPanel->add(buttonAlgorithm);
	//настройка выпадающего списка вершин для выбора в алгоритме Форда-Беллмана
	verticesComboBox = tgui::ComboBox::create();
	verticesComboBox->setPosition(labelAlgorithm->getPosition() + sf::Vector2f(labelAlgorithm->getSize().x + 10, 0));
	verticesComboBox->setItemsToDisplay(5);
	menuPanel->add(verticesComboBox);

	// Enable callback when another tab is selected (pass reference to the gui as first parameter)
	tabs->connect("TabSelected", onTabSelected, std::ref(gui));

	// Select the first tab and only show the first panel
	tabs->select("Graph");
	menuPanel->hide();

	//настройка поля для ввода веса дуги
	inputWeightTextBox = tgui::TextBox::create(); //поле для ввода веса дуги
	inputWeightInProcess = false; //изначально вес дуги не вводится
	inputWeightTextBox->setSize(inputWeightTextBoxSize); //задать размеры этого поля
	inputWeightTextBox->hide(); //скрыть его вначале
	gui.add(inputWeightTextBox); //добавить на экран

	infoApp.close();
}