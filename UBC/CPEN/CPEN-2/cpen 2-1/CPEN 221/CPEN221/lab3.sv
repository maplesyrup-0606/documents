module lab3(input [9:0] SW, input [3:0] KEY,
            output [6:0] HEX0, output [6:0] HEX1, output [6:0] HEX2,
            output [6:0] HEX3, output [6:0] HEX4, output [6:0] HEX5,
            output [9:0] LEDR);
    wire clk = ~KEY[0]; // this is your clock
    wire rst_n = KEY[3]; // this is your reset; your reset should be synchronous and active-low

    // YOUR SOLUTION HERE

    reg[6:0] HEXX0;
    reg[6:0] HEXX1;
    reg[6:0] HEXX2;
    reg[6:0] HEXX3;
    reg[6:0] HEXX4;
    reg[6:0] HEXX5;
    
    assign HEX0 = HEXX0;
    assign HEX1 = HEXX1;
    assign HEX2 = HEXX2;
    assign HEX3 = HEXX3;
    assign HEX4 = HEXX4;
    assign HEX5 = HEXX5;
    

//state enumerating
    `define R1 4'b0000
    `define R2 4'b0001
    `define R3 4'b0010
    `define R4 4'b0100 
    `define R5 4'b1000
    `define R6 4'b1001
    `define RO 4'b0011
    `define W1 4'b1010
    `define W2 4'b1100
    `define W3 4'b1101
    `define W4 4'b1111
    `define W5 4'b0110
    `define WC 4'b0101


    reg[3:0] state;


//light defining
    `define L1 7'b1111001
    `define L2 7'b0100100
    `define L3 7'b0110000
    `define L4 7'b0011001
    `define L5 7'b0010010
    `define L6 7'b0000010
    `define L7 7'b1111000
    `define L8 7'b0000000
    `define L9 7'b0011000
    `define LE 7'b0000110
    `define LR 7'b0101111
    `define LO 7'b1000000
    `define LE 7'b0000110
    `define Ln 7'b1001000
    `define LC 7'b1000110
    `define LL 7'b1000111
    `define LD 7'b0100001
    `define LP 7'b0001100
    `define LN 7'b1111111

//statemachine
    always_ff @( posedge clk ) begin 

        if(~rst_n) begin
            state <= `R1;
        end else begin
            casex({SW[3:0],state})
            {4'b0101,`R1}: state<=`R2;
            {4'b0001,`R2}: state<=`R3;
            {4'b0001,`R3}: state<=`R4;
            {4'b0111,`R4}: state<=`R5;
            {4'b0100,`R5}: state<=`R6;
            {4'b1000,`R6}: state<=`RO;
            {4'bxxxx,`R1}: state<=`W1;
            {4'bxxxx,`R2}: state<=`W2;
            {4'bxxxx,`R3}: state<=`W3;
            {4'bxxxx,`R4}: state<=`W4;
            {4'bxxxx,`R5}: state<=`W5;
            {4'bxxxx,`R6}: state<=`WC;
            {4'bxxxx,`W1}: state<=`W2;
            {4'bxxxx,`W2}: state<=`W3;
            {4'bxxxx,`W3}: state<=`W4;
            {4'bxxxx,`W4}: state<=`W5;
            {4'bxxxx,`W5}: state<=`WC;
            endcase
            
    end
    end


    

//output logic / we need HEX to depend on some state
    always_comb begin
        case(state)
        `RO : {HEXX5,HEXX4,HEXX3,HEXX2,HEXX1,HEXX0} = {`LN,`LN,`LO,`LP,`LE,`Ln};
        `WC : {HEXX5,HEXX4,HEXX3,HEXX2,HEXX1,HEXX0} = {`LC,`LL,`LO,`L5,`LE,`LD};
        default: begin
        case(SW[3:0])
        4'd0 : begin
               HEXX0 = `LO;
               HEXX1 = `LN;
               HEXX2 = `LN;
               HEXX3 = `LN;
               HEXX4 = `LN;
               HEXX5 = `LN;
        end
        4'd1 : begin
               HEXX0 = `L1;
               HEXX1 = `LN;
               HEXX2 = `LN;
               HEXX3 = `LN;
               HEXX4 = `LN;
               HEXX5 = `LN;
        end
        4'd2 : begin 
               HEXX0 = `L2;
               HEXX1 = `LN;
               HEXX2 = `LN;
               HEXX3 = `LN;
               HEXX4 = `LN;
               HEXX5 = `LN;
        end
        4'd3 : begin 
               HEXX0 = `L3;
               HEXX1 = `LN;
               HEXX2 = `LN;
               HEXX3 = `LN;
               HEXX4 = `LN;
               HEXX5 = `LN;
        end
        4'd4 : begin
               HEXX0 = `L4;
               HEXX1 = `LN;
               HEXX2 = `LN;
               HEXX3 = `LN;
               HEXX4 = `LN;
               HEXX5 = `LN;
        end
        4'd5 : begin
               HEXX0 = `L5;
               HEXX1 = `LN;
               HEXX2 = `LN;
               HEXX3 = `LN;
               HEXX4 = `LN;
               HEXX5 = `LN;
        end
        4'd6 : begin 
               HEXX0 = `L6;
               HEXX1 = `LN;
               HEXX2 = `LN;
               HEXX3 = `LN;
               HEXX4 = `LN;
               HEXX5 = `LN;
        end
        4'd7 : begin 
               HEXX0 = `L7;
               HEXX1 = `LN;
               HEXX2 = `LN;
               HEXX3 = `LN;
               HEXX4 = `LN;
               HEXX5 = `LN;
        end 
        4'd8 : begin 
               HEXX0 = `L8;
               HEXX1 = `LN;
               HEXX2 = `LN;
               HEXX3 = `LN;
               HEXX4 = `LN;
               HEXX5 = `LN;
        end
        4'd9 : begin 
               HEXX0 = `L9;
               HEXX1 = `LN;
               HEXX2 = `LN;
               HEXX3 = `LN;
               HEXX4 = `LN;
               HEXX5 = `LN;
        end
        default : {HEXX5,HEXX4,HEXX3,HEXX2,HEXX1,HEXX0} = {`LN,`LE,`LR,`LR,`LO,`LR};
        endcase
        end
        endcase
    end

endmodule: lab3


