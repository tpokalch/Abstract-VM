
#include "abstract_vm.hpp"

MyStack::MyStack()
{
	status = true;
}

void	MyStack::assertOperand(eOperandType type, std::string const & value)
{
	out += BLUE "assert: " WHITE;
	if (empty())
		throw (EmptyException());
	if (type == Float)
	{
		if (toOperandType(type) + "(" + std::to_string(stof(value)) + ")" != top()->toString())
			throw (AssertException());
	}
	else if (type == Double)
	{
		if (toOperandType(type) + "(" + std::to_string(stod(value)) + ")" != top()->toString())
			throw (AssertException());
	}
	else if (toOperandType(type) + "(" + value + ")" != top()->toString())
		throw (AssertException());
	out = (((out + toOperandType(type).c_str()) + "(" + value.c_str()) + ")\n");
}

void	MyStack::mypop(void)
{
	out += MAGENTA "pop: " WHITE;
	if (empty())
		throw MyStack::EmptyException();
	out += MAGENTA "✗" WHITE + top()->toString() + "\n";
	pop();
}

void	MyStack::dump(void)
{
	out += MAGENTA "dump: " WHITE;
	if (empty())
		throw EmptyException();
	MyStack save(*this);
	out += "\n";
	while (!empty())
	{
		out += "\t" + (top())->toString() + "\n";
		pop();
	}
	*this = save;
}

void	MyStack::exit(void)
{
	out += BLUE "EXIT\n" WHITE;
	status = false;
}

void	MyStack::doBinary(const char c)
{
	const IOperand *adder1;
	out = (((out + (ORANGE) + c) + ": ") + WHITE);
	if (!empty())
	{
		adder1 = top();
		pop();
	}
	else
		throw MyStack::EmptyException();
	const IOperand *adder2;
	if (!empty())
	{
		adder2 = top();
		pop();
	}
	else
	{
		delete adder1;
		throw MyStack::EmptyException();
	}
	const IOperand *sum;
	switch (c)
	{
		case '+': sum = *adder1 + *adder2; break;
		case '-': sum = *adder1 - *adder2; break;
		case '*': sum = *adder1 * *adder2; break;
		case '/': sum = *adder1 / *adder2; break;
		case '%': sum = *adder1 % *adder2; break;
	}
	push(sum);
}

void	MyStack::add()
{
	doBinary('+');
}

void	MyStack::sub()
{
	doBinary('-');
}

void	MyStack::mul()
{
	doBinary('*');
}

void	MyStack::div()
{
	doBinary('/');
}

void	MyStack::mod()
{
	doBinary('%');
}



void	MyStack::doCommand(char *command, eOperandType type, std::string const & value)
{
	void	(MyStack::*f[])(eOperandType type, std::string const & value) = {&MyStack::assertOperand, &MyStack::pushOperand};
	eCommandOperand arg = toCommandOperand(command);
	(this->*f[arg])(type, value);
}

void	MyStack::doCommand(char *s)
{
	void	(MyStack::*f[])() = {&MyStack::add, &MyStack::sub, &MyStack::mul, &MyStack::div, &MyStack::mod, &MyStack::mypop, &MyStack::dump, &MyStack::print, &MyStack::exit};
	(this->*f[toCommand(s)])();
}

void	MyStack::print(void)
{
	const Operand<int8_t> *character;

	if (empty())
		throw EmptyException();
	if ((character = dynamic_cast<const Operand<int8_t> *>(top())))
		out = (((out + "'") + static_cast<char>(character->value)) +"'\n");
	else
		throw AssertException();
}

IOperand const * MyStack::createInt8( std::string const & value ) const
{
	Operand<int8_t> * ret = new Operand<int8_t>(Int8, -128, 127, value);
	ret->string_value = "Int8(" + std::to_string(stoi(value)) + ')';
	return (ret);
}

IOperand const * MyStack::createInt16( std::string const & value ) const
{
	Operand<int16_t> *ret = new Operand<int16_t>(Int16, -32768, 32767, value);
	ret->string_value = "Int16(" + std::to_string(stoi(value)) + ')';

	return (ret);
}

IOperand const * MyStack::createInt32( std::string const & value ) const
{
	Operand<int32_t> *ret = new Operand<int32_t>(Int32, INT_MIN, INT_MAX, value);
	ret->string_value = "Int32(" + std::to_string(stoi(value)) + ')';

	return (ret);
}

IOperand const * MyStack::createFloat( std::string const & value ) const
{
	Operand<float> *ret = new Operand<float>(Float, -FLT_MAX, FLT_MAX, value);
	ret->string_value = "Float(" + std::to_string(stof(value)) + ')';

	return (ret);
}

IOperand const * MyStack::createDouble( std::string const & value ) const
{
	Operand<double> *ret = new Operand<double>(Double, -DBL_MAX, DBL_MAX, value);
	ret->string_value = "Double(" + std::to_string(stod(value)) + ')';

	return (ret);
}

IOperand const * MyStack::createOperand( eOperandType type, std::string const & value ) const
{
	IOperand const *(MyStack::*f[])(std::string const & value) const = {&MyStack::createInt8, &MyStack::createInt16, &MyStack::createInt32, &MyStack::createFloat, &MyStack::createDouble};

//wtf??
	IOperand const *ret;
	ret = (this->*f[type])(value);
	return (ret);
}

void	MyStack::pushOperand(eOperandType type, std::string const & value)
{
	out = ((((((out + BLUE) + "push: ") + WHITE) + toOperandType(type).c_str()) + "(" + value.c_str()) + ")\n");
	const IOperand *v;
	v = createOperand(type, value);
	push(v);
}



