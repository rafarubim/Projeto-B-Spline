#pragma once

#include "Vetor.cpp"
#include "Window.hpp"
#include "Iluminacao.hpp"
#include "Lista.cpp"

#include "glut.h"
#define WORLD_SCALE 5 // world_scale = A -> xmin = - A, xmax = A, ymin = -A, ...
#define WIDTH_SCALE_FACTOR 1 // xmin = -A * WIDTH_SCALE_FACTOR, xmax = A * WIDTH_SCALE_FACTOR
#define HEIGHT_SCALE_FACTOR 1 // similar ao caso acima
#define DEPHT_SCALE_FACTOR 2 // similar ao caso acima
#define SCALE_MIN 1 // Escala mínima permitida para o zoom in
#define SCALE_MAX 40 // Escala máxima permitida para o zoom out
#define EIXOS_FACTOR 0.5 // número de 0 a 1: indica até que ponto os eixos vão -> 0 não desenha eixos, 1 desenha até o canto da tela

#define NUM_IMAGENS 9 // número de imagens externas a carregar -- NÃO ALTERAR
#define ICON_WIDTH_FRACTION 20 //Fração da tela ocupada pelos ícones -- NÃO ALTERAR
#define ICON_HEIGHT_FRACTION 20 //Fração da tela ocupada pelos ícones -- NÃO ALTERAR

#define ROTATION_SPEED 1 // (Ângulo de rotação em graus)
#define PLANO_SPEED 0.1 // (Numero de 0 a 1: 0 é parado, 1 é a velocidade máxima)

#define GRAU_DERIVADA 2 // Grau de continuidade mínimo da derivada da B-Spline
#define ACRECIMO_PARAMETRIZACAO 0.1 // Valor usado na parametrização acrescendo em cada ponto
#define POINT_SIZE 0.05 // Tamanho dos pontos de controle

typedef enum tool Tool;

typedef enum clickState ClickState;

typedef struct clickManager {
	ClickState state;
	Tool selectedTool;
} ClickManager;

typedef enum rotVertical RotVertical;
typedef enum rotHorizontal RotHorizontal;
typedef enum rotClock RotClock;
typedef enum planoMov PlanoMov;

class Graphic {

private:

	double phi_, theta_, psi_;

	Vetor<double> widthLimits_;
	Vetor<double> heightLimits_;
	Vetor<double> depthLimits_;

	Tool currentTool_;

	Vetor<double> rotateCompose3D(Vetor<double> eixoRot, Vetor<double> vec, double angle);
	Vetor<double> rotate3D(Vetor<double> eixoRot, Vetor<double> vec, double angle);
	Vetor<double> convertToTrigonometricCircle(Vetor<double> preVec, Vetor<double> preScaleLimits);

	void invertRotationMatrix();
	Vetor<double> rotateByInverted(Vetor<double> vec);

	void updateEulerAngles();

	Vetor<double> u1_, u2_, u3_;

	Vetor<double> u1Inverted_, u2Inverted_, u3Inverted_;

	double planoZ_;

	RotVertical rotVertical_;
	RotHorizontal rotHorizontal_;
	RotClock rotClock_;
	PlanoMov planoMov_;

	GLuint icones_[NUM_IMAGENS];

	ClickManager cManager_;

	int currentCurva_;

	Vetor<double> pontoTemp_;
	ClickState clickPonto_;

	ClickState clickSelect_;
	Vetor<int> indexSelect_;

	Vetor<double> mousePos3D_;

	ClickState confirmClean_;

	int desenhaNumero_;
	Vetor<double> coordNumeroDesenhado_;

	int indiceApertado_;
	int insereEntre_;
	ClickState cliqueDireito_;
	ClickState inserirNoMeio_;

	//Lista<Vetor<double>> parametrizacao_;

public:

	Graphic();
	//Graphic(const Graphic &cpy); // Copiador desnecessário
	//Window(); // Destruidor desnecessário

	void config();

	void alphaOn();

	void alphaOff();

	void updateRotation(Window &window);

	void drawTrackingBall(Window &window);

	void eixos();

	double f(double x, double y);

	void drawF(int n);

	void updateKeyboard(unsigned char key);

	void updateKeyboard(int key);

	void updateKeyboardRelease(unsigned char key);

	void updateKeyboardRelease(int key);

	void updateKeyboardDisplay();

	void drawPlano(Iluminacao &iluminacao, Lista<Lista<Vetor<double>>> &listaCurvas);

	void drawIcons(Window &window);

	void drawRegua(Iluminacao &iluminacao);

	void updateScale(int button);

	void carregarImagens();

	void updateButtonClick(Window &window, int button, int state, Lista<Lista<Vetor<double>>> &listaCurvas);

	void updateCursor(Tool special);

	void createPoints(Lista<Lista<Vetor<double>>> &listaCurvas, Window &window, int button, int state);

	void drawPoints(Lista<Lista<Vetor<double>>> &listaCurvas, Window &window);

	int findSpan(int n, float u, Lista<float> U);

	Lista<float> basicFuncs(int iSpan, double u, Lista<float> U);

	void drawBSplineCurve(Lista<Vetor<double>> &lst);

	int getPointHover(Lista<Vetor<double>> &curvaBSpline);

	Vetor<int> getAllPointHover(Lista<Lista<Vetor<double>>> &listaCurvas);

	void updatePointSelection(Lista<Lista<Vetor<double>>> &listaCurvas, Window &window);

	void selectPoints(Lista<Lista<Vetor<double>>> &listaCurvas, int button, int state);

	void updateMousePos3D(Window &window);

	void updateMovingPoint(Lista<Lista<Vetor<double>>> &listaCurvas);

	void escreveString(const char *str);

	void escreveNumeroNatural(int n);

	void drawMessage(Window &window);
};