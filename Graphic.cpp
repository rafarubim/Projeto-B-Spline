#include "Graphic.hpp"

#define M_PI 3.14159265358979323846
#include "glut.h"
#include "IL/ilut.h"

#include "Window.hpp"
#include "cmath"
#include "Luz.hpp"
#include "Lista.cpp"


enum tool {
	SELECT = 0,
	ROTATE,
	CREATEPOINT,
	DESTROYPOINT
};

enum clickState {
	STATE_NEUTRAL,
	STATE_PRESSED
};

enum rotVertical {
	ROTVERTICAL_DOWN = -1,
	ROTVERTICAL_IDLE,
	ROTVERTICAL_UP
};

enum rotHorizontal {
	ROTHORIZONTAL_LEFT = -1,
	ROTHORIZONTAL_IDLE,
	ROTHORIZONTAL_RIGHT
};

enum rotClock {
	ROTCLOCK_ANTICLOCKWISE,
	ROTCLOCK_IDLE,
	ROTCLOCK_CLOCKWISE
};

enum planoMov {
	PLANOMOV_FRONT,
	PLANOMOV_IDLE,
	PLANOMOV_BACK
};

double arccos(double x) {

	if (x > 1)
		return acos(1.) * 180 / M_PI;
	else if (x < -1)
		return acos(-1.) * 180 / M_PI;
	else
		return acos(x) * 180 / M_PI;
}

Vetor<double> Graphic::rotateCompose3D(Vetor<double> eixoRot, Vetor<double> vec, double angle) {

	angle *=  M_PI / 180;

	float cosAng = cos(angle);
	float sinAng = sin(angle);

	Vetor<double> v1(cosAng + (1 - cosAng) * eixoRot[X] * eixoRot[X], (1 - cosAng) * eixoRot[Y] * eixoRot[X] + sinAng * eixoRot[Z], (1 - cosAng) * eixoRot[Z] * eixoRot[X] - sinAng * eixoRot[Y]); // Primeira coluna da matriz de rotação
	Vetor<double> v2(eixoRot[Y] * eixoRot[X] * (1 - cosAng) - eixoRot[Z] * sinAng, cosAng + eixoRot[Y] * eixoRot[Y] * (1 - cosAng), eixoRot[Y] * eixoRot[Z] * (1- cosAng) + eixoRot[X] * sinAng); // Segunda coluna da matriz de rotação
	Vetor<double> v3(eixoRot[Z] * eixoRot[X] * (1 - cosAng) + eixoRot[Y] * sinAng, eixoRot[Z] * eixoRot[Y] * (1 - cosAng) - eixoRot[X] * sinAng, cosAng + eixoRot[Z] * eixoRot[Z] * (1 - cosAng)); // Terceira coluna da matriz de rotação

	Vetor<double> temp1;
	Vetor<double> temp2;
	Vetor<double> temp3;

	temp1 = u1_[X]*v1 + u1_[Y]*v2 + u1_[Z]*v3;
	temp2 = u2_[X]*v1 + u2_[Y]*v2 + u2_[Z]*v3;
	temp3 = u3_[X]*v1 + u3_[Y]*v2 + u3_[Z]*v3;

	u1_ = temp1;
	u2_ = temp2;
	u3_ = temp3;

	temp1 *= vec[X];
	temp2 *= vec[Y];
	temp3 *= vec[Z];

	Vetor<double> temp = temp1 + temp2 + temp3;

	return temp;
}

Vetor<double> Graphic::rotate3D(Vetor<double> eixoRot, Vetor<double> vec, double angle) {

	angle *=  M_PI / 180;

	float cosAng = cos(angle);
	float sinAng = sin(angle);

	Vetor<double> v1(cosAng + (1 - cosAng) * eixoRot[X] * eixoRot[X], (1 - cosAng) * eixoRot[Y] * eixoRot[X] + sinAng * eixoRot[Z], (1 - cosAng) * eixoRot[Z] * eixoRot[X] - sinAng * eixoRot[Y]); // Primeira coluna da matriz de rotação
	Vetor<double> v2(eixoRot[Y] * eixoRot[X] * (1 - cosAng) - eixoRot[Z] * sinAng, cosAng + eixoRot[Y] * eixoRot[Y] * (1 - cosAng), eixoRot[Y] * eixoRot[Z] * (1- cosAng) + eixoRot[X] * sinAng); // Segunda coluna da matriz de rotação
	Vetor<double> v3(eixoRot[Z] * eixoRot[X] * (1 - cosAng) + eixoRot[Y] * sinAng, eixoRot[Z] * eixoRot[Y] * (1 - cosAng) - eixoRot[X] * sinAng, cosAng + eixoRot[Z] * eixoRot[Z] * (1 - cosAng)); // Terceira coluna da matriz de rotação

	v1 *= vec[X];
	v2 *= vec[Y];
	v3 *= vec[Z];

	Vetor<double> temp = v1 + v2 + v3;

	return temp;
}

Vetor<double> Graphic::convertToTrigonometricCircle(Vetor<double> preVec, Vetor<double> preScaleLimits) {

	Vetor<double> a(-1 + ((preVec[X] - preScaleLimits[0])/(preScaleLimits[1] - preScaleLimits[0])) * 2, -1 + ((preVec[Y] - preScaleLimits[2])/(preScaleLimits[3] - preScaleLimits[2])) * 2);

	if (a.getNorm2D() > 1)
		a.normalize2D();

	return a;
}

void Graphic::invertRotationMatrix() {

	double detA = u1_[X]*u2_[Y]*u3_[Z] + u2_[X]*u3_[Y]*u1_[Z] + u3_[X]*u1_[Y]*u2_[Z] - u1_[Z]*u2_[Y]*u3_[X] - u2_[Z]*u3_[Y]*u1_[X] - u3_[Z]*u1_[Y]*u2_[X];

	if (detA == 0) {
		cout << "Erro: O determinante da matriz de rotacao e nulo (nao deveria)! Ele foi substituido por 1\n";
		detA = 1;
	} // Apesar de, teoricamente, a matriz ser ortogonal e dever ter seu deteminante igual a 1 ou -1, na prática ela sempre tem det positivo (não sei o porquê).
	// Ainda assim, realizei a conta porque sucessivas composições na matriz podem ter alterado levemente seu det (erros numéricos de ponto flutuante), e utilizar o det prático torna o programa mais realista (ou pelo menos imagino que torne)

	//Matriz adjunta:
	u1Inverted_.set(u2_[Y]*u3_[Z] - u2_[Z]*u3_[Y], u1_[Z]*u3_[Y] - u1_[Y]*u3_[Z], u1_[Y]*u2_[Z] - u1_[Z]*u2_[Y]);
	u2Inverted_.set(u2_[Z]*u3_[X] - u2_[X]*u3_[Z], u1_[X]*u3_[Z] - u1_[Z]*u3_[X], u1_[Z]*u2_[X] - u1_[X]*u2_[Z]);
	u3Inverted_.set(u2_[X]*u3_[Y] - u2_[Y]*u3_[X], u1_[Y]*u3_[X] - u1_[X]*u3_[Y], u1_[X]*u2_[Y] - u1_[Y]*u2_[X]);

	//Matriz inversa (método de Cayley-Hamilton):
	u1Inverted_ /= detA;
	u2Inverted_ /= detA;
	u3Inverted_ /= detA;
}

