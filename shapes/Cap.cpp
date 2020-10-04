#include "Cap.h"

Cap::Cap()
{
}

Cap::Cap(int param1, int param2, int sgn) :
    m_param1(param1),
    m_param2(param2),
    m_sgn(sgn),
    m_numFloats(0),
    m_step_r(0.0f),
    m_step_th(0.0f)
{
    create_cap_vertex_data();
    /** build the VAO so that the shape is ready to be drawn */
    buildVAO();
}

Cap::~Cap()
{
}

void Cap::create_cap_vertex_data(){
    m_param1 = std::max(m_param1, 1);
    m_param2 = std::max(m_param2, 3);
    m_numFloats = 18*(2*m_param1-1)*m_param2;
    m_step_r = 0.5f/m_param1;
    m_step_th = (2.0f * M_PI)/m_param2;
    m_vertexData.resize(m_numFloats);
    int ind = 0;
    for (int i = 0; i < m_param1; i++)
        for (int j = 0; j < m_param2; j++)
            add_unit(ind, i, j);
};
void Cap::add_unit(int &ind, int i, int j){
    switch(m_sgn){
    case POS:
        if (i == 0){
            add_vertex(ind, i, j);
            add_vertex(ind, i+1, j);
            add_vertex(ind, i+1, j+1);
        }
        else{
            add_vertex(ind, i, j);
            add_vertex(ind, i+1, j);
            add_vertex(ind, i+1, j+1);
            add_vertex(ind, i, j);
            add_vertex(ind, i+1, j+1);
            add_vertex(ind, i, j+1);
        }
        break;
    case NEG:
        if (i == 0){
            add_vertex(ind, i, j);
            add_vertex(ind, i+1, j+1);
            add_vertex(ind, i+1, j);
        }
        else{
            add_vertex(ind, i, j);
            add_vertex(ind, i+1, j+1);
            add_vertex(ind, i+1, j);
            add_vertex(ind, i, j);
            add_vertex(ind, i, j+1);
            add_vertex(ind, i+1, j+1);
        }
        break;
    }
};
void Cap::add_vertex(int &ind, int i, int j){
        m_vertexData[ind++] = m_step_r*i*sin(m_step_th*j);
        m_vertexData[ind++] = m_sgn*0.5f;
        m_vertexData[ind++] = m_step_r*i*cos(m_step_th*j);
        m_vertexData[ind++] = 0.0f;
        m_vertexData[ind++] = m_sgn*1.0f;
        m_vertexData[ind++] = 0.0f;
};
