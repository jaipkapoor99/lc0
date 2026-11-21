/*
 This file is part of Leela Chess Zero.
 Copyright (C) 2018-2019 The LCZero Authors

 Leela Chess is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 Leela Chess is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with Leela Chess.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

// clang-format off
// math.h include is workaround for Eigen trying to use math functions from global
// namespaces. math.h must be included before Eigen/Core.
#include <math.h>
#include <Eigen/Core>
// clang-format on

// Select the BLAS vendor based on defines

#ifdef USE_OPENBLAS
#include <cblas.h>

// Specific openblas routines.
extern "C" {
int openblas_get_num_procs(void);
void openblas_set_num_threads(int num_threads);
char* openblas_get_corename(void);
char* openblas_get_config(void);
}

#else

#ifdef __APPLE__
#include <Accelerate/Accelerate.h>
#define USE_ACCELERATE
#endif  // __APPLE__

#endif  // USE_OPENBLAS
