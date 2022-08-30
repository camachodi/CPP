// comparative.c
// Escuela Superior de Arte y Tecnologia
// Algoritmos & Inteligencia Artificial
// ESAT 2020-2021
//
// File for comparative of ADTs

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "ABGS_MemoryManager/abgs_memory_manager.h"
#include "ABGS_MemoryManager/abgs_platform_types.h"
#include "common_def.h"
#include "adt_vector.h"
#include "adt_list.h"
#include "adt_dlist.h"

const int data_size = 10000;

void* data[10000];

Vector* vec_src = NULL;
Vector* vec_con = NULL;
Vector* vec_con_aux = NULL;
List* list_src = NULL;
List* list_concat = NULL;

DLList* dlist_src = NULL;
DLList* dlist_concat_1 = NULL;
DLList* dlist_concat_2 = NULL;

void TESTBASE_generateDataForComparative() {
	  
  for (int i = 0; i < data_size; i++) {

    data[i] = MM->malloc(20);
    sprintf(data[i], "JalapenoNoEssFrustum");

  }

	list_concat = LIST_create(1);

  if (NULL == vec_src) 
  { 
    vec_src = VECTOR_create(data_size); 
  }

  if (NULL == vec_con)
  {
    vec_con = VECTOR_create(1);
  }

  if (NULL == vec_con_aux)
  {
    vec_con_aux = VECTOR_create(1);
  }

  vec_con_aux->ops_->insertFirst(vec_con_aux, data[0], 20);

	if (NULL == list_src)
	{
		list_src = LIST_create(data_size);
	}

	list_concat->ops_->insertLast(list_concat,data[0],20);

	if (NULL == dlist_src)
	{
		dlist_src = DLLIST_create(data_size);
	}

	dlist_concat_1 = DLLIST_create(1);
	dlist_concat_2 = DLLIST_create(1);

	dlist_concat_1->ops_->insertLast(dlist_concat_1, data[0], 20);
	dlist_concat_2->ops_->insertLast(dlist_concat_2, data[0], 20);

}

void Free_data() {

  vec_src->ops_->softReset(vec_src);
  vec_src->ops_->destroy(vec_src);

  vec_con->ops_->reset(vec_con);
	vec_con->ops_->destroy(vec_con);

	vec_con_aux->ops_->softReset(vec_con_aux);
	vec_con_aux->ops_->destroy(vec_con_aux);

	//list_src->ops_->softReset(list_src);
	list_src->ops_->reset(list_src);
	list_src->ops_->destroy(list_src);

	list_concat->ops_->softReset(list_concat);
	list_concat->ops_->destroy(list_concat);

	dlist_src->ops_->reset(dlist_src);
	dlist_src->ops_->destroy(dlist_src);

	dlist_concat_1->ops_->softReset(dlist_concat_1);
	dlist_concat_1->ops_->destroy(dlist_concat_1);

	dlist_concat_2->ops_->softReset(dlist_concat_2);
	dlist_concat_2->ops_->destroy(dlist_concat_2);

  for (int i = 0; i < data_size; i++) {
	if(data[i] != NULL)
     MM->free(data[i]);
  }
}

void calculateTimeForFunction_Vector_Inserfrist() {
  LARGE_INTEGER frequency;				// ticks per second
  LARGE_INTEGER  time_start, time_end;    // ticks in interval
  double elapsed_time = 0.0f;
  u32 repetitions = 10000;

  ///////////////////////////////////////////////////////////////////////
  // Frequency: ticks per second
  QueryPerformanceFrequency(&frequency);
  ///////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////
  // Meassurement time
  // start timer
  QueryPerformanceCounter(&time_start);
  // execute function to meassure 'repetitions' times
  for (u32 rep = 0; rep < repetitions; ++rep) {

    vec_src->ops_->insertFirst(vec_src, data[rep], 20);
  }
  // stop timer
  QueryPerformanceCounter(&time_end);
  ///////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////
  // compute the elapsed time in microseconds
  elapsed_time = (time_end.QuadPart - time_start.QuadPart) * 1000000.0f / frequency.QuadPart;
  printf("Elapsed time Vector_InsertFirst: %f ms\n", elapsed_time);
  ///////////////////////////////////////////////////////////////////////
  // compute the average time
  double average_time = elapsed_time / repetitions;
  printf("Average time Vector_InsertFirst: %f ms\n", average_time);
  ///////////////////////////////////////////////////////////////////////

  //vec_src->ops_->print(vec_src);
}

