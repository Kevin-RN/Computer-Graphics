#pragma once
#include "Ray.hpp"

class Plane 
{
	public:
		static Eigen::Vector3f get_hit_point(Eigen::Vector3f& origin, Eigen::Vector3f& ray);
		static bool intersects(Ray& ray, const Eigen::Vector3f point_on_plane, const Eigen::Vector3f normal, float& t);

	private:
		float distance_to_origin;
		Eigen::Vector3f normal;

};
