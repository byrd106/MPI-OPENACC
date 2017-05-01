#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>


#pragma acc routine(randomDraw)
double randomDraw() 
{
    double range = (1 - 0); 
    double div = RAND_MAX / range;
    //return 0 + (rand() / div);
    return 0.5; 
}

int randomNumber(int min, int max) {
   //return min + rand() % (max - min);
   return 1; 
}

int checkArguments(int argc) {
	if( argc == 2 ) {
      return 1; 
   	} else {
   	  printf("Invalid argument number, runtime needs to be passed in\n"); 
   	  return 0; 
   	}
}


#define dimension 250

#pragma acc routine(biomassCalculator) 
double biomassCalculator(int grid[][dimension]) {
	int treecount = 0.0; 
	
	for(int i = 0; i < dimension; i++) {
		for(int j = 0; j < dimension; j++) {
			if(grid[i][j] == 1) {
				treecount++; 
			}
		}
	}

	double totalTrees = dimension * dimension; 
	return (double)treecount/(double)totalTrees; 
}

#pragma acc routine(countFires)
int countFires(int grid[][dimension]) {
	
	int fireCount = 0; 

	for(int i = 0; i < dimension; i++) {
		for(int j = 0; j < dimension; j++) {
			if(grid[i][j] == 2) {
				fireCount++; 
			}
		}
	}
	
	return fireCount; 

}

#pragma acc routine(fireUpdate)
void fireUpdate(int grid[][dimension]) {
	
	// we'd need a DS with the fire outputs 
	
	int fireCount = countFires(grid); 
	// first we need to store the fires 
	int firesICoord[fireCount]; 
	int firesQCoord[fireCount]; 

	int currentFireIndex = 0; 
	for(int i = 0; i < dimension; i++) {
		for(int q = 0; q < dimension; q++) {
			if(grid[i][q] == 2) {
				firesICoord[currentFireIndex] = i; 
				firesQCoord[currentFireIndex] = q;
				currentFireIndex++; 
			}
		}
	}

	for(int a = 0; a < fireCount; a++) {
		
		int i = firesICoord[a]; 
		int q = firesQCoord[a]; 

		int left = q - 1; 
		int right = q + 1; 	
		int top = i - 1; 
		int bottom = i + 1; 	

		if(left > -1) {
			grid[i][left] = 2;
		}
		if(right < dimension) {
			grid[i][right] = 2;
		}
		if(top > -1) {
			grid[top][q] = 2;
		}
		if(bottom < dimension) {
			grid[bottom][q] = 2;
		}

		if(top > -1 && left > -1) {
			grid[top][left] = 2;
		}
		if(top > -1 && right < dimension) {
			grid[top][right] = 2;
		}
		if(bottom < dimension && left > -1) {
			grid[bottom][left] = 2;
		}
		if(bottom < dimension && right < dimension) {
			grid[bottom][right] = 2;
		}

	}

	for(int a = 0; a < fireCount; a++) {
		grid[firesICoord[a]][firesQCoord[a]] = 0; // clear out the fires now 
	}

}

#pragma acc routine(lightning)
void lightning(int grid[][dimension]) {
	for(int i = 0; i < dimension; i++) {
		for(int j = 0; j < dimension; j++) {
			if(randomDraw() < 0.01) {
				grid[i][j] = 2; 
			}
		}
	}
}

#pragma acc routine(growTrees)
void growTrees(int grid[][dimension]) {	
	for(int i = 0; i < dimension; i++) {
		for(int j = 0; j < dimension; j++) {
			if(randomDraw() < 0.1 && grid[i][j] != 2) {
				grid[i][j] = 1; 
			}
		}
	}
}

#pragma acc routine(simulationRound)
double simulationRound(int grid[][dimension],int round) {
 
  fireUpdate(grid); 
  
  // if(round < 200) 
  lightning(grid);   // lightning has to come after fire update   
  
  growTrees(grid); // trees come after this... so only non fire gets changed 
 
  double biomass = biomassCalculator(grid); 

  return biomass; 

}

void createGrid(int grid[][dimension]) {
	
	for(int i = 0; i < dimension; i++) {				
		for(int k = 0; k < dimension; k++) {		
			grid[i][k] = 0.0; 
 		}
	}	
 }

void writeToDataToCSV(double* data, int size) {	

   FILE *fp;
   fp = fopen("data.csv", "w+");
   //fprintf(fp, "This is testing for fprintf...\n");  
   fputs("time,biomass\n", fp);
   for(int i = 0; i < size; i++) {
   		fprintf(fp,"%i,%f\n",i,data[i]);  
   }   
   fclose(fp);
}

void display(int p[][dimension])
{
    printf("\n");
    for (int i = 0; i< dimension;i++)
    {
        for ( int j = 0;j< dimension;j++)
        {
            printf("%i\t",p[i][j]);
        }
        printf("\n");
    }
}

int main(int argc, char *argv[]) {
  acc_set_device_num( 12 , acc_get_device_type());
  if(checkArguments(argc) == 1) {  	
  	

  	int runtime = atoi(argv[1]); 
  	double writeData[runtime]; 
  	
  	int grid[dimension][dimension]; 
  	createGrid(grid); 
  	
  	/*
	 Timer mechanism pulled from this stack overflow answer, and modifed to application needs
  	 http://stackoverflow.com/questions/459691/best-timing-method-in-c
	*/
  	clock_t start = clock(), diff;

        #pragma acc kernels
  	for(int i = 0; i < runtime; i++) {
  		writeData[i] = simulationRound(grid,i); // needs an inital slate and operation should be performed    		
  	}


  	diff = clock() - start;
  	int msec = diff * 1000 / CLOCKS_PER_SEC;
	
	printf("%d", msec);

  	writeToDataToCSV(writeData,runtime);
  	
  }	

  return 0;

}