void calculateTimeForFunction_Vector_extractfrist() {
  LARGE_INTEGER frequency;				// ticks per second
  LARGE_INTEGER  time_start, time_end;    // ticks in interval
  double elapsed_time = 0.0f;
  u32 repetitions = 10000;

  ///////////////////////////////////////////////////////////////////////
  // Frequency: ticks per second
  QueryPerformanceFrequency(&frequency);
  ///////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////
  // Meassurement time
  // start timer
  QueryPerformanceCounter(&time_start);
  // execute function to meassure 'repetitions' times
  for (u32 rep = 0; rep < repetitions; ++rep) {

    vec_src->ops_->extractFirst(vec_src);
  }
  // stop timer
  QueryPerformanceCounter(&time_end);
  ///////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////
  // compute the elapsed time in microseconds
  elapsed_time = (time_end.QuadPart - time_start.QuadPart) * 1000000.0f / frequency.QuadPart;
  printf("Elapsed time Vector_ExtractFirst: %f ms\n", elapsed_time);
  ///////////////////////////////////////////////////////////////////////
  // compute the average time
  double average_time = elapsed_time / repetitions;
  printf("Average time Vector_ExtractFirst: %f ms\n", average_time);
  ///////////////////////////////////////////////////////////////////////

  //vec_src->ops_->print(vec_src);
}

void calculateTimeForFunction_Vector_insertLast() {
  LARGE_INTEGER frequency;				// ticks per second
  LARGE_INTEGER  time_start, time_end;    // ticks in interval
  double elapsed_time = 0.0f;
  u32 repetitions = 10000;

  ///////////////////////////////////////////////////////////////////////
  // Frequency: ticks per second
  QueryPerformanceFrequency(&frequency);
  ///////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////
  // Meassurement time
  // start timer
  QueryPerformanceCounter(&time_start);
  // execute function to meassure 'repetitions' times
  for (u32 rep = 0; rep < repetitions; ++rep) {

    vec_src->ops_->insertLast(vec_src, data[rep], 20);
  }
  // stop timer
  QueryPerformanceCounter(&time_end);
  ///////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////
  // compute the elapsed time in microseconds
  elapsed_time = (time_end.QuadPart - time_start.QuadPart) * 1000000.0f / frequency.QuadPart;
  printf("Elapsed time Vector_insertLast: %f ms\n", elapsed_time);
  ///////////////////////////////////////////////////////////////////////
  // compute the average time
  double average_time = elapsed_time / repetitions;
  printf("Average time Vector_insertLast: %f ms\n", average_time);
  ///////////////////////////////////////////////////////////////////////

  //vec_src->ops_->print(vec_src);
}


void calculateTimeForFunction_Vector_extractLast() {
  LARGE_INTEGER frequency;				// ticks per second
  LARGE_INTEGER  time_start, time_end;    // ticks in interval
  double elapsed_time = 0.0f;
  u32 repetitions = 10000;

  ///////////////////////////////////////////////////////////////////////
  // Frequency: ticks per second
  QueryPerformanceFrequency(&frequency);
  ///////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////
  // Meassurement time
  // start timer
  QueryPerformanceCounter(&time_start);
  // execute function to meassure 'repetitions' times
  for (u32 rep = 0; rep < repetitions; ++rep) {

    vec_src->ops_->extractLast(vec_src);
  }
  // stop timer
  QueryPerformanceCounter(&time_end);
  ///////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////
  // compute the elapsed time in microseconds
  elapsed_time = (time_end.QuadPart - time_start.QuadPart) * 1000000.0f / frequency.QuadPart;
  printf("Elapsed time Vector_extractLast: %f ms\n", elapsed_time);
  ///////////////////////////////////////////////////////////////////////
  // compute the average time
  double average_time = elapsed_time / repetitions;
  printf("Average time Vector_extractLast: %f ms\n", average_time);
  ///////////////////////////////////////////////////////////////////////

  //vec_src->ops_->print(vec_src);
}



