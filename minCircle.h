// 31832248	32120296

#ifndef MINCIRCLE_H_
#define MINCIRCLE_H_

#include <vector>
#include "anomaly_detection_util.h"

using namespace std;

// ------------ DO NOT CHANGE -----------
class Circle {
public:
  Point center;
  float radius;
  Circle(Point c, float r) : center(c), radius(r) {}
};
// --------------------------------------

/**
 * @brief return the circle that two points are generate.
 * we know that two points can generate only one unique circle.
 *
 * @param p1 - point
 * @param p2 - point
 * @return the circle that the two points are generate.
 */
Circle getCircleFrom2(const Point &p1, const Point &p2);

/**
 * @brief return the circle that three points are generate.
 * we know that three points can generate only one unique circle.
 *
 * @param p1 point
 * @param p2 - point
 * @param p3 - point
 * @return the circle that the three points are generate.
 *
 */
Circle getCircleFrom3(const Point &p1, const Point &p2, const Point &p3);

/**
 * @brief the function gets a circle and a point, and checks if the point is in
 * the circle.
 *
 * @param c - circle
 * @param p - point
 * @return the function returns true if the point is in the circle. otherwise,
 * it returns false.
 */
bool pIsInside(const Circle &c, const Point &p);

/**
 * @brief the function gets vector of points that have to be on the edge of the
 * circle, and returns the minimum circle that includes them.
 *
 * @param vec - vector of points
 * @return the minimum circle that these points are on the edge of it.
 */
Circle minCircle(vector<Point> vec);

/**
 * @brief the recursive function.
 * the function gets array of points, the size of the array,
 * and a vector of points that have to be on the edge of the circle.
 * the function returns the minimun circle that encloses all the points.
 *
 * @param points - the points that have to be in the circle.
 * @param size - the size of the array of points.
 * @param vec - the points that have to be on the edge of the circle.
 * @return the minimun circle that encloses all the points.
 */
Circle helper(Point **points, size_t size, vector<Point> vec);

/**
 * @brief the main function that calls the recursive function.
 *
 * @param points - an array points that we want to find a minimum enclosing circle
 * for them.
 * @param size - the size of the array points.
 * @return
 */
Circle findMinCircle(Point **points, size_t size);

#endif /* MINCIRCLE_H_ */
