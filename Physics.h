
// I needed these two for my 10,000,000 particle run, but it's compiler dependent.
// #pragma comment(linker, "/STACK:2000000")
// #pragma comment(linker, "/HEAP:6000000")
#ifndef QUADNODE_H
#define QUADNODE_H

#include "Vector2.h"

/* struct to more easily store point mass positions/velocities */
struct PointMass
{
    Vector2 position;
    Vector2 velocity;
    Vector2 gravforce;
    double mass;
};

/** QuadNode. Quadtree node class.
  The QuadNode class takes in PointMass* pointers and creates a quadtree data
  structure with exactly one PointMass* per node.

  "tl" and "br" are the top-left and bottom-right location vectors of the quadtree node, respectively.

  addPointMass puts a given pointmass into the quadtree, possibly creating new subnodes in the process.

  calculateForces recursively calls any substructure, and modifies the passed in PointMass by calculating/
  adding to the PointMass::force vector based on positions/what-not.
 */
class QuadNode
{
    Vector2 tl; //top-left
    Vector2 br; //bottom-right
    QuadNode *subnodes[2][2]; //array of subnodes. Either all are null or none are null.
    PointMass *val; //The pointmass stored in the array. Possibly null.
    Vector2 masspos; //The sum of the masses in this node and/or any subnodes, times the positions. (mass1*x1+mass2*x2+mass3*x3+...)
    double mass; //the sum of masses (mass1+mass2+...)
    //(center of mass = masspos/mass )


    void createSubNodes(); //fills the subnodes array.
    public:
    ///Creates a new QuadNode. tl is top-left, br is bottom-right. Adding PointMasses outside these bounds will cause a crash!
    QuadNode(Vector2 tl, Vector2 br);
    ~QuadNode();
    ///Adds a point mass to the quadtree.
    void addPointMass(PointMass *arg);
    /**Uses Barnes-Hut calculation to approximate the gravitational (and spring) forces on a passed in particle, according to the
      passed in physical constants (see PhysicsHandler's constructor)*/
    void calculateForces(PointMass *arg, const double& G) const;
};
#endif