void calculateTimeForFunction_Vector_insertAt() {
  LARGE_INTEGER frequency;				// ticks per second
  LARGE_INTEGER  time_start, time_end;    // ticks in interval
  double elapsed_time = 0.0f;
  u32 repetitions = 10000;

  ///////////////////////////////////////////////////////////////////////
  // Frequency: ticks per second
  QueryPerformanceFrequency(&frequency);
  ///////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////
  // Meassurement time
  // start timer
  QueryPerformanceCounter(&time_start);
  // execute function to meassure 'repetitions' times
  for (u32 rep = 0; rep < repetitions; ++rep) {

    vec_src->ops_->insertAt(vec_src, data[rep], 20, rand()% data_size);

  }
  // stop timer
  QueryPerformanceCounter(&time_end);
  ///////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////
  // compute the elapsed time in microseconds
  elapsed_time = (time_end.QuadPart - time_start.QuadPart) * 1000000.0f / frequency.QuadPart;
  printf("Elapsed time Vector_insertAt: %f ms\n", elapsed_time);
  ///////////////////////////////////////////////////////////////////////
  // compute the average time
  double average_time = elapsed_time / repetitions;
  printf("Average time Vector_insertAt: %f ms\n", average_time);
  ///////////////////////////////////////////////////////////////////////

  //vec_src->ops_->print(vec_src);
}


void calculateTimeForFunction_Vector_extractAt() {
  LARGE_INTEGER frequency;				// ticks per second
  LARGE_INTEGER  time_start, time_end;    // ticks in interval
  double elapsed_time = 0.0f;
  u32 repetitions = 10000;

  ///////////////////////////////////////////////////////////////////////
  // Frequency: ticks per second
  QueryPerformanceFrequency(&frequency);
  ///////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////
  // Meassurement time
  // start timer
  QueryPerformanceCounter(&time_start);
  // execute function to meassure 'repetitions' times
  for (u32 rep = 0; rep < repetitions; ++rep) {

    vec_src->ops_->extractAt(vec_src, rand() % vec_src->ops_->length(vec_src));
  }
  // stop timer
  QueryPerformanceCounter(&time_end);
  ///////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////
  // compute the elapsed time in microseconds
  elapsed_time = (time_end.QuadPart - time_start.QuadPart) * 1000000.0f / frequency.QuadPart;
  printf("Elapsed time Vector_extractAt: %f ms\n", elapsed_time);
  ///////////////////////////////////////////////////////////////////////
  // compute the average time
  double average_time = elapsed_time / repetitions;
  printf("Average time Vector_extractAt: %f ms\n", average_time);
  ///////////////////////////////////////////////////////////////////////

  //vec_src->ops_->print(vec_src);
}


void calculateTimeForFunction_Vector_concat() {
  LARGE_INTEGER frequency;				// ticks per second
  LARGE_INTEGER  time_start, time_end;    // ticks in interval
  double elapsed_time = 0.0f;
  u32 repetitions = 10000;

  ///////////////////////////////////////////////////////////////////////
  // Frequency: ticks per second
  QueryPerformanceFrequency(&frequency);
  ///////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////
  // Meassurement time
  // start timer
  QueryPerformanceCounter(&time_start);
  // execute function to meassure 'repetitions' times
  for (u32 rep = 0; rep < repetitions; ++rep) {

    vec_src->ops_->concat(vec_con, vec_con_aux);
  }
  // stop timer
  QueryPerformanceCounter(&time_end);
  ///////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////
  // compute the elapsed time in microseconds
  elapsed_time = (time_end.QuadPart - time_start.QuadPart) * 1000000.0f / frequency.QuadPart;
  printf("Elapsed time Vector_concat: %f ms\n", elapsed_time);
  ///////////////////////////////////////////////////////////////////////
  // compute the average time
  double average_time = elapsed_time / repetitions;
  printf("Average time Vector_concat: %f ms\n", average_time);
  ///////////////////////////////////////////////////////////////////////

  //vec_con->ops_->print(vec_con);
  //vec_con_aux->ops_->print(vec_con_aux);
}



