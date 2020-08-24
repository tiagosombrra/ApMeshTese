#include "PJMesh/Definitions.h"

#include "PJMesh/App.h"
#include "PJMesh/MainDrive.h"
#include "Parallel/Communicator.h"

#include <omp.h>

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		std::cout << "Usage: ./PJMesh <input_filename> <options>" << std::endl;

		std::cout << "Options:\n"
				  << "-o <output_filename> (--output <output_filename>): Save output mesh file (.txt or .pos)\n"
				  << "default: Do not produce output mesh file\n"
				  << "\n"
				  << "-p (--parallel): Enable shared memory parallelism (number of running threads defined by the environment variable OMP_NUM_THREADS or by the total number of cores in the system)\n"
				  << "-p <number_of_threads> (--parallel <number_of_threads>): Enable shared memory parallelism and specify the number of running threads\n"
				  //<< "default: Disable shared memory parallelism (same as \"-p 0\" or \"-p 1\")\n"
				  << "default: Disable shared memory parallelism (same as \"-p 0\")\n"
				  << "\n"
				  << "-g (--greedy): Use Greedy load balancer\n"
				  << "-wg (--weighted-greedy): Use Weighted Greedy load balancer\n"
				  << "-s (--spring): Use Spring load balancer\n"
				  << "-d (--demand): Use Demand load balancer\n"
				  << "default: Use Greedy load balancer\n"
				  << "\n"
				  << "-m (--mean): Use load as the average of the total load by the number of worker processors\n"
				  << "-l <load> (--load <load>): Use a specific maximum load (if <load> = 0, -m is implied)\n"
				  << "default: Use average load\n"
				  << "\n"
				  << "-ni (--not-improve): Do not improve mesh in slaves\n"
				  << "default: Improve mesh in slaves\n"
				  << "\n"
				  << "-si <layer> (--slave-not-improve <layer>): Do not improve <layer> layers of elements in slaves, counting from the front\n"
				  << "default: <layer> = 0, meaning that all elements will be improved in slaves\n"
				  << "\n"
				  << "-mi <layer> (--master-improve <layer>): Improve <layer> layers of elements in master, count from the front\n"
				  << "default: <layer> = 1, meaning that one layer of elements will be improved in master\n"
				  << "\n"
				  << "-uf (--update-full): Update full mesh and front\n"
				  << "-up (--update-partial): Update partial mesh and full front\n"
				  << "-ue (--update-empty): Update only front\n"
				  << "default: Update only front\n"
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

	PJMesh::App *app = new PJMesh::App();

	if (app->getComm()->isMaster())
	{
		//setting default parameters
		PJMesh::MainDrive *drive = dynamic_cast<PJMesh::MainDrive *>(app->getDrive());

		drive->setParallel(false);

		drive->setLoadBalancerType(Parallel::GREEDY_SERIAL_STATIC);

		drive->setUseMeanOfLoads(true);

		drive->setSmoothInSlavesEnabled(true);

		drive->setUnsmoothingLayer(0);

		drive->setSmoothingLayer(1);

		drive->setUpdateType(PJMesh::EMPTY);

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
				else if ((std::string("-p") == argv[i]) || (std::string("--parallel") == argv[i]))
				{
					used[i] = true;

					drive->setParallel(true);

					if (i < argc - 1)
					{
						std::stringstream str(argv[i+1]);

						std::string s = str.str();

						if ((s[0] >= '0') && (s[0] <= '9'))
						{
							used[i+1] = true;

							int nt = 0;

							str >> nt;

							//if ((nt == 0) || (nt == 1))
							if (nt == 0)
							{
								drive->setParallel(false);
							}
							else
							{
								omp_set_num_threads(nt);
							}
						}
					}
				}
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
				else if ((std::string("-m") == argv[i]) || (std::string("--mean") == argv[i]))
				{
					used[i] = true;

					drive->setUseMeanOfLoads(true);
				}
				else if ((std::string("-l") == argv[i]) || (std::string("--load") == argv[i]))
				{
					used[i] = true;

					drive->setUseMeanOfLoads(false);

					if (i < argc - 1)
					{
						used[i+1] = true;

						std::stringstream str(argv[i+1]);

						double load = 0.0;

						str >> load;

						if (load > 0.0)
						{
							drive->setMaxLoad(load);
						}
						else
						{
							drive->setUseMeanOfLoads(true);
						}
					}
					else
					{
						std::cout << "please, enter maximum load" << std::endl;

						delete app;

						return 1;
					}
				}
				else if ((std::string("-ni") == argv[i]) || (std::string("--not-improve") == argv[i]))
				{
					used[i] = true;

					drive->setSmoothInSlavesEnabled(false);
				}
				else if ((std::string("-si") == argv[i]) || (std::string("--slave-not-improve") == argv[i]))
				{
					used[i] = true;

					if (i < argc - 1)
					{
						used[i+1] = true;

						std::stringstream str(argv[i+1]);

						int layer = 0;

						str >> layer;

						drive->setUnsmoothingLayer(layer);
					}
					else
					{
						std::cout << "please, enter layer" << std::endl;

						delete app;

						return 1;
					}
				}
				else if ((std::string("-mi") == argv[i]) || (std::string("--master-improve") == argv[i]))
				{
					used[i] = true;

					if (i < argc - 1)
					{
						used[i+1] = true;

						std::stringstream str(argv[i+1]);

						int layer = 0;

						str >> layer;

						drive->setSmoothingLayer(layer);
					}
					else
					{
						std::cout << "please, enter layer" << std::endl;

						delete app;

						return 1;
					}
				}
				else if ((std::string("-uf") == argv[i]) || (std::string("--update-full") == argv[i]))
				{
					used[i] = true;

					drive->setUpdateType(PJMesh::FULL);
				}
				else if ((std::string("-up") == argv[i]) || (std::string("--update-partial") == argv[i]))
				{
					used[i] = true;

					drive->setUpdateType(PJMesh::PARTIAL);
				}
				else if ((std::string("-ue") == argv[i]) || (std::string("--update-empty") == argv[i]))
				{
					used[i] = true;

					drive->setUpdateType(PJMesh::EMPTY);
				}
			}
		}

		app->setInputFileName(input);
		app->setOutputFileName(output);
	}

	app->execute();

	if (app->getComm()->isMaster())
	{
		PJMesh::MainDrive *drive = dynamic_cast<PJMesh::MainDrive *>(app->getDrive());

		std::cout << drive->qualityString() << std::endl;
	}

	delete app;

	//std::cout << "done" << std::endl;

	return 0;
}
