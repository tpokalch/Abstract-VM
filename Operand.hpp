
#ifndef OPERAND_HPP
# define OPERAND_HPP

#include "MyStack.hpp"

template <typename T>
class Operand : public IOperand, public MyStack
{
	public:
		Operand<T>(eOperandType const & t, long double min, long double max, std::string const & v);
		T value;

		std::string string_value;
		eOperandType type;

		virtual		eOperandType getType( void ) const;
		virtual int	getPrecision( void ) const;

		IOperand const *doOperation(const char c, IOperand const & rhs) const;

		virtual IOperand const * operator+( IOperand const & rhs ) const;
		virtual IOperand const * operator-( IOperand const & rhs ) const;
		virtual IOperand const * operator*( IOperand const & rhs ) const;
		virtual IOperand const * operator/( IOperand const & rhs ) const;
		virtual IOperand const * operator%( IOperand const & rhs ) const;

		virtual std::string const & toString( void ) const;

		class	OverflowException: public std::exception
		{
			public:
				virtual const char * what() const throw()
				{
					return ("Overflow Exception");
				}
		};
		class	DivisionException: public std::exception
		{
			public:
				virtual const char * what() const throw()
				{
					return ("Division Exception");
				}
		};



};

class Int8 : public Operand<int8_t> {};
class Int16 : public Operand<int16_t> {};
class Int32 : public Operand<int32_t> {};
class Double : public Operand<double> {};
class Float : public Operand<float> {};

#endif

