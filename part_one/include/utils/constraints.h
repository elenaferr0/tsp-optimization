//
// Created by elena on 4/18/25.
//

#ifndef CONSTRAINTS_H
#define CONSTRAINTS_H

#include <string>
#include <vector>

using namespace std;

class Constraints {
    int n_rows;
    int non_zero_count;
    vector<int> indexes;
    vector<double> coefficients;
    vector<char> senses;
    vector<double> rhs;
    vector<int> matbeg;

    void new_row(char sense, double rhs);

public:
    Constraints();

    // Utility methods
    /**
     * @brief Updates the matbeg vector with the index of the new constraint. This
     * must be necessarily called when creating a new row.
     */
    void add_term(int index, double coefficient);

    void new_eq(double rhs);
    void new_le(double rhs);
    void new_ge(double rhs);

    //@formatter:off
    int get_nzcnt() const;
    int get_n_rows() const;
    const int *get_indexes() const;
    const double *get_coefficients() const;
    const vector<int> get_indexes_vec() const;
    const vector<double> get_coefficients_vec() const;
    const char *get_senses() const;
    const double *get_rhs() const;
    const int *get_matbeg() const;
    //@formatter:on
};


#endif //CONSTRAINTS_H
