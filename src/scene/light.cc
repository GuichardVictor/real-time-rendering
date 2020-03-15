#include "light.hh"

void DirectionalLight::initBuffer()
{
    Vector3 v = direction;
    v = v.normalize();


    Vector3 right  = crossProduct(Vector3(0,1,0), direction);

    if(right.x_ == 0 && right.y_ == 0 && right.z_ == 0)
    {
        right = crossProduct(Vector3(0,0,1), direction);
    }
    right = right.normalize();

    Vector3 up = crossProduct(right, direction);
    Point3 centerImage = center + v;
    float wlength = this->zDist * tanf(angle / 2.) * 2.0;
    float hlength = this->zDist * tanf(angle / 2.) * 2.0;
    Point3 hrCorner = centerImage + right * (-wlength / 2.0) + up * (hlength / 2.0);

    float padx = wlength / width;
    float pady = hlength / height;
    for(int j = 0; j < height; j++)
    {
        for(int i = 0 ; i < width; i++)
        {
            Point3 toAdd = Point3(hrCorner.x_, hrCorner.y_, hrCorner.z_);
            toAdd = toAdd + right * ((padx * (float) i));
            toAdd = toAdd + (up  * -1. * (pady  * (float) j));
            this->imagePlan.push_back(toAdd);
        }
    }
    depthBuffer = std::vector<float>(height * width, std::numeric_limits<float>::max());
}

Point3 DirectionalLight::projectPoint(const Point3& p) const
{
    Vector3 v = Vector3(this->center, p);
    Vector3 c = direction;
    Vector3 nv = v.normalize();
    float cosAngle = dot(c.normalize(), nv);
    if(cosAngle <= 0)
    {
        Point3 newP = p + c.normalize() * -2 * cosAngle * v.norm() + c * 1;
        v = Vector3(this->center, newP);
        nv = v.normalize();
        cosAngle = dot(c.normalize(), nv);
    }
    float dist = c.norm() / cosAngle;
    auto res = this->center + nv * dist;
    return res;
}

void DirectionalLight::updateBuffer(Triangle& tr)
{
    auto pa = projectPoint(tr.a);
    auto pb = projectPoint(tr.b);
    auto pc = projectPoint(tr.c);

    auto forward = direction.normalize();

    auto vec = Vector3(center, tr.a);
    float distA = vec.norm() * dot(vec.normalize(), forward);
    vec = Vector3(center, tr.b);
    float distB = vec.norm() * dot(vec.normalize(), forward);
    vec = Vector3(center, tr.c);
    float distC = vec.norm() * dot(vec.normalize(), forward);

    auto za = distA;
    auto zb = distB;
    auto zc = distC;
   
    auto coordA = computePointCoordinate(pa);
    auto coordB = computePointCoordinate(pb);
    auto coordC = computePointCoordinate(pc);

    auto f = std::pair<Point2, float>(coordA, za);
    auto s = std::pair<Point2, float>(coordB, zb);
    auto t = std::pair<Point2, float>(coordC, zc);

    if(f.first.y > s.first.y)
        std::swap(f, s);
    if(f.first.y > t.first.y)
        std::swap(f, t);
    if(s.first.y > t.first.y)
        std::swap(s, t);

    if (f.first.y == s.first.y && f.first.x > s.first.x)
        std::swap(f,s);
    if (s.first.y == t.first.y && s.first.x > t.first.x)
        std::swap(s, t);

    Vector3 v1 = Vector3({(float)f.first.x, (float)f.first.y, f.second},
     {(float)s.first.x, (float)s.first.y, s.second});
    Vector3 v2 = Vector3({(float)f.first.x, (float)f.first.y, f.second},
     {(float)t.first.x, (float)t.first.y, t.second});

    Vector3 cross = crossProduct(v1, v2);

    PlaneEquation eq = PlaneEquation(cross.x_, cross.y_, cross.z_,
            - ((float)(cross.x_ * f.first.x + cross.y_ * f.first.y) + cross.z_ * f.second));

    if(f.first.y == s.first.y)
    {
        fillFlat(f.first, s.first, t.first, eq, true);
    }

    else if(s.first.y == t.first.y)
    {
        fillFlat(f.first, s.first, t.first, eq, false);
    }

    else
    {
        float alpha = (float)(s.first.y - f.first.y) / (float)(t.first.y - f.first.y);
        Point2 vi = Point2((float)(f.first.x + ((t.first.x - f.first.x) * alpha)),
                           (float)(f.first.y + ((t.first.y - f.first.y) * alpha)));

        if(s.first.x < vi.x)
        {
            fillFlat(f.first, s.first, vi, eq, false);
            fillFlat(s.first, vi, t.first, eq, true);
        }
        else
        {
            fillFlat(f.first, vi, s.first, eq, false);
            fillFlat(vi, s.first, t.first, eq, true);
        }
    }
    
    return;
}

