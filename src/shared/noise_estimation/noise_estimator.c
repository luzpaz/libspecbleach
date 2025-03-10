/*
libspecbleach - A spectral processing library

Copyright 2022 Luciano Dato <lucianodato@gmail.com>

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "noise_estimator.h"
#include "../utils/spectral_features.h"
#include "../utils/spectral_utils.h"
#include <stdlib.h>
#include <string.h>

struct NoiseEstimator {
  uint32_t fft_size;
  uint32_t real_spectrum_size;

  NoiseProfile *noise_profile;
};

NoiseEstimator *noise_estimation_initialize(const uint32_t fft_size,
                                            NoiseProfile *noise_profile) {
  NoiseEstimator *self = (NoiseEstimator *)calloc(1U, sizeof(NoiseEstimator));

  self->fft_size = fft_size;
  self->real_spectrum_size = self->fft_size / 2U + 1U;

  self->noise_profile = noise_profile;

  return self;
}

void noise_estimation_free(NoiseEstimator *self) {
  noise_profile_free(self->noise_profile);
  free(self);
}

bool noise_estimation_run(NoiseEstimator *self, float *signal_spectrum) {
  if (!self || !signal_spectrum) {
    return false;
  }

  float *noise_profile = get_noise_profile(self->noise_profile);

  get_rolling_mean_spectrum(
      noise_profile, signal_spectrum,
      get_noise_profile_blocks_averaged(self->noise_profile),
      self->real_spectrum_size);

  increment_blocks_averaged(self->noise_profile);

  return true;
}
