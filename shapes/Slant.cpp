#include "Slant.h"

Slant::Slant()
{
}

Slant::Slant(int param1, int param2) :
    m_param1(param1),
    m_param2(param2),
    m_numFloats(0),
    m_begin(0),
    m_step_h(0),
    m_step_th(0)
{
    create_slant_vertex_data();
    /** build the VAO so that the shape is ready to be drawn */
    buildVAO();
}

Slant::~Slant()
{
}

void Slant::create_slant_vertex_data(){
    m_param1 = std::max(m_param1, 1);
    m_param2 = std::max(m_param2, 3);
    m_numFloats = 18 *(2*m_param1-1) * m_param2;
    m_vertexData.resize(m_numFloats);
    m_begin = 0.5f;
    m_step_h = 1.0f/m_param1;
    m_step_th = (2.0f * M_PI)/m_param2;
    int ind = 0;
    for (int i = 0; i < m_param1; i++)
        for (int j = 0; j < m_param2; j++)
            add_unit(ind, i, j);
}

void Slant::add_unit(int &ind, int i, int j){
    if (i == 0){
        add_vertex(ind, i, j);
        add_vertex(ind, i+1, j);
        add_vertex(ind, i+1, j+1);
    }
    else{
        add_vertex(ind, i, j);
        add_vertex(ind, i+1, j);
        add_vertex(ind, i, j+1);
        add_vertex(ind, i, j+1);
        add_vertex(ind, i+1, j);
        add_vertex(ind, i+1, j+1);
    }
}

void Slant::add_vertex(int &ind, int i, int j){
    if (i == 0){
        m_vertexData[ind++] = 0.0f;
        m_vertexData[ind++] = m_begin;
        m_vertexData[ind++] = 0.0f;
        m_vertexData[ind++] = 2.0f/sqrt(5) * sin(m_step_th*(j+0.5f));
        m_vertexData[ind++] = 1.0f/sqrt(5);
        m_vertexData[ind++] = 2.0f/sqrt(5) * cos(m_step_th*(j+0.5f));
    }
    else{
        m_vertexData[ind++] = 0.5*m_step_h*i*sin(m_step_th*j);
        m_vertexData[ind++] = m_begin - m_step_h*i;
        m_vertexData[ind++] = 0.5*m_step_h*i*cos(m_step_th*j);
        m_vertexData[ind++] = 2.0f/sqrt(5) * sin(m_step_th*j);
        m_vertexData[ind++] = 1.0f/sqrt(5);
        m_vertexData[ind++] = 2.0f/sqrt(5) * cos(m_step_th*j);
    }
}
