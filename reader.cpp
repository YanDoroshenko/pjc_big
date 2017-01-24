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


#include	<iostream>
#include	<fstream>
#include	<vector>
#include	<thread>
#include	<future>
#include	"reader.hpp"
#include	"parser.hpp"

using namespace std;

void read_file(vector<string>* buffers, string filename, int threads_num) {
    long line_nr = 0;
    ifstream infile(filename);
    string line;
    int i = 0;
    while(getline(infile, line)) 
	buffers[i++ % threads_num].push_back(to_string(++line_nr) + "!!!" + line);
    infile.close();
}

int main(int c,char* args[]) {
    ifstream cpuinfo("/proc/cpuinfo");
    string s;
    short cpus = 0;
    while (cpuinfo >> s)
	if (s.find("processor") != string::npos)
	    ++cpus;
    cpuinfo.close();
    if (c < 2) {
	cout << "Enter filename!" << endl;
	return 1;
    }
    vector<string>* r = new vector<string>[cpus];
    thread t(read_file, r, args[1], cpus);
    cout << "Enter:" << endl;
    cout << "1 for top failing services" << endl;
    cout << "2 to see the errors in chronological order" << endl;
    cout << "3 to see the services in alphabetic order" << endl;
    cout << "4 to see the top problematic numbers" << endl;
    short selection;
    try {
	cin >> selection;
	if (selection < 1 || selection > 4)
	    throw exception();
    }
    catch (exception& e){
	cout << "Trying to mess with me, mr. user? Huh? Shutting down." << endl;
	t.join();
	delete[] r;
	return 1;
    }
    cout << "Enter maximum number of entries (0 for unlimited):" << endl;
    short limit; 
    try {
	cin >> limit;
	if (limit < 0)
	    throw exception();
    }
    catch (exception& e){
	cout << "Trying to mess with me, mr. user? Huh? Shutting down." << endl;
	t.join();
	delete[] r;
	return 1;
    }
    cout << "Working..." << endl;
   t.join();
    cout << "Reading file complete" << endl;
    future<vector<entry>>* pool = new future<vector<entry>>[cpus];
    for (int i = 0; i < cpus; i++) {
	pool[i] = async(parse_vector, r[i]);
    }
    cout << "Parser threads created" << endl;
    for (int i = 0; i < cpus; i++) {
	pool[i].wait();
    }
    cout << "Waiting for parsers to finish" << endl;
    vector<entry> parse_results;
    for (int i = 0; i < cpus; i++) {
	auto v = pool[i].get();
	parse_results.insert(parse_results.begin(), v.begin(), v.end());
    }
    for (string s : process(parse_results, selection, limit))
	cout << s << endl;
    delete[] r;
    delete[] pool;
}
