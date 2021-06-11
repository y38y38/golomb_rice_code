module golomb_rice_code(
	input clk,
	input reset_n,
//	input logic input_enable,
	input [2:0] k,

	//本当は19bitで足りるが、本関数の処理上桁溢れする可能性があるので、
	//1bit多く用意しておく。
	input [19:0] input_data,
	output reg [23:0] output_enable,//mask
	output reg [23:0] sum,
	output wire [31:0] Q,
	output wire [5:0] CODEWORD_LENGTH

);
assign Q = q;
function [23:0] bitmask;
	input [31:0] val;
	reg [5:0] index = 6'h0;
	begin
		bitmask = 24'h1;
		for(index=1;index<val;index=index+1) begin
			bitmask = (bitmask<<1) | 1;
		end
	end
endfunction

reg [31:0] q = 32'h0;
reg [31:0] code_length = 32'h0;
assign CODEWORD_LENGTH = code_length;


always @(posedge clk, negedge reset_n) begin
	if (!reset_n) begin
		output_enable = 24'h0;
		sum = 24'h0;
		code_length = 32'h0;
	end else begin
		q = input_data >> k;
		if (k==0) begin
			if(q!=0) begin
				sum = 1;
				code_length = q+1;
				output_enable = bitmask(code_length);
			end else begin
				sum = 1;
				code_length = 1;
				output_enable = 1;
			end
		end else begin
			sum = (1<<k) | (input_data & ((1<<k) - 1));
			code_length = q + 1 + k;
			output_enable = bitmask( code_length);	
		end
	end
end

endmodule;
