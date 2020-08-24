#include <iostream>
#include <list>
#include <vector>
#include <algorithm>

#include "JMesh/Definitions.h"
#include "JMesh/App.h"
#include "JMesh/MainDrive.h"

using namespace std;

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		std::cout << "Usage: ./JMesh <input_filename> <options>" << std::endl;

		std::cout << "Options:\n"
				  << "-o (--output) <output_filename>: Save output file (.txt or .pos)\n"
				  << "default: Do not produce output\n"
				  << "\n"
				  << std::endl;

		return 0;
	}

	std::cout << "executing";

	for (Int i = 0; i < argc; i++)
	{
		std::cout << " " << argv[i];
	}

	std::cout << std::endl;

	std::cerr << "executing";

	for (Int i = 0; i < argc; i++)
	{
		std::cerr << " " << argv[i];
	}

	std::cerr << std::endl;

	JMesh::App *app = new JMesh::App();

	//setting default parameters
	JMesh::MainDrive *drive = dynamic_cast<JMesh::MainDrive *>(app->getDrive());

	std::string input, output;

	if (argc == 2)
	{
		std::stringstream str(argv[1]);

		input = str.str();
	}
	else
	{
		bool used[argc];

		for (int i = 1; i < argc; i++)
		{
			used[i] = false;
		}

		used[0] = true;

		for (int i = 0; i < argc; i++)
		{
			if (used[i])
			{
				continue;
			}

			if ('-' != argv[i][0])
			{
				std::stringstream str(argv[i]);

				input = str.str();

				continue;
			}

			if ((std::string("-o") == argv[i]) || (std::string("--output") == argv[i]))
			{
				used[i] = true;

				if (i < argc - 1)
				{
					used[i+1] = true;

					std::stringstream str(argv[i+1]);

					output = str.str();
				}
				else
				{
					std::cout << "please, enter output filename" << std::endl;

					delete app;

					return 1;
				}
			}
		}
	}

	app->setInputFileName(input);
	app->setOutputFileName(output);

	app->execute();

	std::cout << drive->qualityString() << std::endl;

	delete app;

	//std::cout << "done" << std::endl;

	return 0;
}