void calculateTimeForFunction_List_Inserfrist() {
	LARGE_INTEGER frequency;				// ticks per second
	LARGE_INTEGER  time_start, time_end;    // ticks in interval
	double elapsed_time = 0.0f;
	u32 repetitions = 10000;

	///////////////////////////////////////////////////////////////////////
	// Frequency: ticks per second
	QueryPerformanceFrequency(&frequency);
	///////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////
	// Meassurement time
	// start timer
	QueryPerformanceCounter(&time_start);
	// execute function to meassure 'repetitions' times
	for (u32 rep = 0; rep < repetitions; ++rep) {

		list_src->ops_->insertFirst(list_src, data[rep], 20);
	}
	// stop timer
	QueryPerformanceCounter(&time_end);
	///////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////
	// compute the elapsed time in microseconds
	elapsed_time = (time_end.QuadPart - time_start.QuadPart) * 1000000.0f / frequency.QuadPart;
	printf("Elapsed time List_InsertFirst: %f ms\n", elapsed_time);
	///////////////////////////////////////////////////////////////////////
	// compute the average time
	double average_time = elapsed_time / repetitions;
	printf("Average time List_InsertFirst: %f ms\n", average_time);
	///////////////////////////////////////////////////////////////////////

	//list_src->ops_->print(list_src);
}



void calculateTimeForFunction_List_extractfrist() {
	LARGE_INTEGER frequency;				// ticks per second
	LARGE_INTEGER  time_start, time_end;    // ticks in interval
	double elapsed_time = 0.0f;
	u32 repetitions = 10000;

	///////////////////////////////////////////////////////////////////////
	// Frequency: ticks per second
	QueryPerformanceFrequency(&frequency);
	///////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////
	// Meassurement time
	// start timer
	QueryPerformanceCounter(&time_start);
	// execute function to meassure 'repetitions' times
	for (u32 rep = 0; rep < repetitions; ++rep) {

		list_src->ops_->extractFirst(list_src);
	}
	// stop timer
	QueryPerformanceCounter(&time_end);
	///////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////
	// compute the elapsed time in microseconds
	elapsed_time = (time_end.QuadPart - time_start.QuadPart) * 1000000.0f / frequency.QuadPart;
	printf("Elapsed time List_ExtractFirst: %f ms\n", elapsed_time);
	///////////////////////////////////////////////////////////////////////
	// compute the average time
	double average_time = elapsed_time / repetitions;
	printf("Average time List_ExtractFirst: %f ms\n", average_time);
	///////////////////////////////////////////////////////////////////////

	//list_src->ops_->print(list_src);
}



void calculateTimeForFunction_List_insertLast() {
	LARGE_INTEGER frequency;				// ticks per second
	LARGE_INTEGER  time_start, time_end;    // ticks in interval
	double elapsed_time = 0.0f;
	u32 repetitions = 10000;

	///////////////////////////////////////////////////////////////////////
	// Frequency: ticks per second
	QueryPerformanceFrequency(&frequency);
	///////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////
	// Meassurement time
	// start timer
	QueryPerformanceCounter(&time_start);
	// execute function to meassure 'repetitions' times
	for (u32 rep = 0; rep < repetitions; ++rep) {

		list_src->ops_->insertLast(list_src, data[rep], 20);
	}
	// stop timer
	QueryPerformanceCounter(&time_end);
	///////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////
	// compute the elapsed time in microseconds
	elapsed_time = (time_end.QuadPart - time_start.QuadPart) * 1000000.0f / frequency.QuadPart;
	printf("Elapsed time List_insertLast: %f ms\n", elapsed_time);
	///////////////////////////////////////////////////////////////////////
	// compute the average time
	double average_time = elapsed_time / repetitions;
	printf("Average time List_insertLast: %f ms\n", average_time);
	///////////////////////////////////////////////////////////////////////

	//list_src->ops_->print(list_src);
}



