#pragma once
#include "Shape.hpp"
#include "Ray.hpp"

class Sphere
	//public Shape
{
public:
	static bool intersects(Ray& ray, const Eigen::Vector3f center, const float radius, float& t);
	//Eigen::Vector3f get_hit_point(Eigen::Vector3f& origin, Eigen::Vector3f& ray);

private:
	//float distance_to_origin;
	//Eigen::Vector3f normal;

};