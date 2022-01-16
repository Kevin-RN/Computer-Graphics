#pragma once
#include <tucano/mesh.hpp>

class Ray
{
public:
	Ray(Eigen::Vector3f origin, Eigen::Vector3f destination);
	Ray() { };

	Eigen::Vector3f getOrigin();
	Eigen::Vector3f getDirection();
	Eigen::Vector3f getPointAtT(const float& distance);
	Eigen::Vector3f getRayInvDir();
	Eigen::Vector3f getSigns();

private:
	Eigen::Vector3f origin;
	Eigen::Vector3f destination;
};