Vetor<double> Graphic::rotateByInverted(Vetor<double> vec) {

	Vetor<double> v1 = u1Inverted_ * vec[X];
	Vetor<double> v2 = u2Inverted_ * vec[Y];
	Vetor<double> v3 = u3Inverted_ * vec[Z];

	return v1 + v2 + v3;
}

Graphic::Graphic() {

	phi_ = theta_ = psi_ = 0;

	widthLimits_.set(-WORLD_SCALE*WIDTH_SCALE_FACTOR, WORLD_SCALE*WIDTH_SCALE_FACTOR);
	heightLimits_.set(-WORLD_SCALE*HEIGHT_SCALE_FACTOR, WORLD_SCALE*HEIGHT_SCALE_FACTOR);
	depthLimits_.set(-WORLD_SCALE*DEPHT_SCALE_FACTOR, WORLD_SCALE*DEPHT_SCALE_FACTOR);

	currentTool_ = SELECT;

	u1_.set(1, 0, 0); // Primeira coluna da matriz de rotação composta total
	u2_.set(0, 1, 0); // Segunda coluna da matriz de rotação composta total
	u3_.set(0, 0, 1); // Terceira coluna da matriz de rotação composta total

	
	u1Inverted_.set(0, 0, 0); // Primeira coluna da matriz de rotação composta total invertida
	u2Inverted_.set(0, 0, 0); // Segunda coluna da matriz de rotação composta total invertida
	u3Inverted_.set(0, 0, 0); // Terceira coluna da matriz de rotação composta total invertida

	planoZ_ = 0;

	rotVertical_ = ROTVERTICAL_IDLE;
	rotHorizontal_ = ROTHORIZONTAL_IDLE;
	rotClock_ = ROTCLOCK_IDLE;
	planoMov_ = PLANOMOV_IDLE;

	cManager_.state = STATE_NEUTRAL;
	cManager_.selectedTool = SELECT;

	currentCurva_ = 0;

	pontoTemp_.set(0, 0, 0, 0);
	clickPonto_ = STATE_NEUTRAL;

	clickSelect_ = STATE_NEUTRAL;
	indexSelect_.set(-1, -1);

	mousePos3D_.set(0, 0, 0);

	confirmClean_ = STATE_NEUTRAL;

	desenhaNumero_ = -1;
	coordNumeroDesenhado_.set(0, 0);

	indiceApertado_ = -1;
	insereEntre_ = -1;
	cliqueDireito_ = STATE_NEUTRAL;
	inserirNoMeio_ = STATE_NEUTRAL;
}

void Graphic::updateEulerAngles() {

	Vetor<double> eixoZ(0, 0, 1);

		eixoZ = rotateCompose3D(Vetor<double>(1, 0, 0), eixoZ, 0);

		eixoZ.normalize3D(); // Resolve problemas numéricos

		double cosTheta = getCosBetween3D(Vetor<double>(0, 0, 1), eixoZ);

		theta_ = arccos(cosTheta);

		Vetor<double> eixoYPhi(0, 1, 0);

		if (theta_ == 0)
			eixoYPhi = rotateCompose3D(Vetor<double>(1, 0, 0), eixoYPhi, 0);
		else {

			Vetor<double> proj = eixoZ -  (cosTheta * Vetor<double>(0, 0, 1));

			eixoYPhi = -1 * proj.normalize3D();
		}

		eixoYPhi.normalize3D(); // Resolve problemas numéricos

		double cosPhi = getCosBetween3D(Vetor<double> (0, 1, 0), eixoYPhi);
		double sinPhi = getCosBetween3D(Vetor<double> (-1, 0, 0), eixoYPhi);

		phi_ = arccos(cosPhi);

		if (sinPhi < 0)
			phi_ = 360 - phi_;

		if (theta_ == 0)
			psi_ = 0;
		else {

			Vetor<double> eixoXPhi = rotate3D(Vetor<double>(0, 0, 1), Vetor<double>(1, 0, 0), phi_);

			eixoXPhi.normalize3D(); // Resolve problemas numéricos

			Vetor<double> eixoX = rotateCompose3D(Vetor<double>(1, 0, 0), Vetor<double>(1, 0, 0), 0);

			eixoX.normalize3D(); // Resolve problemas numéricos

			Vetor<double> eixoYTheta = vecProd3D(eixoZ, eixoXPhi);

			eixoYTheta.normalize3D(); // Resolve problemas numéricos

			double cosPsi = getCosBetween3D(eixoXPhi, eixoX);
			double sinPsi = getCosBetween3D(eixoYTheta, eixoX);

			psi_ = arccos(cosPsi);

			if (sinPsi < 0)
				psi_ = 360 - psi_;
		}

		Vetor<double> eixoY(0, 1, 0);
		eixoY = rotateCompose3D(Vetor<double>(1, 0, 0), eixoY, 0);
}

void Graphic::config() {

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(widthLimits_[0], widthLimits_[1], heightLimits_[0], heightLimits_[1], depthLimits_[0], depthLimits_[1]);
	//gluPerspective(120,1,0.0001,50);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	
	glRotatef(phi_, 0, 0, 1);
	glRotatef(theta_, 1, 0, 0);
	glRotatef(psi_, 0, 0, 1);
	
	//gluLookAt(1*(cos(-theta)-sin(-theta)),1*(sin(-theta)+cos(-theta)),1,0,0,0,0,0,1);

	glEnable(GL_DEPTH_TEST);

	//glutPostRedisplay();
}

void Graphic::alphaOn() {

	glEnable(GL_BLEND);
	glDisable(GL_LIGHTING);
}

void Graphic::alphaOff() {

	glDisable(GL_BLEND);
	glEnable(GL_LIGHTING);
}

