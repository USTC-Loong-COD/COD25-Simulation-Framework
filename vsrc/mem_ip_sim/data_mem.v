`include "configs.vh"

module 
    CacheOneCycle #(
        parameter WIDTH = 32
    )(
        input                                               clk,
        input           [WIDTH - 1 : 0]                     signal,
        output  reg     [WIDTH - 1 : 0]                     cached_signal
    );

    always @(posedge clk) begin
        cached_signal <= signal;
    end

endmodule

module 
    DataMem (
        input           [`DATA_MEM_DEPTH - 1 : 0]           a,
        input           [31 : 0]                            d,
        input                                               clk,
        input                                               we,
        output          [31 : 0]                            spo,

        input           [`DATA_MEM_DEPTH - 1 : 0]           debug_a,
        output          [31 : 0]                            debug_spo
    );

    reg                 [31 : 0]                            mem             [0 : (1 << `DATA_MEM_DEPTH) - 1];

    wire                                                    cached_we;
    wire                [`DATA_MEM_DEPTH - 1 : 0]           cached_a;
    wire                [31 : 0]                            cached_d;

    initial begin
        $readmemh(`DATA_MEM_INI, mem);
    end

    // cache we, a, d for one cycle if core type is pipeline
    // so that debug_spo will not read values written by next instruction
    generate
        if(`CORE_TYPE == `SINGLE_CYCLE) begin
            assign cached_we = we;
            assign cached_a = a;
            assign cached_d = d;
        end
        else begin
            CacheOneCycle #(
                .WIDTH(`DATA_MEM_DEPTH)
            ) cache_a (
                .clk(clk),
                .signal(a),
                .cached_signal(cached_a)
            );

            CacheOneCycle #(
                .WIDTH(32)
            ) cache_d (
                .clk(clk),
                .signal(d),
                .cached_signal(cached_d)
            );

            CacheOneCycle #(
                .WIDTH(1)
            ) cache_we (
                .clk(clk),
                .signal(we),
                .cached_signal(cached_we)
            );
        end
    endgenerate

    always @(posedge clk) begin
        if(cached_we) mem[cached_a] <= cached_d;
    end

    // only when core type is pipeline we forward cached values
    // to avoid a combination loop
    generate
        if(`CORE_TYPE == `SINGLE_CYCLE) begin
            assign spo = mem[a];
        end
        else begin
            assign spo = (cached_we && (cached_a == a)) ? cached_d : mem[a];
        end
    endgenerate

    assign debug_spo = mem[debug_a];

endmodule