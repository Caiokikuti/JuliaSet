#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <FreeImage.h>

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

void grab()
{
	unsigned char *mpixels = malloc(sizeof(unsigned char) * ROWS * ( COLUMNS - 155 ) * 3);
    glReadBuffer(GL_FRONT);
    glReadPixels(0, 0, ROWS, ( COLUMNS - 155 ), GL_RGB, GL_UNSIGNED_BYTE, mpixels);
    glReadBuffer(GL_BACK);
    for(int i = 0; i < (int)ROWS*( COLUMNS - 155 )*3; i += 3)
    {   
        mpixels[i] ^= mpixels[i+2] ^= mpixels[i] ^= mpixels[i+2];
    }
    FIBITMAP* bitmap = FreeImage_Allocate(ROWS, ( COLUMNS - 155 ), 24, 8, 8, 8);
 
    for(int y = 0 ; y < FreeImage_GetHeight(bitmap); y++)
    {
        BYTE *bits = FreeImage_GetScanLine(bitmap, y);
        for(int x = 0 ; x < FreeImage_GetWidth(bitmap); x++)
        {
            bits[0] = mpixels[(y*COLUMNS+x) * 3 + 0];
            bits[1] = mpixels[(y*COLUMNS+x) * 3 + 1];
            bits[2] = mpixels[(y*COLUMNS+x) * 3 + 2];
            bits += 3;
        }
 
    }
 
	FreeImage_Save(FIF_JPEG, bitmap, "output.jpg", JPEG_DEFAULT);
 
    FreeImage_Unload(bitmap);
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
 
    grab();
      
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
