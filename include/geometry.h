/*********************************************************************
 * Software License Agreement (BSD License)
 *
 *  Copyright (c) 2018, Rice University
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *   * Neither the name of Rice University nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *********************************************************************/

/* Author: Zachary Kingston */
// See https://github.com/KavrakiLab/robowflex/blob/master/robowflex_library/include/robowflex_library/geometry.h

#ifndef GEOMETRY_
#define GEOMETRY_

#include <Eigen/Core>
#include <Eigen/Geometry>

#include <geometric_shapes/shapes.h>
#include <geometric_shapes/bodies.h>

#include <shape_msgs/msg/solid_primitive.hpp>
#include <shape_msgs/msg/mesh.hpp>

#include <moveit/robot_model/aabb.h>

//#include <robowflex_library/adapter.h>
//#include <robowflex_library/class_forward.h>

class Geometry;
using GeometryPtr = std::shared_ptr<Geometry>;
using GeometryConstPtr = std::shared_ptr<const Geometry>;

/** \cond IGNORE */
// ROBOWFLEX_CLASS_FORWARD(Geometry);
/** \endcond */

/** \class robowflex::GeometryPtr
    \brief A shared pointer wrapper for robowflex::Geometry. */

/** \class robowflex::GeometryConstPtr
    \brief A const shared pointer wrapper for robowflex::Geometry. */

/** \brief A class that manages both solid and mesh geometry for various parts of the motion planning
 *  system.
 */
class Geometry
{
public:
  /** \brief Supported shape types.
   */
  class ShapeType
  {
  public:
    enum Type
    {
      BOX = 0,       ///< Solid primitive box. Uses three dimensions (x, y, z).
      SPHERE = 1,    ///< Solid primitive sphere. Uses one dimension (radius).
      CYLINDER = 2,  ///< Solid primitive cylinder. Uses two dimensions (height, radius).
      CONE = 3,      ///< Solid primitive cone. Uses two dimensions (height, radius).
      MESH = 4       ///< Mesh. Dimensions scale along x, y, z.
    };

    static const unsigned int MAX;                  ///< Maximum value of ShapeType.
    static const std::vector<std::string> STRINGS;  ///< Mapping of ShapeType to string.

    static Type toType(const std::string& str);     ///< Converts a string into a ShapeType.
    static const std::string& toString(Type type);  ///< Converts a ShapeType to its string.
  };

  /** \brief Create a sphere.
   *  \param[in] radius The radius of the sphere.
   *  \return The created sphere.
   */
  static GeometryPtr makeSphere(double radius);

  /** \brief Create a box.
   *  \param[in] x The box's x dimension.
   *  \param[in] y The boy's y dimension.
   *  \param[in] z The boz's z dimension.
   *  \return The created box.
   */
  static GeometryPtr makeBox(double x, double y, double z);

  /** \brief Create a box.
   *  \param[in] dimensions The XYZ dimensions of the box.
   *  \return The created box.
   */
  static GeometryPtr makeBox(const Eigen::Vector3d& dimensions);

  /** \brief Create a cylinder.
   *  \param[in] radius The radius of the cylinder.
   *  \param[in] length The length of the cylinder.
   *  \return The created cylinder.
   */
  static GeometryPtr makeCylinder(double radius, double length);

  /** \brief Create a cone.
   *  \param[in] radius The radius of the base of the cone.
   *  \param[in] length The height of the cone.
   *  \return The created cone.
   */
  static GeometryPtr makeCone(double radius, double length);

  /** \brief Create a solid primitive.
   *  \param[in] msg The solid primitive message to load.
   *  \return The created shape.
   */
  static GeometryPtr makeSolidPrimitive(const shape_msgs::msg::SolidPrimitive& msg);

  /** \brief Create a mesh from resource file.
   *  \param[in] resource The resource to load for the mesh.
   *  \param[in] scale The scale of the mesh.
   *  \return The created mesh.
   */
  static GeometryPtr makeMesh(const std::string& resource, const Eigen::Vector3d& scale = { 1, 1, 1 });

  /** \brief Create a mesh from triangles represented as vertices .
   *  \param[in] vertices The vertices that will create the mesh.
   *  \return The created mesh.
   */
  static GeometryPtr makeMesh(const EigenSTL::vector_Vector3d& vertices);

