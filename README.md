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

3.  修改 `include` 目录下的 `configs.hpp` 和 `vsrc/configs` 目录下的 `configs.vh`，具体可修改的内容包括：

    -   `configs.hpp`

        -   对应 ISA 的 `MemoryConfigs`：内存配置，包括指令和数据内存的起始地址、大小和初始化文件路径，一般使用默认值即可；

        -   `Configs::isa_type`：使用的 ISA，可选项为 `IsaType::RISC_V` 和 `IsaType::LOONGARCH`；

        -   `Configs::core_type`：核心的类型，可选项为 `CoreType::SIMPLE` 和 `CoreType::COMPLETE`，对应简单单周期和两个完整核心的实验；

        -   `Configs::difftest_level`：对拍测试的等级，可选项为 `DifftestLevel::NONE`、`DifftestLevel::COMMIT` 和 `DifftestLevel::FULL`，分别对应无对拍测试、只对比提交内容和对比完整 CPU 状态；

            对拍测试能帮助你快速定位到 CPU 设计的问题，但对比完整 CPU 状态会使运行速度大幅下降。检查时需开启对比完整 CPU 状态，并运行通过；

        -   `dump_waveform`：是否开启波形图导出，设置为 `true` 会导出波形图。导出波形图会大幅降低仿真运行速度，并可能会生成很大的波形图文件，请谨慎决定是否开启；

        -   `dump_path`：导出波形图的文件路径，使用相对路径和绝对路径均可。

    -   `configs.vh`

        -   `INSTR_MEM_*` 和 `DATA_MEM_*`：内存配置，和 `configs.hpp` 中对应 ISA 的配置保持一致即可~~，虽然直接使用默认值大概率也不会出问题~~；

        -   `CORE_TYPE`：核心类型（和 `configs.hpp` 里面的不是一回事），可选项为 `SINGLE_CYCLE` 和 `PIPELINE`，对应单周期 CPU 和流水线 CPU。

4.  使用如下命令编译：

    ``` shell
    $ cmake -B build && cmake --build build
    ```

    如果编译出现与 `fmt` 库相关的报错，请使用 `sudo apt install libfmt-dev` 命令安装 `fmt` 库。

    **注意：每次修改 Verilog 源文件，或者修改配置项都需要重新编译！以及最新版的 verilator 可能有问题，如果发现修改了 Verilog 源文件后 `generated` 目录未更新，请删除该目录重新编译。**

5.  编译出的可执行文件在 build 目录下，使用如下命令运行：

    ``` shell
    $ ./build/sim
    ```