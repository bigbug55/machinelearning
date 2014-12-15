//This is my own BPN algorithm

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

typedef int           BOOL;
typedef int           INT;
typedef double        REAL;

#define sqr(x)        ((x)*(x))

#define NUM_LAYERS       4
#define NUM_DATA_SET	 150
#define NUM_INPUT_VECTOR 7
//The first four items mean the input;
//The last three items mean the output;
//1.0, 0, 0 means the  Iris-setosa
//0, 1.0, 0 means the  Iris-versicolor
//0, 0, 1.0 means the  Iris-virginica

#define TrainTimes     200
//TrainTimes denotes the train times.

#define BIAS		1
#define TRUE		1
#define FALSE		0

INT NUM_UNITS[NUM_LAYERS] = {4, 4, 3, 3};
REAL INPUT_DATA[NUM_DATA_SET][NUM_INPUT_VECTOR]=
{ 
5.1,3.5,1.4,0.2,1.0, 0, 0,
4.9,3.0,1.4,0.2,1.0, 0, 0,
4.7,3.2,1.3,0.2,1.0, 0, 0,
4.6,3.1,1.5,0.2,1.0, 0, 0,
5.0,3.6,1.4,0.2,1.0, 0, 0,
5.4,3.9,1.7,0.4,1.0, 0, 0,
4.6,3.4,1.4,0.3,1.0, 0, 0,
5.0,3.4,1.5,0.2,1.0, 0, 0,
4.4,2.9,1.4,0.2,1.0, 0, 0,
4.9,3.1,1.5,0.1,1.0, 0, 0,
5.4,3.7,1.5,0.2,1.0, 0, 0,
4.8,3.4,1.6,0.2,1.0, 0, 0,
4.8,3.0,1.4,0.1,1.0, 0, 0,
4.3,3.0,1.1,0.1,1.0, 0, 0,
5.8,4.0,1.2,0.2,1.0, 0, 0,
5.7,4.4,1.5,0.4,1.0, 0, 0,
5.4,3.9,1.3,0.4,1.0, 0, 0,
5.1,3.5,1.4,0.3,1.0, 0, 0,
5.7,3.8,1.7,0.3,1.0, 0, 0,
5.1,3.8,1.5,0.3,1.0, 0, 0,
5.4,3.4,1.7,0.2,1.0, 0, 0,
5.1,3.7,1.5,0.4,1.0, 0, 0,
4.6,3.6,1.0,0.2,1.0, 0, 0,
5.1,3.3,1.7,0.5,1.0, 0, 0,
4.8,3.4,1.9,0.2,1.0, 0, 0,
5.0,3.0,1.6,0.2,1.0, 0, 0,
5.0,3.4,1.6,0.4,1.0, 0, 0,
5.2,3.5,1.5,0.2,1.0, 0, 0,
5.2,3.4,1.4,0.2,1.0, 0, 0,
4.7,3.2,1.6,0.2,1.0, 0, 0,
4.8,3.1,1.6,0.2,1.0, 0, 0,
5.4,3.4,1.5,0.4,1.0, 0, 0,
5.2,4.1,1.5,0.1,1.0, 0, 0,
5.5,4.2,1.4,0.2,1.0, 0, 0,
4.9,3.1,1.5,0.1,1.0, 0, 0,
5.0,3.2,1.2,0.2,1.0, 0, 0,
5.5,3.5,1.3,0.2,1.0, 0, 0,
4.9,3.1,1.5,0.1,1.0, 0, 0,
4.4,3.0,1.3,0.2,1.0, 0, 0,
5.1,3.4,1.5,0.2,1.0, 0, 0,
5.0,3.5,1.3,0.3,1.0, 0, 0,
4.5,2.3,1.3,0.3,1.0, 0, 0,
4.4,3.2,1.3,0.2,1.0, 0, 0,
5.0,3.5,1.6,0.6,1.0, 0, 0,
5.1,3.8,1.9,0.4,1.0, 0, 0,
4.8,3.0,1.4,0.3,1.0, 0, 0,
5.1,3.8,1.6,0.2,1.0, 0, 0,
4.6,3.2,1.4,0.2,1.0, 0, 0,
5.3,3.7,1.5,0.2,1.0, 0, 0,
5.0,3.3,1.4,0.2,1.0, 0, 0,
7.0,3.2,4.7,1.4,0, 1.0, 0,
6.4,3.2,4.5,1.5,0, 1.0, 0,
6.9,3.1,4.9,1.5,0, 1.0, 0,
5.5,2.3,4.0,1.3,0, 1.0, 0,
6.5,2.8,4.6,1.5,0, 1.0, 0,
5.7,2.8,4.5,1.3,0, 1.0, 0,
6.3,3.3,4.7,1.6,0, 1.0, 0,
4.9,2.4,3.3,1.0,0, 1.0, 0,
6.6,2.9,4.6,1.3,0, 1.0, 0,
5.2,2.7,3.9,1.4,0, 1.0, 0,
5.0,2.0,3.5,1.0,0, 1.0, 0,
5.9,3.0,4.2,1.5,0, 1.0, 0,
6.0,2.2,4.0,1.0,0, 1.0, 0,
6.1,2.9,4.7,1.4,0, 1.0, 0,
5.6,2.9,3.6,1.3,0, 1.0, 0,
6.7,3.1,4.4,1.4,0, 1.0, 0,
5.6,3.0,4.5,1.5,0, 1.0, 0,
5.8,2.7,4.1,1.0,0, 1.0, 0,
6.2,2.2,4.5,1.5,0, 1.0, 0,
5.6,2.5,3.9,1.1,0, 1.0, 0,
5.9,3.2,4.8,1.8,0, 1.0, 0,
6.1,2.8,4.0,1.3,0, 1.0, 0,
6.3,2.5,4.9,1.5,0, 1.0, 0,
6.1,2.8,4.7,1.2,0, 1.0, 0,
6.4,2.9,4.3,1.3,0, 1.0, 0,
6.6,3.0,4.4,1.4,0, 1.0, 0,
6.8,2.8,4.8,1.4,0, 1.0, 0,
6.7,3.0,5.0,1.7,0, 1.0, 0,
6.0,2.9,4.5,1.5,0, 1.0, 0,
5.7,2.6,3.5,1.0,0, 1.0, 0,
5.5,2.4,3.8,1.1,0, 1.0, 0,
5.5,2.4,3.7,1.0,0, 1.0, 0,
5.8,2.7,3.9,1.2,0, 1.0, 0,
6.0,2.7,5.1,1.6,0, 1.0, 0,
5.4,3.0,4.5,1.5,0, 1.0, 0,
6.0,3.4,4.5,1.6,0, 1.0, 0,
6.7,3.1,4.7,1.5,0, 1.0, 0,
6.3,2.3,4.4,1.3,0, 1.0, 0,
5.6,3.0,4.1,1.3,0, 1.0, 0,
5.5,2.5,4.0,1.3,0, 1.0, 0,
5.5,2.6,4.4,1.2,0, 1.0, 0,
6.1,3.0,4.6,1.4,0, 1.0, 0,
5.8,2.6,4.0,1.2,0, 1.0, 0,
5.0,2.3,3.3,1.0,0, 1.0, 0,
5.6,2.7,4.2,1.3,0, 1.0, 0,
5.7,3.0,4.2,1.2,0, 1.0, 0,
5.7,2.9,4.2,1.3,0, 1.0, 0,
6.2,2.9,4.3,1.3,0, 1.0, 0,
5.1,2.5,3.0,1.1,0, 1.0, 0,
5.7,2.8,4.1,1.3,0, 1.0, 0,
6.3,3.3,6.0,2.5,0, 0, 1.0,
5.8,2.7,5.1,1.9,0, 0, 1.0,
7.1,3.0,5.9,2.1,0, 0, 1.0,
6.3,2.9,5.6,1.8,0, 0, 1.0,
6.5,3.0,5.8,2.2,0, 0, 1.0,
7.6,3.0,6.6,2.1,0, 0, 1.0,
4.9,2.5,4.5,1.7,0, 0, 1.0,
7.3,2.9,6.3,1.8,0, 0, 1.0,
6.7,2.5,5.8,1.8,0, 0, 1.0,
7.2,3.6,6.1,2.5,0, 0, 1.0,
6.5,3.2,5.1,2.0,0, 0, 1.0,
6.4,2.7,5.3,1.9,0, 0, 1.0,
6.8,3.0,5.5,2.1,0, 0, 1.0,
5.7,2.5,5.0,2.0,0, 0, 1.0,
5.8,2.8,5.1,2.4,0, 0, 1.0,
6.4,3.2,5.3,2.3,0, 0, 1.0,
6.5,3.0,5.5,1.8,0, 0, 1.0,
7.7,3.8,6.7,2.2,0, 0, 1.0,
7.7,2.6,6.9,2.3,0, 0, 1.0,
6.0,2.2,5.0,1.5,0, 0, 1.0,
6.9,3.2,5.7,2.3,0, 0, 1.0,
5.6,2.8,4.9,2.0,0, 0, 1.0,
7.7,2.8,6.7,2.0,0, 0, 1.0,
6.3,2.7,4.9,1.8,0, 0, 1.0,
6.7,3.3,5.7,2.1,0, 0, 1.0,
7.2,3.2,6.0,1.8,0, 0, 1.0,
6.2,2.8,4.8,1.8,0, 0, 1.0,
6.1,3.0,4.9,1.8,0, 0, 1.0,
6.4,2.8,5.6,2.1,0, 0, 1.0,
7.2,3.0,5.8,1.6,0, 0, 1.0,
7.4,2.8,6.1,1.9,0, 0, 1.0,
7.9,3.8,6.4,2.0,0, 0, 1.0,
6.4,2.8,5.6,2.2,0, 0, 1.0,
6.3,2.8,5.1,1.5,0, 0, 1.0,
6.1,2.6,5.6,1.4,0, 0, 1.0,
7.7,3.0,6.1,2.3,0, 0, 1.0,
6.3,3.4,5.6,2.4,0, 0, 1.0,
6.4,3.1,5.5,1.8,0, 0, 1.0,
6.0,3.0,4.8,1.8,0, 0, 1.0,
6.9,3.1,5.4,2.1,0, 0, 1.0,
6.7,3.1,5.6,2.4,0, 0, 1.0,
6.9,3.1,5.1,2.3,0, 0, 1.0,
5.8,2.7,5.1,1.9,0, 0, 1.0,
6.8,3.2,5.9,2.3,0, 0, 1.0,
6.7,3.3,5.7,2.5,0, 0, 1.0,
6.7,3.0,5.2,2.3,0, 0, 1.0,
6.3,2.5,5.0,1.9,0, 0, 1.0,
6.5,3.0,5.2,2.0,0, 0, 1.0,
6.2,3.4,5.4,2.3,0, 0, 1.0,
5.9,3.0,5.1,1.8,0, 0, 1.0,
};

