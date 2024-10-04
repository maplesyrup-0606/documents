module question2(state, odd, even, terminal, pause, restart, clk, rst, goto_third, Out1, Out2);

input pause, restart clk, rst, goto_third;
output [1:0] state;
output [2:0] Out1, Out2;
output odd, even, terminal;

parameter [2:0] FIRST = 3'b000;
parameter [2:0] SECOND = 3'b001;
parameter [2:0] THIRD = 3'b010;
parameter [2:0] FOURTH = 3'b011;
parameter [2:0] FIFTH = 3'b100;

always_ff @(posedge clk or posedge rst) begin

    if(rst) state <= FIRST;
    else begin
        case(state)
            FIRST : if(restart | pause) state <= FIRST;
                    else state <= SECOND;
            SECOND : if(restart) state <= FIRST;
                     else if(!restart & !pause) state <= THIRD;
                     else if(!restart & pause) state <= SECOND;
            THIRD : if(restart) state <= FIRST;
                    else if(!restart & !pause) state <= FOURTH;
                    else if(!restart & pause) state <= THIRD;
            FOURTH : if(restart) state <= FIRST;
                     else if(!restart & !pause) state <= FIFTH;
                     else if(!restart & pause) state <= FOURTH;
            FIFTH : if(restart & !goto_third) state <= FIRST;
                    else if(goto_third) state <= THIRD;
                    else if(!restart & pause & !goto_third) state <= FIFTH;
            default : state <= FIRST;
        endcase
    end
end

always_comb begin
    case(state)
        FIRST : begin
            terminal = 0;
            Out1 = 3'd3;
            Out2 = 3'd2;
            even = 0;
            odd = 1;
        end
        SECOND : begin
            terminal = 0;
            Out1 = 3'd5;
            Out2 = 3'd4;
            even = 1;
            odd = 0;
        end
        THRID : begin
            terminal = 0;
            Out1 = 3'd2;
            Out2 = 3'd7;
            even = 0;
            odd = 1;
        end
        FOURTH : begin
            terminal = 0;
            Out1 = 3'd6;
            Out2 = 3'd3;
            even = 1;
            odd = 0;
        end
        FIFTH : begin
            terminal = 1;
            Out1 = 3'd5;
            Out2 = 3'd2;
            even = 0;
            odd = 1;
        end

        default : begin
            terminal = 0;
            Out1 = 3'd0;
            Out2 = 3'd0;
            even = 0;
            odd = 0;
        end
    endcase
end

endmodule