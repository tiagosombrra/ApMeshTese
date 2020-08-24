#ifndef _CRAB_MESH_DEFINITIONS_H_
#define _CRAB_MESH_DEFINITIONS_H_

#if USE_GUI

#include <wx/wx.h>
#include <wx/aui/aui.h>
#include <GL/glew.h>

#endif //#if USE_GUI

#include "Performer/Definitions.h"
#include "Data/Definitions.h"

namespace CRAbMesh
{
    using Data::Short;
    using Data::UShort;
    using Data::Int;
    using Data::LInt;
    using Data::UInt;
    using Data::ULInt;
    using Data::Real;
    using Data::LReal;

#if USE_GUI
    //auxiliary classes for interface
    class Camera;
    class Thread;
    class Redirector;

    //modules
    class Module;
    class ViewModule;
    class MeshesModule;
#if USE_SPLINES
    class SplinesModule;
#endif //#if USE_SPLINES
    class MeshModule;
    class MJMeshModule;
    class TemplateMJMeshModule;
    class JmeshModule;
#if USE_MTOOLS
    class MToolsModule;
#endif //#if USE_MTOOLS
#if USE_TECGRAF_MSH_2D
	class TecGrafMsh2DModule;
#endif //#if USE_TECGRAF_MSH_2D
#if USE_TECGRAF_MSH_3D
	class TecGrafMsh3DModule;
#endif //#if USE_TECGRAF_MSH_3D
#if USE_TECGRAF_MSH_SURF
	class TecGrafMshSurfModule;
#endif //#if USE_TECGRAF_MSH_SURF
//#if USE_MPI
    class PMJMeshModule;
    class PJmeshModule;
#if USE_MTOOLS
    class PMToolsModule;
    class PMTools2Module;
#endif //#if USE_MTOOLS
//#endif //#if USE_MPI
#if ((USE_NUMERICAL) && (USE_EXPRTK))
    class NumericalModule;
#endif //#if ((USE_NUMERICAL) && (USE_EXPRTK))

    //dialogs
    class ColorDialog;

    //classes for program interface
    class App;
    class Frame;
    class TreeItemData;
    class Canvas;
#endif //#if USE_GUI

#if (!USE_ONLY_MTOOLS)
#if ((USE_TECGRAF_MSH_2D) && (USE_TECGRAF_MSH_3D) && (USE_TECGRAF_MSH_SURF))
	static const unsigned int NUM_TECGRAF_MODULES = 3;
#elif (((!USE_TECGRAF_MSH_2D) && (USE_TECGRAF_MSH_3D) && (USE_TECGRAF_MSH_SURF)) || \
	   ((USE_TECGRAF_MSH_2D) && (!USE_TECGRAF_MSH_3D) && (USE_TECGRAF_MSH_SURF)) || \
	   ((USE_TECGRAF_MSH_2D) && (USE_TECGRAF_MSH_3D) && (!USE_TECGRAF_MSH_SURF)))
	static const unsigned int NUM_TECGRAF_MODULES = 2;
#elif (((USE_TECGRAF_MSH_2D) && (!USE_TECGRAF_MSH_3D) && (!USE_TECGRAF_MSH_SURF)) || \
	   ((!USE_TECGRAF_MSH_2D) && (USE_TECGRAF_MSH_3D) && (!USE_TECGRAF_MSH_SURF)) || \
	   ((!USE_TECGRAF_MSH_2D) && (!USE_TECGRAF_MSH_3D) && (USE_TECGRAF_MSH_SURF)))
	static const unsigned int NUM_TECGRAF_MODULES = 1;
#else
	static const unsigned int NUM_TECGRAF_MODULES = 0;
#endif //#if ((USE_TECGRAF_MSH_2D) && (USE_TECGRAF_MSH_3D) && (USE_TECGRAF_MSH_SURF))
#endif //#if (!USE_ONLY_MTOOLS)

    //When you add a new module, update the constant below (NUM_MODULES) and
    //  the constant with the name of the module (moduleNames). Write the
    //  module interface classes (inheriting from base class Module)
    //  and write the help and calls from the non-interface version
#if ((USE_ONLY_MTOOLS == 2) || (USE_ONLY_MTOOLS == 3))
    static const unsigned int NUM_MODULES = 1;
