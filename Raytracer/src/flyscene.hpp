#ifndef __FLYSCENE__
#define __FLYSCENE__

// Must be included before glfw.
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <tucano/effects/phongmaterialshader.hpp>
#include <tucano/mesh.hpp>
#include <tucano/shapes/camerarep.hpp>
#include <tucano/shapes/cylinder.hpp>
#include <tucano/shapes/sphere.hpp>
#include "Triangle.hpp"
#include "Light.hpp"
#include <tucano/utils/flycamera.hpp>
#include <tucano/utils/imageIO.hpp>
#include <tucano/utils/mtlIO.hpp>
#include <tucano/utils/objimporter.hpp>
#include "Box.hpp"

class Flyscene {

public:
  Flyscene(void) {}

  /**
   * @brief Initializes the shader effect
   * @param width Window width in pixels
   * @param height Window height in pixels
   */
  void initialize(int width, int height);

  /**
   * Repaints screen buffer.
   **/
  virtual void paintGL();

  /**
   * Perform a single simulation step.
   **/
  virtual void simulate(GLFWwindow *window);

  /**
   * Returns the pointer to the flycamera instance
   * @return pointer to flycamera
   **/
  Tucano::Flycamera *getCamera(void) { return &flycamera; }

  /**
   * @brief Add a new light source
   */
  void addLight(void) { lights.push_back(Light(flycamera.getCenter(), Eigen::Vector4f(1.0, 1.0, 0.0, 1.0))); }


  /**
 * @brief Add a new light source
 */
  void addSphereLight(void) { spherelights.push_back(SphericalLight(flycamera.getCenter(), Eigen::Vector4f(0.5, 0.8, 1.0, 1.0), 0.05F)); }

  /**
   * @brief Create a debug ray at the current camera location and passing
   * through pixel that mouse is over
   * @param mouse_pos Mouse cursor position in pixels
   */
  void createDebugRay(const Eigen::Vector2f &mouse_pos);

  /**
   * @brief raytrace your scene from current camera position   
   */
  void raytraceScene(int width = 0, int height = 0);

  void translateMesh(Eigen::Vector3f& translationMatrix);
  void scaleMesh(Eigen::Vector3f scaleMatrix);
  void rotateMesh(Eigen::Vector3f& rotateMatrix, float& angle);

  /**
   * @brief trace a single ray from the camera passing through dest
   * @param origin Ray origin
   * @param dest Other point on the ray, usually screen coordinates
   * @return a RGB color
   */
  Eigen::Vector3f traceRay(int level, Eigen::Vector3f& origin, Eigen::Vector3f& ray_direction);

   /**
    * @brief compute the color of a hit point
    * @param level Recursive depth
    * @param ray_direction Direction of incomming ray
    * @param hit_face Face of hit triangle
    * @return a RGB color
    */
   Eigen::Vector3f Shade(int level, Eigen::Vector3f hit_point, Eigen::Vector3f ray_direction, Tucano::Face hit_face);

   /*
   * @brief compute the interpolated normal for a face
   * @param hit_face, the current hitface to compute the interpolated normal for
   * @param hit_point, the point in which the ray hits the face
   * @return the interpolated normal of face
   */
   Eigen::Vector3f interpolated_normals(Tucano::Face hit_face, Eigen::Vector3f& hit_point);

 /**
  *brief checks if there are objects between an intersection point and the lightsource
  *param intersection_point Origin of the
  *param light_dir Direction of the light
  *return boolean if there is an object inbetween the points true else false.
  */
	bool checkShadow(Eigen::Vector3f& intersection_point, Eigen::Vector3f& light_dir, float& distance);

	/*
	*brief Creates a float indicating the amount of soft shadows
	*param intersection_point Origin of the ray
	*param light Lightsource
	*return float representing the fraction of spherical light hitting the object
	*/
	float softShadow(Eigen::Vector3f& intersection_point, SphericalLight light);

  /**
	@brief updates the amount of reflections the debug raytracer has
	@param the number of reflections
  */
  void updateDebugRay(int refcount);

  /*
  *brief calculates coordinate of corners of the box and creates a face out of it and pushes it into the faces vector
  *param box Box that will get rendered into the previewer
  */
  void renderBoxes(Box box);

private:
  // A simple phong shader for rendering meshes
  Tucano::Effects::PhongMaterial phong;

  // A fly through camera
  Tucano::Flycamera flycamera;

  // the size of the image generated by ray tracing
  Eigen::Vector2i raytracing_image_size;

  // A camera representation for animating path (false means that we do not
  // render front face)
  Tucano::Shapes::CameraRep camerarep = Tucano::Shapes::CameraRep(false);

  // a frustum to represent the camera in the scene
  Tucano::Shapes::Sphere lightrep;
  Tucano::Shapes::Sphere spherelightrep;

  // light sources for ray tracing
  //vector<Eigen::Vector3f> lights;
  vector<Light> lights;
  vector<SphericalLight> spherelights;

  //all the bounding boxes
  vector<Tucano::Shapes::Cylinder> boxes;

  // Scene light represented as a camera
  Tucano::Camera scene_light;
  Tucano::Camera sphere_scene_light;

  /// A very thin cylinder to draw a debug ray
  Tucano::Shapes::Cylinder ray = Tucano::Shapes::Cylinder(0.1, 1.0, 16, 64);

  //debug ray reflections
  float buildDebugRays(const Eigen::Vector3f position, 
	  const Eigen::Vector3f direction, int count); //private helper function
  vector<Tucano::Shapes::Cylinder> reflectrays;
  vector<Tucano::Shapes::Cylinder> refractrays;
  int reflectcount = 0;
  Eigen::Vector3f storedPosition;
  Eigen::Vector3f storedDirection;

  //Accel structure
  Box accStructBox;


  // Scene meshes
  Tucano::Mesh mesh;

  /// MTL materials
  vector<Tucano::Material::Mtl> materials;

    Eigen::Vector3f phongShade(Eigen::Vector3f &hit_point, const Tucano::Face &hit_face);

    Eigen::Vector3f calculateRefract(int level, Eigen::Vector3f &hit_point, const Eigen::Vector3f &ray_direction,
                                     const Tucano::Face &hit_face, Eigen::Vector3f &refract_color);

    Eigen::Vector3f calculateReflect(int level, Eigen::Vector3f &hit_point, const Eigen::Vector3f &ray_direction,
                                     const Tucano::Face &hit_face, Eigen::Vector3f &reflect_color);

    float calculateFresnel(const Eigen::Vector3f &ray_direction, const Tucano::Face &hit_face) const;
};

#endif // FLYSCENE