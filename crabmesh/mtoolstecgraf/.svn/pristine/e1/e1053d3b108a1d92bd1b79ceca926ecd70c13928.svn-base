#ifndef _TEMPLATE_MJMESH_MAIN_DRIVE_H_
#define _TEMPLATE_MJMESH_MAIN_DRIVE_H_

#include "TemplateMJMesh/Definitions.h"
#include "MJMesh/MainDrive.h"

namespace TemplateMJMesh
{
	class MainDrive : public MJMesh::MainDrive
	{
	public:

		MainDrive();
		virtual ~MainDrive();

		virtual void execute();

		virtual void clear();

	public:

		enum
		{
#if USE_NUMERICAL
			TEMPLATE_MESH_GENERATED = HOTELLING_BACK_TRANSFORM_APPLIED + 1
#else
			TEMPLATE_MESH_GENERATED = MESH_IMPROVED + 1
#endif //#if USE_NUMERICAL
		};

		static const Performer::Status templateMeshGenerated;

	protected:

		virtual bool executeTemplateMeshGenerator();

		virtual Performer::TemplateMeshGenerator *makeTemplateMeshGenerator() const;
		virtual MJMesh::GeometryFrontAdvancer *makeFrontGeometryAdvancer() const;

	protected:

		Performer::TemplateMeshGenerator *templateMeshGenerator;
	};
}

#endif //#define _TEMPLATE_MJMESH_MAIN_DRIVE_H_