typedef struct {                     /* A LAYER OF A NET:                     */
	INT           Units;         /* - number of units in this layer       */
	REAL*         Output;        /* - output of ith unit                  */
	REAL*         Error;         /* - error term of ith unit              */
	REAL**        Weight;        /* - connection weights to ith unit      */
    REAL**        WeightSave;    /* - saved weights for stopped training  */
} LAYER;

typedef struct {                     /* A NET:                                */
        LAYER**       Layer;         /* - layers of this net                  */
        LAYER*        InputLayer;    /* - input layer                         */
        LAYER*        OutputLayer;   /* - output layer                        */
        REAL          Eta;           /* - learning rate                       */
        REAL          Error;         /* - total net error                     */
	    REAL	      Gain;	     /*	- Net Gain */
} NET;

void InitializeRandoms()
{
  srand(4711);
}

void GenerateNetwork(NET* Net)
{
  INT l,i;

  Net->Layer = (LAYER**) calloc(NUM_LAYERS, sizeof(LAYER*));
  
  for (l=0; l<NUM_LAYERS; l++) {
    Net->Layer[l] = (LAYER*) malloc(sizeof(LAYER));
      
    Net->Layer[l]->Units      = NUM_UNITS[l];
    Net->Layer[l]->Output     = (REAL*)malloc((NUM_UNITS[l]+1)*sizeof(REAL));
    Net->Layer[l]->Error      = (REAL*)malloc((NUM_UNITS[l]+1)* sizeof(REAL));
    Net->Layer[l]->Weight     = (REAL**)malloc((NUM_UNITS[l]+1)*sizeof(REAL*));
    Net->Layer[l]->WeightSave = (REAL**)malloc((NUM_UNITS[l]+1)*sizeof(REAL*));
    Net->Layer[l]->Output[0]  = BIAS;
    //The first component of every layer, which usually names bias.
    if (l != 0) {
      for (i=1; i<=NUM_UNITS[l]; i++) {
        Net->Layer[l]->Weight[i]     = (REAL*)malloc((NUM_UNITS[l-1])*sizeof(REAL));
        Net->Layer[l]->WeightSave[i] = (REAL*)malloc((NUM_UNITS[l-1])*sizeof(REAL));
      }
    }

  }
  
  Net->InputLayer  = Net->Layer[0];
  Net->OutputLayer = Net->Layer[NUM_LAYERS - 1];
  Net->Eta         = 0.25;
  Net->Gain	       = 1.0;
}

