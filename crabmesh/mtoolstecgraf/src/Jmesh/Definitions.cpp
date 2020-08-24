#include "Jmesh/Definitions.h"

#if (!USE_GUI)

#include "Jmesh/MainDrive.h"

std::string Jmesh::help(const std::string &programName)
{
	std::stringstream str;

	str << "Usage: " << programName << " Jmesh <input_filename> <options>\n"
		<< "Options:\n"
		<< "-advan_ver: normal version\n"
		<< "-small_ver: recommended for inputs with transition problems\n"
		<< "default: advan version\n"
		<< "\n"
		<< "-o (--output) <output_filename>: Save output file (.txt or .dat)\n"
		<< "default: Do not produce output\n"
		<< "Observation: this version detects the input format by the file extension\n"
		<< "\n"
#if USE_NUMERICAL
		<< "-ht (--hotelling): Apply Hotelling transform\n"
		<< "default: Do not apply Hotelling transform\n"
		<< "\n"
#endif //#if USE_NUMERICAL
		<< "-u (--uniform): Generate a uniform mesh (by largest element)\n"
		<< "-nu (--non-uniform): Do not generate a uniform mesh (by largest element)\n"
		<< "default: Do not generate a uniform mesh\n";

	return str.str();
}

int Jmesh::execute(int argc, char *argv[])
{
	Jmesh::MainDrive *drive = new Jmesh::MainDrive();

	drive->setUsingUniformRefinement(false);
#if USE_NUMERICAL
	drive->setHotellingTransformEnabled(false);
#endif //#if USE_NUMERICAL

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
			else if (std::string("-advan_ver") == argv[i])
			{
				used[i] = true;

				drive->setVer(Jmesh::MainDrive::ADVAN);
			}
			else if (std::string("-small_ver") == argv[i])
			{
				used[i] = true;

				drive->setVer(Jmesh::MainDrive::SMALL);
			}
#if USE_NUMERICAL
			else if ((std::string("-ht") == argv[i]) || (std::string("--hotelling") == argv[i]))
			{
				used[i] = true;

				drive->setHotellingTransformEnabled(true);
			}
#endif //#if USE_NUMERICAL
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
