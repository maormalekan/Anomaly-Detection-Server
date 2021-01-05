
#ifndef MINCIRCLE_H_
#define MINCIRCLE_H_

#include <vector>
#include "anomaly_detection_util.h"

using namespace std;


class Circle{
public:
	Point center;
	float radius;
	Circle(Point c,float r):center(c),radius(r){}
};
// --------------------------------------

float dist(Point a, Point b);

Circle from2points(Point a,Point b);

Circle from3Points(Point a, Point b, Point c);

Circle trivial(vector<Point>& P);


/*
algorithm welzl
    input: Finite sets P and R of points in the plane |R|<= 3.
    output: Minimal disk enclosing P with R on the boundary.

    if P is empty or |R| = 3 then
        return trivial(R)
    choose p in P (randomly and uniformly)
    D := welzl(P - { p }, R)
    if p is in D then
        return D

    return welzl(P - { p }, R U { p })
 */


Circle welzl(Point** P,vector<Point> R, size_t n);

Circle findMinCircle(Point** points,size_t size);

#endif /* MINCIRCLE_H_ */
