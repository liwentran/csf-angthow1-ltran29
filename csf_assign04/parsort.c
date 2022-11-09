#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

void error(char *msg) /* Unix-style error */ {
    fprintf(stderr, "%s\n", msg);
    exit(EXIT_FAILURE);
}

void print_arr(int64_t *arr) {
  for(int loop = 0; loop < 10; loop++)
    printf("%ld ", arr[loop]);

  printf("\n");
}
// Compare function for qsort
int cmpfunc (const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}


void merge(int64_t *arr, size_t begin, size_t mid, size_t end, int64_t *temparr) {
  int i, j, k;
  /* Merge the temp into temparr */
  i = begin;    // Initial index of leftside of the array
  j = mid + 1;  // Initial index of rightside of the array
  k = 0;    // Initial index of temp array
  while (i <= mid && j <= end) {
    if (arr[i] <= arr[j]) {
      temparr[k] = arr[i];
      i++;
    }
    else {
      temparr[k] = arr[j];
      j++;
    }
    k++;
  }

  /* Copy the remaining elements of the left half, if there are any */
  while (i <= mid) {
    temparr[k] = arr[i];
    i++;
    k++;
  }

  /* Copy the remaining elements of of the right half, if there are any */
  while (j <= end) {
    temparr[k] = arr[j];
    j++;
    k++;
  }
  
}

void merge_sort(int64_t *arr, size_t begin, size_t end, size_t threshold) {
  size_t arr_size =  end - begin + 1;

  // problem is small enough to use qsort
  if (arr_size <= threshold) {
    qsort(arr, arr_size, sizeof(int), cmpfunc);
    return;
  } else if (begin < end) {
    int m = begin + (end - begin) / 2;

    /*
    merge_sort(arr, begin, m, threshold);
    merge_sort(arr, m + 1, end, threshold);
    */

    int pid1 = -1;
    int pid2 = -1;

    // Sort first and second halves IN PARALLEL
    pid1 = fork();
    if (pid1 == -1) {
      error("Error: could not create fork 1");
      exit(EXIT_FAILURE);
    } else if (pid1 == 0) {
      // sort the left side
      merge_sort(arr, begin, m, threshold);
      exit(EXIT_SUCCESS);
    }
    
    pid2 = fork();
    if (pid2 == -1) {
      error("Error: could not create fork 2");
      exit(EXIT_FAILURE);
    } else if (pid2 == 0) {
      // sort the right side
      merge_sort(arr, m + 1, end, threshold);
      exit(EXIT_SUCCESS);
    }


    //pause program execution until the child processs has completed
    int wstatus;
    int wstatus2;
    
    // blocks until first proces completes
    pid_t actual_pid1 = waitpid(pid1, &wstatus, 0);
    pid_t actual_pid2 = waitpid(pid2, &wstatus2, 0);

    if (actual_pid1 == -1 || actual_pid2 == -1) {
      error("Error: waitpid failure");
      exit(EXIT_FAILURE);
    }
    if (!WIFEXITED(wstatus) || !WIFEXITED(wstatus2)) {
      error("Error: subprocess crashed, was interrupted, or did not exit normally");
      exit(EXIT_FAILURE);
    }
    if (WEXITSTATUS(wstatus) != 0 || WEXITSTATUS(wstatus2) != 0) {
      // if following standard UNIX conventions, this is also an error
      error("Error: subprocess returned a non-zero exit code");
      exit(EXIT_FAILURE);
    }


    // merge the sorted sequences in a temp array
    int64_t* temp_array = (int64_t*) malloc(arr_size * sizeof(int64_t));
    merge(arr, begin, m, end, temp_array);
    
    // copy the temp array back into the array
    // for (int i = 0; i < arr_size; i++) {
    //   arr[begin+i] = temp_array[i];
    // }
    memcpy(arr, &arr[begin], arr_size * sizeof(int));
    free(temp_array);
  }
}

int main(int argc, char **argv) {
  // check for correct number of command line arguments
  if (argc != 3) {
    fprintf(stderr, "Usage: %s <filename> <sequential threshold>\n", argv[0]);
    return 1;
  }

  // process command line arguments
  const char *filename = argv[1];
  char *end;
  size_t threshold = (size_t) strtoul(argv[2], &end, 10);

  /* report an error (threshold value is invalid) */;
  if (end != argv[2] + strlen(argv[2]))  {
    error("Error: threshold value is invalid");
  }

  // open the file
  int fd = open(filename, O_RDWR);
  if (fd < 0) {
    // file couldn't be opened: handle error and exit
    fprintf(stderr, "Error: file couldn't be opened");
    exit(EXIT_FAILURE);
  }


  // use fstat to determine the size of the file
  struct stat statbuf;
  int rc = fstat(fd, &statbuf);
  if (rc != 0) {
    fprintf(stderr, "Error: could not run fstat on file");
    exit(EXIT_FAILURE);
  }
  size_t file_size_in_bytes = statbuf.st_size;

  // Map the file into memory using mmap
  int64_t *data = mmap(NULL, file_size_in_bytes, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if (data == MAP_FAILED) {
    // handle mmap error and exit
    fprintf(stderr, "Error: mmap error");
    exit(EXIT_FAILURE);
  }
  // *data now behaves like a standard array of int64_t. Be careful though! Going off the end
  // of the array will silently extend the file, which can rapidly lead to disk space
  // depletion!

  // sort the data!
  merge_sort(data, 0, file_size_in_bytes - 1, threshold);

  // TODO: unmap and close the file
  int um = munmap(data, file_size_in_bytes);
  if (um != 0) {
    fprintf(stderr, "Error: could not unmap file");
    exit(EXIT_FAILURE);
  }
  int c = close(fd);
  if (c != 0) {
    fprintf(stderr, "Error: could not close file");
    exit(EXIT_FAILURE);
  }
  // TODO: exit with a 0 exit code if sort was successful
  exit(EXIT_SUCCESS);
}
