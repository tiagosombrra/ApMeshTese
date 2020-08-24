#ifndef _TECGRAF_SURFACE_H_
#define _TECGRAF_SURFACE_H_

#include "TecGraf/Definitions.h"

#if USE_TECGRAF

#include "MJMesh/Boundary.h"

namespace TecGraf
{
	class Surface : public MJMesh::Boundary
	{
#if USE_GUI
	friend
		class TecGraf::OrientedSurface;
#endif //#if USE_GUI

	public:

		Surface(ULInt id = 0);
		Surface(Surface *surface);
		virtual ~Surface();

		virtual void setName(const std::string &name);
		virtual const std::string &getName() const;

		virtual void setType(const std::string &type);
		virtual const std::string &getType() const;

#if USE_GUI
		virtual std::string text() const;
#endif //#if USE_GUI

	protected:

		std::string name;
		std::string type;

	};
}

#endif //#if USE_TECGRAF

#endif //#ifndef _TECGRAF_SURFACE_H_
