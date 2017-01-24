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

#include <regex>
#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include "entry.hpp"

using namespace std;

#define TIME_LENGTH 12
#define DATE_LENGTH 10

entry parse(string str) {
    long line_nr = stol(str.substr(0, str.find("!!!")));
    string date = str.substr(str.find("!!!") + 3, DATE_LENGTH);
    string time = str.substr(str.find(date) + date.length() + 1, TIME_LENGTH);
    int msisdn_start = str.find("[") + 1;
    int msisdn_end = str.find("]");
    string msisdn = str.substr(msisdn_start, msisdn_end - msisdn_start);
    int ws_name_start = str.find("WS,") + 3;
    int ws_name_end = str.find(",Err");
    string service_name = str.substr(ws_name_start, ws_name_end - ws_name_start);
    return *(new entry(line_nr, date, time, service_name, msisdn));
}

vector<entry> parse_vector(vector<string> input) {
    auto start = chrono::high_resolution_clock::now();
    vector<entry> result;
    for (string s: input) {
	bool ws_error = s.find(",Err,") != string::npos;
	if (ws_error)
	    result.push_back(parse(s));
    }
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double, nano>elapsed = end - start;
    cout << "Thread finished processing, " +  to_string(elapsed.count() / input.size()) + "ns per line" << endl;
    return result;
}
