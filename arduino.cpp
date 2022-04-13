// C++ code
//

//Variable to define the interval time (in milliseconds) of each operation
int INTERVAL_TIME;

//Used leds
int led1 = 13;
int led2 = 12;
int led3 = 11;
int led4 = 10;

char auxOperation[3];			//Store the hexa code in char format
int control;					//Store the current position to write in the array auxOperation
int controlOperation;			//Store the next position to WRITE the instruction in the internalMemory (Starts in 4)

int binaryResult[4];			//Store the binary of the result of the operation 

//Binary A and B values 
int aValue[4];
int bValue[4];

//Internal memory to control the operations to be executed
int internalMemory[100];

void setup()
{
  //Define the pin mode
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  
  //Makes the writing in the char array starts by the position 0
  control = 0;
  
  Serial.begin(9600);

  //Set each position of the arrays below to 0
  clearArray(binaryResult);
  clearArray(aValue);
  clearArray(bValue);
  
  INTERVAL_TIME = 2000;
  
  setupInternalMemory();
}

// Starts the memory array -> The first position indicates the next operation(Starts on 4), after that position, the other ones
//are -1 to the program knows when to stop executing an operation 
void setupInternalMemory(){
  	controlOperation=4;
	internalMemory[0] = 4;
  	for(int i=1; i<100; i++){
  		internalMemory[i] = -1;
  	}
  	
}

void loop()
{
  if(Serial.available() > 0) {
    char byte = Serial.read();
    if(byte != ' '){							//the blank space separates operations
      if(byte != 'G'){							//the program stop reading when the operation is 'GGG'
      	auxOperation[control] = byte;
    	control++;
      } else {
       	executeOperations();
        exit(0);
      }
    }
    else{
      //the next block of code set the char array pointer to the beggining of the array
      //it also transforms the three char received in a decimal number and stores it in the internalMemory
      //and, in the end, it moves one step foward the operation writer
      control = 0;
      internalMemory[controlOperation] = ((getNumber(auxOperation[0]) * 16 * 16) + (getNumber(auxOperation[1]) * 16) + getNumber(auxOperation[2]));
      controlOperation++;
	
    }
  }
}

//The following block of code does, respectively: (There's a delay of 5 seconds in each iteration)
// - Executes every operation stored in the internal memory until the first invalid one
// - Transforms the decimal number in hexadecimal ones 
// - Executes the operation stored in the decimal number
void executeOperations(){
  	while(internalMemory[internalMemory[0]] != -1){
      	int aux = internalMemory[internalMemory[0]];
      	int op = aux % 16;
      	aux/=16;
      	internalMemory[3] = aux % 16;
      	aux/=16;
      	internalMemory[2] = aux;
      	processOperation(op);
        internalMemory[0]++;
      	delay(INTERVAL_TIME);
    }
}

//Processes the data to be possible the execution of the operation
void processOperation(int currentOperation){
  	clearArray(binaryResult);
  	initializeValueArrays(internalMemory[2], internalMemory[3]);
  
  	toBinary(doOperation(internalMemory[2], internalMemory[3], currentOperation), binaryResult);
  	useLeds();
  	Serial.println();
  	Serial.print(binaryResult[0]);
  	Serial.print(" ");
  	Serial.print(binaryResult[1]);
  	Serial.print(" ");
  	Serial.print(binaryResult[2]);
  	Serial.print(" ");
  	Serial.print(binaryResult[3]);
  	Serial.println();
}

//Transforms a character into a hexadecimal number
int getNumber(char character){
  	return (character >= 48 && character <= 57) ? (((int) character) - 48) : (((int) character) - 55);
}

