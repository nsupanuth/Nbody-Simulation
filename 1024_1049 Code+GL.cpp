#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <omp.h>
#include <string.h>
#include <glut.h>
#include <math.h>
#include <windows.h>
#include <vector>
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



void cal()
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
	   
		   printf("%lf %lf \n",posx[i][R-1],posy[i][R-1]);
   }
		 
 printf("TIME %f\n",finish-start);

/*outfile = fopen("result.txt", "w+");
   for(i=0;i<num;i++)
   {
		   fprintf(outfile, "%lf %lf %d\n",x[i][R-1],y[i][R-1],m[i]);
   }

   fclose(outfile);*/



}

struct Point
{
    float x, y;
    unsigned char r, g, b, a;
};
std::vector< Point > points;


void initGL() {
 glClearColor(0.0, 0.0, 0.0, 1.0); // Set background (clear) color to black
}

void timer( int value )
{
	for( i = 0; i< num;i++ )
			{
        points[i].x = posx[i][frame];
		points[i].y = posy[i][frame];
	
			}
	frame++;
	if (frame==R-1)
		frame=0;

		glutPostRedisplay();
		glutTimerFunc(10, timer, 1000);
		
		 // subsequent timer call at milliseconds
}

void reshape(int w, int h)
{
    glViewport(0, 0, w, h);
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Set to 2D orthographic projection with the specified clipping area
	glMatrixMode(GL_PROJECTION);      // Select the Projection matrix for operation
	glLoadIdentity();                 // Reset Projection matrix
	gluOrtho2D(0.0, 400.0, 0.0, 400.0); // Set clipping area's left, right, bottom, top

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // draw
    glColor3ub( 255, 255, 255 );
    glEnableClientState( GL_VERTEX_ARRAY );
    glEnableClientState( GL_COLOR_ARRAY );
    glVertexPointer( 2, GL_FLOAT, sizeof(Point), &points[0].x );
    glColorPointer( 4, GL_UNSIGNED_BYTE, sizeof(Point), &points[0].r );
    glPointSize( 3.0 );
    glDrawArrays( GL_POINTS, 0, num );
    glDisableClientState( GL_VERTEX_ARRAY );
    glDisableClientState( GL_COLOR_ARRAY );

    glFlush();
    glutSwapBuffers();

}
void main(int argc, char *argv[])
{
	cal();
	
	glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);

	glutInitWindowSize(1280, 720);  // Initial window width and height
    glutCreateWindow("Supanuth");

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    

     // populate points
	
    for( size_t i = 0; i < num; ++i )
    {
        Point pt;
        pt.x = posx[i][0];
        pt.y = posy[i][0];
        pt.r = rand() % 30;
        pt.g = rand() % 255;
        pt.b = 255;
        pt.a = 255;
        points.push_back(pt);
    }
	initGL();
	glutTimerFunc(0, timer, 1);
    glutMainLoop();
}

#define R 1000
#define t 500
int read=0;
int i,j,k,r=0;
int num;
double G = 6.67428E-11;
double **x;
double **y;
int frame=0;
int f=0;
float start, finish;



