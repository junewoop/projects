#ifndef FILTERBLUR_H
#define FILTERBLUR_H
#include "Filter.h"

class FilterBlur : public Filter {
public:
    FilterBlur(int blurRadius);
    virtual ~FilterBlur() override;

    virtual void apply(Canvas2D *canvas) override;
private:
    int m_r;
};
#endif // FILTERBLUR_H
