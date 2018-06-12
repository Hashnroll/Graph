#include "Settings.h"
#include "FordBellman.h"

void computeFordBellman(Graph* g) {
	if (gui.get("ErrorSelectVertex")) return;

	sf::String selected = verticesComboBox->getSelectedItem();
	int s;
	if (selected != "") { //���� ������� ������� �� ������
		s = tgui::stoi(selected);
		FordBellman fb(g,s);

		infoApp.create(sf::VideoMode(WIDTH / 2, HEIGHT / 2), "Graph wizard info"); //������� ���� ������ ���������� � �����
		infoGui.setWindow(infoApp);
		 
		tgui::TextBox::Ptr pathToScreen(tgui::TextBox::create());//textbox ��� ������ �����
		pathToScreen->setSize(infoGui.getSize());
		pathToScreen->setTextSize(40);
		infoGui.add(pathToScreen);

		for (int v : g->getVertices()) {
			if (v != s && fb.hasPathToVertex(v)) {
				std::stack<edgeGraph> path = fb.pathToVertex(v);
				if (!path.empty()) {
					sf::String pathToString;
					while (!path.empty()) {
						pathToString += (tgui::to_string(path.top().vertexFirst())) + "->";
						path.pop();
					}
					pathToString += tgui::to_string(v)+sf::String(L" ����� = ")+tgui::to_string(fb.distToVertex(v))+"\n";
					pathToScreen->addText(pathToString);

				}
			}
		}
		if (pathToScreen->getText() == "") pathToScreen->setText(sf::String(L"�� ���� ������� ��� ������� �����"));
	}
	else { //���� ������� �� ������� �� ������
		tgui::MessageBox::Ptr error(tgui::MessageBox::create()); //������ ������
		error->setText(L"�������� ������� �� ������");
		error->setTextSize(30);
		error->setPosition(gui.getSize().x/4, gui.getSize().y / 4);
		//error->addButton("Ok");
		//error->connect("ButtonClicked",gui.remove(gui.get("ErrorSelectVertex")));
		gui.add(error, "ErrorSelectVertex");
	}
}