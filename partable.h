#pragma once
#include <map>
#include <string>
#include <vector>
#include <set>
#include <iostream>

class terminator
{
public:
	terminator() = default;
	terminator(char val) : _val(val) {}

	char _val;
};


class non_terminator
{
public:
	non_terminator() = default;
	non_terminator(char val) : _val(val) {}

	char _val;
};


class symbol
{
public:
	symbol() = default;
	symbol(std::string val) : _val(val) {}

	std::string _val;
};

class table_index
{
public:
	table_index() = default;
	table_index(terminator t, non_terminator nt): _t(t), _nt(nt) {}

	terminator _t;
	non_terminator _nt;

};

template <typename T>
constexpr bool _t_less(const T& _Tx, const T& _Ty)
{
	return _Tx < _Ty;
}

template <> constexpr bool _t_less<table_index> (const table_index& _Tx, const table_index& _Ty)
{
	return _Tx._t._val < _Ty._t._val ? true : _Tx._t._val == _Ty._t._val
		? _Tx._nt._val < _Ty._nt._val ? true : false : false;
}

template <> constexpr bool _t_less<non_terminator> (const non_terminator& _Tx, const non_terminator& _Ty)
{
	return _Tx._val < _Ty._val;
}


template <> constexpr bool  _t_less<terminator>(const terminator& _Tx, const terminator& _Ty)
{
	return _Tx._val < _Ty._val;
}

namespace partable
{
	extern std::set<terminator, std::integral_constant<decltype(&_t_less<terminator>), &_t_less<terminator>>> _terminator;
	extern std::vector<std::pair<non_terminator, symbol>> generator;

	extern std::set<non_terminator, std::integral_constant<decltype(&_t_less<non_terminator>), &_t_less<non_terminator>>> _non_terminator;

	// extern std::map<non_terminator, std::set<terminator>> _first_set;
	// extern std::map<non_terminator, std::set<terminator>> _follow_set;
	extern std::map<table_index, int, std::integral_constant<decltype(&_t_less<table_index>), &_t_less<table_index>>> _table;
}

using mns = std::map<non_terminator, symbol>::value_type;
using mnt = std::map<non_terminator, std::set<terminator, std::integral_constant<decltype(&_t_less<terminator>), &_t_less<terminator>>>>::value_type;
using mnp = std::pair<terminator, non_terminator>;
using mti = std::map<table_index, int>::value_type;

void _create_partable();
