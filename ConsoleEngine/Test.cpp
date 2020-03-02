#include "GameProcess.h"
#include <iostream>

int main()
{
	ConsoleEngine x;

	HWND currentWindow = GetConsoleWindow();
	ShowWindow(currentWindow, SW_MAXIMIZE);

	GameProcess y(&x);

	
	while (y.GetStatus() != 0) {
		y.CentralProcess();
	}
	return 0; 
		
}

/*

Two possibilites for a 2D array of two different variables....

	struct charint
	{
		char a;
		int b;
	};

	charint x[10][50];
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 50; j++) {
			x[i][j].a = 'x';
			x[i][j].b = i + (j*100);
		}
	}

	char z1[10][50];
	int z2[10][50];
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 50; j++) {
			z1[i][j] = 'x';
			z2[i][j] = i + (j * 100);
		}
	}

	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 50; j++) {
			std::cout << x[i][j].a << " " << x[i][j].b << " ";
		}
	}

	std::cout << "\n\n\n\n";


	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 50; j++) {
			std::cout << z1[i][j] << " " << z2[i][j] << " ";
		}
	}

	*/