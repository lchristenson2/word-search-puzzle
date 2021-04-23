#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Declarations of the two functions you will implement
// Feel free to declare any helper functions
void printPuzzle(char** arr, int n);
void searchPuzzle(char** arr, int n, char** list, int listSize);

void convert_case(char** arr, int i, int j, int e_i, int e_j) {
  if(*(*(arr+i)+j) < 97) {  //if char at i,j isn't already lower case
    *(*(arr+i)+j) += 32;
  }

  if(i!=e_i || j!=e_j) {
    if(i<e_i && j<e_j) {  //if called from horSearch()
      convert_case(arr, i+1, j+1, e_i, e_j);
    }
    else if (i>e_i && j<e_j){   //if called from vertSearch()
      convert_case(arr, i-1, j+1, e_i, e_j);
    }
    else if(i==e_i) {   //if called from diagSearch(top right to bottom left)
     convert_case(arr, i, j+1, e_i, e_j);
    }
    else if(j==e_j) {   //if called from diagSearch(bottom right to top left)
      convert_case(arr, i+1, j, e_i, e_j);
    }
  }
}

int horSearch(char** arr, int n, char* word, int wordlen) {
  int i, j, k = 0;     //k - traverse thru word's characters to find consecutive char matches

  for(i = 0; i < n; i++) {
    for(j = 0; j < n; j++) {
      //accounts for casing in all chars in word;   if char in arr at i,j matches the that in word, increase counter
      if(*(*(arr + i) + j) == *(word + k) - 32 || *(*(arr + i) + j) == *(word + k) + 32 || *(*(arr + i) + j) == *(word + k)) {
        k++;  //move on to next char in word
      }
      else { k = 0; }   //reset to zero if next char in arr doesn't match with one in word

      if(k == wordlen) {    //if word matches characters with a consecutive string in arr
        convert_case(arr, i, j-wordlen, i, j-1);
        return 1;
      }
    }
  }

  return 0;
}

int vertSearch(char** arr, int n, char* word, int wordlen) {
  int i, j, k = 0;     //k - traverse thru word's characters

  //top to bottom
  for(j = 0; j < n; j++) {
    for(i = 0; i < n; i++) {
      if(*(*(arr + i) + j) == *(word + k) - 32 || *(*(arr + i) + j) == *(word + k) + 32 || *(*(arr + i) + j) == *(word + k)) {
        k++;
      }
      else { k = 0; }

      if(k == wordlen) {
        convert_case(arr, i - wordlen+1, j, i, j);
        return 1;
      }
    }
  }

  //bottom to top
  k = 0;
  for(j = n-1; j >= 0; j--) {
    for(i = n-1; i >= 0; i--) {
      if(*(*(arr + i) + j) == *(word + k) - 32 || *(*(arr + i) + j) == *(word + k) + 32 || *(*(arr + i) + j) == *(word + k)) {
        k++;
      }
      else { k = 0; }

      if(k == wordlen) {
        convert_case(arr, i, j, i + wordlen-1, j);
        return 1;
      }
    }
  }

  return 0;
}

int diagSearch(char** arr, int n, char* word, int wordlen) {
  int i, j, l, k = 0;     //k - traverse thru word's characters

  /* TOP RIGHT TO BOTTOM LEFT */

  for(j = n-wordlen; j >= 0; j--) {
    i = 0, l = j;

    while(i < n && l < n) {
      if(*(*(arr + i) + l) == *(word + k) - 32 || *(*(arr + i) + l) == *(word + k) + 32 || *(*(arr + i) + l) == *(word + k)) {
        k++;
      }
      else { k = 0; }

      if(k == wordlen) {
        convert_case(arr, i-wordlen+1, l-wordlen+1, i, l);
        return 1;
      }

      i++;
      l++;
    }
  }

  //second half of the search (for top right to bottom left), i starts at row 1: row 2 already searached
  k = 0;
  for(i = 1; i < n; i++) {
    j = 0, l = i;

    while(j < n && l < n) {
      if(*(*(arr + l) + j) == *(word + k) - 32 || *(*(arr + l) + j) == *(word + k) + 32 || *(*(arr + l) + j) == *(word + k)) {
        k++;
      }
      else { k = 0; }

      if(k == wordlen) {
        convert_case(arr, l-wordlen+1, j-wordlen+1, l, j);
        return 1;
      }

      j++;
      l++;
    }
  }

/* BOTTOM RIGHT TO TOP LEFT */
  k = 0;
  for(j = n-wordlen; j >= 0; j--) {
    i = n-1, l = j;

    while(i >= 0 && l >= 0) {
      if(*(*(arr + i) + l) == *(word + k) - 32 || *(*(arr + i) + l) == *(word + k) + 32 || *(*(arr + i) + l) == *(word + k)) {
        k++;
      }
      else { k = 0; }

      if(k == wordlen) {
        convert_case(arr, i+wordlen-1, l-wordlen+1, i, l);
        return 1;
      }

      l++;
      i--;
    }
  }

  k = 0;
  for(i = n-2; i >= wordlen-1; i--) {
    j = 0, l = i;

    while(j < n && l >= 0) {
      if(*(*(arr + l) + j) == *(word + k) - 32 || *(*(arr + l) + j) == *(word + k) + 32 || *(*(arr + l) + j) == *(word + k)) {
        k++;
      }
      else { k = 0; }

      if(k == wordlen) {
        convert_case(arr, l+wordlen-1, j-wordlen+1, l, j);
        return 1;
      }

      l--;
      j++;
    }
  }

  return 0;
}

