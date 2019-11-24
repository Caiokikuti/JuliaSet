#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>

#define ROWS 1000
#define COLUMNS 1000
// #define cx -1.0
// #define cy 0.0
#define iterations 50

#define MIN(a,b) (a < b ? a : b)

void array_to_file(int* array)
{
	FILE* out = fopen("result.txt", "w");

	int i = 0;
	for (i = 0; i < ROWS * COLUMNS; i++) {
		fprintf(out, "%d ", array[i]);
	}

	fclose(out);
}

void init_indicesNlengths(int* indices, int* lengths, int remainder, int sub_rows, int num_procs)
{
	for (int i = 0; i < num_procs; i++)
	{
		if (i < remainder)
			lengths[i] = sub_rows + 1;
		else
			lengths[i] = sub_rows;
	}

	for (int i = 0; i < num_procs; i++)
		indices[i] = i * sub_rows + MIN(i, remainder);
}

int main(int argc, char** argv) {
	int num_procs;
	int rank;
 
  float cx = atof(argv[1]);
  float cy = atof(argv[2]);

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

	int remainder = ROWS % num_procs;
	int sub_rows = ROWS / num_procs;

	int* indices = malloc(sizeof(int) * num_procs);
	int* part_lengths = malloc(sizeof(int) * num_procs);

	if (rank == 0)
		init_indicesNlengths(indices, part_lengths, remainder, sub_rows, num_procs);

	MPI_Bcast(indices, num_procs, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(part_lengths, num_procs, MPI_INT, 0, MPI_COMM_WORLD);

	int local_row = part_lengths[rank];
	int local_size = local_row * COLUMNS;
	int* local_colors = malloc(sizeof(int) * local_size);

	if (local_colors == NULL)
	{
		printf("fk you!\n");
		exit(-1);
	}
	
	int local_start = indices[rank];
	int local_end = local_start + local_row;

	float zx, zy;
	float temp;

	int color;
	int counter = 0;
	for (int i = local_start; i < local_end; i++)
	{
		for (int j = 0; j < COLUMNS; j++)
		{
      zx = (float)(i + 1 - 500) / 250;
      zy = (float)(j + 1 - 500) / 250;
			
			color = 0;
			
			while ((float)zx * zx + zy * zy <= 4.00 && color <= iterations)
			{
				temp = (float)zx * zx - zy * zy;
				zy = (float)2 * zx * zy + cy;
				zx = (float)temp + cx;
				color++;
			}

			local_colors[counter] = color;
			counter++;
		}
	}
  if (rank == 1)
    printf("counter: %d\n", counter);

	int* colors;
	int* color_indices;
	int* color_part_lengths;
	if (rank == 0)
	{
		colors = malloc(sizeof(int) * ROWS * COLUMNS);
		color_indices = malloc(sizeof(int) * num_procs);
		color_part_lengths = malloc(sizeof(int) * num_procs);

		for (int i = 0; i < num_procs; i++)
		{
			color_part_lengths[i] = part_lengths[i] * COLUMNS;
			color_indices[i] = indices[i] * COLUMNS;
		}
    printf("\n");
    for (int i = 0; i < num_procs; i++)
      printf("%d ", color_part_lengths[i]);
    printf("\n");
    for (int i = 0; i < num_procs; i++)
      printf("%d ", color_indices[i]);
    printf("\n");
	}
  
	MPI_Gatherv(local_colors, local_size, MPI_INT, colors, color_part_lengths, color_indices, MPI_INT, 0, MPI_COMM_WORLD);
  
	if (rank == 0)
	{
    
		array_to_file(colors);

		free(colors);
		free(color_indices);
		free(color_part_lengths);
	}
	free(indices);
	free(part_lengths);
	free(local_colors);

	printf("\nProcessor %d done!\n", rank);

	MPI_Finalize();

	return 0;
}
