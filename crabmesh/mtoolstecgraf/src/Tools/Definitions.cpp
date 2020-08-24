#include "Tools/Definitions.h"

#if (!USE_GUI)

#include "Tools/MainDrive.h"
#include "MJMesh/Boundary.h"
#include "Numerical/ColumnMatrix.h"
#include "Numerical/RowMatrix.h"
#include "Numerical/DenseMatrix.h"
#include "Numerical/TransformationMatrix.h"

std::string Tools::help(const std::string &programName)
{
	std::stringstream str;

	str << "Usage: " << programName << " Tool <input_filename> <options>\n"
		<< "Options:\n"
		<< "-o (--output) <output_filename>: Save output file (.txt or .dat)\n"
		<< "default: Do not produce output\n"
		<< "Observation: this version detects the input format by the file extension\n";

	return str.str();
}

int Tools::execute(int argc, char *argv[])
{
	Tools::MainDrive *drive = new Tools::MainDrive();

	std::string input, output;

	enum Tools::ToolType type = Tools::UNKNOWN;

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
		else if (std::string("-BoundaryFixNormals") == argv[i])
		{
			used[i] = true;

			type = Tools::BOUNDARY_NORMAL_FIXER;

			drive->setTool(new Tools::BoundaryNormalFixer());
		}
		else if (std::string("-Rotate") == argv[i])
		{
			used[i] = true;

			type = Tools::TRANSFORMER_ROTATION;

			if (i < argc - 4)
            {
                Real ang = 0.0;
                Data::Vector3D v;

                try
                {
                    Real vx = Data::convert<Real>(argv[i+1]);

                    used[i+1] = true;

                    v.setX(vx);
                }
                catch (...)
                {
                    std::cout << "Error: please, enter the X coordinate for the rotation axis" << std::endl;

                    delete drive;

                    return 1;
                }

                try
                {
                    Real vy = Data::convert<Real>(argv[i+2]);

                    used[i+2] = true;

                    v.setY(vy);
                }
                catch (...)
                {
                    std::cout << "Error: please, enter the Y coordinate for the rotation axis" << std::endl;

                    delete drive;

                    return 1;
                }

                try
                {
                    Real vz = Data::convert<Real>(argv[i+3]);

                    used[i+3] = true;

                    v.setZ(vz);
                }
                catch (...)
                {
                    std::cout << "Error: please, enter the Z coordinate for the rotation axis" << std::endl;

                    delete drive;

                    return 1;
                }

                try
                {
                    ang = Data::convert<Real>(argv[i+4]);

                    used[i+4] = true;
                }
                catch (...)
                {
                    std::cout << "Error: please, enter the rotation angle" << std::endl;

                    delete drive;

                    return 1;
                }

                Tools::MeshTransformer *transformer = new Tools::MeshTransformer();

                transformer->rotate(v, ang);

                drive->setTool(transformer);
            }
            else
            {
                std::cout << "Error: please, insert rotation axis (x, y, z) and angle" << std::endl;

                delete drive;

                return 1;
            }
		}
#if USE_NUMERICAL
		else if (std::string("-HotellingTransformer") == argv[i])
		{
			used[i] = true;

			type = Tools::HOTELLING_TRANSFORMER;

			Tools::HotellingTransformer *pca = new Tools::HotellingTransformer();

			drive->setTool(pca);

			for (i++; i < argc; i++)
            {
                if (used[i])
                {
                    continue;
                }

                if (std::string("--mode") == argv[i])
                {
                    used[i] = true;

                    enum Performer::CovarianceMatrixFinder::Mode mode = Performer::CovarianceMatrixFinder::NORMAL;

                    if (std::string("normal") == argv[i+1])
                    {
                        used[i+1] = true;
                    }
                    else if (std::string("accumulate") == argv[i+1])
                    {
                        mode = Performer::CovarianceMatrixFinder::ACCUMULATE;

                        used[i+1] = true;
                    }
#if USE_EXACT
                    else if (std::string("pseudoexact") == argv[i+1])
                    {
                        mode = Performer::CovarianceMatrixFinder::PSEUDO_EXACT;

                        used[i+1] = true;
                    }
#endif //#if USE_EXACT

                    pca->setMode(mode);
                }
                else if (std::string("--points") == argv[i])
                {
                    used[i] = true;

                    bool frontElementCentroids = false;

                    if (std::string("vertices") == argv[i+1])
                    {
                        used[i+1] = true;
                    }
                    else if (std::string("faces") == argv[i+1])
                    {
                        frontElementCentroids = true;

                        used[i+1] = true;
                    }

                    pca->setFrontElementCentroids(frontElementCentroids);
                }
                else if (std::string("--weighted") == argv[i])
                {
                    used[i] = true;

                    bool weighted = false;

                    if (std::string("true") == argv[i+1])
                    {
                        used[i+1] = true;

                        weighted = true;
                    }
                    else if (std::string("false") == argv[i+1])
                    {
                        used[i+1] = true;
                    }

                    pca->setWeighted(weighted);
                }
                else
                {
                    i--;

                    break;
                }
            }
		}
#endif //#if USE_NUMERICAL
		else
		{
			std::cout << "Error: invalid option: " << argv[i] << std::endl;

			delete drive;

			return 1;
		}
	}

	if (input.empty())
	{
		std::cout << "Error: no input file given" << std::endl;

		delete drive;

		return 1;
	}

	if (type == Tools::UNKNOWN)
	{
		std::cout << "Error: no tool given" << std::endl;

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

	switch (type)
	{
	case Tools::BOUNDARY_NORMAL_FIXER:
		static_cast<Tools::BoundaryNormalFixer *>(drive->getTool())->setBoundary(drive->getBoundary());
		break;
	case Tools::TRANSFORMER_ROTATION:
		static_cast<Tools::MeshTransformer *>(drive->getTool())->setBoundary(drive->getBoundary());
		break;
#if USE_NUMERICAL
	case Tools::HOTELLING_TRANSFORMER:
		static_cast<Tools::HotellingTransformer *>(drive->getTool())->setBoundary(drive->getBoundary());
		break;
#endif //#if USE_NUMERICAL
	default:
		break;
	}

	drive->execute();

	switch (type)
	{
	case Tools::BOUNDARY_NORMAL_FIXER:
	case Tools::TRANSFORMER_ROTATION:
		break;
#if USE_NUMERICAL
	case Tools::HOTELLING_TRANSFORMER:
	    {
	        Tools::HotellingTransformer *tool = static_cast<Tools::HotellingTransformer *>(drive->getTool());

            std::cout << "Mid point:" << std::endl;
            std::cout << tool->getMid()->text() << std::endl;

            std::cout << "Eigenvalues:" << std::endl;
            std::cout << tool->getEigenvalues()->text() << std::endl;

            std::cout << "Eigenvectors:" << std::endl;
            std::cout << tool->getEigenvectors()->text() << std::endl;

            std::cout << "Basis Change Matrix:" << std::endl;
            std::cout << tool->getMatrix()->text() << std::endl;
            break;
	    }
#endif //#if USE_NUMERICAL
	default:
		break;
	}

	drive->saveFile();

	delete drive;

	return 0;
}

#endif //#if (!USE_GUI)
