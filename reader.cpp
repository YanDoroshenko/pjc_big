/*
 * =====================================================================================
 *
 *       Filename:  file_reader.cpp
 *
 *    Description:  Sequential line-by-line file reader
 *
 *        Version:  1.0
 *        Created:  15.11.16 20:10:09
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yan Doroshenko
 *        Company:  CTU in Prague
 *
 * =====================================================================================
 */


#include "mode.hpp"
#include "parser.hpp"
#include "reader.hpp"
#include <chrono>
#include <fstream>
#include <future>
#include <iostream>
#include <map>
#include <parallel/algorithm>
#include <thread>
#include <vector>

using namespace std;

// Function that single-threadily reads file, filtering the lines
void read_file(vector<string>* buffers, string filename, int threads_num) {
	long line_nr = 0; // line number counter
	ifstream infile(filename);
	string line;
	int i = 0;
	while(getline(infile, line)) 
		if (line.find("Err,") != string::npos) // filter line
			buffers[i++ % threads_num].push_back(to_string(++line_nr) + "!!!" + line);
	infile.close();
}

int main(int c,char* args[]) {
	auto start = chrono::high_resolution_clock::now(); // time measuring start
	int cpus; // logical CPU number
	short selection; // mode selection
	short limit; //limit of row numbers
	thread t; // file reading thread
	vector<string>* r; // buffers
	if (c == 5) { // non-interactive mode
		cpus = stoi(args[2]);
		selection = stoi(args[3]);
		limit = stoi(args[4]);
		r = new vector<string>[cpus];
		t = thread(read_file, r, args[1], cpus); // start reading file
	}
	else { // interactive mode
		cpus = thread::hardware_concurrency(); // get count of logical CPUs
		if (c < 2) {
			cout << "Enter filename!" << endl;
			return 1;
		}
		r = new vector<string>[cpus];
		t = thread(read_file, r, args[1], cpus); // start reading file
		cout << "Enter:" << endl;
		cout << "1 for top failing services" << endl;
		cout << "2 to see the errors in chronological order" << endl;
		cout << "3 to see the services in alphabetic order" << endl;
		cout << "4 to see the top problematic numbers" << endl;

		/* doing stupid stuff here because I can't throw exceptions */
		try {
			cin >> selection;
			if (selection < 1 || selection > 4) {
				cout << "Trying to mess with me, mr. user? Huh? Shutting down." << endl;
				t.join(); // wait for file reading to finish
				delete[] r; // clear memory
				return 1;
			}
		}
		catch (exception& e){
			cout << "Trying to mess with me, mr. user? Huh? Shutting down." << endl;
			t.join(); // wait for file reading to finish
			delete[] r; // clear memory
			return 1;
		}
		cout << "Enter maximum number of entries (0 for unlimited):" << endl;
		try {
			cin >> limit;
			if (limit < 0) {
				cout << "Trying to mess with me, mr. user? Huh? Shutting down." << endl;
				t.join(); // wait for file reading to finish
				delete[] r; // clear memory
				return 1;
			}
		}
		catch (exception& e){
			cout << "Trying to mess with me, mr. user? Huh? Shutting down." << endl;
			t.join(); // wait for file reading to finish
			delete[] r; // clear memory
			return 1;
		}
	}
	cout << "Working..." << endl;
	t.join(); // wait for file reading to finish
	cout << "Reading file complete" << endl;
	future<void>* pool = new future<void>[cpus]; // executors 
	vector<string> parse_results; // output vector
	mode m = mode(selection - 1); // selected mode
	for (int i = 0; i < cpus; i++) {
		pool[i] = async(parse_vector, &r[i], &parse_results, m); // create futures to process the buffers
	}
	cout << "Parser threads created" << endl;
	for (int i = 0; i < cpus; i++) {
		pool[i].wait(); // wait for parsers to finish
	}
	cout << "Waiting for parsers to finish" << endl;
	for (string s : parse_results)
		cout << s << endl; // print results
	delete[] r; // clear memory
	delete[] pool; // clear memory
	auto end = chrono::high_resolution_clock::now(); // measuring time end
	cout << "Execution took " << to_string((double)(end - start).count() / 1000000000) << endl; // show time elapsed
}
