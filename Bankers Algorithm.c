/** \file      Bankers Algorithm.c
*   \brief     This is a program that implements the Banker's Algorithm
*   \details   
*   \authors   George Hadjimichel, Paraskevas Costa
*   \version   1.00
*   \date      05/2022
*   \bug       
*   \copyright (University of Nicosia)
*/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define NUMBER_OF_CUSTOMERS 5
#define NUMBER_OF_RESOURCES 4
/* the available amount of each resource */
int available[NUMBER_OF_RESOURCES];
/*the maximum demand of each customer */
int maximum[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
/* the amount currently allocated to each customer */
int allocation[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
/* the remaining need of each customer */
int need[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];