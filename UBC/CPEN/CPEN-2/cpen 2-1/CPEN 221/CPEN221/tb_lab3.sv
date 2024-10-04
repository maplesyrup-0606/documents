module tb_lab3();

	reg [9:0] sim_SWITCH;
	reg [3:0] sim_KEY_buttons;
	wire [6:0] sim_HEX0;
	wire [6:0] sim_HEX1;
	wire [6:0] sim_HEX2;
	wire [6:0] sim_HEX3;
	wire [6:0] sim_HEX4;
	wire [9:0] sim_LEDR;

	lab3 DUT (
		.SW(sim_SWITCH),
		.KEY(sim_KEY_buttons),
		.HEX0(sim_HEX0),
		.HEX0(sim_HEX1),
		.HEX0(sim_HEX2),
		.HEX0(sim_HEX3),
		.HEX0(sim_HEX4),
		.HEX0(sim_HEX5),
		.LEDR(sim_LEDR)
	);

	initial begin
		/* open lock */
	// initial setup
		sim_SWITCH[3:0] = 4'b0000;

		sim_KEY_buttons[0] = 1'b1;
		sim_KEY_buttons[3] = 1'b1;

		#5;

	// reset 
		sim_KEY_buttons[3] = 1'b0;
		#5;
		sim_KEY_buttons[0] = 1'b0;

	// enter first digit
		sim_SWITCH[3:0] = 4'b0101;

        
		// check if the hex is updated
        assert(HEX0 === 7'b0010010) $display("[PASS] HEX0 is 5");
        else $error("[FAIL] HEX0 should be 5")
		// delay and press clock
		#5;
		sim_KEY_buttons[0] = 1'b0
		#5;


	// enter second digit
		sim_SWITCH[3:0] = 4'b0001;

		// check if the hex is updated
        assert(HEX1 === 7'b1111001) $display("[PASS] HEX1 is 1");
        else $error("[FAIL] HEX1 should be 1");
		// delay and press clock
		#5;
		sim_KEY_buttons[0] = 1'b0
		#5;


	// enter third digit
		sim_SWITCH[3:0] = 4'b0001;

		// check if the hex is updated
        assert(HEX2 === 7'b1111001) $display("[PASS] HEX2 is 1");
        else $error("[FAIL] HEX2 should be 1");
		// delay and press clock
		#5;
		sim_KEY_buttons[0] = 1'b0
		#5;


	// enter fourth digit
		sim_SWITCH[3:0] = 4'b0111;

		// check if the hex is updated
        assert(HEX3 === 7'b1111000) $display("[PASS] HEX3 is 7");
        else $error("[FAIL] HEX3 should be 7");
		// delay and press clock
		#5;
		sim_KEY_buttons[0] = 1'b0
		#5;


	// enter fifth digit
		sim_SWITCH[3:0] = 4'b1000;

		// check if the hex is updated
        assert(HEX4 === 7'b0011001) $display("[PASS] HEX4 is 4");
        else $error("[FAIL] HEX4 should be 4");
		// delay and press clock
		#5;
		sim_KEY_buttons[0] = 1'b0
		#5;


	// enter sixth digit
		sim_SWITCH[3:0] = 4'b0100;

		// check if the hex is updated
        assert(HEX5 === 7'b0000000) $display("[PASS] HEX5 is 8");
        else $error("[FAIL] HEX5 should be 8");
		// delay and press clock
		#5;
		sim_KEY_buttons[0] = 1'b0
		#5;


	end

endmodule: tb_lab3
