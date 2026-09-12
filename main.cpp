#include <chrono>
#include <iostream>
#include <thread>

int main() {
  while (true) {
    std::cout << "Hello from systemd service!" << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(2));
  }

  return 0;
}