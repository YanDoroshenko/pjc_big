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

vector<string> process_results(vector<entry> entries, short mode, short limit) {
    vector<string> result;
    map<string, short> occurences;
    vector<pair<string, short>> pairs;
    short max_name_length = 0;
    switch (mode) {
	case 1:
	    for (entry e: entries) {
		occurences[e.service_name]++;
	    }
	    for (auto itr = occurences.begin(); itr != occurences.end(); ++itr)
		pairs.push_back(*itr);
	    sort(pairs.begin(), pairs.end(), [=](pair<string, short>& a, pair<string, short>& b)
		    {
		    return a.second > b.second;
		    });
	    for (auto i : pairs)
		result.push_back(i.first + ": " + to_string(i.second));
	    break;
	case 2: 
	    sort(entries.begin(), entries.end(), compare_chronologically);
	    for (entry e: entries)
		result.push_back(e.date + " " + e.time + " " + e.service_name);
	    break;
	case 3: 
	    sort(entries.begin(), entries.end(), compare_alphabetically);
	    for (entry e: entries) {
		if (e.service_name.length() > max_name_length)
		    max_name_length = e.service_name.length();
	    }
	    for (entry e: entries) {
		string s = e.service_name;
		for (short i = 0; i <= max_name_length - e.service_name.length(); i++)
		    s += " ";
		s = s + e.date + " " + e.time;
		result.push_back(s);
	    }
	    break;
	case 4:
	    for (entry e: entries) {
		occurences[e.msisdn]++;
	    }
	    for (auto itr = occurences.begin(); itr != occurences.end(); ++itr)
		pairs.push_back(*itr);
	    sort(pairs.begin(), pairs.end(), [=](pair<string, short>& a, pair<string, short>& b)
		    {
		    return a.second > b.second;
		    });
	    for (auto i : pairs)
		result.push_back(i.first + ": " + to_string(i.second));
	    break;
    }
    if (limit != 0 && result.size() > limit)
	result.erase(result.begin() + limit, result.end());
    return result;
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
    for (string s : process_results(parse_results, selection, limit))
	cout << s << endl;
    delete[] r;
    delete[] pool;
}