//Executes the operation represented by a number and returns the result of that operation
int doOperation(int a, int b, int operation){
  int valueReturn = 0;
  switch(operation){
  	case 0:
        notArray(aValue);
        valueReturn = toDecimal(aValue);
    break;
    case 1:
    	orAB(aValue, bValue);
        notArray(aValue);
        valueReturn = toDecimal(aValue);
    break;
    case 2:
        notArray(aValue);
        andAB(aValue, bValue);
        valueReturn = toDecimal(aValue);
    break;
    case 3:
    	valueReturn = 0;
    break;
    case 4:
    	andAB(aValue, bValue);
    	notArray(aValue);
    	valueReturn = toDecimal(aValue);
    break;
    case 5:
    	notArray(bValue);
    	valueReturn = toDecimal(bValue);
    break;
    case 6:
    	xorAB(aValue, bValue);
    	valueReturn = toDecimal(aValue);
    break;
    case 7:
    	notArray(bValue);
    	andAB(aValue, bValue);
    	valueReturn = toDecimal(aValue);
    break;
    case 8:
    	notArray(aValue);
    	orAB(aValue, bValue);
    	valueReturn = toDecimal(aValue);
    break;
    case 9:
    	xorAB(aValue, bValue);
    	notArray(aValue);
   		valueReturn = toDecimal(aValue);
    break;
    case 10:
    	valueReturn = b;
    break;
    case 11:
    	andAB(aValue, bValue);
    	valueReturn = toDecimal(aValue);
    break;
    case 12:
    	valueReturn = 1;
    break;
    case 13:
    	notArray(bValue);
    	orAB(aValue, bValue);
    	valueReturn = toDecimal(aValue);
    break;
    case 14:
    	orAB(aValue, bValue);
    	valueReturn = toDecimal(aValue);
    break;
    case 15:
    	valueReturn = a;
    break;
  }
  internalMemory[1] = valueReturn;
  return valueReturn;
}

//Represents a decimal number in a binary way in an array
void toBinary(int number, int array[4]){
  int pos = 3;
  while (number != 0){
  	array[pos] = ( number % 2 == 0 ? 0 : 1 );
    number /= 2;
    --pos;
  }
}

//Represents a binary value stored in an array in a decimal way
int toDecimal(int array[4]){
  int value = 0, i=8, j=0;
  for(; i>=1; i >>= 1, j++){
  	value += array[j] == 0 ? 0 : array[j]*i;
  }
  return value;
}

//Switchs the leds according to the binary number stored in the result array 
void useLeds(){
	digitalWrite(led1, binaryResult[0] == 0 ? LOW : HIGH);
  	digitalWrite(led2, binaryResult[1] == 0 ? LOW : HIGH);
  	digitalWrite(led3, binaryResult[2] == 0 ? LOW : HIGH);
  	digitalWrite(led4, binaryResult[3] == 0 ? LOW : HIGH);
}

//Sets each element of an array to 0
void clearArray(int array[4]){
  array[0] = 0x0;
  array[1] = 0x0;
  array[2] = 0x0;
  array[3] = 0x0;
}

//Inverts the binary number (0 -> 1 / 1 -> 0)
void notArray(int array[4]){
  for(int i=0; i<4; i++){
  	array[i] = array[i] == 0 ? 1 : 0;
  }
}

/*
	* The next three methods work the same as the logic ports AND, OR and XOR, respectively 
*/
void andAB(int arrayA[4], int arrayB[4]){
  for(int i=0; i<4; i++){
  	arrayA[i] = arrayA[i] & arrayB[i];
  }
}

void orAB(int arrayA[4], int arrayB[4]){
  for(int i=0; i<4; i++){
  	arrayA[i] = arrayA[i] | arrayB[i];
  }
}

void xorAB(int arrayA[4], int arrayB[4]){
  for(int i=0; i<4; i++){
  	arrayA[i] = arrayA[i] ^ arrayB[i];
  }
}

//Sets the array according the binary number of A and B
void initializeValueArrays(int a, int b){
  clearArray(aValue);
  clearArray(bValue);
  toBinary(a, aValue);
  toBinary(b, bValue);
}












