#include "Sphere.hpp"
#include "Ray.hpp"

bool Sphere::intersects(Ray& ray , const Eigen::Vector3f center, const float radius, float& t ) {

	//geometric calculation of intersection
	//Source: https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-sphere-intersection
	Eigen::Vector3f origin = ray.getOrigin();
	Eigen::Vector3f direction = ray.getDirection();

	Eigen::Vector3f L = center - origin;
	float tca = L.dot(direction);

	//vec points away from circle
	if (tca < 0) return false;

	float dsquared = L.norm() * L.norm() - tca * tca;
	float radiussquared = radius * radius;

	if (dsquared > radiussquared) return false; 

	float thc = sqrt(radiussquared - dsquared);

	float t0 = tca - thc;
	float t1 = tca + thc;

	if (t0 > t1) std::swap(t0, t1);

	if (t0 < 0) {
		t0 = t1;
		if (t0 < 0) return false;
	}

	t = t0;
	return true;
}
