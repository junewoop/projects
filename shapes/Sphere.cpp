#include "Sphere.h"

Sphere::Sphere()
{
}

Sphere::Sphere(int param1, int param2) :
    m_param1(param1),
    m_param2(param2),
    m_numFloats(0),
    m_step_phi(0.0f),
    m_step_th(0.0f)
{
    create_sphere_vertex_data();
    /** build the VAO so that the shape is ready to be drawn */
    buildVAO();
}

Sphere::~Sphere()
{
}

void Sphere::create_sphere_vertex_data(){
    m_param1 = std::max(m_param1, 2);
    m_param2 = std::max(m_param2, 3);
    m_step_phi = M_PI/m_param1;
    m_step_th = (2.0f*M_PI)/m_param2;
    m_numFloats = 36 * (m_param1 - 1) * m_param2;
    m_vertexData.resize(m_numFloats);
    int ind = 0;
    for (int i = 0; i < m_param1; i++)
        for (int j = 0; j < m_param2; j++)
            add_unit(ind, i, j);
}

void Sphere::add_unit(int &ind, int i, int j){
    if (i != 0){
        add_vertex(ind, i, j);
        add_vertex(ind, i+1, j+1);
        add_vertex(ind, i, j+1);
    }
    if (i != m_param1 - 1){
        add_vertex(ind, i, j);
        add_vertex(ind, i + 1, j);
        add_vertex(ind, i + 1, j + 1);
    }
}

void Sphere::add_vertex(int &ind, int i, int j){
    m_vertexData[ind++] = 0.5*sin(m_step_phi*i)*sin(m_step_th*j);
    m_vertexData[ind++] = 0.5*cos(m_step_phi*i);
    m_vertexData[ind++] = 0.5*sin(m_step_phi*i)*cos(m_step_th*j);
    m_vertexData[ind++] = sin(m_step_phi*i)*sin(m_step_th*j);
    m_vertexData[ind++] = cos(m_step_phi*i);
    m_vertexData[ind++] = sin(m_step_phi*i)*cos(m_step_th*j);
}
