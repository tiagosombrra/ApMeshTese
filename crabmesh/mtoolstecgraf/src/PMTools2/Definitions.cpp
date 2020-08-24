#include "PMTools2/Definitions.h"

#if ((USE_MTOOLS) && (!USE_GUI))

#include "PMTools2/MainDrive.h"
#include "PMTools2/Communicator.h"

#include <cstdarg>
#include <list>

std::string PMTools2::help(const std::string &programName)
{
	std::stringstream str;

	str << "Usage: " << programName << " PMJMesh <input_filename> <options>\n"
		<< "Options:\n"
		<< "-o <output_filename> (--output <output_filename>): Save output mesh file (.txt or .dat)\n"
		<< "default: Do not produce output mesh file\n"
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
		//<< "default: Disable shared memory parallelism (same as \"-p 0\" or \"-p 1\")\n"
		<< "default: Disable shared memory parallelism (same as \"-p 0\")\n"
		<< "\n"
#endif //#if USE_OPENMP
        << "-ntt <number_of_tasks_per_thread> (--num-tasks-per-thread <number_of_tasks_per_thread>): Set <number_of_tasks_per_thread> as the number of tasks per thread\n"
        << "default: 1 task per thread\n"
        << "\n"
		<< "-mnt <min_number_of_tasks> (--min-num-of-tasks <min_number_of_tasks>): Changes the number of threads per task so that the minimum number of total tasks is <min_number_of_tasks>\n"
        << "observation: At least, 1 task per thread is set, even if the <min_number_of_tasks> is less than the number of total workers (number of processes times number of threads)\n"
#if USE_OPENMP
		<< "observation: Number of threads must have been already set through -p option\n"
#endif //#if USE_OPENMP
		<< "\n"
		<< "-hi (--height-independent-in-load): Load calculation in a cell does not depend on its height\n"
		<< "-hd (--height-dependent-in-load): Load estimation in a cell depends on its height\n"
		<< "default: Load calculation is not height-dependent\n"
		<< "\n"
		<< "-rl <layers> (--remove-layers-in-load <layers>): Remove <layers> layers of cells out of load calculation\n"
		<< "default: <layers> = 1\n"
#if USE_REMOVED_LAYERS_PROPORTIONS
		<< "\n"
		<< "-rlp <p1> .. <pn> (--removed-layers-proportions <p1> .. <pn>): Multiply the load of layer <i> by <pi> when removing it from the load calculation\n"
        << "default: <pi> = 1\n"
        << "observation: if n is less than <layers> in -rl, only the proportions of the first n layers will be set, the remaining <layers> - n will have proportion 1\n"
		<< "\n"
#endif //#if USE_REMOVED_LAYERS_PROPORTIONS
		<< "-lt (--load-threshold) <threshold>: Threshold of load proportion for interface load being considered\n"
		<< "default: <threshold> = 0.5\n"
		<< "observation: if <threshold> <= 0.0, then this threshold is disabled\n"
		<< "\n"
		<< "-bl (--bsp-load): (Load-based decomposition) Decompose the domain based on the load\n"
		<< "-bg (--bsp-geometry): (Geometry-based decomposition) Decompose the domain based on the geometry\n"
		<< "default: Decompose based on load\n"
		<< "\n"
		<< "-ibac <interface_bad_angle_cos> (--interface-bad-angle-cos <interface_bad_angle_cos>): Cosine of a minimum angle between boundary faces and the interface\n"
		<< "default: <interface_bad_angle_cos> = 0.95\n"
		<< "observation: Enabled only when the interfacing mesh is generated a priori\n"
		<< "\n"
		<< "-isdf <interface_shift_delta_factor> (--interface-shift-delta-factor <interface_shift_delta_factor>): Factor to shift the interface in case of a bad angle\n"
		<< "default: <interface_shift_delta_factor> = 2.0\n"
		<< "observation: Enabled only when the interfacing mesh is generated a priori\n"
		<< "\n"
		<< "\n"
		<< "-apo (--a-posteriori): Enable generation of interfacing mesh a posteriori\n"
		<< "-apr (--a-priori): Enable generation of interfacing mesh a priori\n"
		//<< "-api (--a-priori-internal): Enable generation of interfacing mesh a priori only on internal regions\n"
		<< "default: Interfacing mesh generated a posteriori\n"
		<< "\n"
		<< "-if (--create-inner-front): Create inner front in empty subdomains\n"
		<< "-nif (--do-not-create-inner-front): Do not create inner front in empty subdomains\n"
		<< "default: Inner front in empty subdomains is created\n"
		<< "observation: Disabled when the interfacing mesh is generated a priori\n"
		<< "\n"
		<< "-r <range_proportion> (--range-proportion <range_proportion>): Percentage of the search region range (radius) that should not cross the bounding box of a subdomain (second modification in the serial advancing front technique)\n"
		<< "default: <range_proportion> = 0.75\n"
		<< "observation: Disabled when the interfacing mesh is generated a priori\n"
		<< "\n"
		<< "-isf <interface_size_factor> (--interface-size-factor <interface_size_factor>): Factor to increase/decrease the size of elements in the interfaces\n"
		<< "default: <interface_size_factor> = 2.6\n"
		<< "observation: Enabled only when the interfacing mesh is generated a priori\n"
		<< "\n"
		<< "-ni (--not-improve): Do not improve mesh in slaves\n"
		<< "default: Improve mesh in slaves\n"
		<< "\n"
		<< "-il <layers> (--improvement-layers <layer>): Improve <layers> layers of elements when synchronizing\n"
		<< "default: <layers> = 1\n"
		<< "observation: at least 1 layer is improved, even if <layers> = 0\n"
		<< "\n"
		<< "-uf (--update-full): Update full mesh and front\n"
		<< "-up (--update-partial): Update partial mesh and full front\n"
		<< "-ue (--update-empty): Update only front\n"
		<< "default: Update partial mesh and full front\n"
		<< "\n"
		<< "-fmj (--final-mesh-joined): Final mesh is joined in master process\n"
		<< "-fmd (--final-mesh-distributed): Final mesh remains distributed among processes\n"
		<< "-fmr (--final-mesh-redistributed): Final mesh is redistributed to better balance the mesh among processes\n"
		<< "default: Final mesh remains distributed among processes\n";
#if DEBUG_CELL_COUNTER
    str << "\n"
		<< "--only-count: debug, give information of cells for load estimation, exit after\n"
		<< "default: disabled\n";
#endif //#if DEBUG_CELL_COUNTER
//#if USE_MTOOLS_OCT_TREE_FOR_LOAD_ESTIMATION
    str << "\n"
        << "--not-mtools-tree: debug, generate octree for load estimation from scratch\n"
        << "default: use octree generated in mtools\n";
//#endif //#if USE_MTOOLS_OCT_TREE_FOR_LOAD_ESTIMATION

	return str.str();
}

