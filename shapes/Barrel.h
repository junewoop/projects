#ifndef BARREL_H
#define BARREL_H

#include "Shape.h"

class Barrel : public Shape
{
public:
    Barrel();
    Barrel(int param1, int param2);
    ~Barrel();

private:
    int m_param1;
    int m_param2;
    int m_numFloats;
    float m_begin;
    float m_step_h;
    float m_step_th;
    void create_barrel_vertex_data();
    void add_unit(int &ind, int i, int j);
    void add_vertex(int &ind, int i, int j);
};

#endif // BARREL_H
