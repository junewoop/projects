#ifndef FilterGray_Header
#define FilterGray_Header
#include "Filter.h"

class FilterGray : public Filter {
public:
    FilterGray();
    virtual ~FilterGray() override;
    virtual void apply(Canvas2D *canvas) override;

protected:
    int m_width;
    int m_height;
};

#endif