void Graphic::updateRotation(Window &window) {

	if (window.getMouseLClick()) {

		if (currentTool_ == ROTATE && cManager_.state == STATE_NEUTRAL) {

			Vetor<double> limits = window.getScaleLimits();

			Vetor<double> last = window.getMouseLastPos();

			last = convertToTrigonometricCircle(last, limits);

			Vetor<double> current = window.getMousePos();

			current = convertToTrigonometricCircle(current, limits);

			Vetor<double> rot1(last[X], last[Y], sqrt(1.00000000001 - last[X]*last[X] - last[Y]*last[Y])); // 1.00000000001 pois a normalização feita pela função "convertToTrigonometricCircle" não é numericamente perfeita (ponto flutuante)

			Vetor<double> rot2(current[X], current[Y], sqrt(1.00000000001 - current[X]*current[X] - current[Y]*current[Y]));

			rot1.normalize3D(); // Resolve problemas numéricos
			rot2.normalize3D(); // Resolve problemas numéricos

			Vetor<double> tempRot = vecProd3D(rot1, rot2);

			tempRot.normalize3D(); // Resolve problemas numéricos

			double tempAng = getAngleBetween3D(rot1, rot2);

			rotateCompose3D(tempRot, Vetor<double>(0, 0, 0), tempAng);

			//rotateCompose3D(Vetor<double>(1, -1, 0), Vetor<double>(0, 0, 0), 1);

			updateEulerAngles();
		}
	}
}

void Graphic::drawTrackingBall(Window &window) {

	if (currentTool_ == ROTATE) {

		Vetor<double> limits = window.getScaleLimits();

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		gluOrtho2D(limits[0], limits[1], limits[2], limits[3]);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glDisable(GL_DEPTH_TEST);

		for (double i = 0; i < 7; i += 0.001) {

			glColor3f(0.8, 0.8, 0.8);

			glBegin(GL_POINTS);
				glVertex2f((limits[1] - limits[0])/2*cos(i), (limits[1] - limits[0])/2*sin(i));
			glEnd();
		}

		config();
	}
}

void Graphic::eixos() {

	glBegin(GL_LINES);

		glColor3f(1, 0, 0);
			glVertex3f(0, 0, 0);
			glVertex3f(WORLD_SCALE*EIXOS_FACTOR, 0, 0);

		glColor3f(0, 1, 0);
			glVertex3f(0, 0, 0);
			glVertex3f(0, WORLD_SCALE*EIXOS_FACTOR, 0);

		glColor3f(0, 0, 1);
			glVertex3f(0, 0, 0);
			glVertex3f(0, 0, WORLD_SCALE*EIXOS_FACTOR);
	glEnd();
}

double Graphic::f(double x, double y) {

	return -0.1*x*x - 0.1*y*y - 1;
}

void Graphic::drawF(int n) {

	if (n == 0)
		return;

	double x = -4;

	double dx = (8.) / n;
	double dy = (8.) / n;

	for (int i = 0; i < n; i++, x += dx) {

	double y = -4;

		for (int j = 0; j < n; j++, y += dy) {

			int t = -1;

			glColor3f(0, 1, 0);
			glBegin(GL_QUADS);
				glNormal3f(t*-0.2*x, t*-0.2*y, t*-1);
				glVertex3f(x, y, f(x, y));
				glNormal3f(t*-0.2*(x + dx), t*-0.2*y, t*-1);
				glVertex3f(x + dx, y, f(x + dx, y));
				glNormal3f(t*-0.2*(x + dx), t*-0.2*(y + dy), t*-1);
				glVertex3f(x + dx, y + dy, f(x + dx, y + dy));
				glNormal3f(t*-0.2*x, t*-0.2*(y + dy), t*-1);
				glVertex3f(x, y + dy, f(x, y + dy));
			glEnd();
		}
	}
}

void Graphic::updateKeyboard(unsigned char key) {

	if (key == 'a' || key == 'A' || key == '4')
		rotHorizontal_ = ROTHORIZONTAL_LEFT;
	else if (key == 'd' || key == 'D' || key == '6')
		rotHorizontal_ = ROTHORIZONTAL_RIGHT;
	else if (key == 'w' || key == 'W' || key == '8')
		rotVertical_ = ROTVERTICAL_UP;
	else if (key == 's' || key == 'S' || key == '2')
		rotVertical_ = ROTVERTICAL_DOWN;
	else if (key == 'q' || key == 'Q' || key == '7')
		rotClock_ = ROTCLOCK_ANTICLOCKWISE;
	else if (key == 'e' || key == 'E' || key == '9')
		rotClock_ = ROTCLOCK_CLOCKWISE;
	else if (key == 't' || key == 'T')
		planoMov_ = PLANOMOV_FRONT;
	else if (key == 'g' || key == 'G')
		planoMov_ = PLANOMOV_BACK;
}

void Graphic::updateKeyboard(int key) {

	if (key == GLUT_KEY_LEFT)
		rotHorizontal_ = ROTHORIZONTAL_LEFT;
	else if (key == GLUT_KEY_RIGHT)
		rotHorizontal_ = ROTHORIZONTAL_RIGHT;
	else if (key == GLUT_KEY_UP)
		rotVertical_ = ROTVERTICAL_UP;
	else if (key == GLUT_KEY_DOWN)
		rotVertical_ = ROTVERTICAL_DOWN;
}

void Graphic::updateKeyboardRelease(unsigned char key) {

	if ((key == 'a' || key == 'A' || key == '4') && rotHorizontal_ == ROTHORIZONTAL_LEFT)
		rotHorizontal_ = ROTHORIZONTAL_IDLE;
	else if ((key == 'd' || key == 'D' || key == '6') && rotHorizontal_ == ROTHORIZONTAL_RIGHT)
		rotHorizontal_ = ROTHORIZONTAL_IDLE;
	else if ((key == 'w' || key == 'W' || key == '8') && rotVertical_ == ROTVERTICAL_UP)
		rotVertical_ = ROTVERTICAL_IDLE;
	else if ((key == 's' || key == 'S' || key == '2') && rotVertical_ == ROTVERTICAL_DOWN)
		rotVertical_ = ROTVERTICAL_IDLE;
	else if ((key == 'q' || key == 'Q' || key == '7') && rotClock_ == ROTCLOCK_ANTICLOCKWISE)
		rotClock_ = ROTCLOCK_IDLE;
	else if ((key == 'e' || key == 'E' || key == '9') && rotClock_ == ROTCLOCK_CLOCKWISE)
		rotClock_ = ROTCLOCK_IDLE;
	else if ((key == 't' || key == 'T') && planoMov_ == PLANOMOV_FRONT)
		planoMov_ = PLANOMOV_IDLE;
	else if ((key == 'g' || key == 'G') && planoMov_ == PLANOMOV_BACK)
		planoMov_ = PLANOMOV_IDLE;
}

