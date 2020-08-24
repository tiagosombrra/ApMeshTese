#include "CRAbMesh/Definitions.h"

#if (!USE_GUI)

#if (USE_ONLY_MTOOLS == 2)
#include "MTools/Definitions.h"
#elif (USE_ONLY_MTOOLS == 3) //#if (USE_ONLY_MTOOLS == 2)
#include "PMTools/Definitions.h"
#else
#if (!USE_ONLY_MTOOLS)
#include "Tools/Definitions.h"
#include "MJMesh/Definitions.h"
#include "TemplateMJMesh/Definitions.h"
#include "Jmesh/Definitions.h"
#endif //#if (!USE_ONLY_MTOOLS)
#if USE_MTOOLS
#include "MTools/Definitions.h"
#endif //#if USE_MTOOLS
#if (!USE_ONLY_MTOOLS)
#if ((USE_TECGRAF_MSH_2D) || (USE_TECGRAF_MSH_3D) || (USE_TECGRAF_MSH_SURF))
#include "TecGraf/Definitions.h"
#endif //#if ((USE_TECGRAF_MSH_2D) || (USE_TECGRAF_MSH_3D) || (USE_TECGRAF_MSH_SURF))
#endif //#if (!USE_ONLY_MTOOLS)
//#if USE_MPI
#if (!USE_ONLY_MTOOLS)
#include "PMJMesh/Definitions.h"
#include "PJmesh/Definitions.h"
#endif //#if (!USE_ONLY_MTOOLS)
#if USE_MTOOLS
#include "PMTools/Definitions.h"
#include "PMTools2/Definitions.h"
#endif //#if USE_MTOOLS
#if (!USE_ONLY_MTOOLS)
#include "PMJMesh2/Definitions.h"
#endif //#if (!USE_ONLY_MTOOLS)
//#endif //#if USE_MPI
#endif //#if (USE_ONLY_MTOOLS == 3)

std::string CRAbMesh::help(const std::string &programName)
{
	std::stringstream str;

#if (USE_ONLY_MTOOLS == 2)
	str << CRAbMesh::help(programName, "MTools");
#elif (USE_ONLY_MTOOLS == 3) //#if (USE_ONLY_MTOOLS == 2)
	str << CRAbMesh::help(programName, "PMTools");
#else
	str << "Usage: " << programName << " <module> <module_options>" << std::endl;

	str << "Modules available:" << std::endl;

	for (unsigned int i = 0; i < CRAbMesh::MODULES_COUNT; i++)
	{
		str << CRAbMesh::moduleNames[i] << std::endl;
	}
#endif //#if (USE_ONLY_MTOOLS == 3)

	return str.str();
}

#if ((USE_ONLY_MTOOLS == 2) || (USE_ONLY_MTOOLS == 3))
std::string CRAbMesh::help(const std::string &programName, const std::string &/*module*/)
#else
std::string CRAbMesh::help(const std::string &programName, const std::string &module)
#endif //#if ((USE_ONLY_MTOOLS == 2) || (USE_ONLY_MTOOLS == 3))
{
#if (USE_ONLY_MTOOLS == 2)
	return MTools::help(programName);
#elif (USE_ONLY_MTOOLS == 3) //#if (USE_ONLY_MTOOLS == 2)
	return PMTools::help(programName);
#else
	unsigned int mod = 0;

#if (!USE_ONLY_MTOOLS)
	if (module == CRAbMesh::moduleNames[mod++])
	{
		return Tools::help(programName);
	}

	if (module == CRAbMesh::moduleNames[mod++])
	{
		return MJMesh::help(programName);
	}

	if (module == CRAbMesh::moduleNames[mod++])
	{
		return TemplateMJMesh::help(programName);
	}

	if (module == CRAbMesh::moduleNames[mod++])
	{
		return Jmesh::help(programName);
	}
#endif //#if (!USE_ONLY_MTOOLS)

#if USE_MTOOLS
	if (module == CRAbMesh::moduleNames[mod++])
	{
		return MTools::help(programName);
	}
#endif //#if USE_MTOOLS

//#if USE_MPI
#if (!USE_ONLY_MTOOLS)
#if USE_TECGRAF_MSH_2D
	if (module == CRAbMesh::moduleNames[mod++])
	{
		return TecGraf::Msh2D::help(programName);
	}
#endif //#if USE_TECGRAF_MSH_2D

#if USE_TECGRAF_MSH_3D
	if (module == CRAbMesh::moduleNames[mod++])
	{
		return TecGraf::Msh3D::help(programName);
	}
#endif //#if USE_TECGRAF_MSH_3D

#if USE_TECGRAF_MSH_SURF
	if (module == CRAbMesh::moduleNames[mod++])
	{
		return TecGraf::MshSurf::help(programName);
	}
#endif //#if USE_TECGRAF_MSH_SURF

	if (module == CRAbMesh::moduleNames[mod++])
	{
		return PMJMesh::help(programName);
	}

	if (module == CRAbMesh::moduleNames[mod++])
	{
		return PJmesh::help(programName);
	}
#endif //#if (!USE_ONLY_MTOOLS)

#if USE_MTOOLS
	if (module == CRAbMesh::moduleNames[mod++])
	{
		return PMTools::help(programName);
	}
#endif //#if USE_MTOOLS

#if (!USE_ONLY_MTOOLS)
	if (module == CRAbMesh::moduleNames[mod++])
	{
		return PMJMesh2::help(programName);
	}
#endif //#if (!USE_ONLY_MTOOLS)
//#endif //#if USE_MPI

#if USE_MTOOLS
	if (module == CRAbMesh::moduleNames[mod++])
	{
		return PMTools2::help(programName);
	}
#endif //#if USE_MTOOLS

	std::stringstream str;

	if (module != "")
	{
		str << "Sorry, no module " << module << " available" << std::endl;
	}

	str << CRAbMesh::help(programName);

	return str.str();
#endif //#if (USE_ONLY_MTOOLS == 3)
}

