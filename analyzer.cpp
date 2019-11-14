#include "analyzer.h"

std::stack<char> _symbol_stack;

void analyze(const std::string& _sentense)
{
	using namespace partable;

	int _ip = 0;
	_symbol_stack.push('$');
	_symbol_stack.push('E');
	auto _top = _symbol_stack.top();
	_symbol_stack.pop();
	while (_top != '$')
	{
		if (_terminator.find(_top) != _terminator.end())
		{
			if (_top == _sentense[_ip])
			{
				_sentense.size() - 1 != _ip && _ip++;
				assert(!_symbol_stack.empty());
			}
			else
				error();
		}
		else if (_non_terminator.find(_top) != _non_terminator.end())
		{
			if (_table.find(table_index(_sentense[_ip], _top)) != _table.end())
			{
				symbol _t_sym = generator[_table[table_index(_sentense[_ip], _top)]].second;
				for (auto i = _t_sym._val.size() - 1; i != -1; i--)
					_symbol_stack.push(_t_sym._val[i]);
			}
			else
				error();
		}
		else
			error();
		do { _top = _symbol_stack.top(); _symbol_stack.pop(); }
		while (_top == '$' && _symbol_stack.size() != 1);
		
	}
}

void error()
{
	printf("error\n");
	exit(0);
}
