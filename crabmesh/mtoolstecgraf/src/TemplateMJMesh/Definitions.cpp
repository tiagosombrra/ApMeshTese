#include "TemplateMJMesh/Definitions.h"

#if (!USE_GUI)

#include "TemplateMJMesh/MainDrive.h"

std::string TemplateMJMesh::help(const std::string &programName)
{
	std::stringstream str;

	str << "Usage: " << programName << " TemplateMJMesh <input_filename> <options>\n"
		<< "Options:\n"
		<< "-o (--output) <output_filename>: Save output file (.txt or .dat)\n"
		<< "default: Do not produce output\n"
		<< "\n"
		<< "-sm (--smallest): Advance based on smallest front element\n"
		<< "default: Advance next on list\n"
		<< "\n"
		<< "-u (--uniform): Generate a uniform mesh (by largest element)\n"
		<< "-nu (--non-uniform): Do not generate a uniform mesh (by largest element)\n"
		<< "default: Do not generate a uniform mesh\n";

	return str.str();
}

int TemplateMJMesh::execute(int argc, char *argv[])
{
	TemplateMJMesh::MainDrive *drive = new TemplateMJMesh::MainDrive();

	drive->setBoundarySorted(false);

	drive->setUsingUniformRefinement(false);

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
			else if ((std::string("-sm") == argv[i]) || (std::string("--smallest") == argv[i]))
			{
				used[i] = true;

				drive->setBoundarySorted(true);
			}
			else if ((std::string("-u") == argv[i]) || (std::string("--uniform") == argv[i]))
			{
				used[i] = true;

				drive->setUsingUniformRefinement(true);
			}
			else if ((std::string("-nu") == argv[i]) || (std::string("--non-uniform") == argv[i]))
			{
				used[i] = true;

				drive->setUsingUniformRefinement(false);
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
		std::cout << "Error: mesh could not be generated: " << drive->getStatus().getMessage() << std::endl;
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

