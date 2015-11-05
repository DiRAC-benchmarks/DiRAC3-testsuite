#include <stdio.h>
#include <sys/time.h>

double timeIt()
{
	struct timeval tp;
	gettimeofday(&tp,NULL);
	return (double) (tp.tv_sec + tp.tv_usec/1000000.0);
}


void dummy(double* a, double *b, double* c, double*D)
{
	int i=0;
}

int main()
{
	long MAX_N=1000000000;
	long R = 10000;

		for(int j=10;j<MAX_N;j=j*10)
		{
			double a[j];
			double b[j];
			double c[j];
			double d[j];


			for(int k=0;k<j;k++)
			{
				a[k]=0.0;
				b[k]=1.0;
				c[k]=2.0;
				d[k]=3.0;
			}

			double start = timeIt();	

			for(int i=0;i<R;i++)
			{
				for(int k=0;k<j;k++)
				{
					a[k]=b[k]*c[k]+d[k];
				}
				if(a[0]<-1)
				{
					dummy(a,b,c,d);
				}
			}	

			double end = timeIt();

			double MFLOPS = R*j*2/(end - start);	
			printf("%ld MFLOPS - R %ld - N %ld\n",MFLOPS,R,j);

			
		}
	


	return 0;
}