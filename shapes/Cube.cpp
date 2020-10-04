#include "Cube.h"

Cube::Cube()
{
}

Cube::Cube(int param1) :
    m_param1(param1),
    m_numFloats(0),
    m_begin(0.0f),
    m_step(0.0f)
{
    create_cube_vertex_data();
    /** build the VAO so that the shape is ready to be drawn */
    buildVAO();
}

Cube::~Cube()
{
}

void Cube::create_cube_vertex_data(){
    m_param1 = std::max(m_param1, 1);
    int num_float_one_side = 36 * m_param1 * m_param1;
    m_begin = -0.5f;
    m_step = 1.0f/m_param1;
    m_numFloats = 6 * num_float_one_side;
    m_vertexData.resize(m_numFloats);
    int ind = 0, r;
    for (int i = 0; i < m_param1; i++) {
        for (int j = 0; j < m_param1; j++) {
            add_vertex(ind, i, j, 1);
            add_vertex(ind, i + 1, j, 1);
            add_vertex(ind, i + 1, j +1, 1);
            add_vertex(ind, i, j, 1);
            add_vertex(ind, i + 1, j + 1, 1);
            add_vertex(ind, i, j + 1, 1);
        }
    }
    for (int i = 0; i < m_param1; i++) {
        for (int j = 0; j < m_param1; j++) {
            add_vertex(ind, i, j, -1);
            add_vertex(ind, i + 1, j + 1, -1);
            add_vertex(ind, i + 1, j, -1);
            add_vertex(ind, i, j, -1);
            add_vertex(ind, i, j + 1, -1);
            add_vertex(ind, i + 1, j + 1, -1);
        }
    }
    for (; ind < 6*num_float_one_side; ind++){
        r = ind % 3;
        switch(r){
        case 0:
            m_vertexData[ind] = m_vertexData[ind-2*num_float_one_side+1];
            break;
        case 1:
            m_vertexData[ind] = m_vertexData[ind-2*num_float_one_side+1];
            break;
        case 2:
            m_vertexData[ind] = m_vertexData[ind-2*num_float_one_side-2];
            break;
        }
    }
}
void Cube::add_vertex(int &ind, int i, int j, int sgn){
    m_vertexData[ind++] = m_begin + i * m_step;
    m_vertexData[ind++] = m_begin + j * m_step;
    m_vertexData[ind++] = 0.5f * sgn;
    m_vertexData[ind++] = 0.0f;
    m_vertexData[ind++] = 0.0f;
    m_vertexData[ind++] = 1.0f * sgn;
}
