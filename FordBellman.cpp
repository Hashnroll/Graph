#include "Settings.h"
#include "FordBellman.h"

void computeFordBellman(Graph* g) {
	if (gui.get("ErrorSelectVertex")) return;

	sf::String selected = verticesComboBox->getSelectedItem();
	int s;
	if (selected != "") { //если вершина выбрана из списка
		s = tgui::stoi(selected);
		FordBellman fb(g,s);

		infoApp.create(sf::VideoMode(WIDTH / 2, HEIGHT / 2), "Graph wizard info"); //создать окно вывода информации о путях
		infoGui.setWindow(infoApp);
		 
		tgui::TextBox::Ptr pathToScreen(tgui::TextBox::create());//textbox для вывода путей
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
					pathToString += tgui::to_string(v)+sf::String(L" Длина = ")+tgui::to_string(fb.distToVertex(v))+"\n";
					pathToScreen->addText(pathToString);

				}
			}
		}
		if (pathToScreen->getText() == "") pathToScreen->setText(sf::String(L"От этой вершины нет никаких путей"));
	}
	else { //если вершина не выбрана из списка
		tgui::MessageBox::Ptr error(tgui::MessageBox::create()); //выдать ошибку
		error->setText(L"Выберите вершину из списка");
		error->setTextSize(30);
		error->setPosition(gui.getSize().x/4, gui.getSize().y / 4);
		//error->addButton("Ok");
		//error->connect("ButtonClicked",gui.remove(gui.get("ErrorSelectVertex")));
		gui.add(error, "ErrorSelectVertex");
	}
}