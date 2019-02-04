#include <iostream>
#include "Physics.h"
#include "ImageUtil.h"
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <time.h>

using namespace std;
const int nparticles = 100000;
const double GM = 10.0;

time_t start;

vector<PointMass> p;

QuadNode *Tree;

/* Initialize the array p.
Preconditions: none*/
void initializeParticles();

/* Initialize the quadtree Tree.
Precondition: Tree does not point to a valid QuadNode object. */
void initializeTree();

/* Rebuilds the quadtree Tree.
Precondition: Tree does point to a valid QuadNode object. */
void rebuildQuadtree();

/* Zeros the acceleration vectors, rebuilds the quadtree, and calculates all forces in O(n log(n)) time.
Precondition: Tree has been initialized, p has been initialized. */
void calculateAccel();

//Steps v and then x for all particles in the leapfrog style.
void leapstep(double dt);

//return a random number between 0 and 1.
double unitRand() {
    return double(rand()) / RAND_MAX;
}

//Some string manipulation functions for saving files. pad_int(1234,5) returns "01234". 
string pad_int(int arg, int padcount) {
    stringstream ss;
    ss << setfill('0') << setw(padcount) << arg;
    return ss.str();
}

//Returns a file name in the form of "prefix00###suffix". For example "image0032.bmp"
string getFilename(string prefix, int num, int padcount, string suffix) {
    return prefix + pad_int(num, padcount) + suffix;
}


int main() {
    start = clock();
    initializeParticles();
    initializeTree();

    //"k" labels the frame number. We output 30 frames from 30 timesteps.
    for (int k = 0; k < 30; k++) {
        cout << "k="<<k << endl;

        //fill p[i].acceleration. This is where all the heavy lifting happens.
        calculateAccel();
        //update p[i].velocity, then update p[i].position.
        leapstep(0.01);



        cout << "\tBuilding Image. t="<<(double(clock()-start)/CLOCKS_PER_SEC)<< endl;
        /*
           I build the image in a very particular way which I think provides the best results:
           1. create an array of doubles (DoubleImage)
           2. Do some coordinate transformation ("world coordinates" -> "image coordinates")
           and count how many particles are on each pixel (doublepts.increase(x,y,1))
           3. Turn this count into an actual RGB bitmap through some arbitrary color function. I chose mine to be
           logarithmic like the human eye and bluish/greenish, but more complicated functions are fun. Try things
           like "(cos(scalar)+1)/2.0" for more fun coloring.
         */
        DoubleImage doublepts(1000, 1000); // Just a fancy wrapper for a 2D double array
        Image pic(1000, 1000); // an actual RGB image
        for (size_t j = 0; j < p.size(); j++) {
            int x = int(p[j].position.x*1.2)-600+500; //ensure the center of the simulation domain, position=(500,500), is at the center of the image.
            int y = int(p[j].position.y*1.2)-600+500;
            doublepts.increase(x, y, 100);
        }
        //Turn the scalar array into rgb values.
        for (int x = 0; x < 1000; x++) {
            for (int y = 0; y < 1000; y++) {
                //Human eye sees brightness logarithmically, so take a log.
                double scalar1 = log(1 + 3 * doublepts.get(x, y)) / 20.0;
                //combine everything into a nice color.
                pic.put(x, y, floatToRGB(scalar1, 0 + scalar1 * 1.5, scalar1*1.7));
            }
        }
        //actually save the image
        pic.save(getFilename("out/image", k, 5, ".bmp"));
    }

}



/* Initialize the array p.
Preconditions: none*/
void initializeParticles() {
    p = vector<PointMass>();
    p.reserve(nparticles);
    cout << "Initializing Particles" << endl;
    //Initialize everything with random initial condition
    //NOTE: x.position is at t=0. x.velocity is at t=-dt/2! (Quirk of leapfrog)
    for (int i = 0; i < nparticles; i++) {
        PointMass x;
        x.position = Vector2(unitRand() * 1000, unitRand() * 1000);
        x.velocity = Vector2(0, 0);
        x.gravforce = Vector2(0,0);
        x.mass = 1.0;
        p.push_back(x);
    }
}

/* Initialize the quadtree Tree.
Precondition: Tree does not point to a valid Tree object. */
void initializeTree() {
    Tree = nullptr;
    Tree = new QuadNode(Vector2(-1, 1), Vector2(1, -1));
}

/* Rebuilds the quadtree object Tree.
Precondition: Tree points to a valid QuadNode object. */
void rebuildQuadtree() {
    //Tree deletion calls destructor for all tree nodes recursively, so this can take a bit
    cout << "\tDeleting Tree. t=" << (double(clock() - start) / CLOCKS_PER_SEC) << endl;
    delete Tree;
    cout << "\tBuilding Tree. t=" << (double(clock() - start) / CLOCKS_PER_SEC)<< endl;

    //Calculate the bounding box
    double left = 10000; double right = -10000;
    double top = -10000; double bottom = 10000;
    for (size_t n = 0; n < p.size(); n++) {
        if (p[n].position.x < left)
            left = p[n].position.x;
        if (p[n].position.y > top)
            top = p[n].position.y;
        if (p[n].position.x > right)
            right = p[n].position.x;
        if (p[n].position.y < bottom)
            bottom = p[n].position.y;
    }

    //Create the top-level quadtree node with the correct bounds. Add 1 unit of padding in every direction to the bounding box
    Tree = new QuadNode(Vector2(left - 1, top + 1), Vector2(right + 1, bottom - 1));

    //Add a bunch of points to the quadtree.
    for (size_t n = 0; n < p.size(); n++)
        Tree->addPointMass(&p[n]);

}

/* Zeros the acceleration vectors, rebuilds the quadtree, and calculates all forces in O(n log(n)) time.
Precondition: Tree has been initialized, p has been initialized. */
void calculateAccel() {
    rebuildQuadtree();
    cout << "\tCalculating Accelerations. t=" << (double(clock() - start) / CLOCKS_PER_SEC) << endl;
    for (size_t n = 0; n < p.size(); n++) {
        p[n].gravforce = Vector2(0, 0);
        Tree->calculateForces(&p[n], GM);
    }
}

//Steps v and then x for all particles in the leapfrog style.
void leapstep(double dt) {
    cout << "\tLeapfrogging. t=" << (double(clock() - start) / CLOCKS_PER_SEC) << endl;
    for (size_t n = 0; n < p.size(); n++) {
        p[n].velocity += dt * p[n].gravforce;
        p[n].position += dt * p[n].velocity;
    }
}
