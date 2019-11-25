#include "partable.h"
#include <algorithm>

using namespace partable;

template <typename T, typename less>
std::set<T, less> set_union(const std::set<T, less>& a, const std::set<T, less>& b)
{
	std::set<T, less> result = a;
	result.insert(b.begin(), b.end());
	return result;
}

std::set<char> _get_first(std::string sym)
{
	if (sym.size() == 0)
		return { '$' };
	
	auto _res = sym[0];

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
			return set_union(_T_set, _get_first(sym.substr(1)));
		}
	}
	else
		return {};
}

bool _has_changed(std::map<char, std::set<char>> first_set)
{
	auto _it = _first_set.begin();
	auto it = first_set.begin();

	while (it != first_set.end() && _it != _first_set.end())
	{
		if (it->first == _it->first && it->second == _it->second)
		{
			it++, _it++;
			continue;
		}
		else
		{
			_first_set = first_set;
			return true;
		}
	}
	if (it == first_set.end() && _it == _first_set.end())
		return false;
	else
		return true;
}

void _first_table()
{
	std::map<char, std::set<char>> first_set;

	for (auto x : generator)
		_first_set[x.first] = {}, first_set[x.first] = {};
	do
	{
		for (auto x : generator)
		{
			auto new_set = _get_first(x.second);
			first_set[x.first] = set_union(new_set, first_set[x.first]);
		}
	} while (_has_changed(first_set));
}

bool _follow_changed(std::map<char, std::set<char>> follow_set)
{
	auto _it = _follow_set.begin();
	auto it = follow_set.begin();

	while (it != follow_set.end() && _it != _follow_set.end())
	{
		if (it->first == _it->first && it->second == _it->second)
		{
			it++, _it++;
			continue;
		}
		else
		{
			_follow_set = follow_set;
			return true;
		}
	}
	if (it == follow_set.end() && _it == _follow_set.end())
		return false;
	else
		return true;
}

void _follow_table()
{
	std::map<char, std::set<char>> follow_set;
	for (auto x : generator)
		_follow_set[x.first] = {}, follow_set[x.first] = {};
	_follow_set['E'] = { '$' }, follow_set['E'] = { '$' };

	do
	{
		for (auto x : generator)
		{
			auto left = x.first;
			for (auto it = 0; it != x.second.size(); it++)
			{
				if (_non_terminator.find(x.second[it]) != _non_terminator.end())
				{
					if (it == x.second.size() - 1)
						follow_set[x.second[it]] = set_union(follow_set[x.second[it]], follow_set[left]);
					else
					{
						auto release_set = _get_first(x.second.substr(it + 1));
						if (release_set.find('$') != release_set.end())
						{
							release_set.erase('$');
							follow_set[x.second[it]] = set_union(follow_set[left], release_set);
						}
						follow_set[x.second[it]] = set_union(follow_set[x.second[it]], release_set);
					}
				}
			}
		}
	} while (_follow_changed(follow_set));
}

void partable_init()
{

	_terminator = { '+', '-', '*', '/', '(', ')', 'n', '$' };
	_non_terminator = { 'E', 'A', 'T', 'B', 'F' };

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

	_first_table();
	_follow_table();

	/*_first_set.insert(mnt('A', { '$', '+', '-' }));
	_first_set.insert(mnt('B', { '$', '*', '/' }));
	_first_set.insert(mnt('E', { '(', 'n' }));
	_first_set.insert(mnt('F', { '(', 'n' }));
	_first_set.insert(mnt('T', { '(', 'n' }));*/

	/*_follow_set.insert(mnt('E', { '$', ')' }));
	_follow_set.insert(mnt('A', { '$', ')' }));
	_follow_set.insert(mnt('T', { '$', '+', '-', ')' }));
	_follow_set.insert(mnt('B', { '$', '+', '-', ')' }));
	_follow_set.insert(mnt('F', { '$', '*', '/' }));*/

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