void Graphic::updateKeyboardRelease(int key) {

	if (key == GLUT_KEY_LEFT && rotHorizontal_ == ROTHORIZONTAL_LEFT)
		rotHorizontal_ = ROTHORIZONTAL_IDLE;
	else if (key == GLUT_KEY_RIGHT && rotHorizontal_ == ROTHORIZONTAL_RIGHT)
		rotHorizontal_ = ROTHORIZONTAL_IDLE;
	else if (key == GLUT_KEY_UP && rotVertical_ == ROTVERTICAL_UP)
		rotVertical_ = ROTVERTICAL_IDLE;
	else if (key == GLUT_KEY_DOWN && rotVertical_ == ROTVERTICAL_DOWN)
		rotVertical_ = ROTVERTICAL_IDLE;
}

void Graphic::updateKeyboardDisplay() {

	double rotSpeed = ROTATION_SPEED; // (ângulo em graus)
	
	if (rotHorizontal_ != ROTHORIZONTAL_IDLE) {

		Vetor<double> eixoRot;

		if (rotHorizontal_ == ROTHORIZONTAL_LEFT)
			eixoRot.set(0, -1, 0);
		else
			eixoRot.set(0, 1, 0);

		rotateCompose3D(eixoRot, Vetor<double>(0, 0, 0), rotSpeed);
	}

	if (rotVertical_ != ROTVERTICAL_IDLE) {

		Vetor<double> eixoRot;

		if (rotVertical_ == ROTVERTICAL_UP)
			eixoRot.set(-1, 0, 0);
		else
			eixoRot.set(1, 0, 0);

		rotateCompose3D(eixoRot, Vetor<double>(0, 0, 0), rotSpeed);
	}

	if (rotClock_ != ROTCLOCK_IDLE) {

		Vetor<double> eixoRot;

		if (rotClock_ == ROTCLOCK_ANTICLOCKWISE)
			eixoRot.set(0, 0, 1);
		else
			eixoRot.set(0, 0, -1);

		rotateCompose3D(eixoRot, Vetor<double>(0, 0, 0), rotSpeed);
	}

	if (rotHorizontal_ != ROTHORIZONTAL_IDLE || rotVertical_ != ROTVERTICAL_IDLE || rotClock_ != ROTCLOCK_IDLE) {
		updateEulerAngles();
		glutPostRedisplay();
	}

	double planoSpeed = PLANO_SPEED; // (Numero de 0 a 1: 0 é parado, 1 é a velocidade máxima)

	if (planoMov_ != PLANOMOV_IDLE) {

		if (planoMov_ == PLANOMOV_FRONT) {

			planoZ_ -= 3./100 * planoSpeed;
			if (planoZ_ < -1)
				planoZ_ += 3./100 * planoSpeed;

		} else {

			planoZ_ += 3./100 * planoSpeed;
			if (planoZ_ > 1)
				planoZ_ -= 3./100 * planoSpeed;
		}

		glutPostRedisplay();
	}
}

void Graphic::drawPlano(Iluminacao &iluminacao, Lista<Lista<Vetor<double>>> &listaCurvas) {

	if (currentTool_ == CREATEPOINT || (currentTool_ == SELECT && listaCurvas.size() > currentCurva_ && clickSelect_ == STATE_PRESSED && indexSelect_[0] != -1)) {

		drawRegua(iluminacao);

		glColor4f(1*(planoZ_ + 1)/2, 0.2*(planoZ_ + 1)/2, 0.2*(planoZ_ + 1)/2, 0.2);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		alphaOn();
			glBegin(GL_QUADS);
			
				int n1 = 60;
				int n2 = 60;

				double x = widthLimits_[0] + (widthLimits_[1] - widthLimits_[0])/10;

				double dx = (widthLimits_[1] - widthLimits_[0])*4./5/n1;
				double dy = (heightLimits_[1] - heightLimits_[0])*4./5/n2;

				for(int i = 0; i < n1; i++) {
				
					double y = heightLimits_[0] + (heightLimits_[1] - heightLimits_[0])/10;

					for(int j = 0; j < n2; j++) {

						if ((i + j) % 2 == 0) {
							glNormal3f(1, 1, 1);
							glVertex3f(x, y, depthLimits_[0] + (depthLimits_[1] - depthLimits_[0])/2*(planoZ_ + 1));glNormal3f(1, 1, 1);
							glVertex3f(x + dx, y, depthLimits_[0] + (depthLimits_[1] - depthLimits_[0])/2*(planoZ_ + 1));glNormal3f(1, 1, 1);
							glVertex3f(x + dx, y + dy, depthLimits_[0] + (depthLimits_[1] - depthLimits_[0])/2*(planoZ_ + 1));glNormal3f(1, 1, 1);
							glVertex3f(x, y + dy, depthLimits_[0] + (depthLimits_[1] - depthLimits_[0])/2*(planoZ_ + 1));
						}

						y += dy;
					}
					x += dx;
				}
			glEnd();
		alphaOff();

		glColor3f(0.2, 0.3, 0.6);

		config();
	}
}

void Graphic::drawIcons(Window &window) {

	Vetor<double> limits = window.getScaleLimits();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(limits[0], limits[1], limits[2], limits[3]);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glDisable(GL_DEPTH_TEST);

	Vetor<double> iconLimits;

	double iconWidth = (limits[1] - limits[0])/ICON_WIDTH_FRACTION;
	double iconHeight = (limits[3] - limits[2])/ICON_HEIGHT_FRACTION;

	iconLimits.set(limits[0], limits[0] + iconWidth, limits[3] - iconHeight, limits[3]);

	glColor3f(1, 1, 1);

	for (int i = 0; i < NUM_IMAGENS; i++) {

		glBindTexture(GL_TEXTURE_2D, icones_[i]);

		glBegin(GL_QUADS);
			glTexCoord2f(0, 0);	glVertex3f(iconLimits[0], iconLimits[2], 0);
			glTexCoord2f(1, 0);	glVertex3f(iconLimits[1], iconLimits[2], 0);
			glTexCoord2f(1, 1);	glVertex3f(iconLimits[1], iconLimits[3], 0);
			glTexCoord2f(0, 1);	glVertex3f(iconLimits[0], iconLimits[3], 0);
		glEnd();

		iconLimits += Vetor<double>(iconWidth, iconWidth, 0, 0);
	}

	glEnable(GL_DEPTH_TEST);

	//config();
}

