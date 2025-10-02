#ifndef BEZIERCURVE_H_INCLUDED
#define BEZIERCURVE_H_INCLUDED

struct Point {
	double x, y;
	Point (double x = 0, double y = 0) : x(x), y(y) {}
};

class CBezierCurve2D
{
	Point p0, p1, p2;

public:
	CBezierCurve2D();//Point start, Point control, Point end);
	~CBezierCurve2D();


public:
	void SetPoint(const Point& p0, const Point& p1);

	Point GetPoint(double t);
	std::vector<Point> GetPoints(int numPoints);

	//두 점 사이의 3차 베지어곡선 계산
	//두 점 사이에 제어점 2개를 추가하여 곡선 생성
	std::vector<Point> calcCubicBezierSpline(int numPoints = 100);
	//자연 3차 스플라인 곡선(Natural Cubic Spline)
	//이 함수는 2개 이상의 점이 필요하므로 여기서는 두 점 사이에 추가 보감점 생성
	std::vector<Point> calcNaturalCubicSpline(int numPoints = 100);

	//선형 보간법(Linear Interpolation);
	Point calcLerp(const Point& p0, const Point& p1, double t);
};
#endif // BEZIERCURVE_H_INCLUDED
