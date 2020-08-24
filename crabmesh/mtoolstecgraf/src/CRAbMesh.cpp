#include "CRAbMesh/Definitions.h"

#if (!USE_GUI)

#if USE_MPI
#include <mpi.h>
#endif //#if USE_MPI

std::string getParams(int argc, char *argv[])
{
    std::stringstream str;

	for (int i = 0; i < argc; i++)
	{
		str << " " << argv[i];
	}

	return str.str();
}

void printParams(const std::string &params, bool err)
{
    std::stringstream str;

    str << "executing";

#if USE_MPI
    int size = MPI::COMM_WORLD.Get_size();

    if ((size > 1) && (params.substr(0, 3) != "mpi"))
    {
        str << " mpirun -np " << size;
    }
#endif //#if USE_MPI

    str << params << std::endl;

    std::cout << str.str();

    if (err)
    {
        std::cerr << str.str();
    }
}

int main(int argc, char *argv[])
{
	std::string params = getParams(argc, argv);

	CRAbMesh::init(&argc, argv);

#if ((USE_ONLY_MTOOLS == 2) || (USE_ONLY_MTOOLS == 3))
	if ((argc <= 1) || (std::string("help") == argv[1]))
#else
	if ((argc <= 2) || (std::string("help") == argv[1]))
#endif //#if ((USE_ONLY_MTOOLS == 2) || (USE_ONLY_MTOOLS == 3))
	{
		std::string module = (argc == 2) ? argv[1] : ((argc >= 3) ? argv[2] : "");

		std::cout << CRAbMesh::help(argv[0], module) << std::endl;

		CRAbMesh::finalize();

		return 0;
	}

#if USE_MPI
    if (MPI::COMM_WORLD.Get_rank() == 0)
    {
#endif //#if USE_MPI
        //printParams(argc, argv, true);
        printParams(params, true);
#if USE_MPI
    }
#endif //#if USE_MPI

	int c = argc - 1;
	char **v = &argv[1];

	int ret = CRAbMesh::execute(c, v);

	if (ret == -1)
	{
		std::cout << CRAbMesh::help(argv[0], argv[1]) << std::endl;
	}

#if USE_BUFFER
#if USE_MPI
    int size = static_cast<int>(Data::getBuffer().size()), max = 0;
    MPI::COMM_WORLD.Allreduce(&size, &max, 1, MPI::INT, MPI::MAX);

    if (MPI::COMM_WORLD.Get_rank() > 0)
    {
        int nothing = 0;
        MPI::COMM_WORLD.Recv(&nothing, 1, MPI::INT, MPI::COMM_WORLD.Get_rank() - 1, 0);
    }
#endif //#if USE_MPI

    if (!Data::getBuffer().empty())
    {
#if USE_MPI
        std::cout << "Buffer from process " << MPI::COMM_WORLD.Get_rank() << std::endl;
#else
        std::cout << "Buffer" << std::endl;
#endif //#if USE_MPI
        std::cout << Data::getBuffer() << std::endl;
    }

#if USE_MPI
    if (MPI::COMM_WORLD.Get_rank() < MPI::COMM_WORLD.Get_size() - 1)
    {
        int nothing = 0;
        MPI::COMM_WORLD.Send(&nothing, 1, MPI::INT, MPI::COMM_WORLD.Get_rank() + 1, 0);
    }
#endif //#if USE_MPI
#endif //#if USE_BUFFER

	CRAbMesh::finalize();

	return ret;
}

#endif //#if (!USE_GUI)
