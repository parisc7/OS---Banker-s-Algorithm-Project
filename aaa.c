#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define NUMBER_OF_CUSTOMERS 5
#define NUMBER_OF_RESOURCES 4

//included in ZIP file
#define INIT_FILE "resources.txt"

//prototypes of our functions
/* Available amount of each resource */
int available[NUMBER_OF_RESOURCES];
/* Maximum demand of each customer */
int maximum[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
/* Amount currently allocated to each customer */
int allocation[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
/* Remaining need of each customer */
int need[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];

/**
* Function <code>is_leq</code>, tests some kind of relation between two entities/sources a, b.
* <BR>
* @param a entitie/source in i position (pointer)
* @param b entitie/source in i position (pointer)
* @param n is the total n loops
* @return Returns <code>0</code>, (for success) otherwise something else (non-zero for errors).
*/

//Less than or equal to checker instruction
int is_leq(int *a, int *b, int n) 
{
    for(int i = 0 ; i != n; ++i) 
    {
        if(a[i] > b[i]) 
        {
            return 0;
        }
    }

    return 1;
}

/*According to Banker's Algorithm, to check the system's state, we need to iteratively pick a task that can be finished without 
exceeding the current resources and execute it. If all tasks can be finished, then the system is in a safe state, otherwise, it's unsafe*/

/**
* Function <code>is_safe</code>, checks whether is in safe state or not.
* <BR>
* @return Returns <code>0</code>, (for success) otherwise something else (non-zero for errors).
*/

int is_safe() 
{
    int work[NUMBER_OF_RESOURCES], finish[NUMBER_OF_CUSTOMERS];
    memcpy(work, available, NUMBER_OF_RESOURCES * sizeof(int));
    memset(finish, 0, NUMBER_OF_CUSTOMERS * sizeof(int));

    for(int round = 0; round != NUMBER_OF_CUSTOMERS; ++round) 
    {
        int flag = 0;
        for(int i = 0; i != NUMBER_OF_CUSTOMERS; ++i) 
        {
            if(finish[i] == 0 && is_leq(need[i], work, NUMBER_OF_RESOURCES)) 
            {
                flag = 1;
                finish[i] = 1;
                for(int j = 0; j != NUMBER_OF_RESOURCES; ++j) 
                {
                    work[j] += allocation[i][j];
                }

                break;
            }
        }

        if(!flag) 
        {
            return 0;
        }
    }

    return 1;
}

/* When a customer requests some resources, first check if it is within the available resources. If not, deny the request immediately. 
If so, then grant the request if only after the request the system is still in safe mode. 
To avoid copying the vector, I adopted the rollback-on-error technique*/

/**
* Function <code>request_resources</code>, requests resources (Banker's Algorithm).
* <BR>
* @param customer the target customer
* @param request number of resources to request
* @return Returns <code>0</code>, (for success) otherwise something else (non-zero for errors).
*/

int request_resources(int customer, int request[NUMBER_OF_RESOURCES]) 
{
    if(customer < 0 || customer >= NUMBER_OF_CUSTOMERS) 
    {
        printf("Invalid customer: %d\n", customer);
        return -1;
    }

    int err = 0;
    for(int i = 0; i != NUMBER_OF_RESOURCES; ++i) 
    {
        if(request[i] < 0 || request[i] > need[customer][i]) 
        {
            printf("Invalid number of resources to request: <customer: %d, resource: %d, need: %d, to request: %d>\n", customer, i, need[customer][i], request[i]);
            err = -1;
        }

        if(request[i] > available[i]) 
        {
            printf("No enough resources to allocate: <customer: %d, resource: %d, available: %d, to request: %d>\n", customer, i, available[i], request[i]);
            err = -2;
        }

        if(err != 0) 
        {  
            //Rollback
            while(i--) 
            {
                available[i] += request[i];
                allocation[customer][i] -= request[i];
                need[customer][i] += request[i];
            }

            return err;
        }

        //Allocating resources
        available[i] -= request[i];
        allocation[customer][i] += request[i];
        need[customer][i] -= request[i];
    }
    
    //checks safe state
    if(!is_safe()) 
    {
        // rollback
        printf("Unsafe state after request!\n");

        for(int i = 0; i != NUMBER_OF_RESOURCES; ++i) 
        {
            available[i] += request[i];
            allocation[customer][i] -= request[i];
            need[customer][i] += request[i];
        }

        return -3;
    }
    
    return 0;
}

/**
* Function <code>release_resources</code>, releases resources (Banker's Algorithm).
* <BR>
* @param customer the target customer
* @param release number of resources to release
* @return Returns <code>0</code>, (for success) otherwise something else (non-zero for errors).
*/

int release_resources(int customer, int release[NUMBER_OF_RESOURCES]) 
{
    if(customer < 0 || customer >= NUMBER_OF_CUSTOMERS) 
    {
        printf("Invalid customer: %d\n", customer);
        return -1;
    }

    for(int i = 0; i != NUMBER_OF_RESOURCES; ++i) 
    {
        if(release[i] < 0 || release[i] > allocation[customer][i]) 
        {
            printf("Invalid number of resources to release: <customer: %d, resource: %d, allocated: %d, to release: %d>\n", customer, i, allocation[customer][i], release[i]);

            //Rollback
            while(i--) 
            {
                allocation[customer][i - 1] += release[i - 1];
                available[i] -= release[i];
            }

            return -1;
        }
        
        //Releasing resources
        allocation[customer][i] -= release[i];
        available[i] += release[i];
    }
    return 0;
}

/**
* Function <code>request_wrapper</code>, requests the wrapper.
* <BR>
*/

void request_wrapper() 
{
    int request[NUMBER_OF_RESOURCES], customer;
    scanf("%d", &customer);

    for(int i = 0; i != NUMBER_OF_RESOURCES; ++i) 
    {
        scanf("%d", &request[i]);
    }

    if(request_resources(customer, request) != 0) 
    {
        printf("DENIED.\n");
    } 
    
    else 

    {
        printf("SUCCESS.\n");
    }
}

/**
* Function <code>release_wrapper</code>, displays / prints (releases) the wrapper.
* <BR>
*/

void release_wrapper() 
{
    int release[NUMBER_OF_RESOURCES], customer;
    scanf("%d", &customer);

    for(int i = 0; i != NUMBER_OF_RESOURCES; ++i) 
    {
        scanf("%d", &release[i]);
    }

    if(release_resources(customer, release) != 0) 
    {
        printf("DENIED.\n");
    } 
    
    else 
    
    {
        printf("SUCCESS.\n");
    }
}

/**
* Function <code>display_usage</code>, displays / prints the usage.
* <BR>
*/

void display_usage() 
{
    printf("=============================================================\n");
    printf("<this program> <available resources of size %d>\n", NUMBER_OF_RESOURCES);
    printf("Operations:\n");
    printf("    Request resources: RQ <customer> <resources>\n");
    printf("    Release resources: RL <customer> <resources>\n");
    printf("    Display resources: *\n");
    printf("=============================================================\n");
}

/**
* Function <code>display_resources</code>, displays / prints the resources.
* <BR>
*/

void display_resources() 
{
    printf("=============================================================\n");
    printf("Available resources:\n");

    for(int i = 0; i != NUMBER_OF_RESOURCES; ++i) 
    {
        printf("%d ", available[i]);
    }

    printf("\n");
    printf("-------------------------------------------------------------\n");
    printf("Maximum resources for each customer:\n");

    for(int customer = 0; customer != NUMBER_OF_CUSTOMERS; ++customer) 
    {
        printf("%d: ", customer);

        for(int r = 0; r != NUMBER_OF_RESOURCES; ++r) 
        {
            printf("%d ", maximum[customer][r]);
        }
        printf("\n");
    }

    printf("-------------------------------------------------------------\n");
    printf("Allocated resources for each customer:\n");
    for(int customer = 0; customer != NUMBER_OF_CUSTOMERS; ++customer) 
    {
        printf("%d: ", customer);

        for(int r = 0; r != NUMBER_OF_RESOURCES; ++r) 
        {
            printf("%d ", allocation[customer][r]);
        }
        printf("\n");
    }

    printf("-------------------------------------------------------------\n");
    printf("Needed resources for each customer:\n");
    for(int customer = 0; customer != NUMBER_OF_CUSTOMERS; ++customer) 
    {
        printf("%d: ", customer);

        for(int r = 0; r != NUMBER_OF_RESOURCES; ++r) 
        {
            printf("%d ", need[customer][r]);
        }

        printf("\n");
    }
    printf("=============================================================\n");
}

/**
* Function <code>init</code>, is our initialisation driver.
* <BR>
* @param argc stands for argument count, containing the number of arguments passed to the program
* @param argv stands for argument vector. A vector is a one-dimensional array, and argv is a one-dimensional array of strings (pointer)
* @param resources_file is the resource txt file containing the values (pointer)
* @return Returns <code>0</code>, otherwise something else.
*/

int init(int argc, char *argv[], const char *resources_file) 
{
    if(argc != 1 + NUMBER_OF_RESOURCES) 
    {
        printf("Incorrect number of parameters.\n");
        return -1;
    }

    for(int i = 0; i != NUMBER_OF_RESOURCES; ++i) 
    {
        available[i] = atoi(argv[i + 1]);
    }

    FILE *f = fopen(resources_file, "r");

    if(f == NULL) 
    {
        printf("Unable to open file: %s\n", resources_file);
        return -2;
    }

    for(int c = 0; c != NUMBER_OF_CUSTOMERS; ++c) 
    {
        for(int r = 0; r != NUMBER_OF_RESOURCES; ++r) 
        {
            fscanf(f, "%d", &maximum[c][r]);
            need[c][r] = maximum[c][r];
        }
    }

    fclose(f);
    printf("Initialized, ready to run.\n");
    return 0;
}

/**
* Function <code>main</code>, is our main driver.
* <BR>
* @param argc stands for argument count, containing the number of arguments passed to the program
* @param argv stands for argument vector. A vector is a one-dimensional array, and argv is a one-dimensional array of strings (pointer)
* @return Returns <code>0</code>, otherwise something else.
*/

int main(int argc, char *argv[]) 
{
    if(init(argc, argv, INIT_FILE) != 0) 
    {
        display_usage();
        return 0;
    }

    char op[5];
    printf("> ");

    while(scanf("%s", op) == 1) 
    {
        if(strcmp(op, "RQ") == 0) 
        {
            request_wrapper();
        } 
        
        else if(strcmp(op, "RL") == 0) 
        {
            release_wrapper();
        } 
        
        else if(strcmp(op, "*") == 0) 
        {
            display_resources();
        } 
        
        else 
        
        {
            display_usage();
        }
        printf("> ");

    }

    return 0;
}