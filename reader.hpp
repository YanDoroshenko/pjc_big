/*
 * =====================================================================================
 *
 *       Filename:  file_reader.hpp
 *
 *    Description:  Header for a file-reader
 *
 *        Version:  1.0
 *        Created:  20.11.16 12:50:46
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yan Doroshenko
 *        Company:  CTU in Prague
 *
 * =====================================================================================
 */
#include <iostream>
#include	<vector>
#include	<string>
#include <algorithm>
#include <map>
#ifndef ENTRY_H
#include "entry.hpp"
#endif

void read_file(std::vector<std::string>* buffers, std::string filename, int threads_num);

std::vector<std::string> process(std::vector<entry> entries, short mode, short limit) {
    std::vector<std::string> result;
    std::map<std::string, short> occurences;
    std::vector<std::pair<std::string, short>> pairs;
    short max_name_length = 0;
    switch (mode) {
	case 1:
	    for (entry e: entries) {
		occurences[e.service_name]++;
	    }
	    for (auto itr = occurences.begin(); itr != occurences.end(); ++itr)
		pairs.push_back(*itr);
	    std::sort(pairs.begin(), pairs.end(), [=](std::pair<std::string, short>& a, std::pair<std::string, short>& b)
		    {
		    return a.second > b.second;
		    });
	    for (auto i : pairs)
		result.push_back(i.first + ": " + std::to_string(i.second));
	    break;
	case 2: 
	    std::sort(entries.begin(), entries.end(), compare_chronologically);
	    for (entry e: entries)
		result.push_back(e.date + " " + e.time + " " + e.service_name);
	    break;
	case 3: 
	    std::sort(entries.begin(), entries.end(), compare_alphabetically);
	    for (entry e: entries) {
		if (e.service_name.length() > max_name_length)
		    max_name_length = e.service_name.length();
	    }
	    for (entry e: entries) {
		std::string s = e.service_name;
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
	    std::sort(pairs.begin(), pairs.end(), [=](std::pair<std::string, short>& a, std::pair<std::string, short>& b)
		    {
		    return a.second > b.second;
		    });
	    for (auto i : pairs)
		result.push_back(i.first + ": " + std::to_string(i.second));
	    break;
    }
    if (limit != 0 && result.size() > limit)
	result.erase(result.begin() + limit, result.end());
    return result;
}