void calculateTimeForFunction_List_extractLast() {
	LARGE_INTEGER frequency;				// ticks per second
	LARGE_INTEGER  time_start, time_end;    // ticks in interval
	double elapsed_time = 0.0f;
	u32 repetitions = 10000;

	///////////////////////////////////////////////////////////////////////
	// Frequency: ticks per second
	QueryPerformanceFrequency(&frequency);
	///////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////
	// Meassurement time
	// start timer
	QueryPerformanceCounter(&time_start);
	// execute function to meassure 'repetitions' times
	for (u32 rep = 0; rep < repetitions; ++rep) {

		list_src->ops_->extractLast(list_src);
	}
	// stop timer
	QueryPerformanceCounter(&time_end);
	///////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////
	// compute the elapsed time in microseconds
	elapsed_time = (time_end.QuadPart - time_start.QuadPart) * 1000000.0f / frequency.QuadPart;
	printf("Elapsed time List_extractLast: %f ms\n", elapsed_time);
	///////////////////////////////////////////////////////////////////////
	// compute the average time
	double average_time = elapsed_time / repetitions;
	printf("Average time List_extractLast: %f ms\n", average_time);
	///////////////////////////////////////////////////////////////////////

	//list_src->ops_->print(list_src);
}



void calculateTimeForFunction_List_insertAt() {
	LARGE_INTEGER frequency;				// ticks per second
	LARGE_INTEGER  time_start, time_end;    // ticks in interval
	double elapsed_time = 0.0f;
	u32 repetitions = 10000;

	///////////////////////////////////////////////////////////////////////
	// Frequency: ticks per second
	QueryPerformanceFrequency(&frequency);
	///////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////
	// Meassurement time
	// start timer
	QueryPerformanceCounter(&time_start);
	// execute function to meassure 'repetitions' times
	for (u32 rep = 0; rep < repetitions; ++rep) {

		list_src->ops_->insertAt(list_src, data[rep], 20, rand() % data_size);

	}
	// stop timer
	QueryPerformanceCounter(&time_end);
	///////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////
	// compute the elapsed time in microseconds
	elapsed_time = (time_end.QuadPart - time_start.QuadPart) * 1000000.0f / frequency.QuadPart;
	printf("Elapsed time List_insertAt: %f ms\n", elapsed_time);
	///////////////////////////////////////////////////////////////////////
	// compute the average time
	double average_time = elapsed_time / repetitions;
	printf("Average time List_insertAt: %f ms\n", average_time);
	///////////////////////////////////////////////////////////////////////

	//list_src->ops_->print(list_src);
}

void calculateTimeForFunction_List_extractAt() {
	LARGE_INTEGER frequency;				// ticks per second
	LARGE_INTEGER  time_start, time_end;    // ticks in interval
	double elapsed_time = 0.0f;
	u32 repetitions = 10000;

	///////////////////////////////////////////////////////////////////////
	// Frequency: ticks per second
	QueryPerformanceFrequency(&frequency);
	///////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////
	// Meassurement time
	// start timer
	QueryPerformanceCounter(&time_start);
	// execute function to meassure 'repetitions' times
	for (u32 rep = 0; rep < repetitions; ++rep) {

		list_src->ops_->extractAt(list_src, 5000);
	}
	// stop timer
	QueryPerformanceCounter(&time_end);
	///////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////
	// compute the elapsed time in microseconds
	elapsed_time = (time_end.QuadPart - time_start.QuadPart) * 1000000.0f / frequency.QuadPart;
	printf("Elapsed time List_extractAt: %f ms\n", elapsed_time);
	///////////////////////////////////////////////////////////////////////
	// compute the average time
	double average_time = elapsed_time / repetitions;
	printf("Average time List_extractAt: %f ms\n", average_time);
	///////////////////////////////////////////////////////////////////////

	//list_src->ops_->print(list_src);
}

