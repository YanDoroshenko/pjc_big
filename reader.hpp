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

#ifndef READER_H
#define READER_H

#include <vector>
#include <string>
#include "entry.hpp"

void read_file(std::vector<std::string>* buffers, std::string filename, int threads_num); // read file in a single thread

#endif
