#ifndef CAP_H
#define CAP_H

#include "Shape.h"

enum Sgn {POS = 1, NEG = -1};

class Cap : public Shape
{
public:
    Cap();
    Cap(int param1, int param2, int sgn);
    ~Cap();

private:
    int m_param1;
    int m_param2;
    int m_sgn;
    int m_numFloats;
    float m_step_r;
    float m_step_th;
    void create_cap_vertex_data();
    void add_unit(int &ind, int i, int j);
    void add_vertex(int &ind, int i, int j);
};

#endif // CAP_H
