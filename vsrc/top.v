`include "configs.vh"

module
    Top (
        // basic ports
        input                   [ 0 : 0]                    clk,
        input                   [ 0 : 0]                    rst,

        // ports for simulation and difftest
        output                  [ 0 : 0]                    commit,
        output                  [ 0 : 0]                    commit_halt,

        // for difftest
        output                  [31 : 0]                    commit_pc, 
        output                  [31 : 0]                    commit_instr,

        // compare commit
        output                  [ 0 : 0]                    commit_reg_we,
        output                  [ 4 : 0]                    commit_reg_wa,
        output                  [31 : 0]                    commit_reg_wd,
        output                  [ 0 : 0]                    commit_dmem_we,
        output                  [`DATA_MEM_DEPTH - 1 : 0]   commit_dmem_wa,
        output                  [31 : 0]                    commit_dmem_wd,

        // compare full status
        input                   [ 4 : 0]                    debug_reg_ra,
        output                  [31 : 0]                    debug_reg_rd,
        input                   [`DATA_MEM_DEPTH - 1 : 0]   debug_dmem_ra,
        output                  [31 : 0]                    debug_dmem_rd
    );

    wire [31 : 0] pc;
    wire [31 : 0] instr;

    wire [31 : 0] dmem_addr;
    wire [31 : 0] dmem_rdata;
    wire [31 : 0] dmem_wdata;
    wire [ 0 : 0] dmem_we;

    wire [31 : 0] imem_paddr;
    wire [31 : 0] dmem_paddr;
    wire [31 : 0] commit_dmem_wa_raw;
    wire [31 : 0] commit_dmem_pwa;

    assign imem_paddr = pc - `INSTR_MEM_START;
    assign dmem_paddr = dmem_addr - `DATA_MEM_START;
    assign commit_dmem_pwa = commit_dmem_wa_raw - `DATA_MEM_START;
    assign commit_dmem_wa = commit_dmem_pwa[`DATA_MEM_DEPTH + 1 : 2];

    CPU cpu(
        .clk                    (clk),
        .rst                    (rst),
        .global_en              (1'B1),

        .imem_raddr             (pc),
        .imem_rdata             (instr),

        .dmem_addr              (dmem_addr),
        .dmem_rdata             (dmem_rdata),
        .dmem_wdata             (dmem_wdata),
        .dmem_we                (dmem_we),

        .commit                 (commit),
        .commit_pc              (commit_pc),
        .commit_instr           (commit_instr),
        .commit_halt            (commit_halt),
        .commit_reg_we          (commit_reg_we),
        .commit_reg_wa          (commit_reg_wa),
        .commit_reg_wd          (commit_reg_wd),
        .commit_dmem_we         (commit_dmem_we),
        .commit_dmem_wa         (commit_dmem_wa_raw),
        .commit_dmem_wd         (commit_dmem_wd),

        .debug_reg_ra           (debug_reg_ra),
        .debug_reg_rd           (debug_reg_rd)
    );

    InstrMem instr_mem(
        .a                      (imem_paddr[`INSTR_MEM_DEPTH + 1 : 2]),
        .spo                    (instr)
    );

    DataMem data_mem(
        .a                      (dmem_paddr[`DATA_MEM_DEPTH + 1 : 2]),
        .d                      (dmem_wdata),
        .clk                    (clk),
        .we                     (dmem_we),
        .spo                    (dmem_rdata),
        .debug_a                (debug_dmem_ra),
        .debug_spo              (debug_dmem_rd)
    );

endmodule