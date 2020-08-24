#ifndef _TOOLS_SILHOUETTE_IDENTIFIER_H_
#define _TOOLS_SILHOUETTE_IDENTIFIER_H__

#include "Tools/Definitions.h"
#include "Tools/ExternalTool.h"

namespace Tools
{
	class SilhouetteIdentifier : public Tools::ExternalTool
	{
	public:

		SilhouetteIdentifier(const Tool *tool = NULL);
		virtual ~SilhouetteIdentifier();

		virtual void setBoundary(const Data::Boundary *boundary);
		virtual void setAngle(Real angle);

		virtual Data::Mesh *getSilhouette();

		virtual void execute();

	protected:

		virtual Data::Mesh *makeMesh() const;
		virtual Data::Vertex *makeVertex(const Data::Vertex *v) const;
		virtual Data::FrontElement *makeFrontElement(Data::Vertex *v1, Data::Vertex *v2) const;

		virtual Real angle(const Data::FrontElement *fel1, const Data::FrontElement *fel2) const;

	protected:

		const Data::Boundary *boundary;

		Real angleThreshold;

		Data::Mesh *silhouette;

		Performer::IdManager *idManager;

	};
}

#endif //#ifndef _TOOLS_SILHOUETTE_IDENTIFIER_H_
