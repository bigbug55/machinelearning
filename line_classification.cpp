#include<stdio.h>
#include<stdlib.h>
#include<math.h>

typedef int INT;
typedef double REAL;

#define NUM_NODES  100                     //the number of total data set;
#define NUM_TYPE1  50                      //the numbber of type1 data;
#define NUM_TYPE2  (NUM_NODES-NUM_TYPE1)

//Two types of data are considered in this program;
#define TYPE1      0
#define TYPE2      1

#define VAL        1                    //To donote the node has not been classified.
#define NONVAL     0                    

#define POSITIVE   1                    //to denote the part of plane that a single line devides
#define NEGATIVE   -1

#define MAX	   100                      //The default value for minDist;
#define THRESHOLD 5                    //the minimum number of nodes classifed by two lines;

typedef struct{
	REAL x;                             //the x location of the node;
	REAL y;                             //the y location of the node;
	INT TYPE;                           //the type of the node;
	INT VALID;                          //whether the node has been classified, VAl means classified, and NONVAL means not classified.
}NODE;

//two nodes that will be randomly choosen;
typedef struct{
        int point1;
        int point2;
}TWOPOINT;

typedef struct{
	REAL SLOPE;                           //the slope of the line;
	REAL OFFSET;                          //the offset of the line;
	INT TYPE;                             //the type of the node that line crosses;
	INT POINT1;
	INT POINT2;
}LINE;

void InitializeRandoms()
{
	srand(4751);
}

//Create all of the nodes by reading the data from file data.txt,
//and one critical thing, as there is no type information in the data.txt,
//you should remember to do the type initialization of every single node;

//NUM_TYPE1 nodes are initialized to type 1;
//and others, type 2;
void createNodes(NODE* nodes)
{
    int i;
    FILE *fpout=NULL;

    if((fpout=fopen("data.txt","r"))==NULL)
    {
        printf("Can not open this file\n");
	    exit(0);
    }

    for(i=0; i<NUM_TYPE1+NUM_TYPE2; i++)
    {
         fscanf(fpout, "%lf %lf", &nodes[i].x, &nodes[i].y);
    }
    
    for(i=0;i<NUM_TYPE1;i++)
	{
		nodes[i].TYPE=TYPE1;
		nodes[i].VALID=VAL;
	}
	
	for(i=NUM_TYPE1;i<NUM_NODES;i++)
	{
		nodes[i].TYPE=TYPE2;
		nodes[i].VALID=VAL;
	}	
}

//Randomly choose two points from type1;
TWOPOINT getPoint(NODE* nodes)
{
     TWOPOINT nd;
     
     int i=(int)rand()%NUM_TYPE1;
     while(nodes[i].VALID!=VAL)
         i=(int)rand()%NUM_TYPE1;
     nd.point1=i;
     
     int j=(int)rand()%NUM_TYPE1;
     while( (j==i)||(nodes[i].VALID!=VAL))
     j=(int)rand()%NUM_TYPE1;
     
     nd.point2=j;
     return nd;
}

//A line is generated from the two randomly chosen points;
void makeLine(INT i, INT j, NODE* nodes, LINE* line)
{	
	REAL x1, x2, y1, y2;
	x1=nodes[i].x;
	y1=nodes[i].y;

	x2=nodes[j].x;
	y2=nodes[j].y;
	
	line->SLOPE=(y1-y2)/(x1-x2);
	line->OFFSET=(x1*y2-x2*y1)/(x1-x2);
	
	line->TYPE=nodes[i].TYPE;
	line->POINT1=i;
	line->POINT2=j;
}

//To calculate the distance from a node(x,y) to its destination line(Y=aX+c);
//As the value ||y-Y|| is propotional to the actual distance, for simplicity,
//we adopt ||y-Y|| as the distance value;
REAL distanceCal(LINE* line, INT i, NODE* nodes)
{
	REAL xi, yi;
	xi=nodes[i].x;
	yi=nodes[i].y;
	return (yi-line->SLOPE*xi-line->OFFSET);
}

