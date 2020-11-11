#pragma once
#include "ERGBColor.h"

namespace Elite
{
	class BRDF
	{
	public:
		static RGBColor Lambert(const float diffuseReflectance, const RGBColor& diffuseColor)
		{
			return diffuseColor * diffuseReflectance / float(E_PI);
		}

		static RGBColor Lambert(const RGBColor& diffuseReflectance, const RGBColor& diffuseColor)
		{
			return diffuseColor * diffuseReflectance / float(E_PI);
		}

		static RGBColor Phong(const float specularReflectance, const float phongExponent, const FVector3& viewDirection, const FVector3& lightDirection, const FVector3& surfaceNormal)
		{
			FVector3 reflect = -lightDirection + (2 * (Dot(surfaceNormal, lightDirection)) * surfaceNormal);
			float cosAngle = Dot(reflect, viewDirection);
			float specular = specularReflectance * powf(cosAngle, phongExponent);
			return RGBColor{ specular,specular,specular };
		}

		static RGBColor CookTorrence(const float D, const RGBColor& F, const float G, const FVector3& normal, const FVector3& lightDirection, const FVector3& viewDirection)
		{
			return (F * D * G) / (4 * (Dot(viewDirection, normal) * Dot(lightDirection, normal)));
		}

		static float TrowbridgeReitz_GGX(const FVector3& normal, const FVector3& halfVector, const float a)
		{
			float dotNH = Dot(normal, halfVector);
			float temp = ((dotNH * dotNH) * (a * a - 1) + 1);
			return (a * a) / ((float(E_PI)) * (temp * temp));
		}

		static RGBColor SchlickFresnel(const FVector3& halfVector, const FVector3& viewDirection, const RGBColor& albedo)
		{
			return albedo + (RGBColor{ 1.f,1.f,1.f } -albedo) * powf((1 - (Dot(halfVector, viewDirection))), 5.f);
		}

		static float Schlick_GGX(const FVector3& normal, const FVector3& viewDirection, const float k)
		{
			float dotNV = Dot(normal, viewDirection);
			return dotNV / (dotNV * (1 - k) + k);
		}

		static float Smith_GGX(const FVector3& normal, const FVector3& viewDirection, const FVector3& lightDirection, const float k)
		{
			return Schlick_GGX(normal, viewDirection, k) * Schlick_GGX(normal, lightDirection, k);
		}
	};
}
