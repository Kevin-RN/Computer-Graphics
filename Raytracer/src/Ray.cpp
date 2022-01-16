#include "Ray.hpp"

Ray::Ray(Eigen::Vector3f origin, Eigen::Vector3f destination) {
	this->origin = origin;
	this->destination = destination;
}

Eigen::Vector3f Ray::getOrigin() {
	return origin;
}
Eigen::Vector3f Ray::getDirection() {
	return (destination - origin).normalized();
}

Eigen::Vector3f Ray::getPointAtT(const float & distance) {
	return origin + distance * getDirection();
}

Eigen::Vector3f Ray::getRayInvDir() {
	return Eigen::Vector3f(1 / getDirection()[0], 1 / getDirection()[1], 1 / getDirection()[2]);
}

Eigen::Vector3f Ray::getSigns() {
	Eigen::Vector3f inverse = getRayInvDir();
	return  Eigen::Vector3f(inverse[0] < 0 ? 1 : 0, inverse[1] < 0 ? 1 : 0, inverse[2] < 0 ? 1 : 0 );
}

