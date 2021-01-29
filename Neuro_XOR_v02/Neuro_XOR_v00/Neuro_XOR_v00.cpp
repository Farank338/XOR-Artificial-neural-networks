#include <iostream>
#include <ctime>

struct out_rez {
	int out_n;
	double* out_rez;
};
class neuron {
public:
	double* in_w;
	int lenght;
	double calc(double** in) {
		double sum = 0;
		for (int i = 0; i < lenght - 1; i++) {
			sum = sum + (*in[i]) * in_w[i];
		}
		sum = sum + in_w[lenght - 1];
		return (1 / (1 + exp(-sum)));
	}
	void init(int l) {
		in_w = new double[l];
		lenght = l;
		for (int i = 0; i < lenght; i++) {
			in_w[i] = 1 / (double)(1 + (rand() % 1000));
		}
	}
	out_rez back_prop(double error, double learning_rate, double my_out, double** prev_out) {
		out_rez temp;
		temp.out_n = lenght;
		temp.out_rez = new double[lenght];
		for (int i = 0; i < lenght - 1; i++) {
			in_w[i] = in_w[i] + learning_rate * (error * my_out * (1.0 - my_out) * (*prev_out[i]));
			temp.out_rez[i] = in_w[i];
		}
		in_w[lenght - 1] = in_w[lenght - 1] + learning_rate * (error * my_out * (1.0 - my_out) * 1.0);
		temp.out_rez[lenght - 1] = in_w[lenght - 1];
		return temp;
	}
	void erase() {
		delete[] in_w;
		std::cout << "\nNeuron deleted";
	}
};
class layer {
public:
	neuron* a;
	int in_n, out_n;
	double** in;
	double* out;

	void init(int out_kol, layer prev) {
		in_n = prev.out_n + 1;
		out_n = out_kol;
		a = new neuron[in_n - 1];
		in = new double* [in_n - 1];
		out = new double[out_n];
		for (int i = 0; i < out_n; i++) {
			a[i].init(in_n);
		}
		for (int i = 0; i < in_n - 1; i++) {
			in[i] = prev.out;
		}
	}
	void init(int in_kol, int out_kol) {
		in_n = in_kol + 1;
		out_n = out_kol;
		a = new neuron[out_n];
		in = new double* [in_n - 1];
		out = new double[out_n];
		for (int i = 0; i < out_n; i++) {
			a[i].init(in_n);
		}
	}
	void calc() {
		for (int i = 0; i < out_n; i++) {
			out[i] = a[i].calc(in);
		}
	}
	void calc(double* insert) {
		for (int i = 0; i < in_n - 1; i++) {
			in[i] = &insert[i];
		}
		for (int i = 0; i < out_n; i++) {
			out[i] = a[i].calc(in);
		}
	}
	double back_prop(double error, double learning_rate) {
		double sum = 0;
		for (int i = 0; i < out_n; i++) {
			out_rez temp = a[i].back_prop(error, learning_rate, out[i], in);
			for (int j = 0; j < out_n; j++) {
				sum = sum + temp.out_rez[j];
			}
			delete[] temp.out_rez;
		}
		return sum;
	}
	void erase() {
		for (int i = 0; i < out_n; i++) {
			a[i].erase();
		}
		delete[] a;
		delete[] out;
		in = nullptr;
		std::cout << "\nLayer deleted";
	}
};

class web {
public:
	layer* a;
	int length;
	//Number of layer, Number of input to every neuron on first layer, Array of number of neuron on every layer
	void init(int l, int in_n, int* neuron_n) {
		length = l;
		a = new layer[l];
		a[0].init(in_n, neuron_n[0]);
		for (int i = 1; i < length; i++) {
			a[i].init(neuron_n[i], a[i - 1]);
		}
	}
	//Input set, Output struct out_rez of result of every neuron on last layer
	out_rez calc(double* in) {
		//printf("\n 1 layer");
		a[0].calc(in);

		for (int i = 1; i < length; i++) {
			//printf("\n %i layer", i + 1);
			a[i].calc();
		}
		out_rez out;
		out.out_n = a[length - 1].out_n;
		out.out_rez = a[length - 1].out;
		return out;
	}
	void erase() {
		for (int i = 0; i < length; i++) {
			a[i].erase();
		}
		delete[] a;
		std::cout << "\nWeb deleted";
	}
	double rez = 0;
	void learn(double* learning_set, int full_set_lenght, int set_lenght, double* answer_set, double learning_rate) {
		for (int i = 0; i < full_set_lenght; i = i + set_lenght) {
			double* temp = new double[set_lenght];
			for (int j = 0; j < set_lenght; j++) {
				temp[j] = learning_set[i + j];
				//printf("\n\n%.20lf", temp[j]);
			}
			out_rez b = calc(temp);

			double erra = 0;
			erra = (b.out_rez[0] - answer_set[i / set_lenght]);
			printf("\n\n%.20lf", b.out_rez[0]);
			rez = b.out_rez[0];
			//printf("\n\n%.20lf", erra);
			//printf("\n\n%.20lf", erra);
			erra = a[length - 1].back_prop(erra, learning_rate);
			for (int i = length - 2; i >= 0; i--) {
				erra = a[i].back_prop(erra, learning_rate);
			}
			delete[] temp;
		}
	}
};

int main()
{
	srand(time(0));
	web a;
	int neoron_n[3] = { 2,1 };
	a.init(2, 1, neoron_n);
	double learning_set[] = { 0.0,0.0, 1.0,1.0, 1.0,0.0, 0.0,1.0, };
	double answer_set[] = { 0.0,1.0 , 1.0,0.0, };
	for (int i = 0; i < 10000; i++) {
		a.learn(learning_set, 8, 2, answer_set, 0.1);
	}

	out_rez temp = a.calc(in);
	for (int i = 0; i < temp.out_n; i++) {
		printf("\n\n%.20lf", temp.out_rez[i]);
	}
	printf("\n\n");
	a.erase();

	return 0;
}