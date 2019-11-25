#pragma once
#include <map>
#include <string>
#include <vector>
#include <set>
#include <iostream>

class table_index
{
public:
	table_index() = default;
	table_index(char t, char nt): _t(t), _nt(nt) {}

	char _t;
	char _nt;

};

template <typename T>
constexpr bool _t_less(const T& _Tx, const T& _Ty)
{
	return _Tx < _Ty;
}

template <> constexpr bool _t_less<table_index> (const table_index& _Tx, const table_index& _Ty)
{
	return _Tx._t < _Ty._t ? true : _Tx._t == _Ty._t
		? _Tx._nt < _Ty._nt ? true : false : false;
}

namespace partable
{
	inline std::set<char> _terminator;
	inline std::vector<std::pair<char, std::string>> generator;

	inline std::set<char> _non_terminator;

	inline std::map<char, std::set<char>> _first_set;
	inline std::map<char, std::set<char>> _follow_set;
	inline std::map<table_index, int, std::integral_constant<decltype(&_t_less<table_index>), &_t_less<table_index>>> _table;
}

using mns = std::map<char, std::string>::value_type;
using mnt = std::map<char, std::set<char>>::value_type;
using mnp = std::pair<char, char>;
using mti = std::map<table_index, int>::value_type;

void partable_init();
void _create_partable();
