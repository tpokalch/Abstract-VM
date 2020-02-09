
//#ifndef MYSTACK_HPP
//# define MYSTACK_HPP

//#include "abstract_vm.hpp"
#include "IOperand.hpp"


class	MyStack: public std::stack<const IOperand *>
{
	public:
		MyStack();

		IOperand const * createOperand( eOperandType type, std::string const & value ) const;
		void	doCommand(char * command);
		void	doCommand(char *command, eOperandType type, std::string const & value);

		bool	status;

		void	add();
		void	sub();
		void	mul();
		void	div();
		void	mod();



		void	mypop(); //already exists
		void	dump();

		void	assertOperand(eOperandType type, std::string const & value);
		void	pushOperand(eOperandType type, std::string const & value);

		void	print();
		void	exit();

		stack<eOperandType> types;

		class	EmptyException: public std::exception
		{
			public:
				virtual const char * what() const throw()
				{
					return ("Stack Empty");
				}
		};

		class	AssertException: public std::exception
		{
			public:
				virtual const char * what() const throw()
				{
					return ("Assert Failed");
				}
		};


	private:
		void	doBinary(const char c);


		IOperand const * createInt8( std::string const & value ) const;
		IOperand const * createInt16( std::string const & value ) const;
		IOperand const * createInt32( std::string const & value ) const;
		IOperand const * createFloat( std::string const & value ) const;
		IOperand const * createDouble( std::string const & value ) const;


};

//#endif

