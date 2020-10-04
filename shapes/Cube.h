#ifndef CUBE_H
#define CUBE_H

#include "Shape.h"

class Cube : public Shape
{
public:
    Cube();
    Cube(int param1);
    ~Cube();

private:
    int m_param1;
    int m_numFloats;
    float m_begin;
    float m_step;
    void create_cube_vertex_data();
    void add_vertex(int &ind, int i, int j, int sgn);
};

#endif // CUBE_H
