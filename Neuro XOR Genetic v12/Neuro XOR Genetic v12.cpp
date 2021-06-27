// Neuro XOR Genetic v12.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "MyFrameWork_v4.h"
int main()
{
	if (1) {
		unsigned int start_time = clock();
		srand(start_time);
		Web a;
		std::vector<int> l = { 2,2,1 };
		a.init(2, l);
		Genetic b;
		StudyData a1({ 0.0,0.0 }, { 0.0 });
		StudyData a2({ 0.0,1.0 }, { 1.0 });
		StudyData a3({ 1.0,0.0 }, { 1.0 });
		StudyData a4({ 1.0,1.0 }, { 0.0 });
		a.compute(a1.inputs);

		b.Example.push_back(a1);
		b.Example.push_back(a2);
		b.Example.push_back(a3);
		b.Example.push_back(a4);
		b.init(a, 0.5);
		Web c = b.evolute(50000);
		printf("\n\n");
		//c.print_all();
		c.compute(a1.inputs);
		printf("\n");
		for (int i = 0; i < a1.inputs.size(); i++) {
			printf("%.4fl ", a1.inputs[i]);
		}
		printf("\n");
		for (int i = 0; i < c.outputs.size(); i++) {
			printf("%.4fl ", c.outputs[i]);
		}
		printf("\n");

		c.compute(a2.inputs);
		for (int i = 0; i < a2.inputs.size(); i++) {
			printf("%.4fl ", a2.inputs[i]);
		}
		printf("\n");
		for (int i = 0; i < c.outputs.size(); i++) {
			printf("%.4fl ", c.outputs[i]);
		}
		printf("\n");

		c.compute(a3.inputs);
		for (int i = 0; i < a3.inputs.size(); i++) {
			printf("%.4fl ", a3.inputs[i]);
		}
		printf("\n");

		for (int i = 0; i < c.outputs.size(); i++) {
			printf("%.4fl ", c.outputs[i]);
		}
		printf("\n");

		c.compute(a4.inputs);
		for (int i = 0; i < a4.inputs.size(); i++) {
			printf("%.4fl ", a4.inputs[i]);
		}
		printf("\n");
		for (int i = 0; i < c.outputs.size(); i++) {
			printf("%.4fl ", c.outputs[i]);
		}
		printf("\n");
		printf("\n");
		printf("\n");

		c.save_all();
		Web d;
		d.load_all();
		
		d.compute(a1.inputs);
		printf("\n");
		for (int i = 0; i < a1.inputs.size(); i++) {
			printf("%.4fl ", a1.inputs[i]);
		}
		printf("\n");
		for (int i = 0; i < d.outputs.size(); i++) {
			printf("%.4fl ", d.outputs[i]);
		}
		printf("\n");

		d.compute(a2.inputs);
		printf("\n");
		for (int i = 0; i < a2.inputs.size(); i++) {
			printf("%.4fl ", a2.inputs[i]);
		}
		printf("\n");
		for (int i = 0; i < d.outputs.size(); i++) {
			printf("%.4fl ", d.outputs[i]);
		}
		printf("\n");

		d.compute(a3.inputs);
		printf("\n");
		for (int i = 0; i < a3.inputs.size(); i++) {
			printf("%.4fl ", a3.inputs[i]);
		}
		printf("\n");
		for (int i = 0; i < d.outputs.size(); i++) {
			printf("%.4fl ", d.outputs[i]);
		}
		printf("\n");

		d.compute(a4.inputs);
		printf("\n");
		for (int i = 0; i < a4.inputs.size(); i++) {
			printf("%.4fl ", a4.inputs[i]);
		}
		printf("\n");
		for (int i = 0; i < d.outputs.size(); i++) {
			printf("%.4fl ", d.outputs[i]);
		}
		printf("\n");


		//c.print_all();
		unsigned int end_time = clock();
		printf("%i \n", end_time - start_time);
	}
	/*
	Web a;
	std::vector<int> l = { 2,4,4,1 };
	a.init(2, l);
	a.load_all();
	a.print_all();*/
	//a.save_all();
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.