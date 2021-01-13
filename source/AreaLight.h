#pragma once
#include "Light.h"
#include "Rectangle.h"
#include "Sphere.h"
#include "Disk.h"

namespace Elite
{
	class Material;

	//-----------------------------------------------//
	//               RECTANGLE LIGHT                 //
	//-----------------------------------------------//
	class RectLight : public Light, public Rectangle
	{
	public:
		RectLight(Material* mat, const Transform& transform = {}, float intensity = 1.f);
		virtual ~RectLight() = default;

		virtual RGBColor CalculateLight(const HitRecord& hit, FVector3& wi, float& pdf) override;
		virtual void GetLightDirection(HitRecord& hit) override;
	};

	//------------------------------------------//
	//               DISK LIGHT                 //
	//------------------------------------------//
	class DiskLight : public Light, public Disk
	{
	public: 
		DiskLight(Material* mat, const Transform& transform = {}, float radius = 1.f, float intensity = 1.f);
		virtual ~DiskLight() = default;

		virtual RGBColor CalculateLight(const HitRecord& hit, FVector3& wi, float& pdf) override;
		virtual void GetLightDirection(HitRecord& hit) override;
	};

	//--------------------------------------------//
	//               SPHERE LIGHT                 //
	//--------------------------------------------//

	class SphereLight : public Light, public Sphere
	{
	public:
		SphereLight(Material* mat, const Transform& transform = {}, float intensity = 1.f, float radius = 1.f);
		virtual ~SphereLight() = default;

		virtual RGBColor CalculateLight(const HitRecord& hit, FVector3& wi, float& pdf) override;
		virtual void GetLightDirection(HitRecord& hit) override;
	};
}