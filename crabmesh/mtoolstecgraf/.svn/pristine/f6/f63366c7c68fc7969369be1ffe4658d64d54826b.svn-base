#include "MTools/Definitions.h"

#if USE_MTOOLS
#if DEBUG_DISABLE_IMPROVEMENT
using namespace MTools;

namespace MTools
{
	bool debugImprovementEnabled = true;
}

void MTools::setImprovementEnabled(bool enabled)
{
	MTools::debugImprovementEnabled = enabled;
}

void MTools::disableImprovement()
{
	MTools::setImprovementEnabled(false);
}

void MTools::enableImprovement()
{
	MTools::setImprovementEnabled(true);
}

bool MTools::isImprovementEnabled()
{
	return MTools::debugImprovementEnabled;
}

bool MTools::isImprovementDisabled()
{
	return !MTools::isImprovementEnabled();
}
#endif //#if DEBUG_DISABLE_IMPROVEMENT
#endif //#if USE_MTOOLS

#if ((USE_MTOOLS) && (!USE_GUI))

#include "MTools/MainDrive.h"

std::string MTools::help(const std::string &programName)
{
	std::stringstream str;

#if (USE_ONLY_MTOOLS == 2)
	str << "Usage: " << programName << " <input_filename> <options>\n"
#else
	str << "Usage: " << programName << " MTools <input_filename> <options>\n"
#endif //#if (USE_ONLY_MTOOLS == 2)
		<< "Options:\n"
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
#if DEBUG_DISABLE_IMPROVEMENT
	str << "\n"
		<< "(debug) -d: Disable mesh improvement\n"
		<< "(debug) default: Mesh improvement enabled\n";
#endif //#if DEBUG_DISABLE_IMPROVEMENT

	return str.str();
}

int MTools::execute(int argc, char *argv[])
{
	MTools::MainDrive *drive = new MTools::MainDrive();

	drive->setUsingUniformRefinement(false);
#if USE_NUMERICAL
	drive->setHotellingTransformEnabled(false);
#endif //#if USE_NUMERICAL

	std::string input, output;

#if DEBUG_DISABLE_IMPROVEMENT
	MTools::enableImprovement();
#endif //#if DEBUG_DISABLE_IMPROVEMENT

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

#if (USE_ONLY_MTOOLS != 2)
		used[0] = true;
#endif //#if (USE_ONLY_MTOOLS != 2)

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
#if DEBUG_DISABLE_IMPROVEMENT
			else if (std::string("-d") == argv[i])
			{
				used[i] = true;

				MTools::disableImprovement();
			}
#endif //#if DEBUG_DISABLE_IMPROVEMENT
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

#endif //#if #if ((USE_MTOOLS) && (!USE_GUI))
