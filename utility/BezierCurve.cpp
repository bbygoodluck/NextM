#include "../ginc.h"
#include "BezierCurve.h"
#include <cmath>

CBezierCurve2D::CBezierCurve2D() //Point start, Point control, Point end)
//	: p0(start), p1(control), p2(end)
{

}

CBezierCurve2D::~CBezierCurve2D()
{

}

//t값에 따른 곡선 위의 점 계산(0 <= t <= 1)
Point CBezierCurve2D::GetPoint(double t)
{
	//pow함수를 사용하여 (1 - t) ^ 2와 t ^ 2를 계산함
	double t1 = 1.0 - t;
	double x = pow(t1, 2) * p0.x + 2 * t1 * t * p1.x + pow(t, 2) * p2.x;
	double y = pow(t1, 2) * p0.y + 2 * t1 * t * p1.y + pow(t, 2) * p2.y;

	return Point(x, y);
}

std::vector<Point> CBezierCurve2D::GetPoints(int numPoints)
{
	std::vector<Point> points;
	//t값은 0.0 ~ 1.0 까지..
	for(int i = 0; i <= numPoints; i++)
	{
		double t = static_cast<double>(i) / numPoints;
		points.emplace_back(GetPoint(t));
	}

	return points;
}

void CBezierCurve2D::SetPoint(const Point& p0, const Point& p1)
{
	this->p0 = p0;
	this->p1 = p1;
}

Point CBezierCurve2D::calcLerp(const Point& p0, const Point& p1, double t)
{
	return Point(p0.x + t * (p1.x - p0.x), p0.y + t * (p1.y - p0.y));
}

std::vector<Point> CBezierCurve2D::calcCubicBezierSpline(int numPoints)
{
	std::vector<Point> curve;

	Point control1(p0.x + (p1.x - p0.x) / 3 , p0.y + (p1.y - p0.y) / 3);
	Point control2(p0.x + 2 * (p1.x - p0.x) / 3 , p0.y + 2 * (p1.y - p0.y) / 3);

	for(int i = 0; i < numPoints; ++i)
	{
		double t = static_cast<double>(i) / numPoints;

		//베지어 곡선 계산(3차)
		Point a = calcLerp(p0, control1, t);
		Point b = calcLerp(control1, control2, t);
		Point c = calcLerp(control2, p1, t);

		Point d = calcLerp(a, b, t);
		Point e = calcLerp(b, c, t);

		Point point = calcLerp(d, e, t);

		curve.emplace_back(point);
	}

	return curve;
}

std::vector<Point> CBezierCurve2D::calcNaturalCubicSpline(int numPoints)
{
	std::vector<Point> curve;

	std::vector<Point> controlPoints = {
		p0,
		Point((p0.x + p1.x) / 2, (p0.y + p1.y) / 2), //중간점 추가
		p1
	};

	for(size_t i = 0; i < controlPoints.size() - 1; ++i)
	{
		const Point& start = controlPoints[i];
		const Point& end   = controlPoints[i + 1];

		for(int j = 0; j < numPoints / (controlPoints.size() - 1); ++j)
		{
			double t = static_cast<double>(i) / (numPoints / (controlPoints.size() - 1));

			//간단한 3차 보간법 사용
			double h = end.x - start.x;
			double t2 = t * t;
			double t3 = t2 * t;

			//3차 다항식으로 보간
			double x = start.x + t * h;
			double y = start.y * ( 2 * t3 - 3 * t2 + 1);

			curve.emplace_back(Point(x, y));
		}
	}

	curve.emplace_back(controlPoints.back());
	return curve;
}
