


#ifndef IOPERAND_HPP
# define IOPERAND_HPP

//#include "abstract_vm.hpp"

enum    eOperandType
{
        Int8 = 0,
        Int16 = 1,
        Int32 = 2,
        Float = 3,
        Double = 4
};


class IOperand
{
	public:
		virtual int	getPrecision( void ) const = 0; // Precision of the type of the instance
		virtual		eOperandType getType( void ) const = 0; // Type of the instance

		virtual IOperand const * operator+( IOperand const & rhs ) const = 0;
		virtual IOperand const * operator-( IOperand const & rhs ) const = 0;
		virtual IOperand const * operator*( IOperand const & rhs ) const = 0;
		virtual IOperand const * operator/( IOperand const & rhs ) const = 0;
		virtual IOperand const * operator%( IOperand const & rhs ) const = 0;

		virtual std::string const & toString( void ) const = 0;

		virtual ~IOperand( void ) {}
};

#endif

