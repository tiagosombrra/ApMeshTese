#include "TecGraf/Definitions.h"

#if (!USE_GUI)

#include "TecGraf/MshSurf/MainDrive.h"

std::string TecGraf::MshSurf::help(const std::string &programName)
{
	std::stringstream str;

	str << "Usage: " << programName << " TecGrafMshSurf <input_filename> <options>\n"
		<< "Options:\n"
		<< "-o (--output) <output_filename>: Save output file (.txt or .dat)\n"
		<< "default: Do not produce output\n"
		<< "\n";

	return str.str();
}

int TecGraf::MshSurf::execute(int argc, char *argv[])
{
	TecGraf::MshSurf::MainDrive *drive = new TecGraf::MshSurf::MainDrive();

	std::string input, output;

	/*if (argc == 2)
	{
		input = Data::convert(argv[1]);
	}
	else*/
	{
		bool used[argc];

		for (int i = 0; i < argc; i++)
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
				input = Data::convert(argv[i]);

				continue;
			}

			if ((std::string("-o") == argv[i]) || (std::string("--output") == argv[i]))
			{
				used[i] = true;

				if (i < argc - 1)
				{
					used[i+1] = true;

					output = Data::convert(argv[i+1]);
				}
				else
				{
					std::cout << "please, enter output filename" << std::endl;

					delete drive;

					return 1;
				}
			}
			else
			{
				std::cout << "Error: invalid option: " << argv[i] << std::endl;

				delete drive;

				return 1;
			}
		}
	}

	if (input.empty())
	{
		std::cout << "Error: no input file given" << std::endl;

		delete drive;

		return 1;
	}

	drive->setInputFileName(input);
	drive->setOutputFileName(output);

	if (!drive->loadFile())
	{
		std::cout << "Error: could not load file" << std::endl;

		delete drive;

		return 1;
	}

	drive->execute();

	drive->saveFile();

	if (drive->getStatus().getType() >= drive->error.getType())
	{
		//std::cout << "Error: mesh could not be generated: " << drive->getStatus().getMessage() << std::endl;
	}
	else
	{
		std::cout << drive->qualityString() << std::endl;
	}

	delete drive;

	//std::cout << "done" << std::endl;

	return 0;
}
#endif //#if (!USE_GUI)