int PMTools2::execute(int argc, char *argv[])
{
	PMTools2::MainDrive *drive = new PMTools2::MainDrive();

#if (!USE_WORKER_FILE_LOADER)
	if (drive->getComm()->isMaster())
#endif //#if (!USE_WORKER_FILE_LOADER)
	{
#if USE_NUMERICAL
        drive->setHotellingTransformEnabled(false);
#endif //#if USE_NUMERICAL

		drive->setUsingUniformRefinement(false);

		static_cast<PMTools2::Communicator *>(drive->getComm())->setSharedParallelismEnabled(false);

		drive->setNumTasksPerThread(1);

		drive->setHeightDependentInLoad(false);

		drive->setRemoveLayersInLoad(1);

		drive->setLoadThreshold(0.5);

		drive->setAPrioriInterfaceBadAngleCos(0.95);

		drive->setAPrioriInterfaceShiftDelta(2.0);

		drive->setPartitionerType(PMJMesh2::LOAD_BASED_BSP);

		drive->setInterfaceMeshGenerationType(PMJMesh2::A_POSTERIORI);

		drive->setCreatingInnerFrontInDecomposition(true);

		drive->setRangeProportion(0.75);

		drive->setAPrioriInterfaceElementSizeFactor(1.0);

		drive->setImprovementInSlavesEnabled(true);

		drive->setImprovementLayers(1);

		drive->setUpdateType(PMJMesh2::PARTIAL);

		drive->setFinalMeshState(PMJMesh2::DISTRIBUTED);

#if DEBUG_CELL_COUNTER
        drive->setOnlyCount(false);
#endif //#if DEBUG_CELL_COUNTER

//#if USE_MTOOLS_OCT_TREE_FOR_LOAD_ESTIMATION
        drive->setUsingMtoolsOctreeForLoadEstimation(true);
//#endif //#if USE_MTOOLS_OCT_TREE_FOR_LOAD_ESTIMATION

		std::string input, output;

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
                    std::cout << "Error: please, enter output filename" << std::endl;

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

                static_cast<PMTools2::Communicator *>(drive->getComm())->setSharedParallelismEnabled(true);

                if (i < argc - 1)
                {
                    try
                    {
                        unsigned int nt = Data::convert<unsigned int>(argv[i+1]);

                        used[i+1] = true;

                        if (nt == 0)
                        {
                            static_cast<PMTools2::Communicator *>(drive->getComm())->setSharedParallelismEnabled(false);
                        }
                        else
                        {
                            static_cast<PMTools2::Communicator *>(drive->getComm())->setSharedParallelismEnabled(true);
                            static_cast<PMTools2::Communicator *>(drive->getComm())->setMaxThreads(nt);

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
            else if ((std::string("-ntt") == argv[i]) || (std::string("--num-tasks-per-thread") == argv[i]))
            {
                used[i] = true;

                if (i < argc - 1)
                {
                    try
                    {
                        unsigned int ntt = Data::convert<unsigned int>(argv[i+1]);

                        used[i+1] = true;

                        drive->setNumTasksPerThread(ntt);
                    }
                    catch (...)
                    {
                        std::cout << "Error: please, enter the number of tasks per thread" << std::endl;

                        delete drive;

                        return 1;
                    }
                }
                else
                {
                    std::cout << "Error: please, enter the number of tasks per thread" << std::endl;

                    delete drive;

                    return 1;
                }
            }
            else if ((std::string("-mnt") == argv[i]) || (std::string("--min-num-of-tasks") == argv[i]))
            {
                used[i] = true;

                if (i < argc - 1)
                {
                    try
                    {
                        unsigned int mnt = Data::convert<unsigned int>(argv[i+1]);

                        used[i+1] = true;

                        drive->setMinNumTasks(mnt);
                    }
                    catch (...)
                    {
                        std::cout << "Error: please, enter the minimum number of tasks" << std::endl;

                        delete drive;

                        return 1;
                    }
                }
                else
                {
                    std::cout << "Error: please, enter the minimum number of tasks" << std::endl;

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
#if USE_REMOVED_LAYERS_PROPORTIONS
            else if ((std::string("-rlp") == argv[i]) || (std::string("--removed-layers-proportions") == argv[i]))
            {
                used[i] = true;

                for (UInt layer = 0; (i + layer < argc - 1); layer++)
                {
                    try
                    {
                        Real proportion = Data::convert<Real>(argv[i+layer+1]);

                        used[i+layer+1] = true;

                        drive->setRemovedLayersProportions(layer, proportion);
                    }
                    catch (...)
                    {
                        break;
                    }
                }
            }
#endif //#if USE_REMOVED_LAYERS_PROPORTIONS
            else if ((std::string("-lt") == argv[i]) || (std::string("--load-threshold") == argv[i]))
            {
                used[i] = true;

                if (i < argc - 1)
                {
                    try
                    {
                        Real threshold = Data::convert<Real>(argv[i+1]);

                        used[i+1] = true;

                        drive->setLoadThreshold(threshold);
                    }
                    catch (...)
                    {
                        std::cout << "Error: please, enter load threshold" << std::endl;

                        delete drive;

                        return 1;
                    }
                }
                else
                {
                    std::cout << "Error: please, enter load threshold" << std::endl;

                    delete drive;

                    return 1;
                }
            }
            else if ((std::string("-bl") == argv[i]) || (std::string("--bsp-load") == argv[i]))
            {
                used[i] = true;

                drive->setPartitionerType(PMJMesh2::LOAD_BASED_BSP);
            }
            else if ((std::string("-bg") == argv[i]) || (std::string("--bsp-geometry") == argv[i]))
            {
                used[i] = true;

                drive->setPartitionerType(PMJMesh2::GEOMETRY_BASED_BSP);
            }
            else if ((std::string("-ibac") == argv[i]) || (std::string("--interface-bad-angle-cos") == argv[i]))
            {
                used[i] = true;

                if (i < argc - 1)
                {
                    try
                    {
                        double interfaceBadAngleCos = Data::convert<double>(argv[i+1]);

                        used[i+1] = true;

                        if (interfaceBadAngleCos >= 0.0)
                        {
                            drive->setAPrioriInterfaceBadAngleCos(interfaceBadAngleCos);
                        }
                    }
                    catch (...)
                    {
                        std::cout << "Error: please, enter the cosine of a minimum angle between boundary faces and the interface" << std::endl;

                        delete drive;

                        return 1;
                    }
                }
                else
                {
                    std::cout << "Error: please, enter the cosine of a minimum angle between boundary faces and the interface" << std::endl;

                    delete drive;

                    return 1;
                }
            }
            else if ((std::string("-isdf") == argv[i]) || (std::string("--interface-shift-delta-factor") == argv[i]))
            {
                used[i] = true;

                if (i < argc - 1)
                {
                    try
                    {
                        double interfaceShiftDelta = Data::convert<double>(argv[i+1]);

                        used[i+1] = true;

                        if (interfaceShiftDelta >= 0.0)
                        {
                            drive->setAPrioriInterfaceShiftDelta(interfaceShiftDelta);
                        }
                    }
                    catch (...)
                    {
                        std::cout << "Error: please, enter factor to shift the interface in case of a bad angle" << std::endl;

                        delete drive;

                        return 1;
                    }
                }
                else
                {
                    std::cout << "Error: please, enter factor to shift the interface in case of a bad angle" << std::endl;

                    delete drive;

                    return 1;
                }
            }
            else if ((std::string("-apo") == argv[i]) || (std::string("--a-posteriori") == argv[i]))
            {
                used[i] = true;

                drive->setInterfaceMeshGenerationType(PMJMesh2::A_POSTERIORI);
            }
            else if ((std::string("-apr") == argv[i]) || (std::string("--a-priori") == argv[i]))
            {
                used[i] = true;

                drive->setInterfaceMeshGenerationType(PMJMesh2::A_PRIORI);
            }
            /*else if ((std::string("-api") == argv[i]) || (std::string("--a-priori-internal") == argv[i]))
            {
                used[i] = true;

                drive->setInterfaceMeshGenerationType(PMJMesh2::A_PRIORI_INTERNAL);
            }*/
            else if ((std::string("-if") == argv[i]) || (std::string("--create-inner-front") == argv[i]))
            {
                used[i] = true;

                drive->setCreatingInnerFrontInDecomposition(true);
            }
            else if ((std::string("-nif") == argv[i]) || (std::string("--do-not-create-inner-front") == argv[i]))
            {
                used[i] = true;

                drive->setCreatingInnerFrontInDecomposition(false);
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
            else if ((std::string("-isf") == argv[i]) || (std::string("--interface-size-factor") == argv[i]))
            {
                used[i] = true;

                if (i < argc - 1)
                {
                    try
                    {
                        double interfaceSizeFactor = Data::convert<double>(argv[i+1]);

                        used[i+1] = true;

                        if (interfaceSizeFactor >= 0.0)
                        {
                            drive->setAPrioriInterfaceElementSizeFactor(interfaceSizeFactor);
                        }
                    }
                    catch (...)
                    {
                        std::cout << "Error: please, enter factor to increase/decrease the size of elements in the interfaces" << std::endl;

                        delete drive;

                        return 1;
                    }
                }
                else
                {
                    std::cout << "Error: please, enter factor to increase/decrease the size of elements in the interfaces" << std::endl;

                    delete drive;

                    return 1;
                }
            }
            else if ((std::string("-ni") == argv[i]) || (std::string("--not-improve") == argv[i]))
            {
                used[i] = true;

                drive->setImprovementInSlavesEnabled(false);
            }
            else if ((std::string("-il") == argv[i]) || (std::string("--improvement-layers") == argv[i]))
            {
                used[i] = true;

                if (i < argc - 1)
                {
                    try
                    {
                        unsigned int layers = Data::convert<unsigned int>(argv[i+1]);

                        used[i+1] = true;

                        drive->setImprovementLayers(layers);
                    }
                    catch (...)
                    {
                        std::cout << "Error: please, enter number of layers of elements that will be improved in synchronization" << std::endl;

                        delete drive;

                        return 1;
                    }
                }
                else
                {
                    std::cout << "Error: please, enter number of layers of elements that will be improved in synchronization" << std::endl;

                    delete drive;

                    return 1;
                }
            }
            else if ((std::string("-uf") == argv[i]) || (std::string("--update-full") == argv[i]))
            {
                used[i] = true;

                drive->setUpdateType(PMJMesh2::FULL);
            }
            else if ((std::string("-up") == argv[i]) || (std::string("--update-partial") == argv[i]))
            {
                used[i] = true;

                drive->setUpdateType(PMJMesh2::PARTIAL);

                /*if (drive->getAlgorithm() == A_PRIORI_INTERFACE_MESH)
                {
                    drive->setUpdateType(EMPTY);
                }*/
            }
            else if ((std::string("-ue") == argv[i]) || (std::string("--update-empty") == argv[i]))
            {
                used[i] = true;

                drive->setUpdateType(PMJMesh2::EMPTY);
            }
            else if ((std::string("-fmj") == argv[i]) || (std::string("--final-mesh-joined") == argv[i]))
            {
                used[i] = true;

                drive->setFinalMeshState(PMJMesh2::JOINED);
            }
            else if ((std::string("-fmd") == argv[i]) || (std::string("--final-mesh-distributed") == argv[i]))
            {
                used[i] = true;

                drive->setFinalMeshState(PMJMesh2::DISTRIBUTED);
            }
            else if ((std::string("-fmr") == argv[i]) || (std::string("--final-mesh-redistributed") == argv[i]))
            {
                used[i] = true;

                drive->setFinalMeshState(PMJMesh2::REDISTRIBUTED);
            }
#if DEBUG_CELL_COUNTER
            else if ((std::string("--only-count") == argv[i]))
            {
                used[i] = true;

                drive->setOnlyCount(true);
            }
#endif //#if DEBUG_CELL_COUNTER
//#if USE_MTOOLS_OCT_TREE_FOR_LOAD_ESTIMATION
            else if ((std::string("--not-mtools-tree") == argv[i]))
            {
                used[i] = true;

                drive->setUsingMtoolsOctreeForLoadEstimation(false);
            }
//#endif //#if USE_MTOOLS_OCT_TREE_FOR_LOAD_ESTIMATION
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

		drive->setInputFileName(input);
		drive->setOutputFileName(output);
	}

#if USE_WORKER_FILE_LOADER
	if (!drive->loadFile())
#else
	if ((!drive->loadFile()) && (drive->getComm()->isMaster()))
#endif //#if USE_WORKER_FILE_LOADER
	{
		std::cout << "Error: could not load file" << std::endl;

		delete drive;

		return 1;
	}

	drive->execute();

	drive->saveFile();

	if (drive->getComm()->isMaster())
	{
		if (drive->getStatus().getType() >= drive->error.getType())
		{
			std::cout << "Error: mesh could not be generated: " << drive->getStatus().getMessage() << std::endl;
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
