#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

/*Circuit Variable that stores the name and value of that variable */
typedef struct circuit_variable{
  char name[21];
  unsigned int value;
  struct circuit_variable* next;
}circuit_variable;

/* Grey Code Variable that stores the grey code sequence and # of bits */
typedef struct grey_code_obj{
  unsigned int* grey_code_sequence;
  unsigned int num_of_bits;
}grey_code_obj;

/* Prototype */
unsigned int getValue(char*);
unsigned int not(unsigned int);
void setValue(unsigned int, char*);
unsigned int and(unsigned int, unsigned int);
unsigned int or(unsigned int, unsigned int);
unsigned int xor(unsigned int, unsigned int);

/*Creates the Grey Code and stores in an array */
grey_code_obj* grey_code_constructor(unsigned int num_of_bits){
  grey_code_obj* grey_code = malloc(sizeof(grey_code_obj));
  grey_code->num_of_bits = num_of_bits;

  unsigned int* grey_code_sequence = malloc(sizeof(unsigned int) * pow(2,num_of_bits));
  grey_code->grey_code_sequence = grey_code_sequence;
  
  unsigned int g;
  for(unsigned int i = 0; i < pow(2,num_of_bits); i++){
    g = i ^ (i>>1);
    grey_code_sequence[i] = g;
  }
  return grey_code;
}

/* Global Variables */
circuit_variable* input_variable;
circuit_variable* output_variable;
circuit_variable* temp_variable;//serves as the temp_variable of temp_variable linked list
unsigned int input_size;
unsigned int output_size;

