#include "analyzer.h"

std::stack<char> _symbol_stack;

void analyze(const std::string& _sentense)
{
	using namespace partable;

	int _ip = 0;
	_symbol_stack.push('$');
	_symbol_stack.push('E');
	auto _top = _symbol_stack.top();
	while (_top != '$')
	{
		if (_non_terminator.find(_top) != _non_terminator.end())
		{
			if (_top == _sentense[_ip])
			{
				_ip++;
				assert(!_symbol_stack.empty());
				_symbol_stack.pop();
			}
			else
				error();
		}
		else if (_terminator.find(_top) != _terminator.end())
		{
			if (_table.find(mnp(_sentense[_ip], _top)) != _table.end())
			{
				symbol _t_sym = generator[_table[mnp(_sentense[_ip], _top)]].second;
				for (auto i = _t_sym._val.size() - 1; i != -1; i--)
					_symbol_stack.push(_t_sym._val[i]);
			}
			else
				error();
		}
		else
			error();
	}
}

void error()
{

}