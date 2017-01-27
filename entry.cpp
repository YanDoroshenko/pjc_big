/*
 * =====================================================================================
 *
 *	   Filename:  entry.cpp
 *
 *	Description:  Log entry
 *
 *		Version:  1.0
 *		Created:  14.12.16 19:56:21
 *	   Revision:  none
 *	   Compiler:  gcc
 *
 *		 Author:  Yan Doroshenko (yd), yandoroshenko@protonmail.com
 *		Company:  CTU in Prague
 *
 * =====================================================================================
 */

#include "entry.hpp"

entry::entry(long &line_nr, std::string &date, std::string &time, std::string &service_name, std::string &msisdn) {
	this->line_nr = line_nr;
	this->date = date;
	this->time = time;
	this->service_name = service_name;
	this->msisdn = msisdn;
}

// compare by service name
bool compare_alphabetically(const entry& e1, const entry& e2) {
	if (e1.service_name == e2.service_name) {
		if (e1.date == e2.date)
			return e1.time < e2.time;
		else 
			return e1.date < e2.date;
	}
	return e1.service_name < e2.service_name;
}

// compare chronologically
bool compare_chronologically(const entry& e1, const entry& e2) {
	if (e1.date == e2.date)
		return e1.time > e2.time;
	return e1.date > e2.date;
}