void cal()
{

	FILE *infile;
	FILE *outfile;

	infile = fopen("N:\\CPE-Para\\Midterm\\inputTest.txt","r");
	fscanf(infile,"%d",&num);

	 x = (double **)malloc(num * sizeof(double*));
	   for(i = 0; i < num; i++)
		   x[i] = (double *)malloc(R * sizeof(double));

	 y = (double **)malloc(num * sizeof(double*));
	   for(i = 0; i < num; i++)
		   y[i] = (double *)malloc(R * sizeof(double));
	
	int *m = (int*)malloc(num * sizeof(int));
	double Fy,Fx,vx,vy,sx,sy,dx,dy,gmm,mag;
	double *ux = (double*)calloc(num , sizeof(double));
	double *uy = (double*)calloc(num , sizeof(double));

	

	while(!feof(infile))
	{
		for(i=0;i<num;i++)
        {
            fscanf(infile,"%lf",&x[i][0]);
			fscanf(infile,"%lf",&y[i][0]);
			fscanf(infile,"%d",&m[i]);
        }

    }

	
fclose(infile);

start = omp_get_wtime();


omp_set_num_threads(8);
//# pragma omp parallel private(j)
for(r=0;r<R-1;r++)
{
	
	//# pragma omp parallel for schedule(static)
	#pragma omp for private(j,sx,sy,dx,dy,Fx,Fy)
	for(i=0;i<num;i++)
	{
		Fx=0;
		Fy=0;

		//# pragma omp for schedule(static)
		for(j=0;j<num;j++)
		{
			if(i!=j)
			{
				dx=x[j][r]-x[i][r];
				dy=y[j][r]-y[i][r];
				gmm=G*m[i]*m[j]/(dx*dx+dy*dy);
				 Fx += gmm*dx;
                 Fy += gmm*dy;
			}
		}
		vx=ux[i]+(Fx/m[i])*t;
		vy=uy[i]+(Fy/m[i])*t;
		sx=((ux[i]+vx)/2)*t;
		sy=((uy[i]+vy)/2)*t;
		x[i][r+1]=x[i][r]+sx;
		y[i][r+1]=y[i][r]+sy;
		ux[i]=vx;
		uy[i]=vy;

		
	}
	

}

finish = omp_get_wtime();

printf("Total time : %f\n",finish-start);

 for(i=0;i<num;i++)
   {
	   
		   printf("%lf %lf \n",x[i][R-1],y[i][R-1]);
   }

/*outfile = fopen("result.txt", "w+");
   for(i=0;i<num;i++)
   {
		   fprintf(outfile, "%lf %lf %d\n",x[i][R-1],y[i][R-1],m[i]);
   }

   fclose(outfile);*/



}

struct Point
{
    float x, y;
    unsigned char r, g, b, a;
};
std::vector< Point > points;


void initGL() {
 glClearColor(0.0, 0.0, 0.0, 1.0); // Set background (clear) color to black
}

void timer( int value )
{
	for( i = 0; i< num;i++ )
			{
        points[i].x = x[i][frame];
		points[i].y = y[i][frame];
	
			}
	frame++;
	if (frame==R-2)
		frame=0;

		glutPostRedisplay();
		glutTimerFunc(10, timer, 1000);
		
		 // subsequent timer call at milliseconds
}

void reshape(int w, int h)
{
    glViewport(0, 0, w, h);
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Set to 2D orthographic projection with the specified clipping area
	glMatrixMode(GL_PROJECTION);      // Select the Projection matrix for operation
	glLoadIdentity();                 // Reset Projection matrix
	gluOrtho2D(0.0, 400.0, 0.0, 400.0); // Set clipping area's left, right, bottom, top

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // draw
    glColor3ub( 255, 255, 255 );
    glEnableClientState( GL_VERTEX_ARRAY );
    glEnableClientState( GL_COLOR_ARRAY );
    glVertexPointer( 2, GL_FLOAT, sizeof(Point), &points[0].x );
    glColorPointer( 4, GL_UNSIGNED_BYTE, sizeof(Point), &points[0].r );
    glPointSize( 3.0 );
    glDrawArrays( GL_POINTS, 0, num );
    glDisableClientState( GL_VERTEX_ARRAY );
    glDisableClientState( GL_COLOR_ARRAY );

    glFlush();
    glutSwapBuffers();

}
void main(int argc, char *argv[])
{
	cal();
	
	glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);

	glutInitWindowSize(1280, 720);  // Initial window width and height
    glutCreateWindow("Supanuth");

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    

     // populate points
	
    for( size_t i = 0; i < num; ++i )
    {
        Point pt;
        pt.x = x[i][0];
        pt.y = y[i][0];
        pt.r = rand() % 30;
        pt.g = rand() % 255;
        pt.b = 255;
        pt.a = 255;
        points.push_back(pt);
    }
	initGL();
	glutTimerFunc(0, timer, 1);
    glutMainLoop();
}