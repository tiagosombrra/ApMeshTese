#include "PMTools/Definitions.h"

#if (USE_MTOOLS)
#if DEBUG_DISABLE_IMPROVEMENT
using namespace PMTools;

namespace PMTools
{
	//bool debugWorkerImprovementEnabled = true;
	bool debugMasterImprovementEnabled = true;
}

void PMTools::setWorkerImprovementEnabled(bool enabled)
{
	//PMTools::debugWorkerImprovementEnabled = enabled;
	MTools::setImprovementEnabled(enabled);
}

void PMTools::disableWorkerImprovement()
{
	PMTools::setWorkerImprovementEnabled(false);
}

void PMTools::enableWorkerImprovement()
{
	PMTools::setWorkerImprovementEnabled(true);
}

bool PMTools::isWorkerImprovementEnabled()
{
	//return PMTools::debugWorkerImprovementEnabled;
	return MTools::isImprovementEnabled();
}

bool PMTools::isWorkerImprovementDisabled()
{
	return !PMTools::isWorkerImprovementEnabled();
}

void PMTools::setMasterImprovementEnabled(bool enabled)
{
	PMTools::debugMasterImprovementEnabled = enabled;
}

void PMTools::disableMasterImprovement()
{
	PMTools::setMasterImprovementEnabled(false);
}

void PMTools::enableMasterImprovement()
{
	PMTools::setMasterImprovementEnabled(true);
}

bool PMTools::isMasterImprovementEnabled()
{
	return PMTools::debugMasterImprovementEnabled;
}

bool PMTools::isMasterImprovementDisabled()
{
	return !PMTools::isMasterImprovementEnabled();
}
#endif //#if DEBUG_DISABLE_IMPROVEMENT
#endif //#if (USE_MTOOLS)

#if ((USE_MTOOLS) && (!USE_GUI))

#include "PMTools/MainDrive.h"
#include "PMJMesh/Communicator.h"

