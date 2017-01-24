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
#include	<vector>

entry* parse(std::string);

std::vector<entry> parse_vector(std::vector<std::string>);
