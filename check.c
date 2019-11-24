#include <stdio.h>
#include <stdlib.h>


int main(int argc, char** argv)
{
	FILE* input;
	input = fopen("result.txt", "r");

	printf("Starting to read file...\n");

	int num;
  int counter = 0;
  int temp = 0;
	while (fscanf(input, "%d", &num) != EOF)
	{
    if (num == 51 && counter < 1000)
    {
      temp = counter;
      break;
    }
		  //printf("%d", num);
    counter++;
	}
	fclose(input);
  
  printf("%d", temp);

	printf("File reading finished...\n");
}