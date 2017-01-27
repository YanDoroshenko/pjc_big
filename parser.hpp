/*
 * =====================================================================================
 *
 *       Filename:  parser.hpp
 *
 *    Description:  Line parser that gets useful information
 *
 *        Version:  1.0
 *        Created:  20.11.16 15:56:49
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yan Doroshenko, yandoroshenko@protonmail.com
 *        Company:  CTU in Prague
 *
 * =====================================================================================
 */

#ifndef PARSER_H
#define PARSER_H

#include <memory>
#include <vector>
#include "entry.hpp"
#include "mode.hpp"

// create string from entry
std::string process_entry(std::unique_ptr<entry>, mode&);

// create an entry from string
std::unique_ptr<entry> parse(std::string&);

void parse_vector(std::vector<std::string>*, std::vector<std::string>*, mode); // parse an process a vector of strings and write results into the other vector

void merge(std::vector<std::string>*); // merge results from different threads

#endif
