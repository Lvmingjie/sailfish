#ifndef COLLAPSED_EM_OPTIMIZER_HPP
#define COLLAPSED_EM_OPTIMIZER_HPP

#include <unordered_map>

#include "tbb/atomic.h"
#include "tbb/task_scheduler_init.h"

#include "ReadExperiment.hpp"
#include "SailfishOpts.hpp"

#include "cuckoohash_map.hh"
#include "Eigen/Dense"

class CollapsedEMOptimizer {
    public:
        using VecType = std::vector<tbb::atomic<double>>;
        CollapsedEMOptimizer();

        bool optimize(ReadExperiment& readExp,
                      SailfishOpts& sopt,
                      double tolerance = 0.01,
                      uint32_t maxIter = 1000);

};

#endif // COLLAPSED_EM_OPTIMIZER_HPP