std::string PMTools::help(const std::string &programName)
{
	std::stringstream str;

#if (USE_ONLY_MTOOLS == 3)
	str << "Usage: " << programName << " <input_filename> <options>\n"
#else
	str << "Usage: " << programName << " PMTools <input_filename> <options>\n"
#endif //#if (USE_ONLY_MTOOLS == 3)
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
		<< "default: Do not generate a uniform mesh\n"
		<< "\n"
#if USE_OPENMP
		<< "-p (--parallel): Enable shared memory parallelism (number of running threads defined by the environment variable OMP_NUM_THREADS or by the total number of cores in the system)\n"
		<< "-p <number_of_threads> (--parallel <number_of_threads>): Enable shared memory parallelism and specify the number of running threads\n"
		<< "default: Disable shared memory parallelism (same as \"-p 0\")\n"
		<< "\n"
#endif //#if USE_OPENMP
		<< "-g (--greedy): Use Greedy load balancer\n"
		<< "-wg (--weighted-greedy): Use Weighted Greedy load balancer\n"
		<< "-s (--spring): Use Spring load balancer\n"
		<< "-d (--demand): Use Demand load balancer\n"
		<< "default: Use Demand load balancer\n"
		<< "\n"
		<< "-l (--load): (Quad/OctTree decomposition) Use load as the average of the total load by the number of worker slaves\n"
		<< "-l <load> (--load <load>): (Quad/OctTree decomposition) Use a specific maximum load (if <load> = 0, -l is implied)\n"
		<< "-n (--number-of-partitions): (Quad/OctTree decomposition) Use the number of worker slaves as a minimum number of partitions\n"
		<< "-n <number_of_partitions> (--number-of-partitions <number_of_partitions>): (Quad/OctTree decomposition) Use the number_of_partitions as a minimum number of partitions (if <number_of_partitions> = 0, -n is implied)\n"
		<< "-bl (--load-bsp-number-of-partitions): (Load-based BSP decomposition) Use the number of worker slaves as a minimum number of partitions\n"
		<< "-bl <number_of_partitions> (--load-bsp-number-of-partitions <number_of_partitions>): (Load-based BSP decomposition) Use the number_of_partitions as the number of partitions (if <number_of_partitions> = 0, -bl is implied)\n"
		<< "-bn (--bsp-number-of-partitions): (BSP decomposition) Use the number of worker slaves as the number of partitions\n"
		<< "-bn <number_of_partitions> (--bsp-number-of-partitions <number_of_partitions>): (BSP decomposition) Use the number_of_partitions as the number of partitions (if <number_of_partitions> = 0, -bn is implied)\n"
		<< "default: Use average load (Quad/OctTree decomposition)\n"
		<< "\n"
		<< "-rl <layers> (--remove-layers-in-load <layers>): Remove <layers> layers of cells out of load calculation\n"
		<< "default: <layers> = 3\n"
		<< "\n"
		<< "-hi (--height-independent-in-load): Load calculation in a cell does not depend on its height\n"
		<< "-hd (--height-dependent-in-load): Load estimation in a cell depends on its height\n"
		<< "default: Load calculation is height-dependent\n"
		<< "\n"
		<< "-nsk (--do-not-skip-inner-in-decomposition): Create inner empty subdomains\n"
		<< "-sk (--skip-inner-in-decomposition): Skip creation of inner empty subdomains\n"
		<< "default: Inner empty subdomains are not created\n"
		<< "\n"
		<< "-if (--create-inner-front): Create inner front in empty subdomains\n"
		<< "-nif (--do-not-create-inner-front): Do not create inner front in empty subdomains\n"
		<< "default: Inner front in empty subdomains are not created\n"
		<< "observation: Only works when inner empty subdomains are created (-sk)"
		<< "\n"
		<< "-r <range_proportion> (--range-proportion <range_proportion>): Percentage of the search region range (radius) that should not cross the bounding box of a subdomain (second modification in the serial advancing front technique)\n"
		<< "default: <range_proportion> = 1.0\n"
		<< "\n"
		<< "-mi <layer> (--master-improve <layer>): Improve <layer> layers of elements in master, count from the front\n"
		<< "default: <layer> = 1, meaning that one layer of elements will be improved in master\n"
		<< "\n"
		<< "-uf (--update-full): Update full mesh and front\n"
		<< "-up (--update-partial): Update partial mesh and full front\n"
		<< "-upnb (--update-partial-no-boundary): Update partial mesh and full front, but exclude mesh adjacent to boundary\n"
		<< "-ue (--update-empty): Update only front\n"
		<< "-uenb (--update-empty-no-boundary): Update only front, but exclude mesh adjacent to boundary\n"
		<< "-ui (--update-final): Update only front, but only final mesh\n"
		<< "default: Update only front excluding mesh adjacent to boundary\n"
		<< "\n"
		<< "-sh <shift_mode> (--shift <shift_mode>): Set shift mode, which should be one of the following\n"
		<< "<shift_mode> = 0: no shift performed\n"
		<< "<shift_mode> = 1: 1 side only (lateral)\n"
		<< "<shift_mode> = 2: 2 sides only (diagonal (edge, in 3D))\n"
		<< "<shift_mode> = 3: 1 and 2 sides (lateral and diagonal (edge, in 3D))\n"
		<< "<shift_mode> = 4: 3 sides only (diagonal (vertex, in 3D))\n"
		<< "<shift_mode> = 5: 1 and 3 sides (lateral and diagonal (vertex, in 3D))\n"
		<< "<shift_mode> = 6: 2 and 3 sides (diagonals (edge and vertex, in 3D))\n"
		<< "<shift_mode> = 7: 1, 2 and 3 sides (lateral and diagonals (edge and vertex, in 3D))\n"
		<< "default: <shift_mode> = 1\n"
		<< "observation: modes 4, 5, 6 and 7 are for 3D only, and imply, respectively, 1, 1, 2 and 3 modes in 2D\n"
		<< "observation: any mode different than the above will set the mode to the default value\n";
#if DEBUG_DISABLE_IMPROVEMENT
	str << "\n"
		<< "(debug) -dw: Disable mesh improvement in workers\n"
		<< "(debug) default: Mesh improvement in workers enabled\n"
		<< "\n"
		<< "(debug) -dm: Disable mesh improvement in master\n"
		<< "(debug) default: Mesh improvement in master enabled\n";
#endif //#if DEBUG_DISABLE_IMPROVEMENT

	return str.str();
}

