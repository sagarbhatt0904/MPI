/* Mandlebrot Set

Z_{i+1} = (Z_{i})^2 + c

|Z|<=2 (for mandlebrot set, the value goes out of bound for |Z| greater than 2)
=> sqrt(x^2 +y^2)<=2
*/

#include <iostream>
#include <fstream>
#include <mpi.h>
#include <vector>
#include "timer.h"


using namespace std;

int main(int argc, char *argv[])
{		
	int N; /*Resolution of mandlbrot plot is to be made*/
	int iter, max_iter;

	int my_p, np, ierr; /*Variable for MPI init*/
	int root=0;		/*Root process number*/

	timespec before, after, time_diff;
	double x_max, x_min, y_max, y_min; /*iteration countert and max iterations*/
	
	ifstream fin("input1.txt");
	fin>>N>>max_iter>>x_max>>x_min>>y_max>>y_min;;
	fin.close();

	/*MPI Initialization*/
	ierr = MPI_Init(&argc, &argv);
    ierr= MPI_Comm_rank(MPI_COMM_WORLD, &my_p);
    ierr= MPI_Comm_size(MPI_COMM_WORLD, &np);
    
    /*MPI Success check*/
    if (ierr!= MPI_SUCCESS) 
    {
    	cout<<"Error starting MPI program.  Terminating. \n";
    	MPI_Abort(MPI_COMM_WORLD, ierr);
  	}

  	/*Declaring the range for each node*/
	int count = N/np;
    int start, end ;
    start=(my_p)*count;
    end=(my_p+1)*count;

	std::vector<std::vector<double> > c_real (N,std::vector<double>(N, 0)); /*Real component of c*/
	std::vector<std::vector<double> > c_img (N,std::vector<double>(N, 0)); /*Imaginary component of c*/
	
	double time_s;

	
	double area=0, area_g=0;	/*Initializing area computed per node and global area*/
	 if(my_p==root)
    {
        get_time(&before);      
    }    

	 for(int i=start; i<end; i++)
	{
		for(int j=0; j<N; j++)
		{
			iter=0;
			double x=0,y=0, tmp;
			c_real[i][j] =(x_max*((double)(j))+x_min*((double)(N-j)))/((double)(N));
			c_img[i][j] =(y_max*((double)(i))+y_min*((double)(N-i)))/((double)(N));
			
			while(x*x +y*y < 4 && iter < max_iter)
			{
				tmp= x*x-y*y + c_real[i][j];
				y=2*x*y + c_img[i][j];
				x=tmp;				
				iter++;
			}
			
			if (iter>max_iter/2)
			{
				area+=((x_max-x_min)/N)*((y_max-y_min)/N);
			}
		}			
	}
	
	ierr=MPI_Reduce(&area, &area_g, 1, MPI_DOUBLE, MPI_SUM, root, MPI_COMM_WORLD);
	// cout<<"From Process: "<<my_p<<" Area of the mandlebrot set ="<<area<<endl;
	if(my_p==root)
	{
		get_time(&after);
		diff(&before,&after,&time_diff);
		time_s = time_diff.tv_sec + (double)(time_diff.tv_nsec)/1.0e9;
		cout<<"\nFor "<<np<<" Nodes:"<<endl;
		cout<<endl<<"Time : "<<time_s<<endl<<"Area of the mandlebrot set ="<<area_g<<endl;
		ofstream fout;   
        fout.open("s_scaling.csv",ios::app);
        fout<<np<<","<<time_s<<endl;
        fout.close();	
	}		
	

	ierr= MPI_Finalize();

	return 0;
}