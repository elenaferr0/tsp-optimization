#ifndef MILLER_TUCKER_ZEMLIN_FORMULATION_H
#define MILLER_TUCKER_ZEMLIN_FORMULATION_H
#include "formulation.h"

class MillerTuckerZemlinFormulation final : public Formulation {
    IntMat map_x;
    IntVec map_u;

    void create_variables() override;

    void create_constraints() override;

    string formulation_code() const override;

public:
    explicit MillerTuckerZemlinFormulation(const char *instance_name);
};

#endif //MILLER_TUCKER_ZEMLIN_FORMULATION_H
