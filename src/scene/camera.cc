#include "vector3.hh"
#include "camera.hh"

void Camera::initPoints(int width, int height) {
    Vector3 v = Vector3(this->center_, this->objective_);
    v = v.normalize();
    Vector3 right = crossProduct(v, this->up_);
    right = right.normalize();
    v = v * zDist_;
    Point3 centerImage = this->center_ + v;
    float wlength = this->zDist_ * tanf(openFieldX_ / 2.) * 2.0;
    float hlength = this->zDist_ * tanf(openFieldY_ / 2.) * 2.0;
    Point3 hrCorner = centerImage + right * (-wlength / 2.0) + this->up_ * (hlength / 2.0);

    float padx = wlength / width;
    float pady = hlength / height;
    for(int j = 0; j < height; j++)
    {
        for(int i = 0 ; i < width; i++)
        {
            Point3 toAdd = Point3(hrCorner.getX(), hrCorner.getY(), hrCorner.getZ());
            toAdd = toAdd + right * ((padx * (float) i));
            toAdd = toAdd + (this->up_  * -1. * (pady  * (float) j));
            this->imagePlan.push_back(toAdd);
        }
    }
}

Point3 Camera::projectPoint(const Point3& p) const
{
    Point3 res = Point3(0,0,0);
    Vector3 v = Vector3(this->center_, p);
    Vector3 c = Vector3(this->center_, this->imagePlan[(HEIGHT / 2 - 1) * WIDTH + WIDTH / 2]);
    c = c.normalize();
    Vector3 nv = v.normalize();
    float angle = dot(c, nv);
    float dist = c.norm() * angle;
    res = res + nv * dist;
    return res;
}

const Point3 &Camera::getCenter() const {
    return center_;
}
