#include <stdio.h>
#include<stdlib.h>
#define NUM_NODES                          100
#define NONE                               9999

struct _NODE
{
  int iDist;
  int iPrev;
};
typedef struct _NODE NODE;

struct _QITEM
{
  int iNode;
  int iDist;
  int iPrev;
  struct _QITEM *qNext;
};
typedef struct _QITEM QITEM;

QITEM *qHead = NULL;

             
             
             
int AdjMatrix[NUM_NODES][NUM_NODES];

int g_qCount = 0;
NODE rgnNodes[NUM_NODES];
int ch;
int iPrev, iNode;
int i, iCost, iDist;


void print_path (NODE *rgnNodes, int chNode)
{
  if (rgnNodes[__boundcheck_ptr_cast_to_array_reference(38,16,"print_path",(void *)(rgnNodes),(void *)(rgnNodes+chNode),chNode)].iPrev != NONE)
    {
      print_path(rgnNodes, rgnNodes[__boundcheck_ptr_cast_to_array_reference(40,37,"print_path",(void *)(rgnNodes),(void *)(rgnNodes+chNode),chNode)].iPrev);
    }
  printf (" %d", chNode);
  fflush(stdout);
}


void enqueue (int iNode, int iDist, int iPrev)
{
  QITEM *qNew = (QITEM *) __boundcheck_malloc(sizeof(QITEM));
__boundcheck_metadata_store((void *)(&qNew),(void *)((size_t)(&qNew)+sizeof(qNew)*8-1));

  QITEM *qLast = qHead;
__boundcheck_metadata_store((void *)(&qLast),(void *)((size_t)(&qLast)+sizeof(qLast)*8-1));

  
  if (!qNew) 
    {
      fprintf(stderr, "Out of memory.\n");
      exit(1);
    }
  qNew->iNode = iNode;
  qNew->iDist = iDist;
  qNew->iPrev = iPrev;
  qNew->qNext = NULL;
  
  if (!qLast) 
    {
      qHead = qNew;
__boundcheck_metadata_trans_check((void *)(qHead),(void *)(qNew),(void *)(qNew));

    }
  else
    {
      while (qLast->qNext) qLast = qLast->qNext;
      qLast->qNext = qNew;
    }
  g_qCount++;
  //               ASSERT(g_qCount);
}


void dequeue (int *piNode, int *piDist, int *piPrev)
{
  QITEM *qKill = qHead;
__boundcheck_metadata_store((void *)(&qKill),(void *)((size_t)(&qKill)+sizeof(qKill)*8-1));

  
  if (qHead)
    {
      //                 ASSERT(g_qCount);
      *(int *)(__boundcheck_ptr_reference(83,8,"dequeue",(void *)(piNode),(void *)(piNode))) = qHead->iNode;
      *(int *)(__boundcheck_ptr_reference(84,8,"dequeue",(void *)(piDist),(void *)(piDist))) = qHead->iDist;
      *(int *)(__boundcheck_ptr_reference(85,8,"dequeue",(void *)(piPrev),(void *)(piPrev))) = qHead->iPrev;
      qHead = qHead->qNext;
      __boundcheck_free(qKill);
      g_qCount--;
    }
}


int qcount (void)
{
  return(g_qCount);
}

