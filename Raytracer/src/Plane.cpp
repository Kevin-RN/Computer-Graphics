#include "Plane.hpp"

bool Plane::intersects(Ray& ray, const Eigen::Vector3f point_on_plane, const Eigen::Vector3f normal, float& t ){
	Eigen::Vector3f origin = ray.getOrigin();
	Eigen::Vector3f direction = ray.getDirection();
	float distance_to_origin = point_on_plane.dot(normal);

    // Test if the ray intersects or is parrallel
    float denominator = normal.dot(direction);
    if (fabs(denominator) > 1e-6) {
        float t = (distance_to_origin - (origin.dot(normal))) / denominator;
        // Test if point is in front of behind the origin
        return (t > 0);
    }
	return false;
}