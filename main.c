#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <pthread.h>

#define MATRIX_SIZE 4
#define BATCH_SIZE 10
#define NUM_THREADS 4

typedef struct {
  int *a;
  int *b;
  int *c;
  int column_index;
  int row_index;
} ThreadData;


void initialize_matrix(int *matrix, int rows, int columns) {
  for (int i = 0; i < rows; i++) { 
    for (int j = 0; j < columns; j++) { 
      matrix[i * columns + j] = rand() % 10;
    }
  }
}

void print_matrix(int *matrix, int rows, int columns) {
  for (int i = 0; i < rows; i++) { 
    for (int j = 0; j < columns; j++) { 
      printf("%d ",matrix[i * columns + j]);
    }
    printf("\n");
  }
}

void *multiply_matrix(void *arg) {
  ThreadData *data = (ThreadData *)arg;
  int result_column = 0;
  for (int i = 0; i < MATRIX_SIZE; i++) {
    result_column += data->b[i * MATRIX_SIZE + data->column_index] * data->a[i];
  }
  data->c[data->column_index] = result_column;
  printf("column_index: %d row_index: %d result: %d \n", data->column_index, data->row_index, result_column);
  return NULL;
}

int main(int argc, char *argv[]) { 
  int rank, size;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
 
  int *a = NULL, *b = NULL, *c = NULL;
  int rows_per_process = MATRIX_SIZE / size;

  b = (int *)  malloc(MATRIX_SIZE * MATRIX_SIZE * sizeof(int));
  if (rank == 0) {
    a = (int *)  malloc(MATRIX_SIZE * MATRIX_SIZE * sizeof(int));
    c = (int *)  malloc(MATRIX_SIZE * MATRIX_SIZE * sizeof(int)); 
    initialize_matrix(a, MATRIX_SIZE, MATRIX_SIZE);
    initialize_matrix(b, MATRIX_SIZE, MATRIX_SIZE);
    printf("MATRIZ A \n");
    print_matrix(a, MATRIX_SIZE, MATRIX_SIZE);
    printf("MATRIZ B \n");
    print_matrix(b, MATRIX_SIZE, MATRIX_SIZE);
  }
  
  int *local_a = malloc(rows_per_process * MATRIX_SIZE * sizeof(int));
  int *local_c = malloc(rows_per_process * MATRIX_SIZE * sizeof(int));  

  MPI_Scatter(a, rows_per_process * MATRIX_SIZE,MPI_INT, local_a,
              rows_per_process * MATRIX_SIZE, MPI_INT, 0, MPI_COMM_WORLD);
  
  MPI_Bcast(b, MATRIX_SIZE * MATRIX_SIZE, MPI_INT, 0, MPI_COMM_WORLD);
  printf("rank: %d \n", rank);
  print_matrix(local_a, rows_per_process, rows_per_process * MATRIX_SIZE);
   
  printf("MATRIX B ON RANK: %d \n", rank);
  print_matrix(b, MATRIX_SIZE, MATRIX_SIZE);

  pthread_t threads[NUM_THREADS];
  ThreadData thread_data[NUM_THREADS];

  for (int i = 0; i < NUM_THREADS; i++) { 
    thread_data[i].a = local_a;
    thread_data[i].b = b;
    thread_data[i].c = local_c;
    thread_data[i].column_index = i;
    thread_data[i].row_index = rank;
    pthread_create(&threads[i], NULL, multiply_matrix, &thread_data[i]);
  }

  for (int i = 0; i < NUM_THREADS; i++) {
    pthread_join(threads[i], NULL);
  }

  //printf("local c %d \n", rank);
  //print_matrix(local_c, MATRIX_SIZE, MATRIX_SIZE);
  MPI_Gather(local_c, rows_per_process * MATRIX_SIZE, MPI_INT, c
             ,rows_per_process * MATRIX_SIZE, MPI_INT, 0, MPI_COMM_WORLD);

  if (rank == 0) {
    print_matrix(c, MATRIX_SIZE, MATRIX_SIZE);
    free(a);
    free(c);

  }
}








