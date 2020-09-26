/**
 * @file   ConstantBrush.cpp
 *
 * Implementation of a brush with a constant mask distribution.
 *
 * You should fill this file in while completing the Brush assignment.
 */

#include "ConstantBrush.h"

ConstantBrush::ConstantBrush(RGBA color, int radius)
    : Brush(color, radius)
{
    // @TODO: [BRUSH] You'll probably want to set up the mask right away.
    makeMask();
}


ConstantBrush::~ConstantBrush()
{
}

void ConstantBrush::makeMask() {
    // @TODO: [BRUSH] Set up the mask for your Constant brush here...
    int r2 = m_radius*m_radius;
    m_mask.reserve(m_maskSize*m_maskSize);
    for (int i = 0; i < m_maskSize; i++)
        for (int j = 0; j < m_maskSize; j++)
            if (squaredDistance(i, j) <= r2)
                m_mask[getIndex(i, j)] = static_cast<float>(m_color.a)/255.0f;
            else m_mask[getIndex(i, j)] = 0.0;
}





