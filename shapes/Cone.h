#ifndef CONE_H
#define CONE_H
#include "Shape.h"
#include "Slant.h"
#include "Cap.h"

class Cone : public Shape
{
public:
    Cone();
    Cone(int param1, int param2);
    ~Cone();

private:
    int m_param1;
    int m_param2;
    std::unique_ptr<Slant> m_slant;
    std::unique_ptr<Cap> m_cap;
    void draw() override;
};
#endif // CONE_H