// Main function, DO NOT MODIFY (except line 52 if your output is not as expected -- see the comment there)!!!
int main(int argc, char **argv) {
    int bSize = 15;
	if (argc != 2) {
		fprintf(stderr, "Usage: %s <puzzle file name>\n", argv[0]);
		return 2;
	}
    int i, j;
    FILE *fptr;
    char **block = (char**)malloc(bSize * sizeof(char*));
	  char **words = (char**)malloc(50 * sizeof(char*));

    // Open file for reading puzzle
    fptr = fopen(argv[1], "r");
    if (fptr == NULL) {
        printf("Cannot Open Puzzle File!\n");
        return 0;
    }

	// Read puzzle block into 2D arrays
    for(i = 0; i < bSize; i++) {
        *(block+i) = (char*)malloc(bSize * sizeof(char));

        fscanf(fptr, "%c %c %c %c %c %c %c %c %c %c %c %c %c %c %c\n", *(block+i), *(block+i)+1, *(block+i)+2, *(block+i)+3, *(block+i)+4, *(block+i)+5, *(block+i)+6, *(block+i)+7, *(block+i)+8, *(block+i)+9, *(block+i)+10, *(block+i)+11, *(block+i)+12, *(block+i)+13, *(block+i)+14 );
    }
	fclose(fptr);

	// Open file for reading word list
	fptr = fopen("states.txt", "r");
	if (fptr == NULL) {
        printf("Cannot Open Words File!\n");
        return 0;
    }

	// Save words into arrays
	for(i = 0; i < 50; i++) {
		*(words+i) = (char*)malloc(20 * sizeof(char));
		fgets(*(words+i), 20, fptr);
	}

	// Remove newline characters from each word (except for the last word)
	for(i = 0; i < 49; i++) {
		*(*(words+i) + strlen(*(words+i))-2) = '\0'; // Change -2 to -1 if line 58 outputs words with the last character missing
	}

	// Print out word list
	printf("Printing list of words:\n");
	for(i = 0; i < 50; i++) {
		printf("%s\n", *(words + i));
	}
	printf("\n");

	// Print out original puzzle grid
  printf("Printing puzzle before search:\n");
  printPuzzle(block, bSize);
	printf("\n");

	// Call searchPuzzle to find all words in the puzzle
	searchPuzzle(block, bSize, words, 50);
	printf("\n");

	// Print out final puzzle grid with found words in lower case
  printf("Printing puzzle after search:\n");
  printPuzzle(block, bSize);
	printf("\n");

  return 0;
}

void printPuzzle(char** arr, int n) {
	// This function will print out the complete puzzle grid (arr). It must produce the output in the SAME format as the samples in the instructions.
	// Your implementation here
  int i, j;

  for(i = 0; i < n; i++) {
    for(j = 0; j < n; j++) {
      printf("%c ", *(*(arr + i) + j));
    }
    printf("\n");
  }

}

void searchPuzzle(char** arr, int n, char** list, int listSize) {
	// This function checks if arr contains words from list. If a word appears in arr, it will print out that word and then convert that word entry in arr into lower case.
	// Your implementation here
  int i,j, wordlen;

  for(int cWord = 0; cWord < listSize; cWord++) {
    wordlen = strlen(*(list + cWord));

    if(horSearch(arr, n, *(list + cWord), wordlen) == 1) {
      printf("Word Found: %s\n", *(list + cWord));
    }
    else if (vertSearch(arr, n, *(list + cWord), wordlen) == 1) {
      printf("Word Found: %s\n", *(list + cWord));
    }
    else if (diagSearch(arr, n, *(list + cWord), wordlen) == 1) {
      printf("Word Found: %s\n", *(list + cWord));
    }
  }
}
