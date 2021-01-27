

#include <iostream>
#include <ctime>
/*class neuron {
public:
	double** in = NULL;
	double* in_w = NULL;
	int lenght;
	double out_w;
	void init(int l) {
		lenght = l;
		in = new double*[l];
		in_w = new double[l];
		//std::cout << "\n";
		for (int i = 0; i < lenght; i++) {
			in_w[i] = 1 / (double)(1+rand() % 1000);
			//std::cout << in_w[i]<<' ';
		}
		//std::cout << act_w << ' ';
	}
	void calc() {
		double sum = 0;
		for (int i = 0; i < lenght; i++) {
			sum = sum + (*in[i]) * (in_w[i]);
		}
		out_w = 1 / (1 + exp(-sum));
		//printf("\n%lf", out_w);
	}
};
class neural_network {
public:
	neuron* l_in = NULL,* l_hiden1 = NULL,* l_out = NULL;
	void init() {
		l_in = new neuron[2];
		l_hiden1 = new neuron[2];
		l_out = new neuron[1];
		for (int i = 0; i < 2; i++) {
			l_in[i].init(1);
		}
		for (int i = 0; i < 2; i++) {
			l_hiden1[i].init(2);
			for (int j = 0; j < 2; j++) {
				l_hiden1[i].in[j] = &l_in[j].out_w;
			}
		}
		for (int i = 0; i < 1; i++) {
			l_out[i].init(2);
			for (int j = 0; j < 2; j++) {
				l_out[i].in[j] = &l_hiden1[j].out_w;
			}
		}

	}
	void prop(int* a, int l) {
		for (int i = 0; i < 2; i++) {
			double* temp = new double(a[i]);
			l_in[i].in[0] = temp;
			l_in[i].calc();
			//printf("\n %lf", *l_in[i].in[0]);
			//std::cout << std::endl << l_in[i].out_w << "<-Rezultat\n";
		}
		for (int i = 0; i < 2; i++) {
			l_hiden1[i].calc();
		}
		for (int i = 0; i < 1; i++) {
			l_out[i].calc();

		}
	}
	double error = 0;
	void calc(int* a, int l,int l_full) {
		double rez[4];
		int k = 0;
		for (int i = 0; i < l_full; i = i +2) {
			int temp[] = { a[i], a[i + 1] };
			prop(temp,l);
			rez[k] = l_out[0].out_w;
			if (temp[0] == temp[1]) {
				rez[k] = rez[k]-0;
			}
			else {
				rez[k] = rez[k]-1;
			}
			//printf("\n%i %i %.20lf", temp[0] , temp[1], l_out[0].out_w);
			k++;
		}
		error = (rez[0] * rez[0] + rez[1] * rez[1] + rez[2] * rez[2] + rez[3] * rez[3]);
		//printf("\n%.20lf", error);
	}

};*/

class neuron {
public:	
	double* in_w;
	int lenght;	
	double calc(double ** in) {
		double sum = 0;		
		for (int i = 0; i < lenght; i++) {			
			sum = sum + (*in[i]) * in_w[i];
		}		
		printf("\n%.20lf", sum);
		return (1 / (1 + exp(-sum)));
	}	
	void init(int l) {		
		in_w = new double[l];
		lenght = l;
		std::cout << "\n";
		for (int i = 0; i < lenght; i++) {
			in_w[i] = 1 / (double)(1 + (rand() % 1000));
			std::cout << in_w[i] << ' ';
		}
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
		a = new neuron[out_n];		
		in = new double* [in_n];
		for (int i = 0; i < in_n - 1; i++) {
			in[i] = prev.out;
		}		
		in[in_n-1] = new double(1.0);
		out = new double [out_n];		
		for (int i = 0; i < out_n; i++) {
			a[i].init(in_n);
		}
	}	
	void init(int in_kol, int out_kol) {		
		in_n = in_kol + 1;
		out_n = out_kol;
		a = new neuron[out_n];
		in = new double* [in_n];
		in[in_n - 1] = new double(1.0);
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
};
struct out_rez {
	int out_n;
	double* out_rez;
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
	/*neural_network a[10000];
	int b[] = { 0,0,0,1,1,0,1,1 };
	double min = 10000;
	for (int i = 0; i < 100; i++) {
		a[i].init();
		a[i].calc(b, 2, 8);
		if (a[i].error < min) {
			min = a[i].error;
		}
	}
	printf("\n%.20lf", min);*/
	//neuron a1;
	//a1.init(5);
	//printf("\n%.20lf %.20lf", *a1.in[5],a1.in_w[5]);



}

