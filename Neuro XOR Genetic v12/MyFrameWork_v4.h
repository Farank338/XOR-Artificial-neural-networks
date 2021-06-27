#pragma once
#include <iostream>
#include <vector>
#include "windows.h"
#include "psapi.h"
#include <ctime>
#include <chrono>
#include <future>
#include <thread>
#include <fstream>
void print_mem() {
	PROCESS_MEMORY_COUNTERS_EX pmc;
	GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)& pmc, sizeof(pmc));
	SIZE_T virtualMemUsedByMe = pmc.PrivateUsage;
	SIZE_T physMemUsedByMe = pmc.WorkingSetSize;
	std::cout << virtualMemUsedByMe << " " << physMemUsedByMe << "\n";
}

double rnd() {
	int	accuracy = 1000;
	int precision = ((int)(accuracy / 10));
	double res = ((double)(1 + rand() % accuracy));
	res = res / ((double)(5.0 * precision)) - 1.0;
	return res;
}
class ActFunc {
private:
	double par = 0.0;
public:
	virtual double compute(double input) {
		return par;
	}
	virtual std::shared_ptr<ActFunc> clone() {
		std::shared_ptr<ActFunc> a(new ActFunc());
		a->set_par(par);
		return(a);
	}
	virtual void mutate(double level) {
		par += rnd();
	}
	virtual int get_id() {
		return(0);
	}
	virtual void set_par(double arg) {
		par = arg;
	}
	virtual double get_par() {
		return par;
	}
	virtual ~ActFunc() = default;
};
class AsIs :public ActFunc {
private:
	double par = 1.0;
public:

	double compute(double input) {
		return par * input;
	}
	std::shared_ptr<ActFunc> clone() {
		std::shared_ptr<ActFunc> a(new AsIs());
		a->set_par(par);
		return(a);
	}
	void mutate(double level) {
		par += rnd();
	}
	int get_id() {
		return(1);
	}
	void set_par(double arg) {
		par = arg;
	}
	double get_par() {
		return par;
	}
};
class Sigmoid :public ActFunc {
private:
	double par = 1.0;
public:

	double compute(double input) {
		return 1.0 / (1.0 + exp(-par * input));
	}
	std::shared_ptr<ActFunc> clone() {
		std::shared_ptr<ActFunc> a(new Sigmoid());
		a->set_par(par);
		return(a);
	}
	void mutate(double level) {
		par += rnd();
	}
	int get_id() {
		return(2);
	}
	void set_par(double arg) {
		par = arg;
	}
	double get_par() {
		return par;
	}
};
class Threshold :public ActFunc {
private:
	double par = 0.0;
public:

	double compute(double input) {
		if (input > par) {
			return 1;
		}
		else {
			return 0;
		}
	}
	std::shared_ptr<ActFunc> clone() {
		std::shared_ptr<ActFunc> a(new Threshold());
		a->set_par(par);
		return(a);
	}
	void mutate(double level) {
		par += rnd();
	}
	int get_id() {
		return(3);
	}
	void set_par(double arg) {
		par = arg;
	}
	double get_par() {
		return par;
	}
};