int CRAbMesh::execute(int argc, char *argv[])
{
#if (USE_ONLY_MTOOLS == 2)
	return MTools::execute(argc, argv);
#elif (USE_ONLY_MTOOLS == 3) //#if (USE_ONLY_MTOOLS == 2)
	return PMTools::execute(argc, argv);
#else
	std::string module = argv[0];

	unsigned int mod = 0;

#if (!USE_ONLY_MTOOLS)
	if (module == CRAbMesh::moduleNames[mod++])
	{
		return Tools::execute(argc, argv);
	}

	if (module == CRAbMesh::moduleNames[mod++])
	{
		return MJMesh::execute(argc, argv);
	}

	if (module == CRAbMesh::moduleNames[mod++])
	{
		return TemplateMJMesh::execute(argc, argv);
	}

	if (module == CRAbMesh::moduleNames[mod++])
	{
		return Jmesh::execute(argc, argv);
	}
#endif //#if (!USE_ONLY_MTOOLS)

#if USE_MTOOLS
	if (module == CRAbMesh::moduleNames[mod++])
	{
		return MTools::execute(argc, argv);
	}
#endif //#if USE_MTOOLS

//#if USE_MPI
#if (!USE_ONLY_MTOOLS)
#if USE_TECGRAF_MSH_2D
	if (module == CRAbMesh::moduleNames[mod++])
	{
		return TecGraf::Msh2D::execute(argc, argv);
	}
#endif //#if USE_TECGRAF_MSH_2D

#if USE_TECGRAF_MSH_3D
	if (module == CRAbMesh::moduleNames[mod++])
	{
		return TecGraf::Msh3D::execute(argc, argv);
	}
#endif //#if USE_TECGRAF_MSH_3D

#if USE_TECGRAF_MSH_SURF
	if (module == CRAbMesh::moduleNames[mod++])
	{
		return TecGraf::MshSurf::execute(argc, argv);
	}
#endif //#if USE_TECGRAF_MSH_SURF

	if (module == CRAbMesh::moduleNames[mod++])
	{
		return PMJMesh::execute(argc, argv);
	}

	if (module == CRAbMesh::moduleNames[mod++])
	{
		return PJmesh::execute(argc, argv);
	}
#endif //#if (!USE_ONLY_MTOOLS)

#if USE_MTOOLS
	if (module == CRAbMesh::moduleNames[mod++])
	{
		return PMTools::execute(argc, argv);
	}
#endif //#if USE_MTOOLS

#if (!USE_ONLY_MTOOLS)
	if (module == CRAbMesh::moduleNames[mod++])
	{
		return PMJMesh2::execute(argc, argv);
	}
#endif //#if (!USE_ONLY_MTOOLS)

#if USE_MTOOLS
	if (module == CRAbMesh::moduleNames[mod++])
	{
		return PMTools2::execute(argc, argv);
	}
#endif //#if USE_MTOOLS

//#endif //#if USE_MPI
#endif //#if (USE_ONLY_MTOOLS == 3)

	return -1;
}

#endif //#if (!USE_GUI)

#if USE_POOL
#include "Data/Point2D.h"
#include "Data/Point3D.h"
#include "Data/Triangle2D.h"
#include "Data/Tetrahedron.h"
#include "Data/Box.h"
#include "Data/VertexBoxSearchTreeCell.h"
#include "Data/FrontElementBoxSearchTreeCell.h"
#include "MJMesh/Edge2D.h"
#include "MJMesh/Triangle3D.h"
#include "MJMesh/Vertex.h"
#include "MJMesh/FrontElement2D.h"
#include "MJMesh/FrontElement3D.h"
#include "MJMesh/Element2D.h"
#include "MJMesh/Element3D.h"
#include "MJMesh/QuadTreeCell.h"
#include "MJMesh/OctTreeCell.h"
#include "MJMesh/VertexBoxSearchTreeCell.h"
#include "MJMesh/FrontElementBoxSearchTreeCell.h"
#include "PMJMesh/QuadTreeCell.h"
#include "PMJMesh/OctTreeCell.h"
#endif //#if USE_POOL