void Graphic::drawRegua(Iluminacao &iluminacao) {

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	Luz luzRegua(iluminacao);

	luzRegua.cor(1, 1, 1, 1);
	luzRegua.posicao(widthLimits_[0], heightLimits_[0], depthLimits_[0]);

	glColor3f(0.3, 0.3, 0.3);

	glRotatef(5, 1, -1, 0);

	//glTranslatef(-3.2, -3.2, -5);
		//glutSolidCone(0.5, 20, 10, 10);
	//config();

	glTranslatef(widthLimits_[0] + (widthLimits_[1] - widthLimits_[0])/5.555, heightLimits_[0] + (heightLimits_[1] - heightLimits_[0])/5.555, depthLimits_[0]);
		glutSolidCone(0.7, depthLimits_[1] - depthLimits_[0], 20, 20);
	config();


	/*
	glBegin(GL_QUADS);
		glNormal3f(1, 1, -6);
		glVertex3f(widthLimits_[0] + (widthLimits_[1] - widthLimits_[0])*1.3/10, heightLimits_[0] + (heightLimits_[1] - heightLimits_[0])/10, depthLimits_[1]);
		glVertex3f(widthLimits_[0] + (widthLimits_[1] - widthLimits_[0])*1.07/5, heightLimits_[0] + (heightLimits_[1] - heightLimits_[0])/5, depthLimits_[0]);
		glVertex3f(widthLimits_[0] + (widthLimits_[1] - widthLimits_[0])/5, heightLimits_[0] + (heightLimits_[1] - heightLimits_[0])*1.07/5, depthLimits_[0]);
		glVertex3f(widthLimits_[0] + (widthLimits_[1] - widthLimits_[0])/10, heightLimits_[0] + (heightLimits_[1] - heightLimits_[0])*1.3/10, depthLimits_[1]);
	glEnd();*/

	//config();

	//glutPostRedisplay();
}

void Graphic::updateScale(int button) {

	if (button == 3) {

		widthLimits_ /= 1.1;
		heightLimits_ /= 1.1;
		depthLimits_ /= 1.1;

		if (widthLimits_[1] - widthLimits_[0] < SCALE_MIN*2*WIDTH_SCALE_FACTOR)
			widthLimits_ *= 1.1;
		if (heightLimits_[1] - heightLimits_[0] < SCALE_MIN*2*HEIGHT_SCALE_FACTOR)
			heightLimits_ *= 1.1;
		if (depthLimits_[1] - depthLimits_[0] < SCALE_MIN*2*DEPHT_SCALE_FACTOR)
			depthLimits_ *= 1.1;

		glutPostRedisplay();

	} else if (button == 4) {

		widthLimits_ *= 1.1;
		heightLimits_ *= 1.1;
		depthLimits_ *= 1.1;

		if (widthLimits_[1] - widthLimits_[0] > SCALE_MAX*2*WIDTH_SCALE_FACTOR)
			widthLimits_ /= 1.1;
		if (heightLimits_[1] - heightLimits_[0] > SCALE_MAX*2*HEIGHT_SCALE_FACTOR)
			heightLimits_ /= 1.1;
		if (depthLimits_[1] - depthLimits_[0] > SCALE_MAX*2*DEPHT_SCALE_FACTOR)
			depthLimits_ /= 1.1;

		glutPostRedisplay();
	}
}

void carregaImagem(ILuint *vecImagens, int num, const wchar_t *path, int &count) {

	ILuint devilError;

	ilBindImage(vecImagens[num]);
	ilLoadImage(path);

	devilError = ilGetError();
	if (devilError != IL_NO_ERROR)
		cout << "Erro na abertura da imagem numero " << num << endl;

	count++;
}

void Graphic::carregarImagens() {

	ILuint devilError;

	ilInit();

	devilError = ilGetError();
	if (devilError != IL_NO_ERROR)
		cout << "Erro na inicializacao da biblioteca devIL\n";

	ILuint imagens[NUM_IMAGENS];

	ilGenImages(NUM_IMAGENS, imagens);

	int count = 0;

	carregaImagem(imagens, 0, L"Select.png", count);
	carregaImagem(imagens, 1, L"Rotate.png", count);
	carregaImagem(imagens, 2, L"ADD.png", count);
	carregaImagem(imagens, 3, L"Trash.png", count);
	carregaImagem(imagens, 4, L"ZoomIn.png", count);
	carregaImagem(imagens, 5, L"ZoomOut.png", count);
	carregaImagem(imagens, 6, L"Previous.png", count);
	carregaImagem(imagens, 7, L"Next.png", count);
	carregaImagem(imagens, 8, L"Limpar.png", count);
	/*carregaImagem(imagens, 9, L"Vazio.png", count);
	carregaImagem(imagens, 10, L"Vazio.png", count);
	carregaImagem(imagens, 11, L"Curva.png", count);
	carregaImagem(imagens, 12, L"Superficie.png", count);*/

	if (count != NUM_IMAGENS)
		cout << "Numero de imagens carregadas nao corresponde a numero declarado em Graphic.hpp\n";

	ilutRenderer(ILUT_OPENGL);

	for (int i = 0; i < NUM_IMAGENS; i++) {

		ilBindImage(imagens[i]);
		icones_[i] = ilutGLBindTexImage();

		devilError = ilGetError();
		if (devilError != IL_NO_ERROR)
			cout << "Erro no carregamento da textura numero " << i << " a partir da imagem correspondente" << endl;
	}

	ilDeleteImages(NUM_IMAGENS, imagens);
}