//To work out the node that is nearest to a destination line.
//Given a line(Y=aX+c), it divides the plane into two parts: one with all y >= Y, 
//and the other y<=Y;
//Here we use "side" to denote which part in consideration. If side equals POSITIVE,
//we concentrate on part 1 that with y>=Y, and equals NEGATIVE, otherwise.
//Line has two types and side has two values, which means 2*2 occasions should be
//considered.
INT minimumNode(LINE* line, NODE* nodes, INT side)
{
	INT n=-1;
	INT i;
	REAL minDist=MAX;

	if(line->TYPE==TYPE1 && (side==POSITIVE))
	{
		for(i=NUM_TYPE1; i<NUM_NODES; i++)
		{   //if minDist is bigger than current distance, the minDist is updated and the nearest node is recorded.
			if(minDist>(side*distanceCal(line, i, nodes)) && (distanceCal(line, i, nodes)>0) && (nodes[i].VALID!=NONVAL) )
			{
				n=i;
				minDist=side*distanceCal(line, i, nodes);
			}
		}
	}

	if(line->TYPE==TYPE1 && (side==NEGATIVE))
	{
        for(i=NUM_TYPE1; i<NUM_NODES; i++)
		{   
			if(minDist>(side*distanceCal(line,i, nodes)) && (distanceCal(line,i, nodes)<0) && (nodes[i].VALID!=NONVAL) )
			{
				n=i;	
				minDist=side*distanceCal(line, i, nodes);
			}
		}
	}
	
	if(line->TYPE==TYPE2 && (side==POSITIVE))
	{
		for(i=0; i<NUM_TYPE1; i++)
		{
			if(minDist>(side*distanceCal(line,i,nodes)) && (distanceCal(line,i,nodes)>0) && (nodes[i].VALID!=NONVAL) )
			{
				n=i;
				minDist=side*distanceCal(line, i, nodes);
			}
		}
	}

	if(line->TYPE==TYPE2 && (side==NEGATIVE))
	{
		for(i=0; i<NUM_TYPE1; i++)
		{   
			if(minDist>(side*distanceCal(line,i,nodes)) && (distanceCal(line,i,nodes)<0) && (nodes[i].VALID!=NONVAL) )
			{
				n=i;
				minDist=side*distanceCal(line, i, nodes);
			}
		}
	}
	return n;
}

//To draw a parallel line to line1 by crossing a denoted node.
void makeNewLine1(LINE* line1, LINE* line2, INT i, NODE* nodes)
{
	line2->SLOPE=line1->SLOPE;
	line2->OFFSET=nodes[i].y-nodes[i].x*line1->SLOPE;
	line2->TYPE=nodes[i].TYPE;
	
	line2->POINT1=i;
	line2->POINT2=line1->POINT1;
}

//To draw a parallel line in the middle of line1 and line2;
void makeNewLine2(LINE* line1, LINE* line2, LINE* line3)
{	
	line3->SLOPE=line1->SLOPE;
	line3->OFFSET=(line1->OFFSET+line2->OFFSET)/2;
}

//To test the equality of two lines.
INT sameLine(LINE* l1, LINE* l2)
{
    return( (l1->SLOPE==l2->SLOPE)&&(l1->OFFSET==l2->OFFSET) );
}

