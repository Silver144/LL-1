#pragma once
#include <map>
#include <string>
#include <vector>
#include <set>
#include <iostream>

class terminator
{
public:
	terminator(char val) : _val(val) {}
	char _val;
};


class non_terminator
{
public:
	non_terminator(char val) : _val(val) {}
	char _val;
};


class symbol
{
public:
	symbol(std::string val) : _val(val) {}
	std::string _val;
};

namespace partable
{
	extern std::set<terminator> _terminator;
	extern std::vector<std::pair<non_terminator, symbol>> generator;

	extern std::set<non_terminator> _non_terminator;

	// extern std::map<non_terminator, std::set<terminator>> _first_set;
	// extern std::map<non_terminator, std::set<terminator>> _follow_set;
	extern std::map<std::pair<terminator, non_terminator>, int> _table;
}

using mns = std::map<non_terminator, symbol>::value_type;
using mnt = std::map<non_terminator, std::set<terminator>>::value_type;
using mnp = std::pair<terminator, non_terminator>;
using mpti = std::map<std::pair<terminator, non_terminator>, int>::value_type;