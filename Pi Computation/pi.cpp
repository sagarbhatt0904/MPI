 /* 
Computing Pi using midpoint rule by computing the following integration:
pi=int{0,1}(sqrt(1-x^2))
int{a,b}(f(x))=((b-a)/N)(sum{1,N}(f(a+(k-0.5)((b-a)/N))))
*/
#include <iostream>
#include <math.h>
#include <fstream>
#include "timer.h"
#include <iomanip>

using namespace std;
int main()
{
    int N;
    double p,x;
    double sum;
    double a=0, b=1;
    timespec before, after, time_diff;
    
    ifstream fin("input.txt");
    fin>>N;
    fin.close();

    double time_s, speed, serial_frac;
    int k;
       
    get_time(&before);
    sum=0;
    for (int k = 0; k < N; ++k)
    {
    	x=a+((k-0.5)*(b-a)/N);    	
    	sum+=sqrt(1-pow(x,2));
    }
    p=4*((b-a)/N)*sum;
    get_time(&after);

    diff(&before,&after,&time_diff);
	time_s = time_diff.tv_sec + (double)(time_diff.tv_nsec)/1.0e9;
    cout<<"Time taken: "<<time_s<<" s"<<endl;
    cout<<"Pi="<<setprecision(18)<<p<<endl;  

        
    return 0;
}