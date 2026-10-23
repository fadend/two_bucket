#include "two_bucket.h"

#include <iostream>
#include <string>

int main(int argc, char **argv) {
  if (argc != 5) {
    std::cerr << "Expects 4 args: bucket 0 size, bucket 1 size, target, start "
                 "bucket index\n";
    return 1;
  }
  auto result = two_bucket::measure(
      std::stoi(argv[1]), std::stoi(argv[2]), std::stoi(argv[3]),
      static_cast<two_bucket::bucket_id>(std::stoi(argv[4])));
  std::cout << "num_moves: " << result.num_moves << '\n';
  std::cout << "goal bucket: " << static_cast<int>(result.goal_bucket) << '\n';
  std::cout << "other volume: " << result.other_bucket_volume << '\n';
}