class Neuron {
public:
	std::shared_ptr<ActFunc> func;
	std::vector<double> weight;
	void init(int weight_n, std::shared_ptr<ActFunc> new_func) {
		func = new_func;
		for (int i = 0; i < weight_n + 1; i++) {
			weight.push_back(rnd());
		}
	}
	double compute(std::vector<double>& inputs) {
		double sum = 0;
		//printf(" %i ", weight.size());
		int loop_count = weight.size();
		for (int i = 0; i < loop_count - 1; i++) {
			sum += inputs[i] * weight[i];
		}
		sum += weight[loop_count - 1];

		return (func->compute(sum));
	}
	Neuron clone() {
		Neuron res;
		int loop_count = weight.size();
		for (int i = 0; i < loop_count; i++) {
			res.weight.push_back(weight[i]);
		}
		res.func = func->clone();
		return res;
	}
	void mutate(double& level) {
		int loop_count = weight.size();
		for (int i = 0; i < loop_count; i++) {
			weight[i] = weight[i] + rnd() * level;
		}

		int i = rand() % 4;

		if (i == 0) {
			func = std::shared_ptr<ActFunc>(new ActFunc);
		}
		if (i == 1) {
			func = std::shared_ptr<ActFunc>(new AsIs);
		}
		if (i == 2) {
			func = std::shared_ptr<ActFunc>(new Sigmoid);
		}
		if (i == 3) {
			func = std::shared_ptr<ActFunc>(new Threshold);
		}

		i = rand() % 2;
		if (i == 1) {
			func->mutate(level);
		}
	}
	void print_all(int level) {
		std::cout << "\n\n     Neuron level=" << level + 1;
		std::cout << "\n     weight={";
		int loop_number = weight.size();
		for (int i = 0; i < loop_number; i++) {
			std::cout << weight[i] << " ";
		}
		std::cout << "}\n     func->id=" << func->get_id() << " func->par=" << func->get_par() << " ";;
	}
	void save_all(std::ofstream& file) {
		int loop_number = weight.size();
		file << loop_number << "\n";
		for (int i = 0; i < loop_number; i++) {
			file << weight[i] << " ";
		}
		file << func->get_id() << " " << func->get_par() << "\n";
	}
	void load_all(std::ifstream& file) {
		int loop_number;
		file >> loop_number;
		for (int i = 0; i < loop_number; i++) {
			double temp;
			file >> temp;
			weight.push_back(temp);
		}
		int id;
		file >> id;
		if (id == 0) {
			func = std::shared_ptr<ActFunc>(new ActFunc);
		}
		if (id == 1) {
			func = std::shared_ptr<ActFunc>(new AsIs);
		}
		if (id == 2) {
			func = std::shared_ptr<ActFunc>(new Sigmoid);
		}
		if (id == 3) {
			func = std::shared_ptr<ActFunc>(new Threshold);
		}
		double par;
		file >> par;
		func->set_par(par);
	}
};
class Layer {
public:

	std::vector<Neuron> n;
	std::vector<double> outputs;

	void init(int neuron_count, int inputs_count) {
		for (int i = 0; i < neuron_count; i++) {
			Neuron temp;
			temp.init(inputs_count, std::shared_ptr<ActFunc>(new Sigmoid));
			n.push_back(temp);
		}
		for (int i = 0; i < neuron_count; i++) {
			double temp;
			outputs.push_back(temp);
		}
	}
	void compute(std::vector<double>& inputs) {
		int loop_number = n.size();
		std::vector<std::future<double>> compute_unit;
		for (int i = 0; i < loop_number; i++) {
			compute_unit.emplace_back(std::async(std::launch::async | std::launch::deferred, &Neuron::compute, n[i], std::ref(inputs)));
		}
		for (int i = 0; i < loop_number; i++) {
			outputs[i] = compute_unit[i].get();
		}
	}
	Layer clone() {
		Layer res;
		int loop_number = n.size();
		for (int i = 0; i < loop_number; i++) {
			res.n.push_back(n[i].clone());
		}
		for (int i = 0; i < loop_number; i++) {
			double temp;
			res.outputs.push_back(temp);
		}
		return res;
	}
	void mutate(double& level) {
		int loop_number = n.size();

		for (int i = 0; i < loop_number; i++) {
			n[i].mutate(level);
		}
	}
	void print_all(int level) {
		std::cout << "\n\nLayer level=" << level;
		int loop_number = n.size();
		for (int i = 0; i < loop_number; i++) {
			n[i].print_all(i);
		}
	}
	void save_all(std::ofstream& file) {
		int loop_number = n.size();
		file << loop_number << "\n";
		for (int i = 0; i < loop_number; i++) {
			n[i].save_all(file);
		}
	}
	void load_all(std::ifstream& file) {
		int loop_number;
		file >> loop_number;
		for (int i = 0; i < loop_number; i++) {
			Neuron temp;
			n.emplace_back(temp);
			n[i].load_all(file);
		}
		for (int i = 0; i < loop_number; i++) {
			double temp;
			outputs.push_back(temp);
		}
	}
};
class Web {
public:
	std::vector<double> outputs;
	std::vector<Layer> l;

