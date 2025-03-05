# linux_app_frame 
linux_app_frame是一个通用的嵌入式Linux应用项目的目录框架，旨在为开发人员提供一个结构化、模块化和易于维护的开发环境。

# 目录结构
```shell
./
├── config
│   ├── config.json                  # 系统配置文件，用于存储运行时配置参数
│   └── defaults.ini                 # 默认配置文件，提供初始配置参数
├── doc
│   ├── design_docs                  # 设计文档目录，存放系统设计、架构设计等文档
│   └── reference                    # 参考文档目录，存放技术手册、API文档等
├── include
│   ├── app                          # 应用层头文件目录，存放应用层公共头文件
│   ├── bsp                          # 
│   │   ├── gpio                     # GPIO驱动相关头文件
│   │   │   └── gpio.h               # GPIO接口定义和操作函数声明
│   │   ├── i2c
│   │   │   ├── i2cbusses.h          # I2C总线相关定义
│   │   │   └── smbus.h              # SMBus协议相关定义
│   │   └── spi
│   │       └── spi.h                # SPI接口定义和操作函数声明
│   ├── common                       # 通用头文件目录，存放跨层使用的头文件
│   └── hal                          # 硬件抽象层头文件目录，存放硬件抽象接口
├── lib                              # 第三方库或编译生成的库文件目录
├── Makefile                         # 项目构建文件，用于编译和链接项目
├── README.md                        # 项目说明文档，介绍项目结构和使用方法
├── scripts                          # 脚本目录，存放构建、测试、部署等脚本
├── src
│   ├── app                          # 应用层源代码目录，存放业务逻辑代码
│   ├── bsp
│   │   ├── gpio
│   │   │   └── gpio.c               # GPIO驱动实现代码
│   │   ├── i2c
│   │   │   ├── i2cbusses.c          # I2C总线操作实现代码
│   │   │   └── smbus.c              # SMBus协议实现代码
│   │   └── spi
│   │       └── spi.c                # SPI接口实现代码
│   ├── hal                          # 硬件抽象层源代码目录，提供硬件操作接口
│   └── utils                        # 工具函数目录，存放通用工具函数实现
└── tests                            # 测试目录，存放单元测试、集成测试代码
```

