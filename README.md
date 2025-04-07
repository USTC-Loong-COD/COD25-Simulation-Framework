# 2025 COD 仿真环境食用说明

## USTC 2025 SPRING COD LJL TA's



#### 使用步骤

1.  将你的 CPU 实现文件放到 `vsrc/your_vsrc` 目录下；

    如果你使用了 verilog 的头文件，你可以将其放在 `vsrc/your_include` 目录下；

    注意，你的 CPU 接口应和 `top.v` 中例化的 CPU 相一致。该接口也适用于上板验证。

2.  创建 mem 目录，将指令和数据 Memory 的初始化文件放在该目录下；文件的格式为：

    -   每一行一个 8 位 16 进制值；
    -   无 header；
    -   不要使用逗号或分号等分隔；
    -   **文件尾需要一个空行**；

3.  修改 `vsrc/configs` 目录下的 `configs.vh` 和 `include` 目录下的 `configs.hpp`；

4.  使用如下命令编译：

    ``` shell
    $ cmake -B build && cmake --build build
    ```

    **注意：每次修改 Verilog 源文件都需要重新编译！**

5.  编译出的可执行文件在 build 目录下，使用如下命令运行：

    ``` shell
    $ ./build/sim
    ```