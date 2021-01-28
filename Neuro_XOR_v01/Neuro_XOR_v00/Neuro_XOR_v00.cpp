

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
	double calc(double ** in) {
		double sum = 0;				
		for (int i = 0; i < lenght-1; i++) {			
			sum = sum + (*in[i]) * in_w[i];
		}		
		sum = sum +in_w[lenght - 1];		
		return (1 / (1 + exp(-sum)));
	}	
	void init(int l) {		
		in_w = new double[l];
		lenght = l;		
		for (int i = 0; i < lenght; i++) {
			in_w[i] = 1 / (double)(1 + (rand() % 1000));			
		}
	}
	void erase() {
		
		delete[] in_w;	
		std::cout << "\nNeuron deleted";
	}

};
class layer {
public:
	neuron* a;	
	int in_n,out_n;		
	double** in;
	double* out;
	
	void init(int out_kol, layer prev) {
		in_n = prev.out_n + 1;
		out_n = out_kol;
		a = new neuron[in_n - 1];
		in = new double* [in_n - 1];
		for (int i = 0; i < in_n - 1; i++) {
			in[i] = prev.out;
		}				
		out = new double [out_n];		
		for (int i = 0; i < out_n; i++) {
			a[i].init(in_n);
		}
	}	
	void init(int in_kol, int out_kol) {		
		in_n = in_kol + 1;
		out_n = out_kol;
		a = new neuron[out_n];
		in = new double* [in_n-1];		
		out = new double[out_n];
		for (int i = 0; i < out_n; i++) {
			a[i].init(in_n);
		}		
	}
	void calc() {
		for (int i = 0; i < out_n; i++) {
			out[i]=a[i].calc(in);
		}
	}	
	void calc(double * insert) {
		for (int i = 0; i < in_n - 1; i++) {
			in[i] = &insert[i];
		}
		for (int i = 0; i < out_n; i++) {
			out[i] = a[i].calc(in);
		}
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
	void init(int l, int in_n,int* neuron_n) {
		length = l;
		a = new layer[l];
		a[0].init(in_n, neuron_n[0]);
		for (int i = 1; i < length; i++) {
			a[i].init(neuron_n[i],a[i-1]);
		}
	}
	out_rez calc(double* in) {
		printf("\n 1 layer");
		a[0].calc(in);

		for (int i = 1; i < length; i++) {
			printf("\n %i layer", i + 1);
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
};

int main()
{
	
	srand(time(0));
	web a;
	int neoron_n[3] = { 2,2,1 };
	
	a.init(3,1,neoron_n);
	
	double in[] = { 1.0,0.0 };
	out_rez temp = a.calc(in);
	for (int i = 0; i < temp.out_n; i++) {
		printf("\n\n%.20lf", temp.out_rez[i]);
	}
	printf("\n\n");
	a.erase();
		
	return 0;
}

