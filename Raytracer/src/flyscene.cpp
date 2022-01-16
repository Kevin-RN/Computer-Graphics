#include "flyscene.hpp"
#include "Ray.hpp"
#include "Shape.hpp"
#include "Triangle.hpp"
#include "Plane.hpp"
#include <GLFW/glfw3.h>
#include <thread>
#include <atomic>
#include <future>
#include <limits>
#include <ctime>
#include "Box.hpp"
#include "Sphere.hpp"

#define PI 3.14159265
#define START_LEVEL 4
#define VERTICES_PER_ACCBOX 300
#define RENDERACCBOXES true

void Flyscene::initialize(int width, int height) {
    // initiliaze the Phong Shading effect for the Opengl Previewer
    phong.initialize();

    // set the camera's projection matrix
    flycamera.setPerspectiveMatrix(60.0, width / (float)height, 0.1f, 100.0f);
    flycamera.setViewport(Eigen::Vector2f((float)width, (float)height));

    // load the OBJ file and materials
    Tucano::MeshImporter::loadObjFile(mesh, materials,
                                      "resources/models/dodgeColorTest.obj");


    // normalize the model (scale to unit cube and center at origin)
    mesh.normalizeModelMatrix();

    // pass all the materials to the Phong Shader
    for (int i = 0; i < materials.size(); ++i)
        phong.addMaterial(materials[i]);

	// Create objects that are light sources
	Light light_obj = Light(Eigen::Vector3f(-0.5, 2.0, 3.0), Eigen::Vector4f(1.0, 1.0, 0.0, 1.0));

	// set the color and size of the sphere to represent the light sources
	// same sphere is used for all sources
	lightrep.setColor(light_obj.getColor());
	lightrep.setSize(0.1);
	spherelightrep.setColor(Eigen::Vector4f(0.5, 0.8, 1.0, 1.0));
	spherelightrep.setSize(0.15);

	// create a first ray-tracing light source at some random position
	lights.push_back(light_obj);

    // scale the camera representation (frustum) for the ray debug
    camerarep.shapeMatrix()->scale(0.2);

    // the debug ray is a cylinder, set the radius and length of the cylinder
    ray.setSize(0.005, 10.0);

	//----------------------------------------------------------------------------------just testing simulating the bounding box in the previewer
	//Box box = Box(Eigen::Vector3f(1.0, 1.0, 1.0), Eigen::Vector3f(0.5, 0.5, 0.5));
	//renderBoxes(box);


    // craete a first debug ray pointing at the center of the screen
    createDebugRay(Eigen::Vector2f(width / 2.0, height / 2.0));

    glEnable(GL_DEPTH_TEST);

    // for (int i = 0; i<mesh.getNumberOfFaces(); ++i){
    //   Tucano::Face face = mesh.getFace(i);
    //   for (int j =0; j<face.vertex_ids.size(); ++j){
    //     std::cout<<"vid "<<j<<" "<<face.vertex_ids[j]<<std::endl;
    //     std::cout<<"vertex "<<mesh.getVertex(face.vertex_ids[j]).transpose()<<std::endl;
    //     std::cout<<"normal "<<mesh.getNormal(face.vertex_ids[j]).transpose()<<std::endl;
    //   }
    //   std::cout<<"mat id "<<face.material_id<<std::endl<<std::endl;
    //   std::cout<<"face   normal "<<face.normal.transpose() << std::endl << std::endl;
    // }
}

