
#include <iostream>
#include <verilated.h>
#include "Vgolomb_rice_code.h"
#include <verilated_fst_c.h> 



void golomb_rice_code(int32_t k, uint32_t val,  uint32_t *sum, int32_t *codeword_length)
{
    int32_t q  = val >> k;

    if (k ==0) {
        if (q != 0) {
			*sum = 1;
			*codeword_length = 1+q;
        } else {
			*sum = 1;
			*codeword_length = 1;
		}
    } else {
        uint32_t tmp = (k==0) ? 1 : (2<<(k-1));
        uint32_t r = val & (tmp -1 );

        *sum = (1 << k) | r;
		*codeword_length = q + 1+ k;
		printf("%d %d %d %d\n",k,val, *sum, *codeword_length);
    }
    return;
}


int time_counter = 0;

int main(int argc, char** argv) {

	Verilated::commandArgs(argc, argv);
	FILE *in = fopen(argv[1], "r");
	if (in==NULL) {
		printf("err\n");
	}
	// Instantiate DUT
	Vgolomb_rice_code *dut = new Vgolomb_rice_code();
	// Trace DUMP ON
	Verilated::traceEverOn(true);
	VerilatedFstC* tfp = new VerilatedFstC;

	dut->trace(tfp, 100);  // Trace 100 levels of hierarchy
	tfp->open("simx.fst");

	// Format
	dut->reset_n = 0;
	dut->clk = 0;

	// Reset Time
	while (time_counter < 10) {
		dut->eval();
		tfp->dump(time_counter);  // 波形ダンプ用の記述を追加
		time_counter++;
	}
	// Release reset
	dut->reset_n = 1;

	int state = 0;

	int k,val,codeword_length,sum;
	while (time_counter < 4000 && !Verilated::gotFinish()) {
		dut->clk = !dut->clk; // Toggle clock
		if (dut->clk) {
			fscanf(in, "%d,%d,%d,%d",&k, &val, &sum, &codeword_length);
//			if (state == 0) {
				dut->k = k;
				dut->input_data = val;
//				state = 1;
//			}
		}

		// Evaluate DUT
		dut->eval();
		if (dut->clk) {
//			uint32_t sum;
//		int32_t codeword_length;
//		golomb_rice_code(dut->k, dut->input_data, &sum, &codeword_length);
//			if (k==2) {
//				printf("q=%d %d %d\n",dut->Q,(dut->sum != sum) , (dut->CODEWORD_LENGTH != codeword_length));
//				printf("k=%d,input_data=%d sum=%d len=%d %x, sum=%d len=%d\n", 
//				dut->k, dut->input_data, dut->sum, dut->CODEWORD_LENGTH, dut->output_enable,  sum, codeword_length);
//			}
//			if ((dut->sum != sum) || (dut->CODEWORD_LENGTH != codeword_length)) {
				printf("q=%d %d %d\n",dut->Q,(dut->sum != sum) , (dut->CODEWORD_LENGTH != codeword_length));
				printf("k=%d,input_data=%d sum=%d len=%d %x, sum=%d len=%d\n", 
				dut->k, dut->input_data, dut->sum, dut->CODEWORD_LENGTH, dut->output_enable,  sum, codeword_length);
//				break;
//			}
		}
		tfp->dump(time_counter);  // 波形ダンプ用の記述を追加
		time_counter++;
//		if (time_counter > 12) {
//			break;
//		}
//		break;
	}

	dut->final();
	tfp->close(); 
}