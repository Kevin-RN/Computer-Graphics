#include "Shape.hpp"

#ifndef RAYTRACER_TRIANGLE_H
#define RAYTRACER_TRIANGLE_H


class Triangle:
        public Shape
{
public:
    bool intersects(int index, Tucano::Mesh& mesh, const Tucano::Face& face, const Eigen::Vector3f& ray_direction, const Eigen::Vector3f& origin, float& thit);

private:
    Eigen::Vector3f distance_to_origin;
    Eigen::Vector3f normal;
};



#endif //RAYTRACER_TRIANGLE_H
