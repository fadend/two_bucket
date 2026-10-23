// From https://exercism.org/tracks/cpp/exercises/two-bucket.
//
// MIT License
//
// Copyright (c) 2021 Exercism

#pragma once

namespace two_bucket {

enum class bucket_id { one, two };

struct measure_result {
  int num_moves;
  bucket_id goal_bucket;
  int other_bucket_volume;
};

measure_result measure(int bucket1_capacity, int bucket2_capacity,
                       int target_volume, bucket_id start_bucket);

} // namespace two_bucket
