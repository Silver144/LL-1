#include "partable.h"
#include <algorithm>

namespace partable
{
	std::set<terminator, std::integral_constant<decltype(&_t_less<terminator>), &_t_less<terminator>>> _terminator = { '+', '-', '*', '/', '(', ')', 'n' };
	std::vector<std::pair<non_terminator, symbol>> generator;

	std::set<non_terminator, std::integral_constant<decltype(&_t_less<non_terminator>), &_t_less<non_terminator>>> _non_terminator = { 'E', 'A', 'T', 'B', 'F' };

	std::map<non_terminator, std::set<terminator, std::integral_constant<decltype(&_t_less<terminator>), &_t_less<terminator>>>, std::integral_constant<decltype(&_t_less<non_terminator>), &_t_less<non_terminator>>> _first_set;
	std::map<non_terminator, std::set<terminator, std::integral_constant<decltype(&_t_less<terminator>), &_t_less<terminator>>>, std::integral_constant<decltype(&_t_less<non_terminator>), &_t_less<non_terminator>>> _follow_set;
	std::map<table_index, int, std::integral_constant<decltype(&_t_less<table_index>), &_t_less<table_index>>> _table;
}

template <typename T, typename less>
std::set<T, less> set_union(const std::set<T, less>& a, const std::set<T, less>& b)
{
	std::set<T, less> result = a;
	result.insert(b.begin(), b.end());
	return result;
}

std::set<terminator, std::integral_constant<decltype(&_t_less<terminator>), &_t_less<terminator>>> _get_first(symbol sym)
{
	if (sym._val.size() == 0)
		return { '$' };

	using namespace partable;
	
	auto _res = sym._val[0];
	if (_terminator.find(_res) != _terminator.end())
		return { _res };
	else if (_non_terminator.find(_res) != _non_terminator.end())
	{
		if (_first_set[_res].find('$') == _first_set[_res].end())
			return _first_set[_res];
		else
		{
			auto _T_set = _first_set[_res];
			_T_set.erase('$');
			return set_union(_T_set, _get_first(sym._val.substr(1)));
		}
	}
	else
		throw("unresolved symbol");
}

void partable_init()
{
	using namespace partable;
	generator.push_back(mns('E', "TA"));
	generator.push_back(mns('A', "+TA"));
	generator.push_back(mns('A', "-TA"));
	generator.push_back(mns('A', "$"));
	generator.push_back(mns('T', "FB"));
	generator.push_back(mns('B', "*FB"));
	generator.push_back(mns('B', "/FB"));
	generator.push_back(mns('B', "$"));
	generator.push_back(mns('F', "(E)"));
	generator.push_back(mns('F', "n"));

	_first_set.insert(mnt('E', { '(', 'n' }));
	_first_set.insert(mnt('A', { '+', '-', '$' }));
	_first_set.insert(mnt('T', { '(', 'n' }));
	_first_set.insert(mnt('B', { '*', '/', '$' }));
	_first_set.insert(mnt('F', { '(', 'n' }));

	_follow_set.insert(mnt('E', { '$', ')' }));
	_follow_set.insert(mnt('A', { '$', ')' }));
	_follow_set.insert(mnt('T', { '$', '+', '-' }));
	_follow_set.insert(mnt('B', { '$', '+', '-' }));
	_follow_set.insert(mnt('F', { '$', '*', '/' }));

	_create_partable();
}

void _create_partable()
{
	using namespace partable;
	for (auto it = 0; it != generator.size(); it++)
	{
		auto _select_set = _get_first(generator[it].second);
		if (_select_set.find('$') != _select_set.end())
			_select_set = set_union(_select_set, _follow_set[generator[it].first]);
		for (auto iter = _select_set.begin(); iter != _select_set.end(); iter++)
			_table.insert(mti(table_index(*iter, generator[it].first), it));
	}
}
