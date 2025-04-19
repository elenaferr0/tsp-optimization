#ifndef GAVISH_GRAVES_FORMULATION_H
#define GAVISH_GRAVES_FORMULATION_H
#include "formulation.h"

class GavishGravesFormulation final : public Formulation {
    IntMat map_x;
    IntMat map_y;

    void create_variables() override;

    void create_constraints() override;

    string formulation_code() const override;

public:
    explicit GavishGravesFormulation(const char *instance_name);
};

#endif //GAVISH_GRAVES_FORMULATION_H
