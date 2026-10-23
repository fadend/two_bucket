#include "two_bucket.h"

#include <algorithm>
#include <array>
#include <optional>
#include <set>
#include <stdexcept>

#define RETURN_IF_HAS_VALUE(x) if (auto result = (x); result.has_value()) { \
  return result.value(); \
}

namespace two_bucket {
namespace {
    using Buckets = std::array<int, 2>;

    class Dfs {
        public:
        Dfs(Buckets sizes, int target) : sizes_(sizes), target_(target) {}

        std::optional<measure_result> dfs(const Buckets state, int num_moves) {
            record_visit(state);
            for (int i = 0; i < 2; ++i) {
                if (state[i] == target_) {
                    return measure_result{
                        num_moves,
                        static_cast<bucket_id>(i),
                        state[1 - i]
                    };
                }
            }
            for (int i = 0; i < 2; ++i) {
                const int other = 1 - i;
                // Empty the bucket
                Buckets empty_current = state;
                empty_current[i] = 0;
                if (!has_visited(empty_current)) {
                    RETURN_IF_HAS_VALUE(dfs(empty_current, num_moves + 1));
                }
                // Pour into the other
                if (state[other] < sizes_[other] && state[i] > 0) {
                    Buckets poured = state;
                    const int amount = std::min<int>(state[i], sizes_[other] - state[other]);
                    poured[i] -= amount;
                    poured[other] += amount;
                    if (!has_visited(poured)) {
                        RETURN_IF_HAS_VALUE(dfs(poured, num_moves + 1));
                    }
                }
                // Fill it
                Buckets filled = state;
                filled[i] = sizes_[i];
                if (!has_visited(filled)) {
                    RETURN_IF_HAS_VALUE(dfs(filled, num_moves + 1));
                }
            }
            return std::nullopt;
        }
        private:
        const Buckets sizes_;
        const int target_;
        std::set<Buckets> visited_;

        bool has_visited(Buckets state) const {
            return visited_.count(state) > 0;
        }

        void record_visit(Buckets state) {
            visited_.insert(state);
        }
    };
}  // namespace

measure_result measure(int bucket1_capacity, int bucket2_capacity,
                       int target_volume, bucket_id start_bucket) {
    const Buckets sizes{bucket1_capacity, bucket2_capacity};
    Dfs dfs(sizes, target_volume);
    Buckets initial{};
    const auto start = static_cast<int>(start_bucket);
    initial[start] = sizes[start];
    auto result = dfs.dfs(initial, 0);
    if (result.has_value()) {
        return result.value();
    } else {
        throw std::domain_error("No solution");
    }
}

}  // namespace two_bucket