  /** \brief Constructor.
   *  Builds and loads the specified geometry.
   *  \param[in] type Type of the geometry to create.
   *  \param[in] dimensions Dimensions of the geometry to load.
   *  \param[in] resource If \a type is ShapeType::MESH, then resource or vertices must be specified
   *  \param[in] vertices List of vertices that form the mesh.
   * as the mesh file to load.
   */
  Geometry(ShapeType::Type type, const Eigen::Vector3d& dimensions, const std::string& resource = "",
           const EigenSTL::vector_Vector3d& vertices = EigenSTL::vector_Vector3d{});

  /** \brief Constructor.
   *  Builds and loads the specified geometry from a MoveIt shape.
   *  \param[in] shape Shape to construct geometry from.
   */
  Geometry(const shapes::Shape& shape);

  /** \brief Constructor.
   *  Builds and loads the specified geometry from a MoveIt shape message.
   *  \param[in] msg Shape message to construct geometry from.
   */
  Geometry(const shape_msgs::msg::SolidPrimitive& msg);

  /** \brief Constructor.
   *  Builds and loads the specified geometry from a MoveIt shape message.
   *  \param[in] msg Shape message to construct mesh geometry from.
   */
  Geometry(const shape_msgs::msg::Mesh& msg);

  // non-copyable
  Geometry(const Geometry&) = delete;
  Geometry& operator=(const Geometry&) = delete;

  /** \brief Checks if the geometry contains a \a point.
   *  \param[in] point The point to check, in the geometry's frame.
   *  \return True if the geometry contains the point, false otherwise.
   */
  bool contains(const Eigen::Vector3d& point) const;

  /** \brief Tries to sample a point in the geometry.
   *  \param[in] attempts Number of attempts to sample.
   *  \return The sampled point and true, or the 0 vector and false on failure.
   */
  std::pair<bool, Eigen::Vector3d> sample(const unsigned int attempts = 50) const;

  /** \brief Checks if the geometry is a mesh geometry.
   *  \return True if the \a type_ is a mesh (ShapeType::MESH).
   */
  bool isMesh() const;

  /** \brief Gets the message form of solid primitive geometry (all but ShapeType::MESH).
   *  \return The message.
   */
  const shape_msgs::msg::SolidPrimitive getSolidMsg() const;

  /** \brief Gets the message form of mesh geometry.
   *  \return The message.
   */
  const shape_msgs::msg::Mesh getMeshMsg() const;

  /** \brief Gets the underlying shape.
   *  \return The shape.
   */
  const shapes::ShapePtr& getShape() const;

  /** \brief Gets the underlying body.
   *  \return The body.
   */
  const bodies::BodyPtr& getBody() const;

  /** \brief Gets the type of the geometry.
   *  \return The type of geometry.
   */
  ShapeType::Type getType() const;

  /** \brief Gets the mesh resource of the geometry.
   *  \return The mesh resource of geometry.
   */
  const std::string& getResource() const;

  /** \brief Gets the Vertices of the primitive.
   *  \return The mesh resource of geometry.
   */
  const EigenSTL::vector_Vector3d& getVertices() const;

  /** \brief Gets the dimensions of the geometry.
   *  \return The dimensions of geometry.
   */
  const Eigen::Vector3d& getDimensions() const;

  /** \brief Compute the AABB (axis-aligned bounding box) of the geometry at a given pose.
   *  \param[in] pose Pose to compute AABB of geometry at.
   *  \return The AABB.
   */
  // Eigen::AlignedBox3d getAABB(const RobotPose &pose = RobotPose::Identity()) const;

private:
  /** \brief Loads a shape from the set \a type_ and \a dimensions_, and \a resource_ if a
   * mesh. \return A pointer to a newly allocated shape.
   */
  shapes::Shape* loadShape() const;

  /** \brief Loads a body from the loaded \a shape_.
   *  \return A pointer to a newly allocated body.
   */
  bodies::Body* loadBody() const;

  ShapeType::Type type_{ ShapeType::Type::BOX };           ///< Geometry Type.
  Eigen::Vector3d dimensions_{ Eigen::Vector3d::Ones() };  ///< Dimensions to scale geometry.
  EigenSTL::vector_Vector3d vertices_{ {} };               ///< Vertices of the primitive
  std::string resource_{ "" };                             ///< Resource locator for MESH types.
  shapes::ShapePtr shape_{ nullptr };                      ///< Loaded shape.
  bodies::BodyPtr body_{ nullptr };                        ///< Body operation.
};
//}  // namespace robowflex

#endif
