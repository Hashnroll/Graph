#include "Settings.h"
#include "FordBellman.h"
#include "RandomGraph.h"


std::list<vertex> vertices; //����������� ������������� ������
std::list<edge> edges; //����������� ������������� �����

//������� �������� ����������
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

tgui::ComboBox::Ptr verticesComboBox; //����� ������� �� ����������� ������, �� ������� ����� �������� �������� �����-��������

tgui::TextBox::Ptr verticesRandomTextBox;

tgui::TextBox::Ptr edgesLowTextBox;
tgui::TextBox::Ptr edgesHighTextBox;

sf::RenderWindow app(sf::VideoMode(WIDTH, HEIGHT), "Graph wizard");
sf::RenderWindow infoApp;

sf::Font font;
bool fontLoaded;

sf::Texture arrowTexture; 
sf::Sprite arrowTemplate;

sf::View view;

tgui::Tab::Ptr tabs;

tgui::Gui gui{app};
tgui::Gui infoGui;

//��������� ����� ��� ��������
void init(Graph* g) {

	if (font.loadFromFile("fonts/arial.ttf"))	fontLoaded = true;
	else fontLoaded = false;

	if (arrowTexture.loadFromFile("sprites/arrow.png")) //��������� ������ ��� ������� �� ����
		arrowTemplate.setTexture(arrowTexture);

	view.reset(sf::FloatRect(0, 0, WIDTH, HEIGHT));

	//��������� ������� ��� 1)����������� ����� � 2)����
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
	//�������� ������ � gui
	gui.add(menuPanel, "MenuPanel");
	//�������� ������� �� ������
	tgui::Label::Ptr labelAlgorithm = tgui::Label::create();
	labelAlgorithm->setText(L"����� ����������� ���� �� �������");
	labelAlgorithm->setPosition(10, 10);
	menuPanel->add(labelAlgorithm);
	tgui::Button::Ptr buttonAlgorithm = tgui::Button::create();
	buttonAlgorithm->setText(L"����-�������");
	buttonAlgorithm->setSize(200, 50);
	buttonAlgorithm->setPosition(10, 40);
	buttonAlgorithm->connect("pressed", computeFordBellman, g);
	menuPanel->add(buttonAlgorithm);
	//��������� ����������� ������ ������ ��� ������ � ��������� �����-��������
	verticesComboBox = tgui::ComboBox::create();
	verticesComboBox->setPosition(labelAlgorithm->getPosition() + sf::Vector2f(labelAlgorithm->getSize().x + 10, 0));
	verticesComboBox->setItemsToDisplay(5);
	menuPanel->add(verticesComboBox);
	//�������� ���� ����� ��� ���������� �����
	tgui::Label::Ptr labelRandom = tgui::Label::create();
	labelRandom->setText(L"������ ��������� ����");
	labelRandom->setPosition(buttonAlgorithm->getPosition()+sf::Vector2f(0,100));
	labelRandom->setTextSize(24);
	menuPanel->add(labelRandom);
	tgui::Label::Ptr labelVerticesRandom = tgui::Label::create();
	labelVerticesRandom->setText(L"���-�� ������");
	labelVerticesRandom->setPosition(labelRandom->getPosition() + sf::Vector2f(0, 30));
	menuPanel->add(labelVerticesRandom);
	tgui::Label::Ptr labelEdgesRandom = tgui::Label::create();
	labelEdgesRandom->setText(L"�������� ����� ���(�� - ��)");
	labelEdgesRandom->setPosition(labelVerticesRandom->getPosition() + sf::Vector2f(200, 0));
	menuPanel->add(labelEdgesRandom);
	verticesRandomTextBox = tgui::TextBox::create();
	verticesRandomTextBox->setPosition(labelVerticesRandom->getPosition()+sf::Vector2f(0,30));
	verticesRandomTextBox->setSize(100,20);
	menuPanel->add(verticesRandomTextBox,"verticesRandomTextBox");
	edgesLowTextBox = tgui::TextBox::create();
	edgesHighTextBox = tgui::TextBox::create();
	edgesLowTextBox->setSize(100, 20);
	edgesHighTextBox->setSize(100, 20);
	edgesLowTextBox->setPosition(labelEdgesRandom->getPosition() + sf::Vector2f(0,30));
	edgesHighTextBox->setPosition(edgesLowTextBox->getPosition() + sf::Vector2f(150, 0));
	menuPanel->add(edgesLowTextBox,"edgesLowTextBox");
	menuPanel->add(edgesHighTextBox,"edgesHighTextBox");
	tgui::Button::Ptr buttonRandom = tgui::Button::create();
	buttonRandom->setText(L"������ ��������� ����");
	buttonRandom->setSize(200, 50);
	buttonRandom->setPosition(verticesRandomTextBox->getPosition()+sf::Vector2f(0, 50));
	buttonRandom->connect("pressed", randomizeGraph, g);
	menuPanel->add(buttonRandom);
	// Enable callback when another tab is selected (pass reference to the gui as first parameter)
	tabs->connect("TabSelected", onTabSelected, std::ref(gui));

	// Select the first tab and only show the first panel
	tabs->select("Graph");
	menuPanel->hide();

	//��������� ���� ��� ����� ���� ����
	inputWeightTextBox = tgui::TextBox::create(); //���� ��� ����� ���� ����
	inputWeightInProcess = false; //���������� ��� ���� �� ��������
	inputWeightTextBox->setSize(inputWeightTextBoxSize); //������ ������� ����� ����
	inputWeightTextBox->hide(); //������ ��� �������
	gui.add(inputWeightTextBox); //�������� �� �����

	infoApp.close();
}