	void init(int inputs_count, std::vector<int> layer) {
		int loop_number = layer.size();
		Layer temp0;
		temp0.init(layer[0], inputs_count);
		l.push_back(temp0);
		for (int i = 1; i < loop_number; i++) {
			Layer temp1;
			temp1.init(layer[i], layer[i - 1]);
			l.push_back(temp1);
		}
	};
	void compute(std::vector<double> & inputs) {
		int loop_number = l.size();
		l[0].compute(inputs);
		for (int i = 1; i < loop_number; i++) {
			l[i].compute(l[i - 1].outputs);
		}
		outputs = l[loop_number - 1].outputs;
	};

	Web clone() {
		Web res;
		int loop_number = l.size();
		for (int i = 0; i < loop_number; i++) {
			res.l.push_back(l[i].clone());
		}
		return res;
	}
	void mutate(double level) {
		int loop_number = l.size();
		for (int i = 0; i < loop_number; i++) {
			l[i].mutate(level);
		}
	}
	void print_all() {
		std::cout << "\n ";
		int loop_number = l.size();
		for (int i = 0; i < loop_number; i++) {
			l[i].print_all(i);
		}
		std::cout << "\n ";
	}
	void save_all() {
		std::cout << "Enter file name\n";
		std::string path;
		std::cin >> path;
		std::ofstream file(path);
		int loop_number = l.size();
		file << loop_number << "\n";
		for (int i = 0; i < loop_number; i++) {
			l[i].save_all(file);
		}
	}
	void load_all() {
		std::cout << "Enter file name\n";
		std::string path;
		std::cin >> path;
		std::ifstream file(path);
		if (file.is_open()) {
			if (l.size() > 0) {
				l.clear();
				l.shrink_to_fit();
			}
			if (outputs.size() > 0) {
				outputs.clear();
				outputs.shrink_to_fit();
			}
			int loop_number;
			file >> loop_number;
			for (int i = 0; i < loop_number; i++) {
				Layer temp;
				l.emplace_back(temp);
				l[i].load_all(file);
			}
		}
	}
};
class StudyData {
public:
	std::vector<double> inputs;
	std::vector<double> outputs;
	//bool normalized = false;
	StudyData() {};
	StudyData(std::vector<double> in, std::vector<double> out) {
		inputs.insert(inputs.end(), in.begin(), in.end());
		outputs.insert(outputs.end(), out.begin(), out.end());
	}
	void add_iputs(std::vector<double> in) {
		inputs.insert(inputs.end(), in.begin(), in.end());
	}
	void add_outputs(std::vector<double> out) {
		outputs.insert(outputs.end(), out.begin(), out.end());
	}
};
class fast_sorting {
public:
	struct int_pos_n_double_res {
		int pos;
		double res;
	};
	struct int_pos_N_double_res_N_web {
		int pos;
		double res;
		Web* net;
	};
	void sort(fast_sorting::int_pos_N_double_res_N_web* a, int length) {
		if (length > 1) {
			int wall = 0, cur = 0, pivot = length - 1;
			for (int i = 0; i < length - 1; i++) {
				if (a[i].res <= a[pivot].res) {
					int_pos_N_double_res_N_web temp0 = a[i];
					a[i] = a[wall];
					a[wall] = temp0;
					wall++;
				}
			}
			if (length > 2) {
				int_pos_N_double_res_N_web temp0 = a[pivot];
				a[pivot] = a[wall];
				a[wall] = temp0;
				auto a1 = std::async(std::launch::async | std::launch::deferred, &fast_sorting::sort, this, a, wall);
				auto a2 = std::async(std::launch::async | std::launch::deferred, &fast_sorting::sort, this, &a[wall + 1], length - wall - 1);

				a1.wait();
				a2.wait();
			}
			else {
				return;
			}
		}
	}
	/*
	void sort(double* a, int length) {
		if (length > 1) {
			int wall = 0, cur = 0, pivot = length - 1;
			for (int i = 0; i < length - 1; i++) {
				if (a[i] <= a[pivot]) {
					double temp0 = a[i];
					a[i] = a[wall];
					a[wall] = temp0;
					wall++;
				}
			}
			if (length > 2) {
				double temp0 = a[pivot];
				a[pivot] = a[wall];
				a[wall] = temp0;
				sort(a, wall);
				sort(&a[wall], length - wall);
			}
			else {
				return;
			}
		}
	}*/
};
class Genetic {
public:
	double mutation_level;
	int web_count = 10;
	int max_web_cont = 10;
	std::vector<Web> Network;
	std::vector<StudyData> Example;
	void init(Web a, double mut_level) {
		mutation_level = mut_level;
		Network.clear();
		Network.shrink_to_fit();
		for (int i = 0; i < web_count; i++) {
			Network.push_back(a.clone());
		}
		for (int i = 1; i < web_count; i++) {
			Network[i].mutate(mutation_level);
		}
		sort(Network);
	}
	double calc(Web& net, int data) {
		double error = 0.0;
		net.compute(Example[data].inputs);
		int loop_count = net.outputs.size();

		for (int i = 0; i < loop_count; i++) {
			error = error + abs(Example[data].outputs[i] - net.outputs[i]);
		}
		return error;
	}
	double calc_all(Web & net) {
		double error = 0.0;
		int loop_count = Example.size();
		for (int i = 0; i < loop_count; i++) {
			double temp0 = calc(net, i);
			error = error + temp0;
		}

		return error;
	}
	void sort(std::vector<Web> & net) {
		int loop_count = net.size();
		fast_sorting temp0;
		fast_sorting::int_pos_N_double_res_N_web* collection = new fast_sorting::int_pos_N_double_res_N_web[loop_count];

		std::vector<std::future<double>> compute_unit;
		for (int i = 0; i < loop_count; i++) {
			compute_unit.emplace_back(std::async(std::launch::async | std::launch::deferred, &Genetic::calc_all, this, std::ref(net[i])));
		}
		for (int i = 0; i < loop_count; i++) {
			collection[i].res = compute_unit[i].get();
			collection[i].net = &net[i];
			collection[i].pos = i;
		}
		temp0.sort(collection, loop_count);
		for (int i = 0; i < loop_count; i++) {
			net[i] = *collection[i].net;
		}
		delete[] collection;
		collection = nullptr;
	}
	void reproduse() {
		int loop_count = Network.size();
		for (int i = 0; i < loop_count; i++) {
			for (int j = 0; j < loop_count; j++) {
				Web temp = Network[j].clone();
				temp.mutate(mutation_level);
				Network.push_back(temp);
			}
		}
		sort(Network);
		Network.erase(Network.begin() + max_web_cont, Network.end() - 1);
		/*loop_count = Network.size();
		for (int i = 0; i < loop_count; i++) {
			if (calc_all(Network[i]) < 0.1) {
				printf("\n\n");
				printf("\n\n");
				printf("\n\n");
				Network[i].print_all();
				printf("\n\n");
				//calc_all(Network[i]);
				printf("\n\n");
				printf("%.40fl \n", calc_all(Network[i]));
				Network[i].compute(Example[0].inputs);
				for (int i = 0; i < Network[i].outputs.size(); i++) {
					printf("%.4fl \n", Network[i].outputs[i]);
				}
				printf("\n\n");
				Network[i].compute(Example[1].inputs);
				for (int i = 0; i < Network[i].outputs.size(); i++) {
					printf("%.4fl \n", Network[i].outputs[i]);
				}
				printf("\n\n");
				Network[i].compute(Example[2].inputs);
				for (int i = 0; i < Network[i].outputs.size(); i++) {
					printf("%.4fl \n", Network[i].outputs[i]);
				}
				printf("\n\n");
				Network[i].compute(Example[3].inputs);
				for (int i = 0; i < Network[i].outputs.size(); i++) {
					printf("%.4fl \n", Network[i].outputs[i]);
				}
				printf("\n\n");
				printf("\n\n");
				printf("\n\n");
			}
		}*/
	}
	Web evolute(int epoh) {
		double prev_error = 0.0;
		int count = 0;
		for (int i = 0; i < epoh; i++) {
			unsigned int start_time = clock();
			reproduse();
			if (prev_error == 0) {
				prev_error = calc_all(Network[0]);
			}
			if (prev_error - 0.1 <= calc_all(Network[0])) {
				count++;
			}
			else {
				count = 0;
			}

			printf("%.40fl %i \n", calc_all(Network[0]), i);
			if (calc_all(Network[0]) < 0.1) {
				return Network[0].clone();
			}
			unsigned int end_time = clock();
			if (count >= 10) {
				srand(end_time - start_time);
			}
			//srand(rand() % epoh);
		}
		return Network[0].clone();
	}
};
