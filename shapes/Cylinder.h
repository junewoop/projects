#ifndef CYLINDER_H
#define CYLINDER_H
#include "Shape.h"
#include "Barrel.h"
#include "Cap.h"

class Cylinder : public Shape
{
public:
    Cylinder();
    Cylinder(int param1, int param2);
    ~Cylinder();
    void draw() override;

private:
    int m_param1;
    int m_param2;
    std::unique_ptr<Barrel> m_barrel;
    std::unique_ptr<Cap> m_cap_top;
    std::unique_ptr<Cap> m_cap_bottom;
};

#endif // CYLINDER_H
