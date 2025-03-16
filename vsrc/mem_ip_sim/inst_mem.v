`include "configs.vh"

module
    InstrMem(
        input           [`INSTR_MEM_DEPTH - 1 : 0]           a,
        output          [31 : 0]                            spo
    );

    reg                 [31 : 0]                            mem             [0 : (1 << `INSTR_MEM_DEPTH) - 1];

    initial begin
        $readmemh(`INSTR_MEM_INI, mem);
    end

    assign spo = mem[a];

endmodule