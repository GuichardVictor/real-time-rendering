#include "point3.hh"
#include "vector3.hh"

std::ostream &operator<<(std::ostream &os, const Point3 &p)
{
    return os << "Point x = " << p.x_ << " ,y = " << p.y_ << " ,z = " << p.z_;
}

Point3 Point3::operator-(const Point3 &other) const{
    return Point3(this->x_ - other.x_, this->y_ -other.y_, this->z_ -other.z_);
}

Point3 Point3::operator+(const Vector3 &v) const{
    return Point3(this->x_ + v.x_, this->y_ + v.y_, this->z_ + v.z_);
}