#if USE_MTOOLS
#include "MTools/Definitions.h"
#include "PMTools/Definitions.h"
#endif //#if USE_MTOOLS

#if USE_MPI
#include <mpi.h>
#endif //#if USE_MPI

void CRAbMesh::init(int *argc, char **argv)
{
	Data::init(argc, argv);

#if USE_MPI
    if (!MPI::Is_initialized())
    {
        MPI::Init(*argc, argv);
    }
#endif //#if USE_MPI

#if USE_POOL
	MJMesh::Edge2D::poolMake();
	MJMesh::Triangle3D::poolMake();
	MJMesh::Vertex::poolMake();
	MJMesh::FrontElement2D::poolMake();
	MJMesh::FrontElement3D::poolMake();
	MJMesh::Element2D::poolMake();
	MJMesh::Element3D::poolMake();
#if USE_TREE_CELL_POOL
	MJMesh::QuadTreeCell::poolMake();
	MJMesh::OctTreeCell::poolMake();
#if USE_EXPERIMENTAL_5
	MJMesh::VertexBoxSearchTreeCell2D::poolMake();
	MJMesh::FrontElementBoxSearchTreeCell2D::poolMake();
	MJMesh::VertexBoxSearchTreeCell3D::poolMake();
	MJMesh::FrontElementBoxSearchTreeCell3D::poolMake();
#else
	MJMesh::VertexBoxSearchTreeCell::poolMake();
	MJMesh::FrontElementBoxSearchTreeCell::poolMake();
#endif //#if USE_EXPERIMENTAL_5
	PMJMesh::QuadTreeCell::poolMake();
	PMJMesh::OctTreeCell::poolMake();
#endif //#if USE_TREE_CELL_POOL
#endif //#if USE_POOL

#if ((USE_MTOOLS) && (DEBUG_DISABLE_IMPROVEMENT))
	PMTools::enableMasterImprovement();
	PMTools::enableWorkerImprovement();
#endif //#if ((USE_MTOOLS) && (DEBUG_DISABLE_IMPROVEMENT))
}

void CRAbMesh::finalize()
{
	Data::finalize();

#if USE_MPI
    if (!MPI::Is_finalized())
    {
        MPI::Finalize();
    }
#endif //#if USE_MPI

#if USE_POOL
	MJMesh::Edge2D::poolUnmake();
	MJMesh::Triangle3D::poolUnmake();
	MJMesh::Vertex::poolUnmake();
	MJMesh::FrontElement2D::poolUnmake();
	MJMesh::FrontElement3D::poolUnmake();
	MJMesh::Element2D::poolUnmake();
	MJMesh::Element3D::poolUnmake();
#if USE_TREE_CELL_POOL
	MJMesh::QuadTreeCell::poolUnmake();
	MJMesh::OctTreeCell::poolUnmake();
#if USE_EXPERIMENTAL_5
	MJMesh::VertexBoxSearchTreeCell2D::poolUnmake();
	MJMesh::FrontElementBoxSearchTreeCell2D::poolUnmake();
	MJMesh::VertexBoxSearchTreeCell3D::poolUnmake();
	MJMesh::FrontElementBoxSearchTreeCell3D::poolUnmake();
#else
	MJMesh::VertexBoxSearchTreeCell::poolUnmake();
	MJMesh::FrontElementBoxSearchTreeCell::poolUnmake();
#endif //#if USE_EXPERIMENTAL_5
	PMJMesh::QuadTreeCell::poolUnmake();
	PMJMesh::OctTreeCell::poolUnmake();
#endif //#if USE_TREE_CELL_POOL
#endif //#if USE_POOL
}

#if USE_GUI
#include <wx/mstream.h>

wxBitmap CRAbMesh::BitmapFromPNGData(const void* data, size_t size)
{
    wxMemoryInputStream is(data, size);
    wxImage image(is, wxBITMAP_TYPE_PNG);
    if ( image.IsOk() )
        return wxBitmap(image);

    return wxBitmap();
}

wxImage CRAbMesh::ImageFromPNGData(const void* data, size_t size)
{
    wxMemoryInputStream is(data, size);
    wxImage image(is, wxBITMAP_TYPE_PNG);
    if ( image.IsOk() )
        return image;

    return wxImage();
}
#endif //#if USE_GUI
