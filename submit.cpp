/*
Homework 5 : The Game of Life.
Team member 1 : Vegar Engen 
Team member 2 : Arne Bjune
*/

#include "life.h"
#include "math.h"

unsigned int size;
unsigned char *array;
unsigned char *array2;

int superValue;



//Generate the life matrix any way you want. We would highly recommend that you print the generated
//matrix into a file, so that you can share it with other teams for checking correctness.
/*
//Gets an element with cordinates x,y 
int getElement(int x, int y, int age){
	if(age == 0){
		return array[y*size + x];
	}else{
		return array2[y*size + x];
	}
}

//Sets an element with cordinates x,y to value
int setElement(int x, int y, int age, int value){
	if(age == 0){
		array[y*size +x] = value;
	}else{
		array2[y*size +x] = value;
	}
}
*/

int getElement(int x, int y, int age){
	int currentVal = array[(y*size)/4+x/4];

	if((currentVal  & (int)pow(2,(x%4)*2+age)) == (int)pow(2,(x%4)*2+age)){
		return 1;
	}else if((currentVal & (int)pow(2,(x%4)*2+age)) == 0){
		return 0;
	}
}

void setElement(int x, int y, int age, int value){
	int currentVal = getElement(x,y,age);
	if(currentVal == 0 && value == 1){
		array[(y*size)/4+x/4] += (int)pow(2,(x%4)*2+age);
	
	} else if(currentVal == 1 && value == 0){
		array[(y*size)/4+x/4] -= pow(2,(x%4)*2+age);
	}
}

//Print the board
void printGame(unsigned int n, int age){
	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			printf("%i ", getElement(j,i,age));
		}
		printf("\n");
	}
}

void genlife(unsigned char *a, unsigned int n) {
	size = n;
	array = a;
	srand(time(NULL));
	
	cilk_for (int y = 0; y < n; y++)
	{
		cilk_for (int x = 0; x < n; x++)
		{
			setElement(x,y,0,rand()%2);
		}
	}
}

//Read the life matrix from stdin
void readlife(unsigned char *a, unsigned int n){
	int i,j;
	char *line;
	array = a;
	size = n;

	line = (char *)malloc(2*n+2);
	for(i = 0; i < n; i++){
		fgets(line, 2*n+2, stdin);
		for(j =0; j < n; j++){
			setElement(j,i,0,atoi(&line[2 * j]));
		}
	}
}

//Count the number of living Neighbours
int countNeighbours(int x, int y, int age){

	int neighbours = 0;
	
	neighbours += getElement(x,(y+1+size)%size, age);
	neighbours += getElement(x,(y-1+size)%size, age);
	neighbours += getElement((x+1+size)%size,y, age);
	neighbours += getElement((x-1+size)%size,y, age);
	neighbours += getElement((x+1+size)%size,(y+1+size)%size, age);
	neighbours += getElement((x+1+size)%size,(y-1+size)%size, age);
	neighbours += getElement((x-1+size)%size,(y+1+size)%size, age);
	neighbours += getElement((x-1+size)%size,(y-1+size)%size, age);
	return neighbours;
}

//Do one iteration
void Calculate_next_life(unsigned char *a, unsigned int k) {
	for(int x = 0; x < size;x++){
		for (int y = 0; y < size; ++y)
		{
			setElement(x,y,(k+1)%2,0);
		}
	}
	for (int i = 0; i < size; ++i)	{
		for (int j = 0; j < size; ++j){
			unsigned int neighbours = countNeighbours(j,i,k%2);

			if(neighbours > 3){
				setElement(j,i,(k+1)%2,0);
			}else if(neighbours < 2){
				setElement(j,i,(k+1)%2,0);
			}else if(neighbours == 3){
				setElement(j,i,(k+1)%2,1);
			}else if(neighbours ==2){
				setElement(j,i,(k+1)%2,getElement(j,i,k%2));
			}
		}
	}	
}


void Store_into_livecount(int total_lives) {

}

//Life function
void life(unsigned char *a, unsigned int n, unsigned int iter, int* livecount){

	array2 = (unsigned char *)malloc(sizeof(int)*n*n);
	size = n;
	array = a;



	for (int current_it = 1; current_it <= iter; ++current_it){
		Calculate_next_life(a,current_it-1);

		#if DEBUG == 1
			if(iter%10 == 0) {
				if(current_it%(iter/10) == 0) {
					unsigned char *a = current_it%2 == 0 ? array : array2;
					int total_lives = countlive(a, n);
					livecount[(current_it/(iter/10)) -1] = total_lives;
				}
			}
		#endif
	}
	

}

