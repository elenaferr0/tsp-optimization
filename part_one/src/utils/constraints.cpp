#include "utils/constraints.h"

#include <strings.h>
#include <iostream>

Constraints::Constraints() : n_rows(0), non_zero_count(0),
indexes(vector<int>()),
coefficients(vector<double>()),
senses(vector<char>()),
rhs(vector<double>()),
matbeg(vector<int>())
{
}

void Constraints::new_row(char sense, double rhs) {
    n_rows++;
    this->matbeg.push_back(indexes.size());
    senses.push_back(sense);
    this->rhs.push_back(rhs);
}

void Constraints::new_eq(const double rhs) {
    new_row('E', rhs);
}

void Constraints::new_le(const double rhs) {
    new_row('L', rhs);
}

void Constraints::new_ge(const double rhs) {
    new_row('G', rhs);
}

void Constraints::add_term(const int index, const double coefficient) {
    non_zero_count++;
    indexes.push_back(index);
    coefficients.push_back(coefficient);
}

int Constraints::get_nzcnt() const {
    return non_zero_count;
}

int Constraints::get_n_rows() const {
    return n_rows;
}

const int *Constraints::get_indexes() const {
    return indexes.data();
}

const double *Constraints::get_coefficients() const {
    return coefficients.data();
}

const char *Constraints::get_senses() const {
    return senses.data();
}

const double *Constraints::get_rhs() const {
    return rhs.data();
}

const int *Constraints::get_matbeg() const {
    return matbeg.data();
}