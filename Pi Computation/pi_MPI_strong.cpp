 /* 
Computing Pi using midpoint rule by computing the following integration:
pi=int{0,1}(sqrt(1-x^2))
int{a,b}(f(x))=((b-a)/N)(sum{1,N}(f(a+(k-0.5)((b-a)/N))))
*/
#include <iostream>
#include <math.h>
#include <mpi.h>
#include <fstream>
#include "timer.h"
#include <stdlib.h>
#include <iomanip>


int my_p, np;
int ierr;

using namespace std;

// void my_init(int *argc, char **argv) 
// {
//     ierr = MPI_Init(argc, argv);
//     ierr=MPI_Comm_rank(MPI_COMM_WORLD, &my_p);
//     ierr=MPI_Comm_size(MPI_COMM_WORLD, &np);
// }
    

int main(int argc, char *argv[])

{
    int N;
    double g_p=0;
    timespec before, after, time_diff;
    
    ifstream fin("input.txt");
    fin>>N;
    fin.close();

   


    double time_s;
    
    // my_init(&argc, &argv);
    ierr = MPI_Init(&argc, &argv);
    ierr= MPI_Comm_rank(MPI_COMM_WORLD, &my_p);
    ierr= MPI_Comm_size(MPI_COMM_WORLD, &np);
    if (ierr!= MPI_SUCCESS) {
    	printf("Error starting MPI program.  Terminating.\n");
    	MPI_Abort(MPI_COMM_WORLD, ierr);
  	}
   
    
    double sum;
    double a=0, b=1;
    double p,x;
    int count = N/np;
    int start, end,k, root;
    start=(my_p)*count;
    end=(my_p+1)*count;
    sum=0;
    root=0;
    if(my_p==root)
    {
        get_time(&before);      
    }    
    for (int k = start; k < end; ++k)
    {
    	x=a+((k-0.5)*(b-a)/N);    	
    	sum+=sqrt(1-pow(x,2));
    }
    p=4*((b-a)/N)*sum;

    ierr=MPI_Reduce(&p, &g_p, 1, MPI_DOUBLE, MPI_SUM, root, MPI_COMM_WORLD);
    if(my_p==root)
    {
        get_time(&after);      
        diff(&before,&after,&time_diff);
        time_s = time_diff.tv_sec + (double)(time_diff.tv_nsec)/1.0e9;   
        cout<<"Time taken: "<<time_s<<" s"<<endl;
        cout<<"Pi="<<setprecision(18)<<g_p<<endl;
        ofstream fout;   
        fout.open("strong_scaling.csv",ios::app);
        fout<<np<<","<<time_s<<endl;
        fout.close();
    }
            
    
    ierr= MPI_Finalize();

        
    return 0;
}