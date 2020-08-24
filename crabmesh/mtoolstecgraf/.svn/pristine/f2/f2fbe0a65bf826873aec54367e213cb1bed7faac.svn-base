#ifndef _TOOLS_MESH_COLORIZER_H_
#define _TOOLS_MESH_COLORIZER_H_

#include "Tools/Definitions.h"

#if USE_GUI

#include "Tools/Tool.h"
#include <array>

namespace Tools
{
	class MeshColorizer :
		public Tools::Tool
	{
    public:

        enum ColorizationScheme
        {
            UNIQUE = 0,
            QUALITY,
            ORDER,
            TYPE
        };

	public:

		MeshColorizer();
		virtual ~MeshColorizer();

		virtual void setColorizeVertices(bool vertices);
		virtual void setColorizeFrontElements(bool frontElements);
		virtual void setColorizeElements(bool elements);

		virtual void setScheme(enum ColorizationScheme scheme, UInt first = 0);

		virtual void setMesh(Data::Mesh *mesh);

		virtual bool isColorized() const;

		virtual void execute();

    protected:

        virtual void colorizeUnique();
        virtual void colorizeQuality();
        virtual void colorizeOrder();
        virtual void colorizeType();

    protected:

    	typedef std::unordered_map<std::string, std::array<Real, 3> > StringColorMap;

	protected:

        bool vertices;
        bool frontElements;
        bool elements;

        enum ColorizationScheme scheme;
        UInt first;

        Data::Mesh *mesh;

        bool colorized;

	};
}

#endif //#if USE_GUI

#endif //#ifndef _TOOLS_MESH_COLORIZER_H_