void Graphic::updateButtonClick(Window &window, int button, int state, Lista<Lista<Vetor<double>>> &listaCurvas) {

	Vetor<double> limits = window.getScaleLimits();
	Vetor<double> pos = window.getMousePos();

	Vetor<double> iconLimits[NUM_IMAGENS + 1]; // O 0 é o limite de todos os ícones
	iconLimits[0].set(limits[0], limits[0] + (limits[1] - limits[0])/ICON_WIDTH_FRACTION*NUM_IMAGENS, limits[3] - (limits[3] - limits[2])/ICON_HEIGHT_FRACTION, limits[3]);
	
	for(int i = 1; i <= NUM_IMAGENS; i ++)
		iconLimits[i].set(limits[0] + (limits[1] - limits[0])/ICON_WIDTH_FRACTION*(i-1), limits[0] + (limits[1] - limits[0])/ICON_WIDTH_FRACTION*i, limits[3] - (limits[3] - limits[2])/ICON_HEIGHT_FRACTION, limits[3]);

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {

		for(int i = 0; i < NUM_IMAGENS; i++)
			if (isInLimits2D(pos, iconLimits[i+1])) {

				cManager_.state = STATE_PRESSED;
				cManager_.selectedTool = (Tool) i;
				break;
			}

	} else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP && cManager_.state == STATE_PRESSED) {
		
		for(int i = 0; i < NUM_IMAGENS; i++)
			if (isInLimits2D(pos, iconLimits[i+1])) {

				if (cManager_.selectedTool == (Tool) (i) && i < 4) {

					currentTool_ = cManager_.selectedTool;
					glutPostRedisplay();
					updateCursor((Tool) -1);
					break;
				} else if (cManager_.selectedTool == (Tool) (i) && i >= 4) {
					
					if (i == 4)
						updateScale(3);
					else if (i == 5)
						updateScale(4);
					else if (i == 6 && currentCurva_ > 0) {
						currentCurva_--;
						inserirNoMeio_ = STATE_NEUTRAL;
					} else if (i == 7 && currentCurva_ < listaCurvas.size()) {
						currentCurva_++;
						inserirNoMeio_ = STATE_NEUTRAL;
					} else if (i == 8) {

						if (confirmClean_ == STATE_NEUTRAL)
							confirmClean_ = STATE_PRESSED;
						else {
							for (int i = listaCurvas.size() - 1; i >= 0; i--)
								listaCurvas.retira(i);
							currentCurva_ = 0;
							confirmClean_ = STATE_NEUTRAL;
						}
					}

					glutPostRedisplay();
					break;
				}
			}
		cManager_.state = STATE_NEUTRAL;

	} else if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP) {
		confirmClean_ = STATE_NEUTRAL;
		glutPostRedisplay();
	}
}

void Graphic::updateCursor(Tool special) {

	if(currentTool_ == SELECT) {

		if (special == SELECT)
			glutSetCursor(GLUT_CURSOR_TOP_SIDE);
		else
			glutSetCursor(GLUT_CURSOR_INHERIT);
	}
	if(currentTool_ == ROTATE)
		glutSetCursor(GLUT_CURSOR_INFO);

	if(currentTool_ == CREATEPOINT) {

		if (special == CREATEPOINT)
			glutSetCursor(GLUT_CURSOR_TOP_SIDE);
		else
			glutSetCursor(GLUT_CURSOR_CROSSHAIR);
	}

	if(currentTool_ == DESTROYPOINT) {

		if (special == DESTROYPOINT)
			glutSetCursor(GLUT_CURSOR_DESTROY);
		else
			glutSetCursor(GLUT_CURSOR_HELP);
	}
	
	//GLUT_CURSOR_NONE << sem cursor
	//GLUT_CURSOR_WAIT << carregamento
	//GLUT_CURSOR_HELP << interrogação
	//GLUT_CURSOR_TEXT << texto
	//GLUT_CURSOR_TOP_SIDE << setinha para cima
}

void Graphic::createPoints(Lista<Lista<Vetor<double>>> &listaCurvas, Window &window, int button, int state) {

	if (currentTool_ == CREATEPOINT && cManager_.state == STATE_NEUTRAL)
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {

			if (listaCurvas.size() <= currentCurva_) {

				listaCurvas.push(Lista<Vetor<double>>());
			}

			Vetor<double> limits = window.getScaleLimits();
			Vetor<double> mousePos = window.getMousePos();

			Vetor<double> pointPos3D(widthLimits_[0] + (mousePos[X] - limits[0])/(limits[1] - limits[0]) * (widthLimits_[1] - widthLimits_[0]), heightLimits_[0] + (mousePos[Y] - limits[2])/(limits[3] - limits[2]) * (heightLimits_[1] - heightLimits_[0]), depthLimits_[0] + (planoZ_ + 1)/2*(depthLimits_[1] - depthLimits_[0]));

			invertRotationMatrix();
			pointPos3D = rotateByInverted(pointPos3D);

			clickPonto_ = STATE_PRESSED;

			pontoTemp_ = pointPos3D;

			glutPostRedisplay();

	} else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP && clickPonto_ == STATE_PRESSED) {

		if (listaCurvas.size() <= currentCurva_)
			listaCurvas.push(Lista<Vetor<double>>());
		
		if (inserirNoMeio_ == STATE_NEUTRAL)
			listaCurvas[currentCurva_].push(pontoTemp_);
		else
			listaCurvas[currentCurva_].insere(insereEntre_ + 1,pontoTemp_);

		clickPonto_ = STATE_NEUTRAL;

		updateCursor((Tool) -1);

		inserirNoMeio_ = STATE_NEUTRAL;

		glutPostRedisplay();
	}
}

void Graphic::drawPoints(Lista<Lista<Vetor<double>>> &listaCurvas, Window &window) {

	Vetor<double> corTemp = rotateCompose3D(Vetor<double>(1, 0, 0), pontoTemp_, 0);

	glColor3f(0, 0, (0.5+0.5*(corTemp[Z] - depthLimits_[0]) / (depthLimits_[1] - depthLimits_[0])));

	if (clickPonto_ == STATE_PRESSED) {
		createPoints(listaCurvas, window, GLUT_LEFT_BUTTON, GLUT_DOWN);
		glTranslatef(pontoTemp_[X], pontoTemp_[Y], pontoTemp_[Z]);
			glutSolidSphere(POINT_SIZE, 40, 38);
		config();
		updateCursor(CREATEPOINT);
	}

	for (int i = 0; i < listaCurvas.size(); i++) {

		int azul, verde;

		if (i == currentCurva_) {
			azul = 1;
			verde = 1;
		} else {

			azul = 0;
			verde = 0;
		}

		for (int j = 0; j < listaCurvas[i].size(); j++) {

			Vetor<double> corTemp = rotateCompose3D(Vetor<double>(1, 0, 0), listaCurvas[i][j], 0);

			int verde2 = 0;

			if (inserirNoMeio_ == STATE_PRESSED && (j == insereEntre_ || j == insereEntre_ + 1))
				verde2 = 1;

			glColor3f(0, verde*verde2 * (0.5+0.5*(corTemp[Z] - depthLimits_[0]) / (depthLimits_[1] - depthLimits_[0])), azul * (0.5+0.5*(corTemp[Z] - depthLimits_[0]) / (depthLimits_[1] - depthLimits_[0])));

			glTranslatef(listaCurvas[i][j][X], listaCurvas[i][j][Y], listaCurvas[i][j][Z]);
				glutSolidSphere(POINT_SIZE, 40, 38);
			config();
		}

		Lista<Vetor<double>> temp = listaCurvas[i];

		if (i == currentCurva_ && clickPonto_ == STATE_PRESSED) {
			if (inserirNoMeio_ == STATE_NEUTRAL)
				temp.push(pontoTemp_);
			else
				temp.insere(insereEntre_ + 1, pontoTemp_);
		}

		glColor3f(0, 0, 0.5);

		drawBSplineCurve(temp);
	}
}

