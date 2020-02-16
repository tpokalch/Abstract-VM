
#include "abstract_vm.hpp"

std::string out("");

int	main(int argc, char **argv)
{
	MyStack	s;

	std::string inst;
	if (argc <= 2)
	{
		std::fstream myfile(argv[1]);
		if (argc == 2)
			if (getFile(myfile) == 0)
				return (0);
		while (1)
		{
			if (!getInput(argc, myfile, inst)
				|| ((inst == ";;" && argc == 1)
				|| (!s.status && argc == 2)))
			{
				if (s.status)
					out += RED "Error: " WHITE "terminating without exit\n";
				break ;
			}
			parseInstr(s, &inst[0]);
		}
	}
	std::cout << out;
	system("leaks -q avm");	
}