REAL RandomEqualREAL(REAL Low, REAL High)
{
	return ((REAL)rand()/RAND_MAX)*(High-Low) + Low;
}

void RandomWeights(NET* Net)
{
  INT l,i,j;

  for (l=1; l<NUM_LAYERS; l++) {
  //l starts from "1" because the first layer is the input layer.
    for (i=1; i<=Net->Layer[l]->Units; i++) {
  //i starts from "1" because the first component is the bias.
      for (j=0; j<=Net->Layer[l-1]->Units; j++) {
        Net->Layer[l]->Weight[i][j] = RandomEqualREAL(-0.5, 0.5);
      }
    }
  }
}

void SetInput(NET* Net, INT DataStart)
{
  INT i;
  
  for (i=1; i<=Net->InputLayer->Units; i++) {
    Net->InputLayer->Output[i] = INPUT_DATA[DataStart][i-1];
  }
}

void PropagateLayer(NET* Net, LAYER* Lower, LAYER* Upper)
{
  INT  i,j;
  REAL Sum;

  for (i=1; i<=Upper->Units; i++) {
    Sum = 0;
    for (j=0; j<=Lower->Units; j++) {
      Sum += Upper->Weight[i][j] * Lower->Output[j];
    }
    Upper->Output[i] = 1 / (1 + exp(-Net->Gain * Sum));
  }
}

