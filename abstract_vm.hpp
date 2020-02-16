
#ifndef ABSTRACTVM_HPP
 #define ABSTRACTVM_HPP

#include <stack> 
#include <iostream>
#include <stdlib.h>
#include <cmath>
#include <float.h>
#include <fstream>
#include <float.h>
#include <fstream>
#include <string.h>

#define RED "\033[38;5;1m"
#define BLUE "\033[38;5;75m"
#define CYAN "\033[38;5;87m"
#define WHITE "\033[0m"
#define GREEN "\033[38;5;112m"
#define ORANGE "\033[38;5;208m"
#define MAGENTA "\033[38;5;169m"

#include "Operand.hpp"

extern std::string out;

void	splitInst(char *&instr, std::string &inst, char *&start, char *&command, char c);
void	parseInstr(MyStack &s, char *inst);
void	parse1instr(MyStack &s, char *&command, char *&instr);
void	parse2instr(MyStack &s, char *&command, char *&instr);
int	getFile(std::fstream &myfile);
int	getInput(int &argc, std::fstream &myfile, std::string &inst);
char *		find_first_of(char *&str, char c);
char *		find_first_not_of(char *str, char c);
char *cut(char *&s, size_t end);

enum	eCommandOperand
{
	assertOperand = 0,
	pushOperand = 1
};

enum	eCommand
{
	add = 0, 
	sub = 1,
	mul = 2,
	mydiv = 3,
	mod = 4,
	pop = 5,
	dump = 6,
	print = 7,
	myexit = 8
};

eCommandOperand	toCommandOperand(char *s);
eCommand	toCommand(char *s);
std::string toOperandType(eOperandType const & s);
eOperandType toOperandType(std::string const & s);

template <typename T>
std::string const & Operand<T>::toString( void ) const
{
	return (string_value);
}


template <typename T>
int	Operand<T>::getPrecision( void ) const
{
	return (0);
}

template <typename T>
IOperand const * Operand<T>::doOperation(const char c, IOperand const & rhs) const
{
	const IOperand *ret;
	eOperandType hp_type;

	hp_type = std::max(getType(), rhs.getType());
	std::string string_value = rhs.toString();
	string_value = string_value.substr(string_value.find_first_of('(', 0) + 1, string_value.length() - 2);
	long double rightValue = std::stold(string_value);
	long double newValue;
	switch (c)
	{
		case '+': newValue = value + rightValue; break;
		case '-': newValue = value - rightValue; break;
		case '*': newValue = value * rightValue; break;
		case '/':
		{
			if (rightValue == 0)
			{
				delete &rhs;
				throw DivisionException();
			}
			newValue = value / rightValue; break;
		}
		case '%':
		{
			if (rightValue == 0)
			{
				delete &rhs;	
				throw DivisionException();
			}
			newValue = fmod(value, rightValue); break;
		}
	}
	out = (((((((out + toString()) + ORANGE) + " ") + c) + " " WHITE) + rhs.toString()) + " = ");
	delete &rhs;
	delete this;
	ret = createOperand(hp_type, std::to_string(newValue));
	out += ret->toString() + "\n";

	return (ret);
}


template <typename T>
IOperand const * Operand<T>::operator+( IOperand const & rhs ) const
{
	return doOperation('+', rhs);
}


template <typename T>
IOperand const * Operand<T>::operator-( IOperand const & rhs ) const
{
	return doOperation('-', rhs);
}

template <typename T>
IOperand const * Operand<T>::operator*( IOperand const & rhs ) const
{
	return doOperation('*', rhs);
}

template <typename T>
IOperand const * Operand<T>::operator/( IOperand const & rhs ) const
{
	return doOperation('/', rhs);
}

template <typename T>
IOperand const * Operand<T>::operator%( IOperand const & rhs ) const
{
	return doOperation('%', rhs);
}

template <typename T>
eOperandType Operand<T>::getType(void) const
{
	return (type);
}

template <typename T>
Operand<T>::Operand(eOperandType const & t, long double min, long double max, std::string const & v): value(std::stold(v)), type(t)
{
	long double check;

	check = std::stold(v);
	if (check < min || check > max)
	{
		throw OverflowException();
	}
/*	swit
	string_value = toOperandType(t) + '(' + value + ')';
	"Int8(" + std::to_string(stoi(value)) + ')';
*/
}

#endif

