#ifndef _PERFORMER_MESH_TRANSFORMER_H_
#define _PERFORMER_MESH_TRANSFORMER_H_

#include "Performer/Definitions.h"
#include "Performer/Builder.h"
#include "Data/Vector3D.h"
#include "Data/Point3D.h"

namespace Performer
{
	class MeshTransformer : public Performer::Builder
	{
	public:

		enum Transformation
		{
			UNKNOWN = 0,
			TRANSLATION,
			SCALE,
			ROTATION,
		};

	public:

		MeshTransformer();
		virtual ~MeshTransformer();

		virtual void setBoundary(Data::Boundary *boundary);
		virtual void setMesh(Data::Mesh *mesh);

		virtual void translate(const Data::Vector3D &translation);

		virtual void scale(Real scale, bool mid = true, const Data::Point3D &reference = Data::Point3D());
		virtual void scale(const Data::Vector3D &scale, bool mid = true, const Data::Point3D &reference = Data::Point3D());

		virtual void rotate(const Data::Vector3D &axis, Real angle, bool mid = true, const Data::Point3D &reference = Data::Point3D());

		virtual enum Transformation getTransformation() const;

		virtual void execute();

	protected:

		virtual Data::Point3D findMid() const;

		virtual void applyTranslation(Data::Point *p) const;
		virtual void applyScale(Data::Point *p) const;
		virtual void applyRotation(Data::Point *p) const;

		virtual void applyTransform(Data::Point *p, Real *matrix[4]) const;

		virtual void translate();
		virtual void scale();
		virtual void rotate();

	protected:

		UInt dimension;

		enum Transformation transformation;

		Data::Vector3D translation;
		Data::Vector3D scaling;
		Data::Vector3D rotationAxis;
		Real rotationAngle;

		bool mid;
		Data::Point3D reference;

		Data::Boundary *boundary;
		Data::Mesh *mesh;
	};
}

#endif //#ifndef _PERFORMER_MESH_TRANSFORMER_H_
