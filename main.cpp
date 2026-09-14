#include <atomic>
#include <chrono>
#include <csignal>
#include <iostream>
#include <systemd/sd-daemon.h>
#include <thread>

static std::atomic<bool> running{true};

void signal_handler(int signal) {
  if (signal == SIGTERM || signal == SIGINT) {
    running = false;
  }
}

int main() {
  // 1. 注册信号处理
  std::signal(SIGTERM, signal_handler);
  std::signal(SIGINT, signal_handler);

  std::cout << "hello-service started" << std::endl;
  // 2. 通知 systemd：服务已经启动
  sd_notify(0, "READY=1");

  auto next_heartbeat = std::chrono::steady_clock::now();

  while (running) {
    // 模拟业务工作
    std::cout << "Hello from systemd service!" << std::endl;
    /*
     * 3. Watchdog 心跳
     *
     * systemd 会要求程序周期性发送 WATCHDOG=1
     */
    auto now = std::chrono::steady_clock::now();

    if (now >= next_heartbeat) {
      sd_notify(0, "WATCHDOG=1");

      next_heartbeat = now + std::chrono::seconds(2);
    }

    std::this_thread::sleep_for(std::chrono::seconds(1));
  }

  // 4. 优雅退出
  std::cout << "hello-service stopping..." << std::endl;
  sd_notify(0, "STOPPING=1");
  std::cout << "hello-service stopped" << std::endl;

  return 0;
}