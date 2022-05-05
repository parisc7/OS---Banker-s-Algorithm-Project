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


#define NUMBER OF CUSTOMERS 5
#define NUMBER OF RESOURCES 4
/* the available amount of each resource */
int available[NUMBER OF RESOURCES];
/*the maximum demand of each customer */
int maximum[NUMBER OF CUSTOMERS][NUMBER OF RESOURCES];
/* the amount currently allocated to each customer */
int allocation[NUMBER OF CUSTOMERS][NUMBER OF RESOURCES];
/* the remaining need of each customer */
int need[NUMBER OF CUSTOMERS][NUMBER OF RESOURCES];