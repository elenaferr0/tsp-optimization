#ifndef GAVISH_GRAVES_FORMULATION_H
#define GAVISH_GRAVES_FORMULATION_H
#include "formulation.h"

class GavishGravesFormulation final : Formulation {
    IntMat map_x;
    IntMat map_y;

    void create_variables() const;

    void create_constraints();

public:
    void setup() override;

    void solve() override;

    explicit GavishGravesFormulation(const string &graph_file);
};

#endif //GAVISH_GRAVES_FORMULATION_H
