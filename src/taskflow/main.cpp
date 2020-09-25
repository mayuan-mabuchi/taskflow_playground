#include <taskflow/taskflow.hpp>
#include <unistd.h>

int main()
{
  tf::Taskflow f;
  int ac{};
  int bc{};
  int cc{};

  auto A = [&ac]() { sleep(1); std::cout << "This is TaskA" << ++ac << "\n"; };
  auto B = [&bc]() { sleep(2); std::cout << "This is TaskB" << ++bc << "\n"; };
  auto C = [&cc]() { sleep(3); std::cout << "This is TaskC" << ++cc << "\n"; };

  auto S = f.emplace([]() {});
  auto [A1, B1, C1] = f.emplace(A, B, C);
  auto [A2, B2, C2] = f.emplace(A, B, C);
  auto [A3, B3, C3] = f.emplace(A, B, C);

  S.precede(A1, A2, A3);

  A1.precede(B1);
  B1.precede(C1);

  A2.precede(B2);
  B2.precede(C2);

  A3.precede(B3);
  B3.precede(C3);

  A1.precede(A2);
  A2.precede(A3);
  B1.precede(B2);
  B2.precede(B3);
  C1.precede(C2);
  C2.precede(C3);

  tf::Executor executor;
  executor.run_until(f, [&cc]() { return (cc == 3); });
}