void PropagateNet(NET* Net)
{
  INT l;
   
  for (l=0; l<NUM_LAYERS-1; l++) {
    PropagateLayer(Net, Net->Layer[l], Net->Layer[l+1]);
  }
}

/*
void GetOutput(NET* Net, REAL* Output)
{
  INT i;
   
  for (i=1; i<=Net->OutputLayer->Units; i++) {
    Output[i-1] = Net->OutputLayer->Output[i];
  }
}
*/

void ComputeOutputError(NET* Net, INT DataStart)
{
  INT  i;
  REAL Out, Err;
   
  Net->Error = 0;
  for (i=1; i<=Net->OutputLayer->Units; i++) {
    Out = Net->OutputLayer->Output[i];
    Err = INPUT_DATA[DataStart][4-1+i]-Out;
    //The real output is 4, 5, 6;
    Net->OutputLayer->Error[i] = Net->Gain * Out * (1-Out) * Err;
    Net->Error += 0.5 * sqr(Err);
  }
}


void BackpropagateLayer(NET* Net, LAYER* Upper, LAYER* Lower)
{
  INT  i,j;
  REAL Out, Err;
   
  for (i=1; i<=Lower->Units; i++) {
    Out = Lower->Output[i];
    Err = 0;
    for (j=1; j<=Upper->Units; j++) {
      Err += Upper->Weight[j][i] * Upper->Error[j];
    }
    Lower->Error[i] = Net->Gain * Out * (1-Out) * Err;
  }
}


void BackpropagateNet(NET* Net)
{
  INT l;
   
  for (l=NUM_LAYERS-1; l>1; l--) {
    BackpropagateLayer(Net, Net->Layer[l], Net->Layer[l-1]);
  }
}

void AdjustWeights(NET* Net)
{
  INT  l,i,j;
  REAL Out, Err;
   
  for (l=1; l<NUM_LAYERS; l++) {
    for (i=1; i<=Net->Layer[l]->Units; i++) {
      for (j=0; j<=Net->Layer[l-1]->Units; j++) {
        Out = Net->Layer[l-1]->Output[j];
        Err = Net->Layer[l]->Error[i];
        Net->Layer[l]->Weight[i][j] += Net->Eta * Err * Out;
      }
    }
  }
}

void GetOutput(NET* Net,INT j)
{
	INT l;
	REAL max = -1.0;
	INT temp;
	
	printf("%d: ", j);

	for(l=1; l<=3; l++){
		if(Net->OutputLayer->Output[l]>max){
			max=Net->OutputLayer->Output[l];
			temp = l;
		}
		printf("%f ", Net->OutputLayer->Output[l]);
	}
	if (temp == ((j/50)+1) ) printf("correct");
	else printf("error");

	printf("\n");
}

void SimulateNet(NET* Net, INT DataStart, BOOL Training)
{
  SetInput(Net, DataStart);
  PropagateNet(Net); 
  ComputeOutputError(Net, DataStart);

  if (Training) {
    BackpropagateNet(Net);
    AdjustWeights(Net);
  }
}

int main(void)
{
  NET  Net;
  INT i,j;

  InitializeRandoms();
  GenerateNetwork(&Net);
  RandomWeights(&Net);

  //Train for one time;
  for(i=0; i<TrainTimes; i++)
	for(j=0; j<150; j++)
		SimulateNet(&Net,j,TRUE);
  printf("Trainning Done\n");

  printf("Evaluating Begin\n");
  for(j=0; j<150; j++){
  	SimulateNet(&Net,j,FALSE);
  	GetOutput(&Net,j);
  }
  
  //getchar();

  return 0;
}
