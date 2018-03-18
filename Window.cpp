#include "Window.hpp"

#include <iostream>
#include "glut.h"

using namespace std;

Vetor<double> Window::convertToScale(Vetor<int> pos) {

	return Vetor<double>(scale2DLimits_[0] + (double) pos[X]/width_ * (scale2DLimits_[1] - scale2DLimits_[0]), scale2DLimits_[3] + (double) (pos[Y] + 1)/height_ * (scale2DLimits_[2] - scale2DLimits_[3]));
}

Window::Window() {

	width_ = 0;
	height_ = 0;
	mouseLeftClick_ = false;
	mouseRightClick_ = false;
	mousePosWindow_.set(0, 0);
	mousePosScale2D_.set(0, 0);
	mouseLastPosScale2D_.set(0, 0);
	scale2DLimits_.set(-1, 1, -1, 1);
}

void Window::updateSize() {

	width_ = glutGet(GLUT_WINDOW_WIDTH);
	height_ = glutGet(GLUT_WINDOW_HEIGHT);
	//cout << "largura janela: " << width_ << ", altura janela: " << height_ << endl;
}

void Window::updateMousePos(int x, int y) {

	mousePosWindow_.set(x, y);
	mouseLastPosScale2D_ = mousePosScale2D_;
	mousePosScale2D_ = convertToScale(mousePosWindow_);

	//cout << "MouseX: " << mousePosWindow_.getX() << ", MouseY: " << mousePosWindow_.getY() << endl << "MouseXScale: " << mousePosScale2D_.getX() << ", MouseYScale: " << mousePosScale2D_.getY() << endl << "MouseLastXScale: " << mouseLastPosScale2D_.getX() << ", MouseLastYScale: " << mouseLastPosScale2D_.getY() << endl;
}

void Window::updateMouseClick(int button, int state) {

	if (button == GLUT_LEFT_BUTTON) {

		if (state == GLUT_DOWN)
			mouseLeftClick_ = true;
		else
			mouseLeftClick_ = false;
	} else {

		if (state == GLUT_DOWN)
			mouseRightClick_ = true;
		else
			mouseRightClick_ = false;
	}
	//cout << "Lclick: " << mouseLeftClick_ << ", Rclick: " << mouseRightClick_ << endl;
}

bool Window::getMouseLClick() {

	return mouseLeftClick_;
}

bool Window::getMouseRClick() {

	return mouseRightClick_;
}

Vetor<double> Window::getMousePos() {

	return mousePosScale2D_;
}

Vetor<double> Window::getMouseLastPos() {

	return mouseLastPosScale2D_;
}

Vetor<double> Window::getScaleLimits() {

	return scale2DLimits_;
}