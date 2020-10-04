#include "Cone.h"

Cone::Cone()
{
}

Cone::Cone(int param1, int param2) :
    m_param1(param1),
    m_param2(param2),
    m_slant(nullptr),
    m_cap(nullptr)
{
    m_slant = std::make_unique<Slant>(m_param1, m_param2);
    m_cap = std::make_unique<Cap>(m_param1, m_param2, -1);
}

Cone::~Cone()
{
}

void Cone::draw(){
    m_slant->draw();
    m_cap->draw();
}