/* Main Method */
int main(int argc, char **argv){
    //variable
    char gate[21];
    char name[21];
    char input1[21];
    char input2[21];
    char output[21];
    unsigned int temp, temp1, temp2, result;
    //Read in The File
    FILE *fp = fopen(argv[1], "r");
    /* Find Inputvar and Outputvar first */
    while(fscanf(fp, "%s", gate) != EOF){
        if(strcmp(gate, "INPUTVAR") == 0){//input gate
        /* Input comes first always */
            fscanf(fp, "%d ", &input_size);//size of input
            input_variable = malloc(sizeof(circuit_variable) * input_size);
            for(unsigned int i = 0; i < input_size; i++){
                fscanf(fp, "%s", name);
                strcpy(input_variable[i].name, name);
                input_variable[i].value = 2;
            }
            fscanf(fp, "%*[^\n]\n");
        /* Output comes second always */
        }else if(strcmp(gate, "OUTPUTVAR") == 0){//output gate */
            fscanf(fp, "%s", name);
            fscanf(fp, "%d ", &output_size);
            output_variable = malloc(sizeof(circuit_variable) * output_size);
            for(unsigned int i = 0; i < output_size; i++){
                fscanf(fp, "%s", name);
                strcpy(output_variable[i].name, name);
                output_variable[i].value = 2;
            }
            fscanf(fp, "%*[^\n]\n");
        }
    }
    rewind(fp);
    /* loop this part */
    grey_code_obj* input_grey_code = grey_code_constructor(input_size);
    for(unsigned int i = 0; i < pow(2, input_size); i++){//loops base on the number of grey code
        /* Feed the circuit the correct input */
        unsigned int grey_code_10 = input_grey_code->grey_code_sequence[i];
        for(unsigned int i = 0; grey_code_10 > 0; i++){
            input_variable[input_size-1-i].value = grey_code_10 & 1;
            grey_code_10 >>= 1;
        }
        for(unsigned int i = 0; i < input_size; i++){
            if(input_variable[i].value == 2){
                input_variable[i].value = 0;
            }
        }
        while(1){
            /* Building the circuit */
            while(fscanf(fp, "%s", gate) != EOF){
                if(strcmp(gate, "NOT") == 0){
                    fscanf(fp, "%s %s\n", input1, output);
                    temp1 = getValue(input1);
                    if(temp1 == 3){ continue; }
                    result = not(temp1);
                    setValue(result, output);

                }else if(strcmp(gate, "AND") == 0){
                    fscanf(fp, "%s %s %s\n", input1, input2, output);
                    temp1 = getValue(input1);
                    temp2 = getValue(input2);
                    if(temp1 == 3 || temp2 == 3){ continue; } 
                    result = and(temp1, temp2);
                    setValue(result, output);

                }else if(strcmp(gate, "OR") == 0){
                    fscanf(fp, "%s %s %s\n", input1, input2, output);
                    temp1 = getValue(input1);
                    temp2 = getValue(input2);
                    if(temp1 == 3 || temp2 == 3){ continue; } 
                    result = or(temp1, temp2);
                    setValue(result, output);

                }else if(strcmp(gate, "NAND") == 0){//call AND then NOT
                    fscanf(fp, "%s %s %s\n", input1, input2, output);
                    temp1 = getValue(input1);
                    temp2 = getValue(input2);
                    if(temp1 == 3 || temp2 == 3){ continue; } 
                    result = and(temp1, temp2);
                    result = not(result);
                    setValue(result, output);

                }else if(strcmp(gate, "NOR") == 0){
                    fscanf(fp, "%s %s %s\n", input1, input2, output);
                    temp1 = getValue(input1);
                    temp2 = getValue(input2);
                    if(temp1 == 3 || temp2 == 3){ continue; } 
                    result = or(temp1, temp2);
                    result = not(result);
                    setValue(result, output);

                }else if(strcmp(gate, "XOR") == 0){
                    fscanf(fp, "%s %s %s\n", input1, input2, output);
                    temp1 = getValue(input1);
                    temp2 = getValue(input2);
                    if(temp1 == 3 || temp2 == 3){ continue; } 
                    result = xor(temp1, temp2);
                    setValue(result, output);

                }else if(strcmp(gate, "XNOR") == 0){
                    fscanf(fp, "%s %s %s\n", input1, input2, output);
                    temp1 = getValue(input1);
                    temp2 = getValue(input2);
                    if(temp1 == 3 || temp2 == 3){ continue; } 
                    result = xor(temp1, temp2);
                    result = not(result);
                    setValue(result, output);

                }else if(strcmp(gate, "DECODER") == 0){
                    unsigned int decoder_input_size;
                    fscanf(fp, "%d ", &decoder_input_size);
                    unsigned int decoder_output_size =  1 << decoder_input_size;//Get the size of output by raising 2 to the power of input size
                    /* Reading in Input and generating number to compare against grey code*/
                    temp = 0;//the generated value
                    /*Which Output gets a 1 starting with the MSB*/
                    int notReady = 0;
                    for(unsigned int i = 0; i < decoder_input_size; i++){
                        fscanf(fp, "%s", input1);//the input value
                        temp1 = getValue(input1);
                        if(temp1 == 3){ 
                            notReady = 1;
                            break;
                        }
                        temp += getValue(input1) * pow(2, decoder_input_size-1-i);
                    }
                    if(notReady){continue;}
                    /* Generate Grey code */
                    grey_code_obj* decode_grey_code = grey_code_constructor(decoder_input_size);

                    /* Saving Output */
                    for(unsigned int i = 0; i < decoder_output_size; i++){
                        fscanf(fp, "%s", output);
                        if(decode_grey_code->grey_code_sequence[i] == temp){
                                setValue(1, output);
                        }else{
                                setValue(0, output);
                            }
                        }
                    fscanf(fp, "%*[^\n]\n"); 

                }else if(strcmp(gate, "MULTIPLEXER") == 0){
                    unsigned int multiplexer_input_size;
                    fscanf(fp, "%d ", &multiplexer_input_size);
                    unsigned int selector_size = 0;
                    unsigned int temp = multiplexer_input_size;
                    while(temp >>= 1) selector_size++; //log2 of multiplexer_input_size
                    grey_code_obj* grey_code = grey_code_constructor(selector_size);//Generate Gray Code
                    unsigned int* input_arr = malloc(sizeof(unsigned int) * multiplexer_input_size);
                    /* Input */
                    int notReady = 0;
                    for(unsigned int i = 0; i < multiplexer_input_size; i++){
                        fscanf(fp, "%s", input1);
                        temp = getValue(input1);
                        if(temp == 3){
                            notReady = 1;
                            break;
                        }
                        input_arr[i] = temp;
                    }
                    if(notReady){ continue; }
                    /* Selector */
                    temp = 0;
                    for(unsigned int i = 0; i < selector_size; i++){
                        fscanf(fp, "%s", input1);//the input value
                            temp1 = getValue(input1);
                            if(temp1 == 3){
                                notReady = 1;
                                break;
                            }
                            temp += temp1 * pow(2, selector_size-1-i);
                    }
                    if(notReady == 1){
                        continue;
                    }
                    /* Output */
                    fscanf(fp, "%s", output);
                    for(unsigned int i = 0; i < multiplexer_input_size; i++){
                        if(grey_code->grey_code_sequence[i] == temp){
                            setValue(input_arr[i], output);
                        }
                    }
                    fscanf(fp, "%*[^\n]\n");
                }
            }
            for(int i = 0; i < output_size; i++){
                if(output_variable[i].value == 2){
                    rewind(fp);
                    continue;
                }
            }
            break;
        }
        /* Print Result for this particular Grey Code Sequence */
        for(unsigned int i = 0; i < input_size; i++){
            printf("%d ", input_variable[i].value);
        }
        for(unsigned int i = 0; i < output_size; i++){
            printf("%d ", output_variable[i].value);
        }
        printf("\n");
        /* Clear our Input I believe leaving temp and output the same is fine */
        for(unsigned int i = 0; i < input_size; i++){
            input_variable[0].value = 2;
        }
        for(unsigned int i = 0; i < input_size; i++){
            output_variable[0].value = 2;
        }
        rewind(fp);//Point fp to the beginning of circuit file
        fscanf(fp, "%*[^\n]\n");//skip inputvar
        fscanf(fp, "%*[^\n]\n");//skip outputvar
    }
    
    fclose(fp);
    return 0;
    /*Free Input, Output, Temp, and Grey Code Object */
  }
  unsigned int not(unsigned int input){
    return input ^ 1;
  }
  unsigned int and(unsigned int input1, unsigned int input2){
    return input1 & input2;
  }
  unsigned int or(unsigned int input1,unsigned int input2){
    return input1 | input2;
  }
  unsigned int xor(unsigned int input1,unsigned int input2){
    return input1 ^ input2;
  }
  void insert(char* name, unsigned int value)
{
    circuit_variable *ptr = temp_variable;
    circuit_variable *prev = NULL;
    /* Constructor */
    circuit_variable *temp = malloc(sizeof(circuit_variable));
    strcpy(temp->name, name);
    temp->value = value;
    temp->next = NULL;

    if (ptr == NULL)
    { //linked list is empty
        temp_variable = temp;
        return;
    }
    while (ptr != NULL)
    { //the general case
        if (strcmp(ptr->name, name) == 0)
        {
            ptr->value = value;
            free(temp);
            return;
        }
        prev = ptr;
        ptr = ptr->next;
    }
    if(ptr == NULL){
        prev->next = temp;
    }
    return;
}
  unsigned int getValue(char* name){
    if(strcmp(name, "1") == 0){
      return 1;
    }else if(strcmp(name, "0") == 0){
      return 0;
    }else{
      /* Searching within the Inputs */
      for(unsigned int i = 0; i < input_size; i++){
      if(strcmp(input_variable[i].name, name) == 0){
        return input_variable[i].value;
      }
    }
    /* Searching within the Outputs */
    for(unsigned int i = 0; i < output_size; i++){
      if(strcmp(output_variable[i].name, name) == 0){
        return output_variable[i].value;
      }
    }
    circuit_variable* ptr = temp_variable;
    while(ptr != NULL){
      if(strcmp(ptr->name, name) == 0){
        return ptr->value;
      }
      ptr = ptr->next;
    }
    return 3;
    }
  }
  void setValue(unsigned int value, char* name){
    /* Searching within the Inputs */
    for(unsigned int i = 0; i < input_size; i++){
      if(strcmp(input_variable[i].name, name) == 0){
        input_variable[i].value = value;
        return;
      }
    }
    /* Searching within the Outputs */
    for(unsigned int i = 0; i < output_size; i++){
      if(strcmp(output_variable[i].name, name) == 0){
        output_variable[i].value = value;
        return;
      }
    }
    /* Searching within the Temporary */
    insert(name, value);
  }