void calculateTimeForFunction_List_concat() {
	LARGE_INTEGER frequency;				// ticks per second
	LARGE_INTEGER  time_start, time_end;    // ticks in interval
	double elapsed_time = 0.0f;
	u32 repetitions = 10000;

	///////////////////////////////////////////////////////////////////////
	// Frequency: ticks per second
	QueryPerformanceFrequency(&frequency);
	///////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////
	// Meassurement time
	// start timer
	QueryPerformanceCounter(&time_start);
	// execute function to meassure 'repetitions' times
	for (u32 rep = 0; rep < repetitions; ++rep) {

		list_src->ops_->concat(list_src, list_concat);
	}
	// stop timer
	QueryPerformanceCounter(&time_end);
	///////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////
	// compute the elapsed time in microseconds
	elapsed_time = (time_end.QuadPart - time_start.QuadPart) * 1000000.0f / frequency.QuadPart;
	printf("Elapsed time List_concat: %f ms\n", elapsed_time);
	///////////////////////////////////////////////////////////////////////
	// compute the average time
	double average_time = elapsed_time / repetitions;
	printf("Average time List_concat: %f ms\n", average_time);
	///////////////////////////////////////////////////////////////////////

	//list_src->ops_->print(list_src);
	//list_concat->ops_->print(list_concat);
}



void calculateTimeForFunction_DList_Inserfrist() {
	LARGE_INTEGER frequency;				// ticks per second
	LARGE_INTEGER  time_start, time_end;    // ticks in interval
	double elapsed_time = 0.0f;
	u32 repetitions = 10000;

	///////////////////////////////////////////////////////////////////////
	// Frequency: ticks per second
	QueryPerformanceFrequency(&frequency);
	///////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////
	// Meassurement time
	// start timer
	QueryPerformanceCounter(&time_start);
	// execute function to meassure 'repetitions' times
	for (u32 rep = 0; rep < repetitions; ++rep) {

		dlist_src->ops_->insertFirst(dlist_src, data[rep], 20);
	}
	// stop timer
	QueryPerformanceCounter(&time_end);
	///////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////
	// compute the elapsed time in microseconds
	elapsed_time = (time_end.QuadPart - time_start.QuadPart) * 1000000.0f / frequency.QuadPart;
	printf("Elapsed time DList_InsertFirst: %f ms\n", elapsed_time);
	///////////////////////////////////////////////////////////////////////
	// compute the average time
	double average_time = elapsed_time / repetitions;
	printf("Average time DList_InsertFirst: %f ms\n", average_time);
	///////////////////////////////////////////////////////////////////////

	//dlist_src->ops_->print(dlist_src);
}


void calculateTimeForFunction_DList_extractfrist() {
	LARGE_INTEGER frequency;				// ticks per second
	LARGE_INTEGER  time_start, time_end;    // ticks in interval
	double elapsed_time = 0.0f;
	u32 repetitions = 10000;

	///////////////////////////////////////////////////////////////////////
	// Frequency: ticks per second
	QueryPerformanceFrequency(&frequency);
	///////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////
	// Meassurement time
	// start timer
	QueryPerformanceCounter(&time_start);
	// execute function to meassure 'repetitions' times
	for (u32 rep = 0; rep < repetitions; ++rep) {

		dlist_src->ops_->extractFirst(dlist_src);
	}
	// stop timer
	QueryPerformanceCounter(&time_end);
	///////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////
	// compute the elapsed time in microseconds
	elapsed_time = (time_end.QuadPart - time_start.QuadPart) * 1000000.0f / frequency.QuadPart;
	printf("Elapsed time DList_ExtractFirst: %f ms\n", elapsed_time);
	///////////////////////////////////////////////////////////////////////
	// compute the average time
	double average_time = elapsed_time / repetitions;
	printf("Average time DList_ExtractFirst: %f ms\n", average_time);
	///////////////////////////////////////////////////////////////////////

	//dlist_src->ops_->print(dlist_src);
}



