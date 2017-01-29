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

	int my_p, np, new_np, new_my_p, ierr; /*Variable for MPI init*/
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
  	 int color = np%2;

	// Split the communicator based on the color and use the
	// original rank for ordering
	MPI_Comm new_comm;
	MPI_Comm_split(MPI_COMM_WORLD, color, my_p, &new_comm);
	ierr= MPI_Comm_rank(MPI_COMM_WORLD, &new_my_p);
    ierr= MPI_Comm_size(MPI_COMM_WORLD, &new_np);
   
  
	

	std::vector<std::vector<double> > c_real (N,std::vector<double>(N, 0)); /*Real component of c*/
	std::vector<std::vector<double> > c_img (N,std::vector<double>(N, 0)); /*Imaginary component of c*/
	
	double time_s;

	
	double area1=0, area_g1=0, area2=0, area_g2=0, area_net=0;	/*Initializing area computed per process and global area*/
	 if(my_p==root)
	
    {
        get_time(&before);      
    }    
	if( color ==  0)
	{
	
	int count = N/new_np;
    int start, end ;
    start=(my_p)*count;
    end=(my_p+1)*count;
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
				area1+=((x_max-x_min)/N)*((y_max-y_min)/N);
			}
		}			
	}
	
	ierr=MPI_Reduce(&area1, &area_g1, 1, MPI_DOUBLE, MPI_SUM, root, new_comm);
}
else
{	
	int count = N/new_np;
    int start, end ;
    start=(my_p)*count;
    end=(my_p+1)*count;
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
				area2+=((x_max-x_min)/N)*((y_max-y_min)/N);
			}
		}			
	}
	ierr=MPI_Reduce(&area2, &area_g2, 1, MPI_DOUBLE, MPI_SUM, root, new_comm);
}
	// cout<<"From Process: "<<my_p<<" Area of the mandlebrot set ="<<area<<endl;
	if(my_p==root)
	{
		get_time(&after);
		diff(&before,&after,&time_diff);
		time_s = time_diff.tv_sec + (double)(time_diff.tv_nsec)/1.0e9;
		area_net=area_g1+area_g2;
		cout<<"\nFor "<<np<<" Nodes:"<<endl;
		cout<<endl<<"Time : "<<time_s<<endl<<"Area of the mandlebrot set ="<<area_net<<endl;
		ofstream fout;   
        fout.open("s_scaling_s.csv",ios::app);
        fout<<np<<","<<time_s<<endl;
        fout.close();	
	}		
	

	ierr= MPI_Finalize();

	return 0;
}