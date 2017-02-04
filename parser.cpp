/*
 * =====================================================================================
 *
 *       Filename:  parser.cpp
 *
 *    Description:  Parses useful information from strings
 *
 *        Version:  1.0
 *        Created:  20.11.16 16:39:56
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yan Doroshenko (yd), yandoroshenko@protonmail.com
 *        Company:  CTU in Prague
 *
 * =====================================================================================
 */

#include <iostream>
#include <algorithm>
#include <map>
#include "parser.hpp"

using namespace std;

#define TIME_LENGTH 12
#define DATE_LENGTH 10

// create string from entry
string process_entry(unique_ptr<entry> e, mode &m) {
	switch (m) {
		case top_services:
			return e->service_name;
		case chronological:
			return e->time + " " + e->date + " " + e->service_name;
		case alphabetical:
			return e->service_name + " " + e->date + " " + e->time;
		case top_msisdn:
			return e->msisdn;
		default:
			return "WRONG MODE";
	}
}

// create an entry from string
unique_ptr<entry> parse(string &str) {
	long line_nr = stol(str.substr(0, str.find("!!!")));
	string date = str.substr(str.find("!!!") + 3, DATE_LENGTH);
	string time = str.substr(str.find(date) + date.length() + 1, TIME_LENGTH);
	int msisdn_start = str.find("[") + 1;
	int msisdn_end = str.find("]");
	string msisdn = str.substr(msisdn_start, msisdn_end - msisdn_start);
	int ws_name_start = str.find("WS,") + 3;
	int ws_name_end = str.find(",Err");
	string service_name = str.substr(ws_name_start, ws_name_end - ws_name_start);
	return unique_ptr<entry>(new entry(line_nr, date, time, service_name, msisdn));
}

// process input vector, write results to output vector
void parse_vector(vector<string> *input, vector<string> *output, mode m) {
	mutex mtx;
	map<string, int> occurences;
	if (m == chronological || m == alphabetical)
		for (string s: *input) {
			mtx.lock();
			output->push_back(process_entry(parse(s), m));
			mtx.unlock();
		}
	else {
		for (string s: *input)
			occurences[process_entry(parse(s), m)]++;
		for (auto i : occurences) {
			string s = to_string(i.second);
			s+=  ": ";
			s+= i.first;
			output->push_back(s);
		}
	}
	mtx.lock();
	sort(output->begin(), output->end()); // sort output
	mtx.unlock();
	input->clear(); // clear memory
}

// merge results
void merge(vector<string> *input) {
	map<string, int> occurences;
	vector<pair<string, int>> pairs;
	for (string s: *input)
		occurences[s.substr(s.find(": ") + 2)]+=stoi(s.substr(0, s.find(": ")));
	input->clear();
	for (auto i : occurences) {
		pairs.push_back({i.first, i.second});
	}
	sort(pairs.begin(), pairs.end(), [=](const pair<string, int>&p1, const pair<string, int> &p2) {
			return p2.second < p1.second;
			});
	for (auto i : pairs) {
		string s = to_string(i.second);
		s+=  ": ";
		s+= i.first;
		input->push_back(s);
	}
}

void limit_results(vector<string>* input, short &limit) {
	if (limit != 0 && limit > input->size())
		input->erase(input->begin() + limit, input->end());
}
