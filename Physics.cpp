#include "Physics.h"

QuadNode::QuadNode(Vector2 tl, Vector2 br) : tl(tl),br(br),val(nullptr),masspos(0,0),mass(0)
{
    for(int n=0;n<2;n++)
        for(int m=0;m<2;m++)
            subnodes[n][m]=nullptr;
}

QuadNode::~QuadNode() //recursive destructor
{
    for(int n=0;n<2;n++)
        for(int m=0;m<2;m++)
            if(subnodes[n][m])
                delete subnodes[n][m];
}

void QuadNode::createSubNodes() //private function for constructing the four subnodes.
{
    Vector2 mid=(tl+br)*.5;
    Vector2 midleft=Vector2(tl.x,mid.y);
    Vector2 midtop=Vector2(mid.x,tl.y);
    Vector2 midright=Vector2(br.x,mid.y);
    Vector2 midbottom=Vector2(mid.x,br.y);

    subnodes[0][0]=new QuadNode(tl,mid);
    subnodes[0][1]=new QuadNode(midtop,midright);
    subnodes[1][0]=new QuadNode(midleft,midbottom);
    subnodes[1][1]=new QuadNode(mid,br);
}

//The magic function that builds the quadtree 
void QuadNode::addPointMass(PointMass *arg)
{
    if(val==nullptr) { //if this node does not store a single particle
        if(subnodes[0][0]==nullptr)//no current mass & no subnodes
        {
            val=arg; //store the mass at this node
        } else { //else we have subnodes: we should pass the mass along so that the subnodes can deal with the particle.
            //update COM and total M.
            masspos+=arg->position*arg->mass;
            mass+=arg->mass;
            Vector2 mid=(tl+br)*.5;
            int x=arg->position.x>mid.x; //  (int)true==1, (int)false==0
            int y=arg->position.y<mid.y;
            subnodes[y][x]->addPointMass(arg);
        }
    }
    else {
        //Else this node stores a single particle and no subnodes. We must create four subnodes and decide which subnode to add 
        //each particle (arg and val) to.

        //If the particles are right next to each other, just give up. In a scientific quality algorithm this might be bad.
        if((fabs(val->position.x-arg->position.x )<0.0001) && (fabs(val->position.y - arg->position.y)<0.0001))
            return;
        PointMass *valtemp=val;
        val=nullptr;
        createSubNodes();
        masspos=Vector2(0,0);
        mass=0;
        addPointMass(valtemp);
        addPointMass(arg);
    }
}

//Add to the PointMass's acceleration vector.
//This is the force calculation with the algorithm of Barnes and Hut.
void QuadNode::calculateForces(PointMass *arg, const double& G) const
{
    if (subnodes[0][0] != nullptr)
    {
        // we get here iff this node has subnodes
        Vector2 pt = masspos / mass;  //center of mass position
        Vector2 diff = pt - arg->position; //vector to the center of mass
        double d = diff.x*diff.x + diff.y*diff.y;
        register double diffw = (tl.x - br.x);

        if (diffw*diffw >= d * .7) //if the size of the node divided by the distance is large, then recurse
        {
            for (int n = 0; n<2; n++)
                for (int m = 0; m<2; m++)
                    subnodes[m][n]->calculateForces(arg, G);
        }
        else //else just calculate the force to the center of mass
        {
            Vector2 diff = pt - arg->position;
            double d = diff.x*diff.x + diff.y*diff.y;
            double force = G * arg->mass*mass / d;
            d = sqrt(d);
            if (d>0.000001)
                arg->gravforce += diff * force / d;
        }
    }
    else if (val != nullptr)
    {
        //we get here iff this node does not have subnodes and it does have a particle.
        Vector2 diff = val->position - arg->position;
        double d = diff.x*diff.x + diff.y*diff.y;
        double force = G * arg->mass*val->mass / d;
        d = sqrt(d);
        if (d>0.5)
            arg->gravforce += diff * force / d;
    }
}
