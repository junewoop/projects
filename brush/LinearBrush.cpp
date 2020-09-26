/**
 * @file   LinearBrush.cpp
 *
 * Implementation of a brush with a linear mask distribution.
 *
 * You should fill this file in while completing the Brush assignment.
 */

#include "LinearBrush.h"

#include <math.h>

LinearBrush::LinearBrush(RGBA color, int radius)
    : Brush(color, radius)
{
    // @TODO: [BRUSH] You'll probably want to set up the mask right away.
    makeMask();
}


LinearBrush::~LinearBrush()
{
}

void LinearBrush::makeMask() {
    // @TODO: [BRUSH] Set up the mask for your Linear brush here...
    int r2 = m_radius*m_radius;
    int d2;
    float a = static_cast<float>(m_color.a)/255.0f;
    m_mask.reserve(m_maskSize*m_maskSize);
    if (r2 == 0)
        m_mask[0] = 1;
    else
        for (int i = 0; i < m_maskSize; i++){
            for (int j = 0; j < m_maskSize; j++){
                d2 = squaredDistance(i, j);
                if (d2 <= r2)
                    m_mask[getIndex(i, j)] = (1-sqrtf(static_cast<float>(d2)/static_cast<float>(r2)))*a;
                else m_mask[getIndex(i, j)] = 0;
            }
        }
}


