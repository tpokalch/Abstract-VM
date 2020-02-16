
#include "abstract_vm.hpp"

char * find_first_not_of(char *str, char c)
{
	int i = 0;
	for (i = 0; str[i] && str[i] == c; i++);
	return (str + i);
}

char * find_first_of(char *&str, char c)
{
	int i = 0;
	for (i = 0; str[i] && str[i] != c; i++);
	return (str + i);
}

int	getInput(int &argc, std::fstream &myfile, std::string &inst)
{
	if (argc == 1)
		getline(std::cin, inst);
	else
		if(!getline(myfile, inst))
			return (0);
//
// It's a feature nobody likes
//
//	 out = (((((out + CYAN) + "____Your input: '") + inst) + "'\n") + WHITE);
//
	return (1);
}

int	getFile(std::fstream &myfile)
{
	if (myfile.is_open())
		return (1);
	else
	{
		std::cerr << "Error: " << std::strerror(errno) << std::endl;
		return (0);
	}
}


void	parse2instr(MyStack &s, char *&inst, char *& spaces)
{
	char *type = find_first_not_of(spaces, ' ');
	if (*find_first_of(type, '(') == '\0')
	{
		out = (((out + RED) + "Expected '('\n") + WHITE);
		return;
	}
	char *value = find_first_of(type, '(');
	*value = '\0';
	value++;
	char *trash;
	if (*(trash = find_first_of(value, ')')) == '\0')
	{
		out = (((out + RED) + "Expected ')'\n") + WHITE);
	}
	else
	{
		*trash = '\0';
		trash++;
		trash = find_first_not_of(trash, ' ');
	}
	if (*trash != '\0')
	{
		out = (((((out + CYAN) + "Unexpected symbols after instruction ignored: ") + WHITE) + "'" + trash + "'") + "\n");
	}
	if (toOperandType(type) == Int8 && strcmp(type, "int8") != 0)
		out = (((((out + RED) + "Unknown type: ") + WHITE) + "'" + type + "' " RED "defaults to Int8" WHITE) + "\n");
	try
	{
		s.doCommand(inst, toOperandType(type), value);
	}
	catch (const std::invalid_argument& ia)
	{
		out = (((((out + RED) + "Invalid argument: ") + WHITE) + ia.what()) + "\n");
	}
	catch (const std::out_of_range& e)
	{
		out = (((out + (RED "Out of range: " WHITE)) + e.what()) + "\n");
	}
	catch (Int16::OverflowException& e)
	{
		out = ((out + (RED "Overflow exception: " WHITE) + e.what()) + "\n");
	}
	catch (MyStack::AssertException& e)
	{
		out = (((out + (RED "Assert exception: " WHITE)) + e.what()) + "\n");
	}
	catch (const std::exception& e)
	{
		out = (((out + (RED "Operand error: " WHITE "" )) + e.what()) + "\n");
	}
}

void	parse1instr(MyStack &s, char *& inst, char *&spaces)
{
	if (spaces[0] != '\0' && *find_first_not_of(spaces, ' ') != '\0')
	{
		out = ((((((out + CYAN) + "Unexpected symbols after instruction ignored: ") + WHITE) + "'" + find_first_not_of(spaces, ' ')) + "'") + "\n");
	}
	try
	{
		s.doCommand(inst);
	}
	catch (Operand<double>::OverflowException& e)
	{
		out = (((out + (RED "Overflow error: " WHITE "'" )) + e.what()) + "'\n");
	}
	catch (MyStack::EmptyException & e)
	{
		out = (((out + (RED "Empty exception: " WHITE "'" )) + e.what()) + "'\n");
	}
	catch (Operand<int8_t>::DivisionException & e)
	{
		out = (((out + (RED "Operand: " WHITE "'" )) + e.what()) + "'\n");
	}
	catch (const std::exception& e)
	{
		out = (((out + (RED "Operand error: " WHITE "'" )) + e.what()) + "'\n");
	}
}

void	parseInstr(MyStack &s, char *inst)
{
	inst = find_first_not_of(inst, ' ');
	char * comment = find_first_of(inst, ';');
	if (*comment != '\0')
	{
		*comment = '\0';
		out = ((((out + GREEN) + ";" + (comment + 1)) + "\n") + WHITE);
	}
	char * spaces = find_first_of(inst, ' ');
	if (spaces[0] != '\0')
	{
		spaces[0] = '\0';
		spaces++;
	}
	if (s.status && (strcmp(inst, "push") == 0 || strcmp(inst, "assert") == 0))
	{
		parse2instr(s, inst, spaces);
	}
	else if (s.status &&
	(strcmp(inst, "sub") == 0
	|| strcmp(inst, "add") == 0
	|| strcmp(inst, "mul") == 0
	|| strcmp(inst, "div") == 0
	|| strcmp(inst, "mod") == 0
	|| strcmp(inst, "dump") == 0
	|| strcmp(inst, "pop") == 0
	|| strcmp(inst, "print") == 0
	|| strcmp(inst, "exit") == 0))
	{
		parse1instr(s, inst, spaces);
	}
	else if (*inst)
		std::cout << RED "Illegal command ignored: " WHITE "'" << inst << "'" << std::endl;
}

eCommand	toCommand(char *s)
{
	if (strcmp(s, "add") == 0)
		return eCommand::add;
	if (strcmp(s, "sub") == 0)
		return eCommand::sub;
	if (strcmp(s, "mul") == 0)
		return eCommand::mul;
	if (strcmp(s, "div") == 0)
		return eCommand::mydiv;
	if (strcmp(s, "mod") == 0)
		return eCommand::mod;
	if (strcmp(s, "dump") == 0)
		return eCommand::dump;
	if (strcmp(s, "print") == 0)
		return eCommand::print;
	if (strcmp(s, "pop") == 0)
		return eCommand::pop;
	if (strcmp(s, "exit") == 0)
		return eCommand::myexit;
	return (add);
}

std::string toOperandType(eOperandType const & s)
{
	if (s == Int8)
		return "Int8";
	if (s == Int16)
		return "Int16";
	if (s == Int32)
		return "Int32";
	if (s == Float)
		return "Float";
	if (s == Double)
		return "Double";
	return ("Int8");
}

eOperandType toOperandType(std::string const & s)
{
	if (s == "int8")
		return eOperandType::Int8;
	if (s == "int16")
		return eOperandType::Int16;
	if (s == "int32")
		return eOperandType::Int32;
	if (s == "float")
		return eOperandType::Float;
	if (s == "double")
		return eOperandType::Double;
	return (eOperandType::Int8);
}

eCommandOperand	toCommandOperand(char *s)
{

	if (strcmp(s, "assert") == 0)
		return (eCommandOperand::assertOperand);
	else if (strcmp(s, "push") == 0)
		return (eCommandOperand::pushOperand);
	return (eCommandOperand::pushOperand);
}

