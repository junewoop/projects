#ifndef FILTEREDGE_H
#define FILTEREDGE_H
#include "FilterGray.h"

class FilterEdge : public FilterGray {
public:
    FilterEdge(float p);
    ~FilterEdge() override;

    virtual void apply(Canvas2D *canvas) override;
protected:
    float m_p;
};
#endif // FILTEREDGE_H
