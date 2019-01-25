/***************************************************************
Author:	Nathan Festoso
Date: Jan 7, 2018
***************************************************************/

#define _CRT_SECURE_NO_WARNINGS

#include <string>
#include <fstream>
#include <iostream>
#include <vector>
using namespace std;

double ODE(double a, double x, double v);
 
double const STEP = 0.0461;
double const K = 20;
double const B = 10;

/***************************************************************
Function name:		main
Purpose:			Prompt and calculate equations
In parameters:		-
Out parameters:		-
Version:			1.0
Author:				Nathan Festoso
***************************************************************/
int main(void) {
	ifstream input;
	ofstream output;
	string src_file;
	string dst_file;
	vector<double> data[5];
	string num;
	int option = 1;

	while(option != 2) {
		// main menu
		cout << "Earthquake Simulation" << endl;
		cout << " 1. Run simulation" << endl << " 2. Quit" << endl;
		cin >> option;

		// get source file name
		cout << "Please enter the name of the earthquake file to open: ";
		cin >> src_file;

		// load source file
		input.open(src_file);
		unsigned int i = 0;
		if (input.is_open()) {
			for (i = 0; input >> num; i++) {
				if (i % 2)
					data[1].push_back(atof(num.c_str()));
				else
					data[0].push_back(atof(num.c_str()));
			}
		}
		else { cout << "Invalid source file" << endl; continue; }
		input.close();
		cout << "File opened; " << (i / 2) << " rows of data" << endl;

		// get destination file
		cout << "OPEN FILE TO SAVE" << endl;
		cout << "Please enter the name of the file to open: ";
		cin >> dst_file;
		cout << endl;

		// Populate initial data values
		data[2].push_back(0);
		data[3].push_back(0);
		data[4].push_back(0);

		// COLUMN 3
		// f(x+h) - 2f(x) + f(x-h) / h^2
		double result;
		for (i = 1; i < data[0].size() - 1; i++) {
			result = ((data[1][i + 1]/100) - (2*(data[1][i]/100)) + (data[1][i - 1]/100)) / (STEP * STEP);
			data[2].push_back(result);
		}

		// COLUMN 4 & 5
		double a;
		double v;
		double x;
		double v_;
		double x_;
		double v_p;
		double x_p;
		double corrector;
		for (i = 0; i < data[0].size()-1; i++) {			
			double a = data[2][i];
			double v = data[4][i];
			double x = data[1][i];
			// Step 1 - find V'i+1 = vi + (a-kxi-bvi) * h
			v_p = data[4][i] + (ODE(a,x,v)*STEP);
			// Step 2 - find Vi+1 = vi + ((a-kxi-bvi) (a-kxi-bV'i+1))* h )/ 2
			v_ = data[4][i] + ((ODE(a,x,v) + ODE(a,x,v_p)*STEP)/2);
			data[4].push_back(v_);

			// cant figure out how to do column 4 or if column 5 is correct
			data[3].push_back(0.000);
		}

		// write to output file
		output.open(dst_file);
		if (output.is_open()) {
			for (i = 0; i < data[0].size()-1; i++) {
				output << data[0][i] << "\t";
				output << data[1][i] << "\t\t";
				output << data[2][i] << "\t\t";
				output << "0.0000" << "\t\t";
				output << data[4][i] << "\n";
			}
		}
		else { cout << "Invalid destination file" << endl; continue; }

	}
}

/***************************************************************
Function name:		ODE
Purpose:			Ordinary differential equation
In parameters:		a, x, v
Out parameters:		result of calculation
Version:			1.0
Author:				Nathan Festoso
***************************************************************/
double ODE(double a, double x, double v) {
	return (a - (K*x) - (B*v));
}