#include <cmath>
#include <string>
#include <vector>

const double PI = 3.141592653589793;
struct Point {
  double x = 0;
  double y = 0;
  Point(){};
  Point operator=(const Point& other) {
    if (this == &other) {
      return *this;
    }
    x = other.x;
    y = other.y;
    return *this;
  }
  Point(double l1, double l2) : x(l1), y(l2) {}
  Point(const Point& ptr1, const Point& ptr2) {
    x = ptr2.x - ptr1.x;
    y = ptr2.y - ptr1.y;
  }
  double Length() const {
    return sqrt(x * x + y * y);
  }
  double ratio() const {
    return y / x;
  }

  Point& operator+=(const Point& other) {
    x += other.x;
    y += other.y;
    return *this;
  }

  Point& operator-=(const Point& other) {
    x -= other.x;
    y -= other.y;
    return *this;
  }

  Point& operator*=(double ratio) {
    x *= ratio;
    y *= ratio;
    return *this;
  }
  Point& operator/=(double ratio) {
    return (*this) *= (1.0 / ratio);
  }

  Point operator+(const Point& other) const {
    Point res = *this;
    return res += other;
  }
  Point operator-(const Point& other) const {
    Point res = *this;
    return res -= other;
  }
  Point operator*(double ratio) const {
    Point res = *this;
    return res *= ratio;
  }
  Point operator/(double ratio) const {
    Point res = *this;
    return res /= ratio;
  }
};

bool Same(const Point& p1, const Point& p2) {
  return round(p1.x * 1e9) == round(p2.x * 1e9) && round(p1.y * 1e9) == round(p2.y * 1e9);
}


double Scalar(const Point& point1, const Point& point2) {
  return point1.x * point2.x + point1.y * point2.y;
}

double Vec(const Point& p1, const Point& p2) {
  return p1.x * p2.y - p1.y * p2.x;
}

double Sin(const Point& p1, const Point& p2) {
  return Vec(p1, p2) / (p1.Length() * p2.Length());
}

double Cos(const Point& p1, const Point& p2) {
  return Scalar(p1, p2) / (p1.Length() * p2.Length());
}

bool operator==(const Point& poi1, const Point& poi2) {
  return Same(poi1, poi2);
}

bool operator!=(const Point& poi1, const Point& poi2) {
  return !(poi1 == poi2);
}

double len(const Point& poi1, const Point& poi2) {
  double fir = std::pow((poi1.x - poi2.x), 2) + std::pow((poi1.y - poi2.y), 2);
  return std::sqrt(fir);
}

Point middle(const Point& poi1, const Point& poi2) {
  Point result;
  result.x = (poi1.x + poi2.x) / 2;
  result.y = (poi1.y + poi2.y) / 2;
  return result;
}

Point Standard(const Point& p1) {
  return p1 / p1.Length();
}


Point Orthogonal(const Point& p1) {
  return Point(-p1.y, p1.x);
}

Point Bisector(const Point& p1, const Point& p2) {
  return Standard(p1) + Standard(p2);
}






class Line {
  private:
  Point direction_;
  Point begin_;

  public:
  Line(const Point& poi1, const Point& poi2) {
    begin_ = poi1;
    direction_ = Point(poi1, poi2);
  }
  Line(double ratio, double shift) {
    begin_ = Point(0, shift);
    direction_ = Point(1, ratio);
  }
  Line(const Point& point, double ratio) {
    begin_ = point;
    direction_ = Point(1, ratio);
  }
  double shift() const {
    return begin_.y - (begin_.x * direction_.ratio());
  }
  bool include(const Point& point) const {
    if (Point(begin_, point) == direction_) {
      return true;
    }
    if (Point(begin_, point) == -direction_) {
      return true;
    }
    return false;
  }
  friend bool operator==(const Line& line1, const Line& line2);
  friend Point intersection(const Line& line1, const Line& line2);
};

bool operator==(const Line& line1, const Line& line2) {
  if ((line1.direction_ == line2.direction_) or (line1.direction_ == -line2.direction_)) {
    return line1.include(line2.begin_);
  }
  return false;
}

bool operator!=(const Line& line1, const Line& line2) {
  return !(line1 == line2);
}

Point intersection(const Line& line1, const Line& line2) {
  Point result;
  result.x = (line1.direction_.ratio() - line2.direction_.ratio()) / (line2.shift() - line1.shift());
  result.y = result.x * line1.direction_.ratio() + line1.shift();
}

class Shape {
  virtual double perimeter(){};
  virtual double area(){};
  virtual bool operator==(const Shape& another){};
  virtual bool isCongruentTo(const Shape& another){};
  virtual bool isSimilarTo(const Shape& another){};
  virtual bool containsPoint(const Point& point){};
};

class Polygon : protected Shape {
  protected:
  std::vector<Point> vertices_;

  public:
  Polygon(const std::vector<Point>& vertices) {
    for (auto& element: vertices) {
      vertices_.push_back(element);
    }
  }
  Polygon(const std::string& line) {
    //надо реализовать
  }
  size_t verticesCount() const {
    return vertices_.size();
  }
  std::vector<Point> getVertices() const {
    return vertices_;
  }
  bool isConvex() {
    //надо реализовать
  }
};

class Ellipse : protected Shape {
  protected:
  std::pair<Point, Point> focuses_;
  double a = 0;
  double b = 0;
  double c = 0;

  public:
  Ellipse(const std::pair<Point, Point>& focuses, double sum_length) {
    a = sum_length / 2;
    c = len(focuses.first, focuses.second);
    b = std::sqrt(a * a - c * c);
    if (focuses.first.x > focuses.second.x) {
      focuses_ = {focuses.second, focuses.first};
      return;
    }
    focuses_ = focuses;
  }
  Ellipse() {
  }
  std::pair<Point, Point> focuses() {
    return focuses_;
  }
  double eccentricity() const {
    return c / a;
  }
  Point center() const {
    Point result;
    result.x = (focuses_.first.x + focuses_.second.x) / 2;
    result.y = (focuses_.first.y + focuses_.second.y) / 2;
    return result;
  }
  double perimeter() const {
    return PI * (3 * (a + b) - std::sqrt((3 * a + b) * (3 * b + a)));
  }
  double area() const {
    return PI * a * b;
  }
  bool containsPoint(const Point& point) const {
    return (2 * a >= (len(point, focuses_.first) + len(point, focuses_.second)));
  }
};

class Circle : private Ellipse {
  private:
  Point center_ = focuses_.first;
  double radius_ = a;

  public:
  Circle(const Point& center, const double& radius) {
    center_ = center;
    radius_ = radius;
  }
  double radius() const {
    return radius_;
  }
};

class Rectangle : protected Polygon {
  protected:
  public:
  Point center() const {
    return middle(vertices_[0], vertices_[2]);
  }
  std::pair<Line, Line> diagonals() {
    Line line1 = Line(vertices_[0], vertices_[2]);
    Line line2 = Line(vertices_[1], vertices_[3]);
    return {line1, line2};
  }
};