int Graphic::findSpan(int n, float u, Lista<float> U) {

	for (int i = GRAU_DERIVADA; i < n + 1; i++)
		if (u <= U[i])
			return i - 1;
	cout << "Erro: Span nao encontrado. Retornado o ultimo indice (antes dos 1's)\n";
	return n - 1;
}

Lista<float> Graphic::basicFuncs(int iSpan, double u, Lista<float> U) {

	Lista<float> N;

	N.push(1);

	float left[GRAU_DERIVADA + 1];
	float right[GRAU_DERIVADA + 1];

	for(int j = 1; j <= GRAU_DERIVADA; j++) {

		left[j] = u - U[iSpan+1-j];
		right[j] = U[iSpan+j] - u;

		float saved = 0;

		for(int r = 0; r < j; r++) {

			float temp = N[r]/(right[r+1]+left[j-r]);
			N[r] = saved + right[r+1] * temp;
			saved = left[j-r]*temp;
		}
		N.push(saved);
	}

	return N;
}

void Graphic::drawBSplineCurve(Lista<Vetor<double>> &lst) {

	int n = lst.size();

	if (n >= GRAU_DERIVADA+1) {

		Lista<float> U;

		for (int i = 0; i <= GRAU_DERIVADA; i++)
			U.push(0);
		for (int i = 0; i <= GRAU_DERIVADA; i++)
			U.push(1);

		for(int i = GRAU_DERIVADA+1; i < n; i++)
			U.insere(i, (float) (i - GRAU_DERIVADA)/(n - GRAU_DERIVADA));

		glBegin(GL_LINE_STRIP);

		glVertex3f(lst[0][X], lst[0][Y], lst[0][Z]);

		for (float u = ACRECIMO_PARAMETRIZACAO/n; u < 1; u+= ACRECIMO_PARAMETRIZACAO/n) {

			int iSpan = findSpan(n, u, U);
			Lista<float> N = basicFuncs(iSpan, u, U);

			Vetor<double> c(0, 0, 0);

			for (int i = 0; i <= GRAU_DERIVADA; i++)
				c += N[i] * lst[iSpan-GRAU_DERIVADA+i];


			glVertex3f(c[X], c[Y], c[Z]);
			//glTranslatef(c[X], c[Y], c[Z]);
				//glutSolidSphere(POINT_SIZE/3.33, 6, 4);
			//config();
		}
		glVertex3f(lst[n-1][X], lst[n-1][Y], lst[n-1][Z]);

		glEnd();
	}
}

int Graphic::getPointHover(Lista<Vetor<double>> &curvaBSpline) {

	Lista<Vetor<double>> pontosInRange;
	Lista<int> indices;

	for (int i = 0; i < curvaBSpline.size(); i++) {

		Vetor<double> ponto = curvaBSpline[i];
		ponto = rotateCompose3D(Vetor<double>(1, 0, 0), ponto, 0);

		Vetor<double> pontoLimits(ponto[X]-POINT_SIZE, ponto[X]+POINT_SIZE, ponto[Y] - POINT_SIZE, ponto[Y] + POINT_SIZE);

		if (isInLimits2D(mousePos3D_, pontoLimits)) {
			pontosInRange.push(ponto);
			indices.push(i);
		}
	}

	if (pontosInRange.size() == 0)
		return -1;

	Vetor<double> maiorZ = pontosInRange[0];
	int maiorIndice = 0;

	for (int i = 1; i < pontosInRange.size(); i++) {
		if (pontosInRange[i][Z] > maiorZ[Z]) {
			maiorZ = pontosInRange[i];
			maiorIndice = i;
		}
	}

	return indices[maiorIndice];
}

Vetor<int> Graphic::getAllPointHover(Lista<Lista<Vetor<double>>> &listaCurvas) {

	Lista<Vetor<double>> pontos;
	Lista<Vetor<int>> indices;

	for (int i = 0; i < listaCurvas.size(); i++) {

		int getHover = getPointHover(listaCurvas[i]);

		if (getHover != -1) {
			Vetor<double> ponto = listaCurvas[i][getHover];
			ponto = rotateCompose3D(Vetor<double>(1, 0, 0), ponto, 0);

			pontos.push(ponto);
			indices.push(Vetor<int>(i, getHover));
		}
	}

	if (pontos.size() == 0)
		return Vetor<int>(-1, -1);

	Vetor<double> maiorZ = pontos[0];
	int maiorIndice = 0;

	for (int i = 1; i < pontos.size(); i++) {

		if (pontos[i][Z] > maiorZ[Z]) {

			maiorZ = pontos[i];
			maiorIndice = i;
		}
	}

	return indices[maiorIndice];
}

void Graphic::updatePointSelection(Lista<Lista<Vetor<double>>> &listaCurvas, Window &window) {

	if (currentTool_ == DESTROYPOINT) {
		
		Vetor<int> getHover = getAllPointHover(listaCurvas);

		if (getHover[0] != -1)
			updateCursor(DESTROYPOINT);
		else
			updateCursor((Tool) -1);

	} else if (currentTool_ == SELECT && listaCurvas.size() > currentCurva_) {

		int getHover = getPointHover(listaCurvas[currentCurva_]);

		if (getHover != -1) {
			updateCursor(SELECT);
			desenhaNumero_ = getHover + 1;

			Vetor<double> coord2D = rotateCompose3D(Vetor<double>(1, 0, 0), listaCurvas[currentCurva_][getHover], 0);

			coord2D.set(-1 + 0.01 + 2 * (coord2D[X] - widthLimits_[0])/(widthLimits_[1] - widthLimits_[0]), -1 + 0.02 + 2 * (coord2D[Y] - heightLimits_[0])/(heightLimits_[1] - heightLimits_[0]), 0);
			
			coordNumeroDesenhado_ = coord2D;

		} else {
			updateCursor((Tool) -1);
			desenhaNumero_ = -1;
		}

		glutPostRedisplay();

	} else if (currentTool_ == CREATEPOINT && listaCurvas.size() > currentCurva_) {

		int getHover = getPointHover(listaCurvas[currentCurva_]);

		if (getHover != -1) {

			desenhaNumero_ = getHover + 1;

			Vetor<double> coord2D = rotateCompose3D(Vetor<double>(1, 0, 0), listaCurvas[currentCurva_][getHover], 0);

			coord2D.set(-1 + 0.01 + 2 * (coord2D[X] - widthLimits_[0])/(widthLimits_[1] - widthLimits_[0]), -1 + 0.02 + 2 * (coord2D[Y] - heightLimits_[0])/(heightLimits_[1] - heightLimits_[0]), 0);
			
			coordNumeroDesenhado_ = coord2D;

		} else {

			desenhaNumero_ = -1;
		}

		glutPostRedisplay();
	}
}

