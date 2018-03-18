#pragma once

#include "Vetor.hpp"
#include "Vetor.cpp"

class Window {

private:

	int width_, height_;
	bool mouseLeftClick_, mouseRightClick_;
	Vetor<int> mousePosWindow_;
	Vetor<double> mousePosScale2D_;
	Vetor<double> mouseLastPosScale2D_;
	Vetor<double> scale2DLimits_;

	Vetor<double> convertToScale(Vetor<int> pos);

public:

	Window();
	//Window(const Window &cpy); // Copiador desnecessário
	//~Window(); // Construtor desnecessário
	
	void updateSize();
	void updateMousePos(int x, int y);
	void updateMouseClick(int button, int state);
	//void updateLastKeyPressed

	bool getMouseLClick();
	bool getMouseRClick();
	Vetor<double> getMousePos();
	Vetor<double> getMouseLastPos();
	Vetor<double> getScaleLimits();
};