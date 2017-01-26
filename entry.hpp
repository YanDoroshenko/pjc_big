/*
 * =====================================================================================
 *
 *       Filename:  entry.hpp
 *
 *    Description:  Log entry
 *
 *        Version:  1.0
 *        Created:  14.12.16 19:52:28
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yan Doroshenko (yd), yandoroshenko@protonmail.com
 *        Company:  CTU in Prague
 *
 * =====================================================================================
 */

#ifndef ENTRY_H
#define ENTRY_H

#include<string>

class entry {
	public:
		std::string date, time, msisdn, service_name;
		long line_nr;
		entry(long &line_nr, std::string &date, std::string &time, std::string &service_name, std::string &msisdn);
};

bool compare_alphabetically(const entry& e1, const entry& e2);

bool compare_chronologically(const entry& e1, const entry& e2);

#endif
