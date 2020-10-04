#ifndef SLANT_H
#define SLANT_H

#include "Shape.h"

class Slant : public Shape
{
public:
    Slant();
    Slant(int param1, int param2);
    ~Slant();

private:
    int m_param1;
    int m_param2;
    int m_numFloats;
    float m_begin;
    float m_step_h;
    float m_step_th;
    void create_slant_vertex_data();
    void add_unit(int &ind, int i, int j);
    void add_vertex(int &ind, int i, int j);
};

#endif // SLANT_H
