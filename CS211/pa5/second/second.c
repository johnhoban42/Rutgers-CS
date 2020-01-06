#include "second.h"


int main(int argc, char** argv){

	// Open file
	FILE* f = fopen(argv[1], "r");

	// Read inputs from file
	char* var = malloc(20*sizeof(char));
	fscanf(f, "%s ", var); // "INPUTVAR"
	int input_count;
	fscanf(f, "%d ", &input_count);
	inputs = malloc(input_count*sizeof(Variable));
	for(int i = 0; i < input_count; i++){
		char* var_name = malloc(20*sizeof(char));
		fscanf(f, "%s ", var_name);
		inputs[i].name = var_name;
	}

	// Read outputs from file
	fscanf(f, "%s ", var); // "OUTPUTVAR"
	int output_count;
	fscanf(f, "%d ", &output_count);
	outputs = malloc(output_count*sizeof(Variable));
	for(int i = 0; i < output_count; i++){
		char* var_name = malloc(20*sizeof(char));
		fscanf(f, "%s ", var_name);
		outputs[i].name = var_name;
	}

	char* cmd = malloc(150*sizeof(char)); // holds gate types

    // Scan file to count number of gates
    int gate_count = 0;
    while(fscanf(f, "%[^\n]\n", cmd) == 1){
        gate_count++;
    }

    // Reset file pointer
    rewind(f);
    fscanf(f, "%[^\n]\n", cmd); // skip 1st line
    fscanf(f, "%[^\n]\n", cmd); // skip 2nd line

	for(int g = 0; g < power(input_count); g++){

		int graycode = (g >> 1) ^ g;

		// Store graycode values in INPUT variables
		// Variables are filled in reverse order
		for(int i = input_count-1; i >= 0; i--){
			inputs[i].value = graycode % 2;
			graycode = graycode >> 1;
		}

        // Indicates if the i-th gate has been evaluated
        int* seen = malloc(gate_count*sizeof(int));
        for(int i = 0; i < gate_count; i++){
            seen[i] = 0;
        }

        int done = 1;

		for(int gate = 0; gate < gate_count; gate++){

            // Skip the gate entirely if it's been evaluated already
            if(seen[gate]){
                fscanf(f, "%[^\n]\n", cmd);
                goto GATE_END;
            }

            fscanf(f, "%s ", cmd);

			// DECODER
			if(strcmp(cmd, "DECODER") == 0){

				// Get dimenstions
				int size;
				fscanf(f, "%d ", &size);

				int* d_inputs = malloc(size*sizeof(int));
				for(int j = 0; j < size; j++){
					char* var_name = malloc(20*sizeof(char));
					fscanf(f, "%s ", var_name);
					d_inputs[j] = get_variable(var_name);
                    if(d_inputs[j] == -1){
                        done = 0;
                        break;
                    }else if(j == size-1){ // all inputs are valid
                        seen[gate] = 1;
                    }
				}

                // If one of the inputs couldn't be fetched, skip the rest of
                // the line and evaluate the gate later
                if(!seen[gate]){
                    fscanf(f, "%[^\n]\n", cmd);
                    goto GATE_END;
                }

				// For total input value I, only the matching output value
				// gets a 1
				int* d_outputs = malloc(power(size)*sizeof(Variable));
				int output_index = 0;
				for(int i = 0; i < size; i++){
					output_index += d_inputs[i]*power(size-(i+1));
				}
				for(int i = 0; i < power(size); i++){
					if(((i >> 1) ^ i) == output_index){
						d_outputs[i] = 1;
						for(int j = i+1; j < power(size); j++){
							d_outputs[j] = 0;
						}
						break;
					}
					d_outputs[i] = 0;
				}

				// Store outputs
				for(int i = 0; i < power(size); i++){
					char* var_name = malloc(20*sizeof(char));
					fscanf(f, "%s ", var_name);
					set_variable(var_name, d_outputs[i]);
				}

			// MULTIPLEXER
			}else if(strcmp(cmd, "MULTIPLEXER") == 0){

				// Get dimensions
				int size;
				fscanf(f, "%d ", &size);

				int* m_inputs = malloc(size*sizeof(int));
				for(int j = 0; j < size; j++){
					char* var_name = malloc(20*sizeof(char));
					fscanf(f, "%s ", var_name);
					m_inputs[j] = get_variable(var_name);
                    if(m_inputs[j] == -1){
                        done = 0;
                        break;
                    }else if(j == size-1){ // all inputs are valid
                        seen[gate] = 1; // This is only intermediate to proceed
                    }                   // to scanning selectors
				}

                if(!seen[gate]){
                    fscanf(f, "%[^\n]\n", cmd);
                    goto GATE_END;
                }

				int* selectors = malloc(log_2(size)*sizeof(int));
				for(int j = 0; j < log_2(size); j++){
					char* var_name = malloc(20*sizeof(char));
					fscanf(f, "%s ", var_name);
					selectors[j] = get_variable(var_name);
                    if(selectors[j] == -1){
                        done = 0;
                        break;
                    }else if(j == log_2(size)-1){ // all inputs are valid
                        seen[gate] = 1;
                    }
                }

                if(!seen[gate]){
                    fscanf(f, "%[^\n]\n", cmd);
                    goto GATE_END;
                }

				// For total selector value I, the output value gets the value
				// of input value (I >> 1) ^ I
				char* o = malloc(20*sizeof(char));
				fscanf(f, "%s\n", o);
				int OUT1;
				int output_index = 0;
				for(int i = 0; i < log_2(size); i++){
					output_index += selectors[i]*power(log_2(size)-(i+1));
				}
				for(int i = 0; i < size; i++){
					if(((i >> 1) ^ i) == output_index){
						OUT1 = m_inputs[i];
						break;
					}
				}

				// Store output
				set_variable(o, OUT1);

			// NOT GATE
			}else if(strcmp(cmd, "NOT") == 0){

				// Scan gate input and output
				char* i1 = malloc(20*sizeof(char));
				char* o = malloc(20*sizeof(char));
				fscanf(f, "%s %s\n", i1, o);

				int IN1 = get_variable(i1);

                if(IN1 == -1){
                    done = 0;
                    goto GATE_END;
                }

                seen[gate] = 1;

				// NOT
				int OUT1 = IN1 ^ 1;

				// Store OUT1 in the appropraite variable
				set_variable(o, OUT1);


			// ALL BINARY GATES
			}else{

				// Scan 2 gate inputs and 1 gate output
				char* i1 = malloc(20*sizeof(char));
				char* i2 = malloc(20*sizeof(char));
				char* o = malloc(20*sizeof(char));
				fscanf(f, "%s %s %s\n", i1, i2, o);

				// Get input values. If the var name is in all caps, then it is an
				// initial INPUT value. Otherwise, it is a temp value. This
				// determines whether we fetch the value from the INPUT array or
				// the TEMP linked list.

				int IN1 = get_variable(i1);
				int IN2 = get_variable(i2);

                if(IN1 == -1 || IN2 == -1){
                    done = 0;
                    goto GATE_END;
                }

				seen[gate] = 1;

				int OUT1;

				// Assign a value to OUT1 based on the gate type
				if(strcmp(cmd, "AND") == 0){
					OUT1 = IN1 & IN2;
				}else if(strcmp(cmd, "OR") == 0){
					OUT1 = IN1 | IN2;
				}else if(strcmp(cmd, "NAND") == 0){
					OUT1 = (IN1 & IN2) ^ 1;
				}else if(strcmp(cmd, "NOR") == 0){
					OUT1 = (IN1 | IN2) ^ 1;
				}else if(strcmp(cmd, "XOR") == 0){
					OUT1 = IN1 ^ IN2;
				}else if(strcmp(cmd, "XNOR") == 0){
					OUT1 = (IN1 ^ IN2) ^ 1;
				}

				// Store OUT1 in the appropraite variable
				set_variable(o, OUT1);

			}

            // GATE_END: RESET IF NECESSARY
            // If not all gates have been seen, i.e. done == 0, reset the file
            // pointer to the first gate and reset the gate counter
            // This will be repeated UNTIL all gates are seen and done == 1
            GATE_END:
            if(gate == gate_count-1){
                if(!done){
                    rewind(f);
            		fscanf(f, "%[^\n]\n", cmd); // skip 1st line
            		fscanf(f, "%[^\n]\n", cmd); // skip 2nd line
                    gate = -1;
					done = 1;
                }
            }

		}

		// Print the graycode, along with the given result
		for(int i = 0; i < input_count; i++){
			printf("%d ", inputs[i].value);
		}
		for(int i = 0; i < output_count; i++){
			printf("%d ", outputs[i].value);
		}
		printf("\n");

		// Clear temp variables
		free_temps();

		// Reset file pointer to the start of the gates, i.e. the 3rd line
		rewind(f);
		fscanf(f, "%[^\n]\n", cmd); // skip 1st line
		fscanf(f, "%[^\n]\n", cmd); // skip 2nd line

	}

	return 0;

}