#else
#if USE_ONLY_MTOOLS
    static const unsigned int NUM_MODULES = 3;
#else
#if USE_MTOOLS
    static const unsigned int NUM_MODULES = NUM_TECGRAF_MODULES + 11;
#else
    static const unsigned int NUM_MODULES = NUM_TECGRAF_MODULES + 8;
#endif //#if USE_MTOOLS
#endif //#if USE_ONLY_MTOOLS
#endif //#if ((USE_ONLY_MTOOLS == 2) || (USE_ONLY_MTOOLS == 3))

    static const unsigned int MODULES_COUNT =
#if USE_GUI
        NUM_MODULES + 1 + 1 ///View and Splines modules
#if ((USE_NUMERICAL) && (USE_EXPRTK))
            + 1             ///Numerical module
#endif //#if ((USE_NUMERICAL) && (USE_EXPRTK))
            ;
#else
        NUM_MODULES
#if ((USE_NUMERICAL) && (USE_EXPRTK))
            + 1             ///Numerical module
#endif //#if ((USE_NUMERICAL) && (USE_EXPRTK))
        ;
#endif //#if USE_GUI

    static const std::string moduleNames[MODULES_COUNT] =
    {
#if USE_GUI
        "View",          //moduleNames[0] MUST be the View Module
#endif //#if USE_GUI
#if (USE_ONLY_MTOOLS == 2)
        "MTools"
#elif (USE_ONLY_MTOOLS == 3) //#if (USE_ONLY_MTOOLS == 2)
        "PMTools"
#else
#if (!USE_ONLY_MTOOLS)
        "Tools",
#if USE_GUI
        "Splines",
#endif //#if USE_GUI
        "MJMesh",
        "TemplateMJMesh",
        "Jmesh",
#endif //#if (!USE_ONLY_MTOOLS)
#if USE_MTOOLS
        "MTools",
#endif //#if USE_MTOOLS
#if USE_TECGRAF_MSH_2D
		"TecGrafMsh2D",
#endif //#if USE_TECGRAF_MSH_2D
#if USE_TECGRAF_MSH_3D
		"TecGrafMsh3D",
#endif //#if USE_TECGRAF_MSH_3D
#if USE_TECGRAF_MSH_SURF
		"TecGrafMshSurf",
#endif //#if USE_TECGRAF_MSH_SURF
//#if USE_MPI
#if (!USE_ONLY_MTOOLS)
        "PMJMesh",
        "PJmesh",
#endif //#if (!USE_ONLY_MTOOLS)
#if USE_MTOOLS
        "PMTools",
#endif //#if USE_MTOOLS
//#endif //#if USE_MPI
#if (!USE_ONLY_MTOOLS)
        "PMJMesh2",
#if USE_MTOOLS
        "PMTools2",
#endif //#if USE_MTOOLS
        "CMesh"
#if ((USE_NUMERICAL) && (USE_EXPRTK))
        , "Numerical"
#endif //#if ((USE_NUMERICAL) && (USE_EXPRTK))
#endif //#if (!USE_ONLY_MTOOLS)
#endif //#if (USE_ONLY_MTOOLS == 3)
    };

#if USE_GUI
    typedef std::vector<wxMenu *> MenuVector;
    typedef std::vector<wxAuiToolBar *> ToolBarVector;
    typedef std::vector<wxPanel *> PanelVector;
    typedef std::list<wxImage *> ImageList;
    typedef std::map<std::string, Module *> ModuleMap;
#endif //#if USE_GUI

#if (!USE_GUI)
    std::string help(const std::string &programName);
    std::string help(const std::string &programName, const std::string &module);

    int execute(int argc, char *argv[]);
#endif //#if USE_GUI

    //functions
    //initialize global stuff
    void init(int *argc, char **argv);
    void finalize();

#if USE_GUI
    #define BITMAP_PNG_FROM_DATA(name) \
        BitmapFromPNGData(name##_png, WXSIZEOF(name##_png))
    #define  IMAGE_PNG_FROM_DATA(name) \
         ImageFromPNGData(name##_png, WXSIZEOF(name##_png))

    wxBitmap BitmapFromPNGData(const void* data, size_t size);
    wxImage   ImageFromPNGData(const void* data, size_t size);
#endif //#if USE_GUI
}

#endif //#ifndef _CRAB_MESH_DEFINITIONS_H_