void DirectionalLight::fillFlat(const Point2& a,
                         const Point2& b, const Point2& c,
                         PlaneEquation& eq, bool top)
{
    auto down = Vector3(imagePlan[0], imagePlan[width]);
    
    auto slopeNormalizedF = Vector3(getCoord(a),getCoord(c)).normalize();
    auto slopeNormalizedS = Vector3(getCoord(b), getCoord(c)).normalize();

    if (!top)
    {
        slopeNormalizedF = Vector3(getCoord(a), getCoord(b)).normalize();
        slopeNormalizedS = Vector3(getCoord(a), getCoord(c)).normalize();
    }

    Point2 topRightVertex = b;
    if(!top)
    {
        topRightVertex = a;
    }

    auto distF = down.norm() / dot(slopeNormalizedF, down.normalize());
    auto distS = down.norm() / dot(slopeNormalizedS, down.normalize());
    for(int j = 0; j < c.y - a.y; j++)
    {
        if (a.y + j < 0)
            continue;
        if (a.y + j >= height)
            break;
        auto pinf = getCoord(a) + slopeNormalizedF * (distF * (float)j);
        auto inf = computePointCoordinate(pinf);
        auto psup = getCoord(topRightVertex) + slopeNormalizedS * (distS * (float)j);
        auto sup = computePointCoordinate(psup);

        if(inf.x > sup.x)
            std::swap(inf, sup);
        
        float zCur = 0;
        if(eq.c != 0)
        {
            zCur = (- eq.a * (float) inf.x - eq.b * (float)inf.y - eq.d) / eq.c;
        }
        for (int i = inf.x; i <= sup.x; i++)
        {
            int index = inf.y * width + i;
            if(i < 0)
                continue;
            if(i >= width)
                break;

            if(depthBuffer[index] > zCur && zCur > 0)
            {
                depthBuffer[index] = zCur;
            }
            if(eq.c == 0)
            {
                zCur = 0;
            }
            else
            {
               zCur = zCur - eq.a / eq.c; 
            }
        }
    }
}

Point3 DirectionalLight::getCoord(const Point2& p) const
{
    auto down = Vector3(imagePlan[0], imagePlan[width]);
    auto right = Vector3(imagePlan[0], imagePlan[1]);

    return imagePlan[0] + right * p.x + down * p.y;
}

Point2 DirectionalLight::computePointCoordinate(const Point3& p) const
{
    auto hrCorner = imagePlan[0];   
    auto right = Vector3(hrCorner, imagePlan[1]);
    auto rightNormalized = right.normalize();
    auto down = Vector3(hrCorner, imagePlan[width]);
    auto downNormalized = down.normalize();
    
    Vector3 tmp = Vector3(hrCorner, p);
    int coefx = round(dot(rightNormalized, tmp) / right.norm());
    int coefy = round(dot(downNormalized, tmp) / down.norm());
    return Point2(coefx, coefy);
}