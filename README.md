## Linux systemd Service example

### 1. prepare env
```
sudo apt install libsystemd-dev
```

### 1. prepare execution file
- build
```
g++ main.cpp \
    -o hello-service \
    -lsystemd \
    -pthread
```

- install
```
sudo cp hello-service /usr/local/bin/
```

### 3. prepare service file
- install service file
```
sudo cp hello.service /etc/systemd/system/
```

### 4. explain
- notify
```
systemd
   │
   │ 启动
   ▼
hello-service
   │
   │ 初始化
   │
   │ sd_notify("READY=1")
   ▼
systemd
   │
   └── 服务真正 Ready
```

- watchdog
```
                 WATCHDOG=1
              ◄──────────────
systemd                         robotd
   │                              │
   │                              │
   │        5 seconds             │
   │◄─────────────────────────────
   │                              │
   │                              │
   └── 超时？                     │
         │                        │
         ├── YES → kill/restart  │
         │                        │
         └── NO  → 正常运行      │
```

- graceful shutdown

```
sudo systemctl stop hello

systemctl stop
       │
       ▼
    SIGTERM
       │
       ▼
signal_handler()
       │
       ▼
running = false
       │
       ▼
while 退出
       │
       ▼
清理资源
       │
       ▼
sd_notify("STOPPING=1")
       │
       ▼
return 0
```

- start
```
sudo systemctl daemon-reload

sudo systemctl start hello
```

- query
```
systemctl status hello
```

- auto boot
```
sudo systemctl enable hello
systemctl is-enabled hello
```

- check current log
```
journalctl -u hello -f
```