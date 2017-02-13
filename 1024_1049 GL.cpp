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
int frame=0;
int f=0;
float start, finish;
int pre,fu,ipre,jpre,ifu;
double **posx;
double **posy;


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
	FILE *infile;

	infile = fopen("N:\\CPE-Para\\Midterm\\result.txt","r");
	fscanf(infile,"%d ",&num);

	posx = (double **)malloc(num * sizeof(double*));
	   for(i = 0; i < num; i++)
		   posx[i] = (double *)malloc(R * sizeof(double));

	posy = (double **)malloc(num * sizeof(double*));
	   for(i = 0; i < num; i++)
		   posy[i] = (double *)malloc(R * sizeof(double));

	   while(!feof(infile))
	{
		for(r=0;r<R;r++)
		{
		for(i=0;i<num;i++)
        {
            fscanf(infile,"%lf",&posx[i][r]);
			fscanf(infile,"%lf",&posy[i][r]);
		}
		}

    }
fclose(infile);

	
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