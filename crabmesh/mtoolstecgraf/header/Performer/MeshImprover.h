#ifndef _PERFORMER_MESH_IMPROVER_H_
#define _PERFORMER_MESH_IMPROVER_H_

#include "Performer/Definitions.h"

#include "Performer/Builder.h"

namespace Performer
{
	class MeshImprover : public Performer::Builder
	{
	public:

		MeshImprover(Data::Mesh *mesh = NULL, UInt numImproves = 5);
		virtual ~MeshImprover();

		virtual void setMesh(Data::Mesh *mesh);

		virtual void setNumImproves(UInt numImproves);

		virtual void setOptimizers(const MeshOptimizerList &optimizers);

		virtual void add(MeshOptimizer *opt);

		virtual void execute();

	public:

		enum
		{
			NOT_INIT = 0,
			INIT,
			ENDED
		};

		static const Performer::Status notInit;
		static const Performer::Status init;
		static const Performer::Status ended;

	protected:

		Data::Mesh *mesh;

		UInt numImproves;

		MeshOptimizerList optimizers;

	private:

		UInt step;

		MeshOptimizerList::iterator currOpt;
	};
}

#endif //#ifndef _PERFORMER_MESH_IMPROVER_H_