//the main funtion of the classification;
//all the "printf" comment is used for verification.
INT nodesMerge(NODE* nodes)
{
	int i,j;
	int sum;
	int minNode12, minNode13, minNode24, minNode25;
	minNode12=minNode13=minNode24=minNode25=-1;	
	TWOPOINT tw;
	
	REAL x1, y1;
	REAL x2, y2;

	LINE* line1=(LINE*)malloc(sizeof(LINE));
	LINE* line2=(LINE*)malloc(sizeof(LINE));
    LINE* line3=(LINE*)malloc(sizeof(LINE));
    LINE* line23=(LINE*)malloc(sizeof(LINE));
	LINE* line4=(LINE*)malloc(sizeof(LINE));
    LINE* line5=(LINE*)malloc(sizeof(LINE));
    LINE* line45=(LINE*)malloc(sizeof(LINE));	
    
    //first, get two randomly choosen points;
    tw=getPoint(nodes);
	i=tw.point1;
	j=tw.point2;
    //printf("(%d, %d)(%lf, %lf)(%lf, %lf)\n", i, j, nodes[i].x, nodes[i].y, nodes[j].x, nodes[j].y);
    
    //if one of the index of a nodes is -1, we should return because only one point left.
	if( (i==-1)||(j==-1) )
		return 0;
	//printf("i:%d,nodes[%d].x=%lf,nodes[%d].y=%lf, \nj:%d, node[%d].x=%lf, nodes[%d].y=%lf \n", 
    //i, i, nodes[i].x,i, nodes[i].y, j, j, nodes[j].x,j, nodes[j].y);
	
	//the first line, generated from two randomly chosen points, is created;
	makeLine(i, j, nodes, line1);
    
    //To derive the nearest node to line1.
	minNode12=minimumNode(line1, nodes, POSITIVE);
    //printf("the minimumNode to line1: node[%d]=%lf, %lf,\n", minNode12, nodes[minNode12].x, nodes[minNode12].y);
    	
	if(minNode12!=-1)
	{   
        //To create line2 that is parallel to line1 and crossing minNode12.
		makeNewLine1(line1, line2, minNode12, nodes);
		//printf("line2: %lf, %lf\n", line2->SLOPE, line2->OFFSET);
		
		//To derive the nearest node to line2.
		minNode13=minimumNode(line2, nodes, NEGATIVE);
		//printf("the minimumNode to line2: node[%d]: %lf, %lf\n", minNode13, nodes[minNode13].x, nodes[minNode13].y);
		
		//To create line3 that is parallel to line1 and crossing minNode13;
        makeNewLine1(line1, line3, minNode13, nodes);
        //printf("line3: %lf, %lf\n", line3->SLOPE, line3->OFFSET);
	}
	
	else
        line23=NULL;
	
	//The same procedure with the other side of line1;
	minNode24=minimumNode(line1, nodes, NEGATIVE);
	//printf("the minimumNode to line1: node[%d]=%lf, %lf\n", minNode24, nodes[minNode24].x, nodes[minNode24].y);
	
	if(minNode24!=-1)
	{
		makeNewLine1(line1, line4, minNode24, nodes);
		//printf("line4: %lf, %lf\n", line4->SLOPE, line4->OFFSET);
		
		minNode25=minimumNode(line4, nodes, POSITIVE);
		//printf("minimum to line4: node[%d]=%lf, %lf\n", minNode25, nodes[minNode25].x, nodes[minNode25].y);
		
		makeNewLine1(line1, line5, minNode25, nodes);
		//printf("line5: %lf, %lf\n", line5->SLOPE, line5->OFFSET);
	}
	else
	    line45=NULL;
	
    //It means no different type node(different from line1 type) at POSITIVE side of line1 if line23 equals NULL;
    //and line45 is NULL, otherwise.
    if( (line23==NULL) || (line45==NULL) )
        return 0;
	
	//If line1 and line3 are the same, the line23 should be derived from line1 and line2;
	//otherwise, from line2 and line3.
	if( sameLine(line1, line3) )
	{
        makeNewLine2(line1, line2, line23);
    }else{
         makeNewLine2(line2, line3, line23);
    }
    
    if( sameLine(line1, line5) )
    {
        makeNewLine2(line1, line4, line45);
    }else{
         makeNewLine2(line4, line5, line45);
    }
    
    //sum denotes the number of nodes that will be classified.
	sum=0;
	for(i=0;i<NUM_TYPE1;i++)
	{
        //The nodes between line23 and line45 will be classified.
		if((nodes[i].VALID!=NONVAL)&&(distanceCal(line23, i, nodes)<=0)&&(distanceCal(line45, i, nodes)>=0)){
			sum++;
		}
	}
	
	//if sum is bigger than a threshold then line23 and line45 will be accept, otherwise,
	//0 is returned and the work above will start over.
	if(sum>=THRESHOLD)
	{
	          for(i=0;i<NUM_TYPE1;i++)
	          {
		       if((nodes[i].VALID!=NONVAL)&&(distanceCal(line23, i, nodes)<=0)&&(distanceCal(line45, i, nodes)>=0)){
                   nodes[i].VALID=NONVAL;
		       }
              }
	printf("l23: %f, %f\n", line23->SLOPE, line23->OFFSET);
	printf("l45: %f, %f\n", line45->SLOPE, line45->OFFSET);
	printf("the number of TYPE1 between these two line: %d\n\n", sum);
	
    free(line1);
	free(line2);
	free(line3);
	free(line4);
	free(line5);
	free(line23);
	free(line45);
	return sum;
    }
    
	free(line1);
	free(line2);
	free(line3);
	free(line4);
	free(line5);
	free(line23);
	free(line45);
	return 0;
}

//Used to verify if the createNodes function works.
void printNodes(NODE* nodes)
{
	INT i;
	
	for(i=0;i<NUM_NODES;i++)
	{
		printf("%d: %f, y:%f, type:%d, valid:%d\n",i, nodes[i].x,nodes[i].y, nodes[i].TYPE,nodes[i].VALID);
	}
}

//the function to invoke the main funtion;
void Simulate(NODE* nodes)
{
	INT i,j;
	i=0;
    
	while(i<=(NUM_TYPE1-THRESHOLD)){
		j=0;
		j=nodesMerge(nodes);
		i+=j;
	}
}

//the entry point;
int main(void)
{
	NODE Nodes[NUM_NODES];
    InitializeRandoms();
	createNodes(Nodes);
	Simulate(Nodes);
	
	getchar();
	return 0;
}
