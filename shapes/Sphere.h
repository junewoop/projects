#ifndef SPHERE_H
#define SPHERE_H

#include "Shape.h"

class Sphere : public Shape
{
public:
    Sphere();
    Sphere(int param1, int param2);
    ~Sphere();

private:
    int m_param1;
    int m_param2;
    int m_numFloats;
    float m_step_phi;
    float m_step_th;
    void create_sphere_vertex_data();
    void add_unit(int &ind, int i, int j);
    void add_vertex(int &ind, int i, int j);
};

#endif // SPHERE_H
