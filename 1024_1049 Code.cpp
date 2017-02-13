#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <omp.h>
#include <string.h>
#include <glut.h>
#include <math.h>
#include <windows.h>
#include <vector>


#define R 1001
#define t 250
int read=0;
int i,j,k,l,r=0;
int num;
double G = 6.67428E-11;
double *x;
double *y;
int frame=0;
int f=0;
float start, finish;
int pre,fu,ipre,jpre,ifu;
double **posx;
double **posy;



void main(int argc, char *argv[])
{

	FILE *infile;
	FILE *outfile;

	infile = fopen("N:\\CPE-Para\\Midterm\\inputTest.txt","r");
	fscanf(infile,"%d",&num);

	 x = (double*)malloc(num * R * sizeof(double));
	 y = (double*)malloc(num * R * sizeof(double));
	
	int *m = (int*)malloc(num * sizeof(int));
	double Fy,Fx,vx,vy,sx,sy,dx,dy,gmm;
	double *ux = (double*)calloc(num , sizeof(double));
	double *uy = (double*)calloc(num , sizeof(double));

	

	while(!feof(infile))
	{
		for(i=0;i<num;i++)
        {
            fscanf(infile,"%lf",&x[i]);
			fscanf(infile,"%lf",&y[i]);
			fscanf(infile,"%d",&m[i]);
        }

    }

	
fclose(infile);


start = omp_get_wtime();


omp_set_num_threads(8);
//# pragma omp parallel private(j)
for(r=0;r<R-1;r++)
{
	pre=r*num;
	fu=(r+1)*num;
	//# pragma omp parallel for schedule(static)
	#pragma omp for private(i,j,sx,sy,dx,dy,Fx,Fy,vx,vy)
	for(i=0;i<num;i++)
	{
		Fx=0;
		Fy=0;
		//# pragma omp for schedule(static)
		for(j=0;j<num;j++,l++)
		{
			if(i!=j)
			{
				dx=x[j+pre]-x[i+pre];
				dy=y[j+pre]-y[i+pre];
				gmm=G*m[i]*m[j]/(dx*dx+dy*dy);
				 Fx += gmm*dx;
                 Fy += gmm*dy;
			}
		}
		vx=ux[i]+(Fx/m[i])*t;
		vy=uy[i]+(Fy/m[i])*t;
		sx=((ux[i]+vx)/2)*t;
		sy=((uy[i]+vy)/2)*t;
		x[i+fu]=x[i+pre]+sx;
		y[i+fu]=y[i+pre]+sy;
		ux[i]=vx;
		uy[i]=vy;
	}
	

}

finish = omp_get_wtime();


	posx = (double **)malloc(num * sizeof(double*));
	   for(i = 0; i < num; i++)
		   posx[i] = (double *)malloc(R * sizeof(double));

	posy = (double **)malloc(num * sizeof(double*));
	   for(i = 0; i < num; i++)
		   posy[i] = (double *)malloc(R * sizeof(double));
k=0;
 for(j=0;j<R;j++)
 {
	 for(i=0;i<num;i++)
	 {
		 posx[i][j]=x[k];
		 posy[i][j]=y[k];
		 k++;
	 }
 }

 for(i=0;i<num;i++)
   {
	   
		   printf("%lf %lf %d\n",posx[i][R-1],posy[i][R-1],m[i]);
   }
		 
 printf("TIME %f\n",finish-start);

outfile = fopen("N:\\CPE-Para\\Midterm\\result.txt", "w+");

   fprintf(outfile,"%d",num);
for(r=0;r<R;r++)
{
	for(i=0;i<num;i++)
   {
		   fprintf(outfile, "%lf %lf\n",posx[i][r],posy[i][r]);
   }
}

   fclose(outfile);



}