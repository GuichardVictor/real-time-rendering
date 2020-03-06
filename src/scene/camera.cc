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

const Point3 &Camera::getCenter() const {
    return center_;
}
