# Proximity Attack for 3D ICs with Obfuscated F2F Mappings

This is a simple proximity attack, as proposed in the following paper: *Patnaik, S.; Ashraf, M.; Sinanoglu, O. & Knechtel, J. Best of Both
Worlds: Integration of Split Manufacturing and Camouflaging into a Security-Driven CAD Flow for 3D ICs Proc. Int. Conf. Comp.-Aided Des.,
	2018*

Please cite our paper when you make use of the attack in any way.
We provide the source code and various sample files.  The latter are for ISCAS85 and ITC99 benchmarks, and we already provide all the
possible F2F mappings which an attacker would be able to infer from the obfuscated layouts as advocated in our paper.
Note that, being sample files, the true connectivity could
be easily inferred from the names of the nets/ports. However, these names are *not* used for the actual attack, only for calculation of
correct connection rates (CCR).  Beside, in reality, such clearly identifiable names would not be present.  As of now, the main heuristics
for the attack are to derive a netlist without inducing any combinatorial loops, without inducing false multi-driver scenarios, and
while resolving all obfuscated mappings.

The code/binary has been tested on two systems: a 64-bit Ubuntu 16.04 LTS, and a 64-bit CentOS 6.9.
To compile, default *C++-11* libraries are required as well as *pthread*.

After compilation, to invoke the attack, use something like the following:

	cd sample_files/ISCAS85/c432
	../../../3D_F2F_Proximity_Attack top.v bottom.v wrapper.v mappings cells.inputs cells.outputs out.v

Note that the files *cells.inputs* and *cells.outputs* define all the input/output pins of the cells used when generating the layouts. In
our case for the provided sample files, the *NanGate 45nm Open Cell Library* was used.

For runs on larger benchmarks or benchmarks with more complex F2F mappings (e.g., c6288), you can provide another, final parameter, namely
the number of threads for running the attack in parallel:

	../../../3D_F2F_Proximity_Attack top.v bottom.v wrapper.v mappings cells.inputs cells.outputs out.v 16

The output is something like this:

	Proximity attack on F2F interconnects camouflaging
	--------------------------------------------------
	
	IO> Parsing the cells and their inputs ...
	IO> Done
	IO>  Cells: 133
	IO>
	IO> Parsing the cells and their outputs ...
	IO> Done
	IO>  Cells: 133
	IO>
	IO> Parsing the wrapper netlist for the regular ports, not the F2F ports ...
	IO> Done
	IO>  Regular input ports: 36
	IO>  Regular output ports: 7
	IO>
	IO> Parsing the netlist of the bottom tier ...
	IO> Done
	IO>  Total F2F ports (inputs/outputs): 73
	IO>  Total wires: 20
	IO>  Total gates: 61
	IO> Note that counts also contain the instances from the parsing of the other netlist, if done already
	IO>
	IO> Parsing the netlist of the top tier ...
	IO> Done
	IO>  Total F2F ports (inputs/outputs): 147
	IO>  Total wires: 42
	IO>  Total gates: 121
	IO> Note that counts also contain the instances from the parsing of the other netlist, if done already
	IO>
	IO> Parsing the obfuscated F2F mappings ...
	IO> Done
	IO>  F2F outputs in bottom tier (there may be POs among them): 41
	IO>   All the mapping candidates from bottom to top tier: 161
	IO>  F2F outputs in top tier (there may be POs among them): 34
	IO>   All the mapping candidates from top to bottom tier: 130
	IO>
	Attack> Trials so far: 1
	Attack> Trials so far: 2
	Attack> Trials so far: 3
	Attack>
	Attack> Success! Found F2F assignment without cycles
	Attack>
	Attack> Writing out netlist ...
	Attack> Done
	Attack>
	Attack> Evaluating connections:
	Attack>  Correct connections: 29
	Attack>  Total connections: 75
	Attack>  Correct connections ratio: 0.386667

In general, the attack will run until some netlist can be inferred, then save this netlist, and terminate. For large or complex benchmarks,
it may take many trials, which the attack handles by itself, by leveraging some randomness when picking F2F mappings during the various trials.