void Flyscene::paintGL(void) {

    // update the camera view matrix with the last mouse interactions
    flycamera.updateViewMatrix();
    Eigen::Vector4f viewport = flycamera.getViewport();

    // clear the screen and set background color
    glClearColor(0.9, 0.9, 0.9, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // position the scene light at the last ray-tracing light source
	scene_light.resetViewMatrix();
	scene_light.viewMatrix()->translate(-lights.back().getCenter());

    // render the scene using OpenGL and one light source
    phong.render(mesh, flycamera, scene_light);

    // render the ray and camera representation for ray debug
    ray.render(flycamera, scene_light);
    camerarep.render(flycamera, scene_light);

	for (Tucano::Shapes::Cylinder r : reflectrays) {
		r.render(flycamera, scene_light);
	}

	for (Tucano::Shapes::Cylinder r : refractrays) {
		r.render(flycamera, scene_light);
	}

    //render the edges of a box
    for(Tucano::Shapes::Cylinder edge : boxes) {
        edge.render(flycamera, scene_light);
    }


    for (int i = 0; i < lights.size(); ++i) {
		lightrep.resetModelMatrix();
		lightrep.modelMatrix()->translate(lights[i].getCenter());
		//lightrep.modelMatrix()->translate(lights[i]);
		lightrep.render(flycamera, scene_light);
	}

	for (int i = 0; i < spherelights.size(); ++i) {
		spherelightrep.resetModelMatrix();
		spherelightrep.modelMatrix()->translate(spherelights[i].getCenter());
		spherelightrep.render(flycamera, sphere_scene_light);
	}

    // render coordinate system at lower right corner
    flycamera.renderAtCorner();
}

void Flyscene::simulate(GLFWwindow *window) {
    // Update the camera.
    // NOTE(mickvangelderen): GLFW 3.2 has a problem on ubuntu where some key
    // events are repeated: https://github.com/glfw/glfw/issues/747. Sucks.
    float dx = (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS ? 0.1 : 0.0) -
               (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS ? 0.1 : 0.0);
    float dy = (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS ||
                glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS
                ? 0.1
                : 0.0) -
               (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS ||
                glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS
                ? 0.1
                : 0.0);
    float dz = (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS ? 0.1 : 0.0) -
               (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS ? 0.1 : 0.0);
    flycamera.translate(dx, dy, dz);
}
void Flyscene::scaleMesh(Eigen::Vector3f scaleMatrix) {
	Eigen::Affine3f modelMatrix = mesh.getModelMatrix().scale(scaleMatrix);
	mesh.setModelMatrix(modelMatrix);
}
void Flyscene::translateMesh(Eigen::Vector3f& translationMatrix) {
	Eigen::Affine3f modelMatrix = mesh.getModelMatrix().translate(translationMatrix);
	mesh.setModelMatrix(modelMatrix);
}

void Flyscene::rotateMesh(Eigen::Vector3f& rotateMatrix, float& angle) {
	Eigen::Vector3f a = Eigen::Vector3f(1, 1, 1);
	Eigen::AngleAxis<float> aa(angle, rotateMatrix);
	Eigen::Affine3f modelMatrix = mesh.getModelMatrix().rotate(aa);
	mesh.setModelMatrix(modelMatrix);
}

void Flyscene::createDebugRay(const Eigen::Vector2f &mouse_pos) {
    ray.resetModelMatrix();

    // from pixel position to world coordinates
    Eigen::Vector3f screen_pos = flycamera.screenToWorld(mouse_pos);

    // direction from camera center to click position
    Eigen::Vector3f dir = (screen_pos - flycamera.getCenter()).normalized();

    // position and orient the cylinder representing the ray
    ray.setOriginOrientation(flycamera.getCenter(), dir);

    // place the camera representation (frustum) on current camera location,
    camerarep.resetModelMatrix();
    camerarep.setModelMatrix(flycamera.getViewMatrix().inverse());
	
	///////////////////// debug ray reflection  ///////////////////////

	//could also calc this from ray.modelmatrix*[0,0,0,1] i think, dunno what's better
	storedPosition = flycamera.getCenter();

	//could also calc this from ray.modelmatrix*[0,0,1,0]
	storedDirection = dir.normalized();

	updateDebugRay(reflectcount);
}

void Flyscene::updateDebugRay(int refcount) {
	reflectcount = refcount;
	reflectrays.clear();
	refractrays.clear();

	ray.getModelMatrix();

	if (reflectcount > 0) {
		float t = buildDebugRays(storedPosition,storedDirection, reflectcount - 1);
		ray.setSize(0.005, t);
	}
	else {
		ray.setSize(0.005, 1.0);
	}
}

float Flyscene::buildDebugRays(const Eigen::Vector3f position, const Eigen::Vector3f direction, int count) {
	//check if ray intersects (repeated code, should find a way to reuse other code block)
	float thit;
	float tclosest = INFINITY;
	int hitindex = -1;
	bool ins = false;
	for (int i = 0; i < mesh.getNumberOfFaces();++i) {
		if (Triangle::intersects(mesh, mesh.getFace(i), direction, position, thit)) {
			ins = true;
			if (thit < tclosest) {
				hitindex = i;
				tclosest = thit;
			}
		}
	}

	if (RENDERACCBOXES) {
		Eigen::Vector3f vmin;
		Eigen::Vector3f vmax;
		Box::findMinMax(mesh, vmin, vmax);
		Box bigbox = Box(mesh, vmin, vmax);
		bigbox.splitboxLimit(mesh, VERTICES_PER_ACCBOX);

		//render boxes
		std::vector<Box> boxes = bigbox.getBoxesList();
		for (Box b : boxes) {
			renderBoxes(b);
		}
	}
	//bigbox.intersects(mesh, position, direction, tbox, hitface);
	//float tbox;
	//if (Box::intersects(position, direction, vmin, vmax, tbox))
	//	cout << "BOX IC" << endl;

	//sphere check
	//Ray pew = Ray(position, direction);
	////implement ourselves?
	//Eigen::Vector3f center = mesh.getShapeModelMatrix() * mesh.getCentroid();
	//Eigen::Vector3f somevertex = mesh.getShapeModelMatrix() * mesh.getVertex(0).head<3>();
	//float radius = (center - somevertex).norm();
	//float tsphere;
	//bool ins = Sphere::intersects(pew, center, radius, tsphere);
	//float tclosest = tsphere;


	if (ins) { //we have an intersection point, draw the reflection vector
		Tucano::Shapes::Cylinder reflectray = Tucano::Shapes::Cylinder(0.1, 1.0, 16, 64);
		reflectray.resetModelMatrix();
		Tucano::Shapes::Cylinder refractray = Tucano::Shapes::Cylinder(0.1, 1.0, 16, 64);
		refractray.resetModelMatrix();

		//point of intersection
		Eigen::Vector3f inspoint = position + tclosest * direction;

		//getting reflection
		//src: https://math.stackexchange.com/questions/13261/how-to-get-a-reflection-vector

		Eigen::Vector3f color; //we don't use this here
		Eigen::Vector3f reflect = calculateReflect(0, inspoint, direction, mesh.getFace(hitindex), color);

		//add reflect ray
		reflectray.setColor(Eigen::Vector4f(1.0, 0.0, 0.0, 1.0));
		reflectray.setOriginOrientation(inspoint, reflect);

		if (count > 0) {
			reflectray.setSize(0.005, buildDebugRays(inspoint, reflect, count - 1));
		}
		else
			reflectray.setSize(0.001, 1.0);

		reflectrays.push_back(reflectray);




		Eigen::Vector3f refract = calculateRefract(0, inspoint, direction, mesh.getFace(hitindex), color);

		if (refract != Eigen::Vector3f(0,0,0)) {

			refractray.setColor(Eigen::Vector4f(0.0, 1.0, 0.0, 1.0));
			refractray.setOriginOrientation(inspoint, refract);

			//recursive call for refraction, returns t of this vector
			if (count > 0)
				refractray.setSize(0.005, buildDebugRays(inspoint,refract, count - 1));
			else
				refractray.setSize(0.001, 1.0);

			refractrays.push_back(refractray);
		}
		
		return tclosest;
	}
	else {  //no intersection point
		return 1.0;
	}
}

void Flyscene::raytraceScene(int width, int height) {
    std::cout << "ray tracing ..." << std::endl;
    clock_t begin = clock();
	//For accel structure, can be commented out if not used
	std::cout << "Creating acceleration structure..." << std::endl;
	Eigen::Vector3f vmin;
	Eigen::Vector3f vmax;
	Box::findMinMax(mesh, vmin, vmax);
	accStructBox = Box(mesh, vmin, vmax);
	accStructBox.splitboxLimit(mesh, VERTICES_PER_ACCBOX);
	std::cout << "Acceleration structure created" << std::endl;

    // if no width or height passed, use dimensions of current viewport
    Eigen::Vector2i image_size(width, height);
    if (width == 0 || height == 0) {
        image_size = flycamera.getViewportSize();
    }


    // create 2d vector to hold pixel colors and resize to match image size
    vector<vector<Eigen::Vector3f>> pixel_data;
    pixel_data.resize(image_size[1]);
    for (int i = 0; i < image_size[1]; ++i)
        pixel_data[i].resize(image_size[0]);


    // origin of the ray is always the camera center
    Eigen::Vector3f origin = flycamera.getCenter();
    Eigen::Vector3f screen_coords;

    
	// Multi Threading
	// Comment this if you don't want multi-threading
    //-----------------------------------------------------//
    int max_pixels = (image_size[0] * image_size[1]); //width * height
	// Get amount of cores of your CPU
    int cores = std::thread::hardware_concurrency();
	// Keep track of # of pixels (atomic making sure no 2 threads render the same pixel)
    volatile std::atomic<std::size_t> curr_pixel(0);
	// Stores all cores assigned to a task
    std::vector<std::future<void>> future_vector;
    cout << "Threads supported: " << cores << "\n";
    while (cores--)
        future_vector.emplace_back(
            std::async([=, &origin, &curr_pixel, &pixel_data]()
                {
                    while (true)
                    {
                        int index = curr_pixel++;
                        if (index >= max_pixels)
                            break;
                        std::size_t i = index % image_size[1];
                        std::size_t j = index / image_size[1];
						//cout << "at index: " << index << std::endl;

                        // create a ray from the camera passing through the pixel (i,j)
                        auto screen_coords = flycamera.screenToWorld(Eigen::Vector2f(i, j));
                        // launch raytracing for the given ray and write result to pixel data
                        pixel_data[j][i] = traceRay(START_LEVEL,origin, screen_coords);
						if (index % 10000 == 0) {
							std::cout << "Percentage done (mt): " << (float)(index / 10000) << "%" << std::endl;
						}
                    }
                }));

	// Call futures (Async jobs), this will activate all process on the cores
	for (auto& e : future_vector) {
		e.get();
	}
    //-----------------------------------------------------//
    
	// Uncomment this if you don't want multi-threading
    // for every pixel shoot a ray from the origin through the pixel coords
    //for (int j = 0; j < image_size[1]; j+=1) {
    //    for (int i = 0; i < image_size[0]; i+=1) {
    //        // create a ray from the camera passing through the pixel (i,j)
    //        screen_coords = flycamera.screenToWorld(Eigen::Vector2f(i, j));
    //        // launch raytracing for the given ray and write result to pixel data
    //        pixel_data[i][j] = traceRay(origin, screen_coords);
    //    }
    //    if (j % 10 == 0) {
    //        std::cout << "Percentage done (1t): " << (float)j / image_size[1] * 100 + 1 << "%" << std::endl;
    //    }
    //}

    // write the ray tracing result to a PPM image
    Tucano::ImageImporter::writePPMImage("result.ppm", pixel_data);
    std::cout << "ray tracing done! " << std::endl;
	clock_t end = clock();
	double elapsed_secs = double((double) end - (double) begin) / CLOCKS_PER_SEC;
	std::cout << "Time passed: " << elapsed_secs << "seconds" << std::endl;
}

Eigen::Vector3f Flyscene::traceRay(int level, Eigen::Vector3f& origin, Eigen::Vector3f& dest) {
	if (level <= 0) {
		return Eigen::Vector3f(0.0, 0.0, 0.0);
	}

	float tclosest = INFINITY;
	float thit;
	Tucano::Face closest_triangle;
	Tucano::Face temp_face;
	Eigen::Vector3f hit_point;

	Ray ray = Ray(origin, dest);
	bool intersected = false;

	//Use to avoid accel structure
	//for (int i = 0; i < mesh.getNumberOfFaces(); ++i) {
	//	temp_face = mesh.getFace(i);
	//	// Check if there is an intersection
	//	if (Triangle::intersects(mesh, temp_face, ray_direction, origin, thit)) {
	//		// If found a closer triangle, update the current triangle
	//		if (thit < tclosest) {
	//			intersected = true;
	//			closest_triangle = temp_face;
	//			tclosest = thit;
	//		}
	//	}
	//}

	//use acceleration structure
	intersected = accStructBox.intersects(mesh, origin, ray.getDirection(), tclosest, closest_triangle);


	// If intersection, return the closest traingle color
	if (intersected) {
		return Shade(level, ray.getPointAtT(tclosest), ray.getDirection(), closest_triangle);

	} else if (!level == START_LEVEL) {
		return Eigen::Vector3f(0.0, 0.0, 0.0);
	} //else, return default color
	else {
		return Eigen::Vector3f(1.0, 1.0, 1.0);
	}
}

//check for shadows
//Source: https://www.scratchapixel.com/lessons/3d-basic-rendering/introduction-to-shading/ligth-and-shadows
bool Flyscene::checkShadow(Eigen::Vector3f& intersection_point, Eigen::Vector3f& light_dir, float& distance) {
	Tucano::Face temp_face;
	//loop over all the faces to check if there is an object in between
	for (int i = 0; i < mesh.getNumberOfFaces(); ++i) {
		temp_face = mesh.getFace(i);
		// Check if there is an intersection
		if (Triangle::intersects(mesh, temp_face, light_dir, intersection_point, distance)) {
			return true;
		}
	}
	return false;
}

//handles soft shadow
//Source: https://www.scratchapixel.com/lessons/3d-basic-rendering/introduction-to-shading/shading-spherical-light
float Flyscene::softShadow(Eigen::Vector3f& intersection_point, SphericalLight light) {
	float amount_light_points = 9.0F, intensity = 1.0F, decrement = (1.0F / amount_light_points), distance =  0;
	Eigen::Vector3f hit_direction, hit_point_soft;

	Ray ray_light = Ray(intersection_point, light.getCenter());
	Eigen::Vector3f ray_direction = ray_light.getDirection();
	Eigen::Vector3f perpendicular = Eigen::Vector3f(-ray_direction.y(), ray_direction.x(), ray_direction.z());

	Eigen::Affine3f rotationMatrix = Eigen::Affine3f::Identity();
	rotationMatrix.rotate(Eigen::AngleAxisf((M_PI_2/ (int) amount_light_points), ray_direction));

	for(int i = 0; i < (int) amount_light_points; i++) {
		perpendicular = rotationMatrix * perpendicular;

		hit_point_soft = light.getCenter() + perpendicular * light.getRadius();
		hit_direction = (hit_point_soft - intersection_point).normalized();

		if (checkShadow(intersection_point, hit_direction, distance)) {
				intensity -= decrement;
		}
	}

	return intensity;
}

Eigen::Vector3f Flyscene::Shade(int level, Eigen::Vector3f hit_point, Eigen::Vector3f ray_direction, Tucano::Face hit_face) {
	float illum = materials[hit_face.material_id].getIlluminationModel();

    Eigen::Vector3f direct_color = phongShade(hit_point, hit_face);
	Eigen::Vector3f refract_color; //= Eigen::Vector3f(0.0, 0.0, 0.0);
	Eigen::Vector3f reflect_color; // = Eigen::Vector3f(0.0, 0.0, 0.0);

	float reflection = calculateFresnel(ray_direction, hit_face);
	float refraction = 1 - reflection;

	if (illum == 1) { //diffuse material

		return direct_color;
	}
	else if (illum == 3) { // full reflection
		calculateReflect(level, hit_point, ray_direction, hit_face, reflect_color);
		return (direct_color + reflect_color) * 0.9 * materials[hit_face.material_id].getDissolveFactor();
	}
	else if (illum == 6) { // full refraction
		calculateRefract(level, hit_point, ray_direction, hit_face, refract_color);
		return (direct_color + refract_color) * 0.7 * materials[hit_face.material_id].getDissolveFactor();
	}
	else { // reflect and refract using fresnel
		//Refraction
		calculateRefract(level, hit_point, ray_direction, hit_face, refract_color);

		//Reflection
		calculateReflect(level, hit_point, ray_direction, hit_face, reflect_color);

		return (direct_color + refract_color * refraction + reflect_color * reflection) * 0.7 * materials[hit_face.material_id].getDissolveFactor();
	}
}

float Flyscene::calculateFresnel(const Eigen::Vector3f &ray_direction, const Tucano::Face &hit_face) const {
    ///HOW MUCH DO WE REFELCT /REFRACT

    float fresnel_perpendicular = 0;
    float fresnel_parallel = 0;
    float fresnel = 0;

    float cos = hit_face.normal.dot(ray_direction);
    float index_refraction_outside = 1;
    float index_refraction_material = materials[hit_face.material_id].getOpticalDensity();
    Eigen::Vector3f normal = hit_face.normal;

    if (cos < 0) {
        cos = -cos;
    } else {
        normal = -normal;
        swap(index_refraction_outside, index_refraction_material);
    }

    float refraction_index = index_refraction_outside / index_refraction_material;
    float square_root_term = 1 - refraction_index * refraction_index * (1 - cos * cos);

    //Angle of refraction in terms of sin using Snells law.
    float sin_refraction = refraction_index * sqrtf(max(0.f, 1 - cos * cos));
    float cos_refraction = sqrtf(max(0.f, 1 - sin_refraction * sin_refraction));

    //Total internal reflection
    if (square_root_term < 0) {
        fresnel = 1;
    } else {
		cos = fabsf(cos);
        fresnel_perpendicular = (
                                        (index_refraction_material * cos) - (index_refraction_outside * cos_refraction))
                                / ((index_refraction_material * cos) + (index_refraction_outside * cos_refraction)
                                );

        fresnel_parallel = ((index_refraction_outside * cos_refraction) - (index_refraction_material * cos))
                           / ((index_refraction_outside * cos_refraction) + (index_refraction_material * cos)
                           );

        fresnel = 0.5f * (pow(fresnel_perpendicular, 2) + pow(fresnel_parallel, 2));
    }

    return fresnel;
}

Eigen::Vector3f Flyscene::calculateReflect(int level, Eigen::Vector3f &hit_point, const Eigen::Vector3f &ray_direction,
                                           const Tucano::Face &hit_face, Eigen::Vector3f &reflect_color) {

    //Eigen::Vector3f
	reflect_color = Eigen::Vector3f(0.0, 0.0, 0.0);
    Eigen::Vector3f normal = hit_face.normal.normalized();
    Eigen::Vector3f res = Eigen::Vector3f(0, 0, 0);
    Eigen::Vector3f reflect_ray = (ray_direction - 2 * ray_direction.dot(normal) * normal);
    Eigen::Vector3f reflect_destination = reflect_ray + hit_point;

    reflect_color = traceRay(level - 1, hit_point, reflect_destination);

    if(level == 0) {
        std::cout << "Calculated angle of reflection: " << acos(reflect_ray.dot(-normal)) * 180.0 / PI << std::endl;

    }

    return reflect_ray;
}

Eigen::Vector3f Flyscene::calculateRefract(int level, Eigen::Vector3f &hit_point, const Eigen::Vector3f &ray_direction,
                                           const Tucano::Face &hit_face, Eigen::Vector3f &refract_color) {

    //Eigen::Vector3f
	refract_color = Eigen::Vector3f(0.0, 0.0, 0.0);

    Eigen::Vector3f normal = hit_face.normal.normalized();
    float cos = normal.dot(ray_direction.normalized());
    float index_refraction_outside = 1;
    float index_refraction_material = materials[hit_face.material_id].getOpticalDensity();
	Eigen::Vector3f refract_ray = Eigen::Vector3f(0,0,0);

    if (cos < 0) {
        cos = -cos;
    } else {
        normal = -normal;
        swap(index_refraction_outside, index_refraction_material);
    }

    float refraction_index = index_refraction_outside / index_refraction_material;
    float square_root_term = 1 - refraction_index * refraction_index * (1 - cos * cos);

    //If the angle of refraction is positive then there is refraction. Otherwise there is total internal reflection.
    if (square_root_term > 0) {
        refract_ray = refraction_index * ray_direction + (refraction_index * cos - sqrtf(square_root_term)) * normal;
        Eigen::Vector3f refract_destination = refract_ray + hit_point;
        //Recursively call the trace function with new parameters.
        refract_color = traceRay(level - 1, hit_point, refract_destination);
    }

    if(level == 0) {
        std::cout << "Debug Ray information" << std::endl;
        std::cout << "Index of refraction: " << index_refraction_material << std::endl;
        std::cout << "Angle of incident: " << acos(cos) * 180.0 / PI << std::endl;
        std::cout << "Calculated angle of refraction: " << acos(refract_ray.dot(-normal)) * 180.0 / PI << std::endl;

    }

    return refract_ray;
}

Eigen::Vector3f Flyscene::phongShade(Eigen::Vector3f &hit_point, const Tucano::Face &hit_face) {
    Eigen::Vector3f direct_color = Eigen::Vector3f(0.0, 0.0, 0.0);
	Eigen::Vector3f ka = materials[hit_face.material_id].getAmbient();
	Eigen::Vector3f kd = materials[hit_face.material_id].getDiffuse();
	Eigen::Vector3f ks = materials[hit_face.material_id].getSpecular();
	float shininess = materials[hit_face.material_id].getShininess();

    //Direct light
	for (Light light : lights) {
		Eigen::Vector3f light_direction = light.getCenter() - hit_point;

		float light_distance = light_direction.dot(light_direction);
		Eigen::Vector3f light_intensity = Eigen::Vector3f(1, 1, 1);
		Triangle triangle;
		float object_distance = INFINITY;

		bool isInShadow = checkShadow(hit_point, light_direction, object_distance);

		if (!isInShadow) {

			// Get the interpolated normal of the hit_face, smooth shading
			Eigen::Vector3f interpolated_normal = interpolated_normals(hit_face, hit_point).normalized();
			light_direction = (light.getCenter() - hit_point).normalized();

			Eigen::Vector3f ambient = light_intensity.cwiseProduct(ka);
			Eigen::Vector3f diffuse = light_intensity.cwiseProduct(kd * max((light_direction.dot(interpolated_normal)), 0.0f));

			// Reflected light vector
			Eigen::Vector3f reflect_direction = (light_direction -
				2 * (light_direction.dot(interpolated_normal)) * interpolated_normal).normalized();

			// Vector to viewer
			Eigen::Vector3f view_direction = (flycamera.getCenter() - hit_point).normalized();
			// Compute the specular term
			//float specAngle = max(R.dot(V), 0.0f);

			float dotProduct = max(view_direction.dot(-1 * reflect_direction), 0.0f);

			Eigen::Vector3f specular = light_intensity.cwiseProduct(ks * pow(dotProduct, shininess));

			direct_color += ambient + diffuse + specular;
		}
	}

	//Spherelight
	for (SphericalLight sphericalLight : spherelights) {
		Eigen::Vector3f light_direction = sphericalLight.getCenter() - hit_point;

		float light_distance = light_direction.dot(light_direction);
		Eigen::Vector3f light_intensity = Eigen::Vector3f(0.5, 0.5, 0.5);
		Triangle triangle;
		float object_distance = INFINITY;

		bool isInShadow = checkShadow(hit_point, light_direction, object_distance);

		if (!isInShadow) {

			// Get the interpolated normal of the hit_face, smooth shading
			Eigen::Vector3f interpolated_normal = hit_face.normal.normalized();
			light_direction = (sphericalLight.getCenter() - hit_point).normalized();

			Eigen::Vector3f ambient = light_intensity.cwiseProduct(ka);
			Eigen::Vector3f diffuse = light_intensity.cwiseProduct(kd * max((light_direction.dot(interpolated_normal)), 0.0f));

			// Reflected light vector
			Eigen::Vector3f reflect_direction = (light_direction -
				2 * (light_direction.dot(interpolated_normal)) * interpolated_normal).normalized();

			// Vector to viewer
			Eigen::Vector3f view_direction = (flycamera.getCenter() - hit_point).normalized();
			// Compute the specular term
			//float specAngle = max(R.dot(V), 0.0f);

			float dotProduct = max(view_direction.dot(-1 * reflect_direction), 0.0f);

			Eigen::Vector3f specular = light_intensity.cwiseProduct(ks * pow(dotProduct, shininess));
			//std::cout << pow(specAngle, shininess) << endl;

			float intensity = softShadow(hit_point, sphericalLight);
			direct_color += ambient + (diffuse + specular)*intensity;
		}

	}
	return direct_color;
	}




	/*
	1. we maken een nieuwe ray aan hit.hitPos + dir * SURFACE_OFFSET als origin en  dir als direction.
	2.float intensity = softshadow(nieuwe ray, spherelight)
	3. Vector3f lightDir = -(hit.hitPos - spherelight.center).normalized();
	4. float angle = max(lightDir.dot(normal), 0.0f);
	5. incrementeer je var van je ambient met diffuse en specular(hier doen we diffuse * de nieuwe angle van stap 4)
*/


Eigen::Vector3f Flyscene::interpolated_normals(Tucano::Face hit_face, Eigen::Vector3f& hit_point){
	// Get the vertices of the hit_triangle
	const Eigen::Vector3f vertex_0 = (mesh.getShapeModelMatrix() * mesh.getVertex(hit_face.vertex_ids[0])).head<3>();
	const Eigen::Vector3f vertex_1 = (mesh.getShapeModelMatrix() * mesh.getVertex(hit_face.vertex_ids[1])).head<3>();
	const Eigen::Vector3f vertex_2 = (mesh.getShapeModelMatrix() * mesh.getVertex(hit_face.vertex_ids[2])).head<3>();

	// Get the spanning vectors for the hit_traingle
	Eigen::Vector3f v0v1 = vertex_1 - vertex_0;
	Eigen::Vector3f v0v2 = vertex_2 - vertex_0;

	// Get T, which is the distance of one of the vertices to the hit_point
	Eigen::Vector3f T = hit_point - vertex_0;

	// Get the normals for all three vertices
	Eigen::Vector3f normal_0 = mesh.getNormal(hit_face.vertex_ids[0]);
	Eigen::Vector3f normal_1 = mesh.getNormal(hit_face.vertex_ids[1]);
	Eigen::Vector3f normal_2 = mesh.getNormal(hit_face.vertex_ids[2]);

	// Calculate the intermediate values needed to calculate denom,
	// Source : https://computergraphics.stackexchange.com/questions/5006/how-do-i-use-barycentric-coordinates-to-interpolate-vertex-normal
	// Source : https://codeplea.com/triangular-interpolation
	// Source : https://courses.cs.washington.edu/courses/cse457/17au/assets/lectures/ray-tracing-1pp.pdf
	float d00 = v0v1.dot(v0v1);
	float d01 = v0v1.dot(v0v2);
	float d11 = v0v2.dot(v0v2);
	float d20 = T.dot(v0v1);
	float d21 = T.dot(v0v2);
	float denom = d00 * d11 - d01 * d01;

	// Calculate the barycentric coordinates
	float a = ((d11 * d20 - d01 * d21) / denom);
	float b = ((d00 * d21 - d01 * d20) / denom);
	float c = 1.0f - a - b;

	// Return the interpolated normal
	return c * normal_0 + a * normal_1 + b * normal_2;
}

void Flyscene::renderBoxes(Box box) {

	float length = abs(box.vmax.x() - box.vmin.x());
	Tucano::Shapes::Cylinder edge = Tucano::Shapes::Cylinder(0.005, length, 32, 1);
	edge.setColor(Eigen::Vector4f(1.0, 1.0, 0.0, 1.0));

	Eigen::Vector3f v1 = box.vmin;
	Eigen::Vector3f v2 = Eigen::Vector3f(box.vmin.x(), box.vmax.y(), box.vmin.z());
	Eigen::Vector3f v3 = Eigen::Vector3f(box.vmin.x(), box.vmin.y(), box.vmax.z());
	Eigen::Vector3f v4 = Eigen::Vector3f(box.vmin.x(), box.vmax.y(), box.vmax.z());
	Eigen::Vector3f v5 = Eigen::Vector3f(box.vmax.x(), box.vmin.y(), box.vmin.z());
	Eigen::Vector3f v6 = Eigen::Vector3f(box.vmax.x(), box.vmax.y(), box.vmin.z());
	Eigen::Vector3f v7 = Eigen::Vector3f(box.vmax.x(), box.vmin.y(), box.vmax.z());
	Eigen::Vector3f v8 = box.vmax;

	edge.setOriginOrientation(v1, (v2-v1).normalized());
	edge.setSize(0.002, abs(v2[1] - v1[1]));
	boxes.push_back(edge);
	edge.setOriginOrientation(v1, (v5-v1).normalized());
	edge.setSize(0.002, abs(v5[0] - v1[0]));
	boxes.push_back(edge);
	edge.setOriginOrientation(v1, (v3-v1).normalized());
	edge.setSize(0.002, abs(v3[2] - v1[2]));
	boxes.push_back(edge);
	edge.setOriginOrientation(v4, (v3-v4).normalized());
	edge.setSize(0.002, abs(v3[1] - v4[1]));
	boxes.push_back(edge);
	edge.setOriginOrientation(v4, (v2-v4).normalized());
	edge.setSize(0.002, abs(v2[2] - v4[2]));
	boxes.push_back(edge);
	edge.setOriginOrientation(v4, (v8-v4).normalized());
	edge.setSize(0.002, abs(v8[0] - v4[0]));
	boxes.push_back(edge);
	edge.setOriginOrientation(v6, (v2-v6).normalized());
	edge.setSize(0.002, abs(v2[0] - v6[0]));
	boxes.push_back(edge);
	edge.setOriginOrientation(v6, (v8-v6).normalized());
	edge.setSize(0.002, abs(v8[2] - v6[2]));
	boxes.push_back(edge);
	edge.setOriginOrientation(v6, (v5-v6).normalized());
	edge.setSize(0.002, abs(v5[1] - v6[1]));
	boxes.push_back(edge);
	edge.setOriginOrientation(v7, (v3-v7).normalized());
	edge.setSize(0.002, abs(v3[0] - v7[0]));
	boxes.push_back(edge);
	edge.setOriginOrientation(v7, (v5-v7).normalized());
	edge.setSize(0.002, abs(v5[2] - v7[2]));
	boxes.push_back(edge);
	edge.setOriginOrientation(v7, (v8-v7).normalized());
	edge.setSize(0.002, abs(v8[1] - v7[1]));
	boxes.push_back(edge);
}