void calculateTimeForFunction_DList_insertLast() {
	LARGE_INTEGER frequency;				// ticks per second
	LARGE_INTEGER  time_start, time_end;    // ticks in interval
	double elapsed_time = 0.0f;
	u32 repetitions = 10000;

	///////////////////////////////////////////////////////////////////////
	// Frequency: ticks per second
	QueryPerformanceFrequency(&frequency);
	///////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////
	// Meassurement time
	// start timer
	QueryPerformanceCounter(&time_start);
	// execute function to meassure 'repetitions' times
	for (u32 rep = 0; rep < repetitions; ++rep) {

		dlist_src->ops_->insertLast(dlist_src, data[rep], 20);
	}
	// stop timer
	QueryPerformanceCounter(&time_end);
	///////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////
	// compute the elapsed time in microseconds
	elapsed_time = (time_end.QuadPart - time_start.QuadPart) * 1000000.0f / frequency.QuadPart;
	printf("Elapsed time DList_insertLast: %f ms\n", elapsed_time);
	///////////////////////////////////////////////////////////////////////
	// compute the average time
	double average_time = elapsed_time / repetitions;
	printf("Average time DList_insertLast: %f ms\n", average_time);
	///////////////////////////////////////////////////////////////////////

	//dlist_src->ops_->print(dlist_src);
}



void calculateTimeForFunction_DList_extractLast() {
	LARGE_INTEGER frequency;				// ticks per second
	LARGE_INTEGER  time_start, time_end;    // ticks in interval
	double elapsed_time = 0.0f;
	u32 repetitions = 10000;

	///////////////////////////////////////////////////////////////////////
	// Frequency: ticks per second
	QueryPerformanceFrequency(&frequency);
	///////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////
	// Meassurement time
	// start timer
	QueryPerformanceCounter(&time_start);
	// execute function to meassure 'repetitions' times
	for (u32 rep = 0; rep < repetitions; ++rep) {

		dlist_src->ops_->extractLast(dlist_src);
	}
	// stop timer
	QueryPerformanceCounter(&time_end);
	///////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////
	// compute the elapsed time in microseconds
	elapsed_time = (time_end.QuadPart - time_start.QuadPart) * 1000000.0f / frequency.QuadPart;
	printf("Elapsed time DList_extractLast: %f ms\n", elapsed_time);
	///////////////////////////////////////////////////////////////////////
	// compute the average time
	double average_time = elapsed_time / repetitions;
	printf("Average time DList_extractLast: %f ms\n", average_time);
	///////////////////////////////////////////////////////////////////////

	//vec_src->ops_->print(vec_src);
}




void calculateTimeForFunction_DList_insertAt() {
	LARGE_INTEGER frequency;				// ticks per second
	LARGE_INTEGER  time_start, time_end;    // ticks in interval
	double elapsed_time = 0.0f;
	u32 repetitions = 10000;

	///////////////////////////////////////////////////////////////////////
	// Frequency: ticks per second
	QueryPerformanceFrequency(&frequency);
	///////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////
	// Meassurement time
	// start timer
	QueryPerformanceCounter(&time_start);
	// execute function to meassure 'repetitions' times
	for (u32 rep = 0; rep < repetitions; ++rep) {

		dlist_src->ops_->insertAt(dlist_src, data[rep], 20, 5000);

	}
	// stop timer
	QueryPerformanceCounter(&time_end);
	///////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////
	// compute the elapsed time in microseconds
	elapsed_time = (time_end.QuadPart - time_start.QuadPart) * 1000000.0f / frequency.QuadPart;
	printf("Elapsed time DList_insertAt: %f ms\n", elapsed_time);
	///////////////////////////////////////////////////////////////////////
	// compute the average time
	double average_time = elapsed_time / repetitions;
	printf("Average time DList_insertAt: %f ms\n", average_time);
	///////////////////////////////////////////////////////////////////////

	//dlist_src->ops_->print(dlist_src);
}