int dijkstra(int chStart, int chEnd) 
{
  

  
  for (ch = 0; ch < NUM_NODES; ch++)
    {
      rgnNodes[_RV_insert_check(0,100,105,16,"dijkstra",ch)].iDist = NONE;
      rgnNodes[_RV_insert_check(0,100,106,16,"dijkstra",ch)].iPrev = NONE;
    }

  if (chStart == chEnd) 
    {
      printf("Shortest path is 0 in cost. Just stay where you are.\n");
    }
  else
    {
      rgnNodes[_RV_insert_check(0,100,115,16,"dijkstra",chStart)].iDist = 0;
      rgnNodes[_RV_insert_check(0,100,116,16,"dijkstra",chStart)].iPrev = NONE;
      
      enqueue (chStart, 0, NONE);
      
     while (qcount() > 0)
	{
	  dequeue (&iNode, &iDist, &iPrev);
	  for (i = 0; i < NUM_NODES; i++)
	    {
	      if ((iCost = AdjMatrix[_RV_insert_check(0,100,125,31,"dijkstra",iNode)][_RV_insert_check(0,100,125,38,"dijkstra",i)]) != NONE)
		{
		  if ((NONE == rgnNodes[_RV_insert_check(0,100,127,27,"dijkstra",i)].iDist) || 
		      (rgnNodes[_RV_insert_check(0,100,128,19,"dijkstra",i)].iDist > (iCost + iDist)))
		    {
		      rgnNodes[_RV_insert_check(0,100,130,18,"dijkstra",i)].iDist = iDist + iCost;
		      rgnNodes[_RV_insert_check(0,100,131,18,"dijkstra",i)].iPrev = iNode;
		      enqueue (i, iDist + iCost, iNode);
		    }
		}
	    }
	}
      
      printf("Shortest path is %d in cost. ", rgnNodes[_RV_insert_check(0,100,138,56,"dijkstra",chEnd)].iDist);
      printf("Path is: ");
      print_path(rgnNodes, chEnd);
      printf("\n");
    }
}

void __global_variables_init(){
__boundcheck_metadata_store((void *)(&(qHead)),(void *)((size_t)(&(qHead))+sizeof(qHead)*8-1));
__boundcheck_metadata_store((void *)(&AdjMatrix),(void *)((size_t)(&AdjMatrix)+sizeof(AdjMatrix)*8-1));
__boundcheck_metadata_store((void *)(&g_qCount),(void *)((size_t)(&g_qCount)+sizeof(g_qCount)*8-1));
__boundcheck_metadata_store((void *)(&rgnNodes),(void *)((size_t)(&rgnNodes)+sizeof(rgnNodes)*8-1));
__boundcheck_metadata_store((void *)(&ch),(void *)((size_t)(&ch)+sizeof(ch)*8-1));
__boundcheck_metadata_store((void *)(&iPrev),(void *)((size_t)(&iPrev)+sizeof(iPrev)*8-1));
__boundcheck_metadata_store((void *)(&iNode),(void *)((size_t)(&iNode)+sizeof(iNode)*8-1));
__boundcheck_metadata_store((void *)(&i),(void *)((size_t)(&i)+sizeof(i)*8-1));
__boundcheck_metadata_store((void *)(&iCost),(void *)((size_t)(&iCost)+sizeof(iCost)*8-1));
__boundcheck_metadata_store((void *)(&iDist),(void *)((size_t)(&iDist)+sizeof(iDist)*8-1));
}
int main(int argc, char *argv[]) {
	__global_variables_init();

  int i;
__boundcheck_metadata_store((void *)(&i),(void *)((size_t)(&i)+sizeof(i)*8-1));
int j;
__boundcheck_metadata_store((void *)(&j),(void *)((size_t)(&j)+sizeof(j)*8-1));
int k;
__boundcheck_metadata_store((void *)(&k),(void *)((size_t)(&k)+sizeof(k)*8-1));

  FILE *fp;
__boundcheck_metadata_store((void *)(&fp),(void *)((size_t)(&fp)+sizeof(fp)*8-1));

  
  if (argc<2) {
    fprintf(stderr, "Usage: dijkstra <filename>\n");
    fprintf(stderr, "Only supports matrix size is #define'd.\n");
  }

  /* open the adjacency matrix file */
  fp = fopen (argv[1],"r");

  /* make a fully connected matrix */
  for (i=0;i<NUM_NODES;i++) {
    for (j=0;j<NUM_NODES;j++) {
      /* make it more sparce */
      fscanf(fp,"%d",&k);
			AdjMatrix[_RV_insert_check(0,100,162,14,"main",i)][_RV_insert_check(0,100,162,17,"main",j)]= k;
    }
  }

  /* finds 10 shortest paths between nodes */
  for (i=0,j=NUM_NODES/2;i<20;i++,j++) {
			j=j%NUM_NODES;
      dijkstra(i,j);
  }
  exit(0);
  

}
