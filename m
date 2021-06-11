verilator --cc --exe  --trace-fst --trace-params --trace-structs --trace-underscore \
    golomb_rice_code.v -exe test_golomb_rice_code.cpp
make -C obj_dir -f Vgolomb_rice_code.mk