int PMTools::execute(int argc, char *argv[])
{
	PMTools::MainDrive *drive = new PMTools::MainDrive();

#if DEBUG_DISABLE_IMPROVEMENT
	PMTools::enableMasterImprovement();
	PMTools::enableWorkerImprovement();
#endif //#if DEBUG_DISABLE_IMPROVEMENT

#if (!USE_WORKER_FILE_LOADER)
	if (drive->getComm()->isMaster())
#endif //#if (!USE_WORKER_FILE_LOADER)
	{
#if USE_NUMERICAL
        drive->setHotellingTransformEnabled(false);
#endif //#if USE_NUMERICAL

		drive->setUsingUniformRefinement(false);

		drive->setParallel(false);

		drive->setLoadBalancerType(Parallel::DEMAND_PARALLEL_DYNAMIC);

		drive->setPartitionerType(PMJMesh::BOX_LOAD);

		drive->setUsePartitioningThreshold(false);

		drive->setRemoveLayersInLoad(3);

		drive->setHeightDependentInLoad(true);

		drive->setSkippingInnerInDecomposition(true);

		drive->setCreateInnerFrontInDecomposition(false);

		drive->setRangeProportion(1.0);

		drive->setSmoothingLayer(1);

		drive->setUpdateType(PMJMesh::EMPTY_NO_BOUNDARY);

		drive->setShiftType(PMJMesh::LATERAL);

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

#if (USE_ONLY_MTOOLS != 3)
			used[0] = true;
#endif //#if (USE_ONLY_MTOOLS != 3)

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
#if USE_OPENMP
				else if ((std::string("-p") == argv[i]) || (std::string("--parallel") == argv[i]))
				{
					used[i] = true;

					drive->setParallel(true);

					if (i < argc - 1)
					{
						try
						{
							unsigned int nt = Data::convert<unsigned int>(argv[i+1]);

							used[i+1] = true;

							if (nt == 0)
							{
								drive->setParallel(false);
							}
							else
							{
								drive->setParallel(true);
								static_cast<PMJMesh::Communicator *>(drive->getComm())->setMaxThreads(nt);

#if USE_POOL
								PMJMesh::resizeThreadPools();
#endif //#if USE_POOL
							}
						}
						catch (...)
						{

						}
					}
				}
#endif //#if USE_OPENMP
				else if ((std::string("-g") == argv[i]) || (std::string("--greedy") == argv[i]))
				{
					used[i] = true;

					drive->setLoadBalancerType(Parallel::GREEDY_SERIAL_STATIC);
				}
				else if ((std::string("-wg") == argv[i]) || (std::string("--weighted-greedy") == argv[i]))
				{
					used[i] = true;

					drive->setLoadBalancerType(Parallel::WEIGHTED_GREEDY_SERIAL_STATIC);
				}
				else if ((std::string("-s") == argv[i]) || (std::string("--spring") == argv[i]))
				{
					used[i] = true;

					drive->setLoadBalancerType(Parallel::SPRING_SERIAL_STATIC);
				}
				else if ((std::string("-d") == argv[i]) || (std::string("--demand") == argv[i]))
				{
					used[i] = true;

					drive->setLoadBalancerType(Parallel::DEMAND_PARALLEL_DYNAMIC);
				}
				else if ((std::string("-l") == argv[i]) || (std::string("--load") == argv[i]))
				{
					used[i] = true;

					drive->setPartitionerType(PMJMesh::BOX_LOAD);

					drive->setUsePartitioningThreshold(false);

					if (i < argc - 1)
					{
						try
						{
							double load = Data::convert<double>(argv[i+1]);

							used[i+1] = true;

							if (load > 0.0)
							{
								drive->setUsePartitioningThreshold(true);

								drive->setPartitioningThreshold(load);
							}
						}
						catch (...)
						{

						}
					}
					else
					{

					}
				}
				else if ((std::string("-n") == argv[i]) || (std::string("--number-of-partitions") == argv[i]))
				{
					used[i] = true;

					drive->setPartitionerType(PMJMesh::BOX_NUMBER);

					drive->setUsePartitioningThreshold(false);

					if (i < argc - 1)
					{
						try
						{
							unsigned int np = Data::convert<unsigned int>(argv[i+1]);

							used[i+1] = true;

							if (np > 0)
							{
								drive->setUsePartitioningThreshold(true);

								drive->setPartitioningThreshold(np);
							}
						}
						catch (...)
						{

						}
					}
				}
				else if ((std::string("-bl") == argv[i]) || (std::string("--load-bsp-number-of-partitions") == argv[i]))
				{
					used[i] = true;

					drive->setPartitionerType(PMJMesh::SMART_BSP_NUMBER);

					drive->setUsePartitioningThreshold(false);

					if (i < argc - 1)
					{
						try
						{
							unsigned int np = Data::convert<unsigned int>(argv[i+1]);

							used[i+1] = true;

							if (np > 0)
							{
								drive->setUsePartitioningThreshold(true);

								drive->setPartitioningThreshold(np);
							}
						}
						catch (...)
						{

						}
					}
				}
				else if ((std::string("-bn") == argv[i]) || (std::string("--bsp-number-of-partitions") == argv[i]))
				{
					used[i] = true;

					drive->setPartitionerType(PMJMesh::BSP_NUMBER);

					drive->setUsePartitioningThreshold(false);

					if (i < argc - 1)
					{
						try
						{
							unsigned int np = Data::convert<unsigned int>(argv[i+1]);

							used[i+1] = true;

							if (np > 0)
							{
								drive->setUsePartitioningThreshold(true);

								drive->setPartitioningThreshold(np);
							}
						}
						catch (...)
						{

						}
					}
				}
				else if ((std::string("-rl") == argv[i]) || (std::string("--remove-layers-in-load") == argv[i]))
				{
					used[i] = true;

					if (i < argc - 1)
					{
						try
						{
							unsigned int layers = Data::convert<unsigned int>(argv[i+1]);

							used[i+1] = true;

							drive->setRemoveLayersInLoad(layers);
						}
						catch (...)
						{
							std::cout << "Error: please, enter number of layers of cells to be removed out of load calculation" << std::endl;

							delete drive;

							return 1;
						}
					}
					else
					{
						std::cout << "Error: please, enter number of layers of cells to be removed out of load calculation" << std::endl;

						delete drive;

						return 1;
					}
				}
				else if ((std::string("-hi") == argv[i]) || (std::string("--height-independent-in-load") == argv[i]))
				{
					used[i] = true;

					drive->setHeightDependentInLoad(false);
				}
				else if ((std::string("-hd") == argv[i]) || (std::string("--height-dependent-in-load") == argv[i]))
				{
					used[i] = true;

					drive->setHeightDependentInLoad(true);
				}
				else if ((std::string("-nsk") == argv[i]) || (std::string("--do-not-skip-inner-in-decomposition") == argv[i]))
				{
					used[i] = true;

					drive->setSkippingInnerInDecomposition(false);
				}
				else if ((std::string("-sk") == argv[i]) || (std::string("--skip-inner-in-decomposition") == argv[i]))
				{
					used[i] = true;

					drive->setSkippingInnerInDecomposition(true);
				}
				else if ((std::string("-if") == argv[i]) || (std::string("--create-inner-front") == argv[i]))
				{
					used[i] = true;

					drive->setCreateInnerFrontInDecomposition(true);
				}
				else if ((std::string("-nif") == argv[i]) || (std::string("--do-not-create-inner-front") == argv[i]))
				{
					used[i] = true;

					drive->setCreateInnerFrontInDecomposition(false);
				}
				else if ((std::string("-r") == argv[i]) || (std::string("--range-proportion") == argv[i]))
				{
					used[i] = true;

					if (i < argc - 1)
					{
						try
						{
							double rangeProportion = Data::convert<double>(argv[i+1]);

							used[i+1] = true;

							if (rangeProportion >= 0.0)
							{
								drive->setRangeProportion(rangeProportion);
							}
						}
						catch (...)
						{
							std::cout << "Error: please, enter percentage of the search region range (radius) that should not cross the bounding box of a subdomain" << std::endl;

							delete drive;

							return 1;
						}
					}
					else
					{
						std::cout << "Error: please, enter percentage of the search region range (radius) that should not cross the bounding box of a subdomain" << std::endl;

						delete drive;

						return 1;
					}
				}
				else if ((std::string("-mi") == argv[i]) || (std::string("--master-improve") == argv[i]))
				{
					used[i] = true;

					if (i < argc - 1)
					{
						try
						{
							unsigned int layer = Data::convert<unsigned int>(argv[i+1]);

							used[i+1] = true;

							drive->setSmoothingLayer(layer);
						}
						catch (...)
						{
							std::cout << "Error: please, enter number of layers of elements that will be improved in master" << std::endl;

							delete drive;

							return 1;
						}
					}
					else
					{
						std::cout << "Error: please, enter number of layers of elements that will be improved in master" << std::endl;

						delete drive;

						return 1;
					}
				}
				else if ((std::string("-uf") == argv[i]) || (std::string("--update-full") == argv[i]))
				{
					used[i] = true;

					drive->setUpdateType(PMJMesh::FULL);
				}
				else if ((std::string("-up") == argv[i]) || (std::string("--update-partial") == argv[i]))
				{
					used[i] = true;

					drive->setUpdateType(PMJMesh::PARTIAL);
				}
				else if ((std::string("-upnb") == argv[i]) || (std::string("--update-partial-no-boundary") == argv[i]))
				{
					used[i] = true;

					drive->setUpdateType(PMJMesh::PARTIAL_NO_BOUNDARY);
				}
				else if ((std::string("-ue") == argv[i]) || (std::string("--update-empty") == argv[i]))
				{
					used[i] = true;

					drive->setUpdateType(PMJMesh::EMPTY);
				}
				else if ((std::string("-uenb") == argv[i]) || (std::string("--update-empty-no-boundary") == argv[i]))
				{
					used[i] = true;

					drive->setUpdateType(PMJMesh::EMPTY_NO_BOUNDARY);
				}
				else if ((std::string("-ui") == argv[i]) || (std::string("--update-final") == argv[i]))
				{
					used[i] = true;

					drive->setUpdateType(PMJMesh::FINAL);
				}
				else if ((std::string("-sh") == argv[i]) || (std::string("--shift") == argv[i]))
				{
					used[i] = true;

					if (i < argc - 1)
					{
						try
						{
							unsigned int shift = Data::convert<unsigned int>(argv[i+1]);

							used[i+1] = true;

							switch (shift)
							{
							case 0:
								drive->setShiftType(PMJMesh::NONE);
								break;

							case 1:
								drive->setShiftType(PMJMesh::LATERAL);
								break;

							case 2:
								drive->setShiftType(PMJMesh::DIAGONAL);
								break;

							case 3:
								drive->setShiftType(PMJMesh::LATERAL_DIAGONAL);
								break;

							case 4:
								drive->setShiftType(PMJMesh::DIAGONAL_VERTEX);
								break;

							case 5:
								drive->setShiftType(PMJMesh::LATERAL_DIAGONAL_VERTEX);
								break;

							case 6:
								drive->setShiftType(PMJMesh::DIAGONAL_DIAGONAL_VERTEX);
								break;

							case 7:
								drive->setShiftType(PMJMesh::LATERAL_DIAGONAL_DIAGONAL_VERTEX);
								break;

							default:
								std::cout << "invalid shift mode" << std::endl;

								delete drive;

								return 1;
								break;
							}
						}
						catch (...)
						{
							std::cout << "Error: please, enter shift mode" << std::endl;

							delete drive;

							return 1;
						}
					}
					else
					{
						std::cout << "Error: please, enter shift mode" << std::endl;

						delete drive;

						return 1;
					}
				}
				else if ((std::string("-m") == argv[i]) || (std::string("--mean") == argv[i]))
				{
					std::cout << "Error: deprecated option: " << argv[i] << ", please use -l or --load instead" << std::endl;

					delete drive;

					return 1;
				}
#if DEBUG_DISABLE_IMPROVEMENT
				else if (std::string("-dw") == argv[i])
				{
					used[i] = true;

					PMTools::disableWorkerImprovement();
				}
				else if (std::string("-dm") == argv[i])
				{
					used[i] = true;

					PMTools::disableMasterImprovement();
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
	}

#if USE_WORKER_FILE_LOADER
	if (!drive->loadFile())
#else
	if ((!drive->loadFile()) && (this->comm->isMaster()))
#endif //#if USE_WORKER_FILE_LOADER
	{
		std::cout << "Error: mesh could not be generated: " << drive->getStatus().getMessage() << std::endl;

		delete drive;

		return 1;
	}

	drive->execute();

	drive->saveFile();

	if (drive->getComm()->isMaster())
	{
		if (drive->getStatus().getType() >= drive->error.getType())
		{
			std::cout << "Error: mesh could not be generated" << std::endl;
		}
		else
		{
			std::cout << drive->qualityString() << std::endl;
		}
	}

	delete drive;

	//std::cout << "done" << std::endl;

	return 0;
}

#endif //#if ((USE_MTOOLS) && (!USE_GUI))