void Graphic::selectPoints(Lista<Lista<Vetor<double>>> &listaCurvas, int button, int state) {

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {

		if (currentTool_ == DESTROYPOINT) {

			Vetor<int> getHover = getAllPointHover(listaCurvas);

			if (getHover[0] != -1) {
				clickSelect_ = STATE_PRESSED;
				indexSelect_ = getHover;
			}
		} else if (currentTool_ == SELECT && listaCurvas.size() > currentCurva_) {

			int getHover = getPointHover(listaCurvas[currentCurva_]);

			if (getHover != -1) {
				clickSelect_ = STATE_PRESSED;
				Vetor<double> temp = rotateCompose3D(Vetor<double>(1, 0, 0), listaCurvas[currentCurva_][getHover], 0);
				planoZ_ = (temp[Z] - depthLimits_[0])/(depthLimits_[1] - depthLimits_[0])*2-1;
				indexSelect_[0] = getHover;
			}
		}
	} else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {


		if (currentTool_ == DESTROYPOINT) {

			Vetor<int> getHover = getAllPointHover(listaCurvas);

			if (getHover[0] == indexSelect_[0] && getHover[1] == indexSelect_[1] && getHover[0] != -1) {

					listaCurvas[getHover[0]].retira(getHover[1]);
					glutPostRedisplay();
			}

		}

		clickSelect_ = STATE_NEUTRAL;
		indexSelect_.set(-1, -1);
		glutPostRedisplay();

	} else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN && listaCurvas.size() > currentCurva_) {

		if (currentTool_ == CREATEPOINT) {

			int getHover = getPointHover(listaCurvas[currentCurva_]);

			if (getHover != -1 && getHover != listaCurvas[currentCurva_].size() - 1) {

				cliqueDireito_ = STATE_PRESSED;
				indiceApertado_ = getHover;
			}
		}
	} else if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP && listaCurvas.size() > currentCurva_) {

		if (currentTool_ == CREATEPOINT) {

			if (inserirNoMeio_ == STATE_NEUTRAL) {

				int getHover = getPointHover(listaCurvas[currentCurva_]);

				if (getHover == indiceApertado_ && getHover != -1 && cliqueDireito_ == STATE_PRESSED) {

					inserirNoMeio_ = STATE_PRESSED;
					insereEntre_ = getHover;
				}
			} else
				inserirNoMeio_ = STATE_NEUTRAL;
		}

		cliqueDireito_ = STATE_NEUTRAL;
		indiceApertado_ = -1;
		glutPostRedisplay();
	}
}

void Graphic::updateMousePos3D(Window &window) {

	Vetor<double> limits = window.getScaleLimits();
	Vetor<double> mousePos = window.getMousePos();

	mousePos3D_.set(widthLimits_[0] + (mousePos[X] - limits[0])/(limits[1] - limits[0]) * (widthLimits_[1] - widthLimits_[0]), heightLimits_[0] + (mousePos[Y] - limits[2])/(limits[3] - limits[2]) * (heightLimits_[1] - heightLimits_[0]), depthLimits_[0] + (planoZ_ + 1)/2*(depthLimits_[1] - depthLimits_[0]));

}

void Graphic::updateMovingPoint(Lista<Lista<Vetor<double>>> &listaCurvas) {

	if (currentTool_ == SELECT && listaCurvas.size() > currentCurva_ && clickSelect_ == STATE_PRESSED && indexSelect_[0] != -1) {

		invertRotationMatrix();

		Vetor<double> mouse = rotateByInverted(mousePos3D_);

		listaCurvas[currentCurva_][indexSelect_[0]] = mouse;
		updateCursor(SELECT);
		glutPostRedisplay();
	}
}

void Graphic::escreveString(const char *str) {

	char c;

	for (int i = 0, c = str[i]; c != '\0'; ++i, c = str[i])
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
}

void Graphic::escreveNumeroNatural(int n) {
	
	Lista<int> lst;

	if (n == 0)
		lst.push(0);

	for (; n > 0; n/= 10)
		lst.push(n % 10);

	for (int i = lst.size() - 1; i >= 0; i--) {

		switch(lst[i]) {
		case 0:		lst[i] = '0';	break;
		case 1:		lst[i] = '1';	break;
		case 2:		lst[i] = '2';	break;
		case 3:		lst[i] = '3';	break;
		case 4:		lst[i] = '4';	break;
		case 5:		lst[i] = '5';	break;
		case 6:		lst[i] = '6';	break;
		case 7:		lst[i] = '7';	break;
		case 8:		lst[i] = '8';	break;
		case 9:		lst[i] = '9';	break;
		}

		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, lst[i]);
	}
}

void Graphic::drawMessage(Window &window) {

	glColor3f(0, 0, 0);
	glDisable(GL_DEPTH_TEST);

	if (confirmClean_ == STATE_PRESSED) {

		Vetor<double> limits = window.getScaleLimits();

		glRasterPos2f(-1 + 0.02, 1 - 2./(ICON_WIDTH_FRACTION - 6));
		escreveString("Apagar tudo? Aperte novamente para confirmar ou use o botao direito para cancelar...");
	}

	if (desenhaNumero_ != -1) {

		glRasterPos2f(coordNumeroDesenhado_[X], coordNumeroDesenhado_[Y]);
		escreveNumeroNatural(desenhaNumero_);
	}

	if (inserirNoMeio_ == STATE_PRESSED) {

		glRasterPos2f(-1 + 0.02, -1 + 0.03);
		escreveString("O proximo ponto sera inserido entre os pontos ");
		escreveNumeroNatural(insereEntre_ + 1);
		escreveString(" e ");
		escreveNumeroNatural(insereEntre_ + 2);
		escreveString(". Para cancelar, use o botao direito novamente.");
	}


	glRasterPos2f(1 - 0.4, 1 - 0.07);
	escreveString("Curva atual: ");
	escreveNumeroNatural(currentCurva_ + 1);
	glEnable(GL_DEPTH_TEST);
}