void calculateTimeForFunction_DList_extractAt() {
	LARGE_INTEGER frequency;				// ticks per second
	LARGE_INTEGER  time_start, time_end;    // ticks in interval
	double elapsed_time = 0.0f;
	u32 repetitions = 10000;

	///////////////////////////////////////////////////////////////////////
	// Frequency: ticks per second
	QueryPerformanceFrequency(&frequency);
	///////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////
	// Meassurement time
	// start timer
	QueryPerformanceCounter(&time_start);
	// execute function to meassure 'repetitions' times
	for (u32 rep = 0; rep < repetitions; ++rep) {

		dlist_src->ops_->extractAt(dlist_src, 5000);
	}
	// stop timer
	QueryPerformanceCounter(&time_end);
	///////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////
	// compute the elapsed time in microseconds
	elapsed_time = (time_end.QuadPart - time_start.QuadPart) * 1000000.0f / frequency.QuadPart;
	printf("Elapsed time DList_extractAt: %f ms\n", elapsed_time);
	///////////////////////////////////////////////////////////////////////
	// compute the average time
	double average_time = elapsed_time / repetitions;
	printf("Average time DList_extractAt: %f ms\n", average_time);
	///////////////////////////////////////////////////////////////////////

	//dlist_src->ops_->print(dlist_src);
}




void calculateTimeForFunction_DList_concat() {
	LARGE_INTEGER frequency;				// ticks per second
	LARGE_INTEGER  time_start, time_end;    // ticks in interval
	double elapsed_time = 0.0f;
	u32 repetitions = 10000;

	///////////////////////////////////////////////////////////////////////
	// Frequency: ticks per second
	QueryPerformanceFrequency(&frequency);
	///////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////
	// Meassurement time
	// start timer
	QueryPerformanceCounter(&time_start);
	// execute function to meassure 'repetitions' times
	for (u32 rep = 0; rep < repetitions; ++rep) {

		dlist_src->ops_->concat(dlist_src, dlist_concat_1);
	}
	// stop timer
	QueryPerformanceCounter(&time_end);
	///////////////////////////////////////////////////////////////////////
	//dlist_src->ops_->print(dlist_src);
	///////////////////////////////////////////////////////////////////////
	// compute the elapsed time in microseconds
	elapsed_time = (time_end.QuadPart - time_start.QuadPart) * 1000000.0f / frequency.QuadPart;
	printf("Elapsed time DList_concat: %f ms\n", elapsed_time);
	///////////////////////////////////////////////////////////////////////
	// compute the average time
	double average_time = elapsed_time / repetitions;
	printf("Average time DList_concat: %f ms\n", average_time);
	///////////////////////////////////////////////////////////////////////

	//vec_con->ops_->print(vec_con);
	//vec_con_aux->ops_->print(vec_con_aux);
}

//send help



int main(int argc, char** argv) {
	srand(time(NULL));
	TESTBASE_generateDataForComparative();
	
  calculateTimeForFunction_Vector_Inserfrist();
  calculateTimeForFunction_Vector_extractfrist();
  calculateTimeForFunction_Vector_insertLast();
  calculateTimeForFunction_Vector_extractLast();
  calculateTimeForFunction_Vector_insertAt();
  calculateTimeForFunction_Vector_extractAt();
  calculateTimeForFunction_Vector_concat();
	calculateTimeForFunction_List_Inserfrist();
	calculateTimeForFunction_List_extractfrist();
	calculateTimeForFunction_List_insertLast();
	calculateTimeForFunction_List_extractLast();
	calculateTimeForFunction_List_insertAt();
	calculateTimeForFunction_List_extractAt();
	list_src->ops_->softReset(list_src);
	calculateTimeForFunction_List_concat();

	calculateTimeForFunction_DList_Inserfrist();
  calculateTimeForFunction_DList_extractfrist();
	calculateTimeForFunction_DList_insertLast();
	calculateTimeForFunction_DList_extractLast();
	calculateTimeForFunction_DList_insertAt();
	calculateTimeForFunction_DList_extractAt();
	calculateTimeForFunction_DList_concat();

  Free_data();
	MM->status();
	printf("Press ENTER to continue\n");
	getchar();
	MM->destroy();
	return 0;
}
