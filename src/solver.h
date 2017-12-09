#ifndef CHESSY_SOLVER_H
#define CHESSY_SOLVER_H

#include <vector>
#include <memory>
#include <iostream>
#include "chessman.h"
#include "board.h"
#include "solution.h"

namespace chessy {

class solver {
    int size_;
    std::shared_ptr<board> board_;
    int indexes_[CHESSMAN_TYPES];
    int chessman_count_;
public:
    using i_solution = solution<int>;
    using i_shared_ptr = std::shared_ptr<int>;

    solver(const std::shared_ptr<board> &b);

    ~solver() = default;

    unsigned long long int recursive_count = 0;

    std::vector<i_solution> solve(const i_shared_ptr &f);

private:
    void reset();

    void recursive_solve(std::vector<i_solution> *solutions, int f_number, int *figures, int prev_index, int prev_x, int prev_y);

    int chessman_count();
};

}


#endif //CHESSY_SOLVER_H
