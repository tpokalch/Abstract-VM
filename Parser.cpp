
#include "abstract_vm.hpp"

char *cut(char *&s, int end)
{
	if (end == -1)
		return (s);
	s[end] = '\0';
	s = s + end + 1;
	return (s - end - 1);
}

int find_first_not_of(char *&str, char c)
{
	int i = 0;
	for (i = 0; str[i] && str[i] == c; i++);
	if (!str[i])
		return (-1);
	return (i);
}

int find_first_of(char *&str, char c)
{
	int i = 0;
	for (i = 0; str[i] && str[i] != c; i++);
	if (!str[i])
		return (-1);
	return (i);
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
//	out = (((((out + CYAN) + "____Your input: '") + inst) + "'\n") + WHITE);
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

int	limits_defined(char *&instr, int &s, int &e)
{
	s = find_first_of(instr, '(');
	e = find_first_of(instr, ')');

	if (s == -1)
	{
		out = (((out + RED) + "Expected '('\n") + WHITE);		
		return 0;
	}
	else if (e == -1)
	{
		out = (((out + RED) + "Expected ')'\n") + WHITE);
		return 0;
	}
	return 1;
}


void	parse2instr(MyStack &s, char *&command, char *&instr)
{
	int start;
	int end;
	if (limits_defined(instr, start, end) == -1)
		return ;
//	cut(instr, find_first_not_of(instr, ' ') - 1);
	//printf("instr before shift is '%s'\n", instr);
	instr = instr + find_first_not_of(instr, ' ');
	//printf("instr after shift is '%s'\n", instr);
	char *type = cut(instr, find_first_of(instr, '('));
//	instr++;
	//printf("type is '%s'\n", type);
	//printf("instr before is '%s'\n", instr);
	
//	instr[end] = '\0';
//fix this
	instr[find_first_of(instr, ')')] = '\0';
	//printf("instr after  is '%s'\n", instr);	
	char *value = instr;
	//teset
	instr = instr + strlen(instr) + 1;
	if (*instr != '\0' && find_first_not_of(instr, ' ') != -1)
		out = (((((out + CYAN) + "Unexpected symbols after instruction ignored: ") + WHITE) + "'" + instr + "'") + "\n");

//	value[end - 2] = '\0';
	//printf("calue is %s\n", value);
	if (toOperandType(type) == Int8 && strcmp(type, "int8") != 0)
		out = (((((out + RED) + "Unknown type: ") + WHITE) + "'" + type + "' " RED "defaults to Int8" WHITE )+ "\n");
	try
	{
		s.doCommand(command, toOperandType(type), value);
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

void	parse1instr(MyStack &s, char *&command, char *&instr)
{
	if (command != instr && find_first_not_of(instr, ' ') != -1)
		out = (((((out + CYAN) + "Unexpected symbols after instruction ignored: ") + WHITE) + "'" + instr + "'") + "\n");
		try
	{
		s.doCommand(command);
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

void	splitInst(char *&instr, std::string &inst, char *&start, char *&command, char c)
{
	instr = new char[inst.length()];
	start = instr;
	strcpy(instr, inst.c_str());
	if (find_first_of(instr, c) != -1)
		command = cut(instr, find_first_of(instr, c));
	else
		command = instr;
}

void	parseInstr(MyStack &s, char *&instr, std::string &inst, char *&start, char *&command)
{
	//printf("find first of %d\n", comment);
	//printf("here1\n");
	int comment = inst.find_first_of(';', 0);
	if (comment != -1)
	{
		inst[comment] = '\0';
		out = ((((out + GREEN) + ";" + (inst.c_str() + comment + 1)) + "\n") + WHITE);
	}
	//printf("here\n");
	splitInst(instr, inst, start, command, ' ');

	if (s.status && command != inst &&
		(strcmp(command, "push") == 0
		|| strcmp(command, "assert") == 0))
	{
		parse2instr(s, command, instr);
	}
	else if (s.status &&
	(strcmp(command, "sub") == 0
	|| strcmp(command, "add") == 0
	|| strcmp(command, "mul") == 0
	|| strcmp(command, "div") == 0
	|| strcmp(command, "mod") == 0
	|| strcmp(command, "dump") == 0
	|| strcmp(command, "pop") == 0
	|| strcmp(command, "print") == 0
	|| strcmp(command, "exit") == 0))
	{
		parse1instr(s, command, instr);
	}
	else
		std::cout << RED "Illegal command ignored: " WHITE "'" << command << "'" << std::endl;
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

std::string toOperandType(eOperandType & s)
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

