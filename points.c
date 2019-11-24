#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>

#define ROWS 1000
#define COLUMNS 1000
#define iterations 50

void init(void)
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glMatrixMode(GL_PROJECTION);    
    gluOrtho2D(0.0, 1000.0, 0.0, 1000.0);
}

int get_color(int ite_time)
{
	if (ite_time - 1 == 50)
		return 0;

	int temp = (ite_time - 1) % iterations;

	if (temp >= 0 && temp <= 10)
		return 1;
	else if (temp > 10 && temp <= 20)
		return 2;
	else if (temp > 20 && temp <= 30)
		return 3;
	else if (temp > 30 && temp <= 40)
		return 4;
	else
		return 5;
}

void display(void)
{
  glClear(GL_COLOR_BUFFER_BIT);
    
  // read the file
  FILE* input = fopen("result.txt", "r");
	
	printf("Starting to read file...\n");

	int num;
	int color;
	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLUMNS; j++)
		{
			fscanf(input, "%d", &num);
      
			color = get_color(num);
      
      /*
      if (i == 0)
      {
        printf("%d ", num - 1);
        printf("%d ", color);
      }*/

			if (color == 0)
				glColor3f(0.0, 0.0, 0.0);
			else if (color == 1)
				glColor3f(0.0, 0.0, 1.0);
			else if (color == 2)
				glColor3f(0.0, 1.0, 0.0);
			else if (color == 3)
				glColor3f(0.0, 1.0, 1.0);
			else if (color == 4)
				glColor3f(1.0, 0.0, 0.0);
			else
				glColor3f(1.0, 0.0, 1.0);

			glBegin(GL_POINTS);
				glVertex2i(i, j);
			glEnd();
		}
	}
	fclose(input);

	printf("File reading finished...\n");
      
  glFlush();

}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(1000, 1000);
    glutCreateWindow("Julia Set View");
    
    init();
    glutDisplayFunc(display);
    
    glutMainLoop();

    return 0;
}
