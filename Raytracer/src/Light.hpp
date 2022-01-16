#pragma once
#include <tucano/mesh.hpp>

class Light
{
public:
	Light(Eigen::Vector3f center, Eigen::Vector4f color);
	Eigen::Vector3f getCenter();
	Eigen::Vector4f getColor();
private:
	Eigen::Vector3f center;
	Eigen::Vector4f color;
};

class SphericalLight
{
public:
	SphericalLight(Eigen::Vector3f center, Eigen::Vector4f color, float radius);
	Eigen::Vector3f getCenter();
	Eigen::Vector4f getColor();
	float getRadius();
private:
	Eigen::Vector3f center;
	Eigen::Vector4f color;
	float radius;
};
