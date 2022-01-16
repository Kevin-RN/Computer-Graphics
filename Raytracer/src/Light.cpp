#include "Light.hpp"

Light::Light(Eigen::Vector3f center, Eigen::Vector4f color)
{
	this->center = center;
	this->color = color;
}

Eigen::Vector3f Light::getCenter() {
	return center;
}

Eigen::Vector4f Light::getColor() {
	return color;
}

SphericalLight::SphericalLight(Eigen::Vector3f center, Eigen::Vector4f color, float radius)
{
	this->center = center;
	this->color = color;
	this->radius = radius;
}

Eigen::Vector3f SphericalLight::getCenter() {
	return center;
}

Eigen::Vector4f SphericalLight::getColor() {
	return color;
}

float SphericalLight::getRadius() {
	return radius;
}
