#include "Cylinder.h"

Cylinder::Cylinder()
{
}

Cylinder::Cylinder(int param1, int param2) :
    m_param1(param1),
    m_param2(param2),
    m_barrel(nullptr),
    m_cap_top(nullptr),
    m_cap_bottom(nullptr)
{
    m_barrel = std::make_unique<Barrel>(m_param1, m_param2);
    m_cap_top = std::make_unique<Cap>(m_param1, m_param2, POS);
    m_cap_bottom = std::make_unique<Cap>(m_param1, m_param2, NEG);
}

Cylinder::~Cylinder()
{
}

void Cylinder::draw(){
    m_barrel->draw();
    m_cap_top->draw();
    m_cap_bottom->draw();
}
