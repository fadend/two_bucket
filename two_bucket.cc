#include "two_bucket.h"

#include <algorithm>
#include <array>
#include <deque>
#include <optional>
#include <set>
#include <stdexcept>

namespace two_bucket {
namespace {
using Buckets = std::array<int, 2>;
struct State {
  Buckets buckets;
  int num_moves;
};

class Bfs {
public:
  Bfs(Buckets sizes, int target) : sizes_(sizes), target_(target) {}

  std::optional<measure_result> search(const Buckets initial_buckets) {
    std::deque<State> queue;
    queue.push_back({initial_buckets, 0});
    record_visit(initial_buckets);
    while (!queue.empty()) {
      const auto state = queue.front();
      queue.pop_front();
      const Buckets buckets = state.buckets;
      const int num_moves = state.num_moves;
      for (int i = 0; i < 2; ++i) {
        if (buckets[i] == target_) {
          return measure_result{num_moves, static_cast<bucket_id>(i),
                                buckets[1 - i]};
        }
      }
      for (int i = 0; i < 2; ++i) {
        const int other = 1 - i;
        // Empty the bucket
        Buckets empty_current = buckets;
        empty_current[i] = 0;
        if (!has_visited(empty_current)) {
          record_visit(empty_current);
          queue.push_back(State{empty_current, num_moves + 1});
        }
        // Pour into the other
        if (buckets[other] < sizes_[other] && buckets[i] > 0) {
          Buckets poured = buckets;
          const int amount =
              std::min<int>(buckets[i], sizes_[other] - buckets[other]);
          poured[i] -= amount;
          poured[other] += amount;
          if (!has_visited(poured)) {
            record_visit(poured);
            queue.push_back({poured, num_moves + 1});
          }
        }
        // Fill it
        Buckets filled = buckets;
        filled[i] = sizes_[i];
        if (!has_visited(filled)) {
          record_visit(filled);
          queue.push_back({filled, num_moves + 1});
        }
      }
    }
    return std::nullopt;
  }

private:
  const Buckets sizes_;
  const int target_;
  std::set<Buckets> visited_;

  bool has_visited(Buckets state) const { return visited_.count(state) > 0; }

  void record_visit(Buckets state) { visited_.insert(state); }
};
} // namespace

measure_result measure(int bucket1_capacity, int bucket2_capacity,
                       int target_volume, bucket_id start_bucket) {
  const Buckets sizes{bucket1_capacity, bucket2_capacity};
  Bfs bfs(sizes, target_volume);
  Buckets initial{};
  const auto start = static_cast<int>(start_bucket);
  initial[start] = sizes[start];
  auto result = bfs.search(initial);
  if (result.has_value()) {
    return result.value();
  } else {
    throw std::domain_error("No solution");
  }
}

} // namespace two_bucket
