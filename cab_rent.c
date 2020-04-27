#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
// DRIVER STATUS
typedef enum{frie,booked} status;

// CAB TYPE
typedef enum{rickshaw,micro,sedan,electric} cab_type;

// LOCATION
struct location
{
    float x;
    float y;
};

// CAB_DRIVER LIST (CIRCULAR LL)
typedef struct Cab_Driver_tag
{
    int d_ID;
    char d_name[10];
    struct Cab_Driver_tag *next;
}   Driver_Node;

// PASSENGER LIST (SINGLY LL)
typedef struct Passenger_tag
{
    int p_ID;
    char p_name[10];
    char p_mobile[11];
    int p_frequency;
    struct location pickup;
    struct location drop;
    struct Passenger_tag *next;
}   Customer_Node;

// BOOKING HISTORY LIST (SINGLY LL)
typedef struct Booking_History_tag
{
    int d_id;
    int p_id;
    char p_mob[11];
    int frequency;
    float distance;
    struct Booking_History_tag *next;
}   B_Hist_Node;

// CURRENT STATUS LIST (CIRCULAR LL)
typedef struct Current_Status_tag
{
    int dr_id;
    struct location d_position;
    status sc;
    cab_type type;
    float earning;
    struct Current_Status_tag *next;
}   Driver_Status_Node;

// ADDING NEW DRIVER
void add_driver(Driver_Node **d_last, Driver_Status_Node **d_node)
{
    int x;
    char c[10];

    // INSERTION IN DRIVE_TAG LIST
    Driver_Node *nptr,*ptr;
    ptr=*d_last;
    nptr=(Driver_Node*)malloc(sizeof(Driver_Node));
    if(ptr!=NULL)
    {
        nptr->d_ID=ptr->d_ID + 1;
    }
    else
    {
        nptr->d_ID=0;
    }
    
    printf("Enter driver name:\n");
    scanf("%s",nptr->d_name);
    if(*d_last==NULL)
    {
        *d_last=nptr;
        (*d_last)->next=*d_last;
    }
    else
    {
        nptr->next=ptr->next;
        ptr->next=nptr;
        (*d_last)=nptr;
    }
    
    // INSERTION IN DRIVER_STATUS_TAG LIST
    Driver_Status_Node *new,*last;
    last=*d_node;
    new=(Driver_Status_Node*)malloc(sizeof(Driver_Status_Node));
    new->dr_id=nptr->d_ID;
    printf("Enter x and y co-ordinates of driver:\n");
    scanf("%f %f",&new->d_position.x,&new->d_position.y);
    new->sc=frie;
    printf("Enter cab_type:\n0-auto\t1-micro\t2-sedan\t3-electric\n");
    scanf("%d",&new->type);
    new->earning=0.0;
    if(*d_node==NULL)
    {
        *d_node=new;
        (*d_node)->next=*d_node;
    }
    else
    {
        new->next=last->next;
        last->next=new;
        (*d_node)=new;
    }

    printf("DRIVER ADDED SUCCESSFULLY!\n");
    
}

// ADD PASSENGER
void add_passenger(Customer_Node **c_node, char *c)
{
    Customer_Node *ptr,*nptr;
    ptr=*c_node;
    nptr=(Customer_Node*)malloc(sizeof(Customer_Node));
    nptr->drop.x=nptr->drop.y=0.0;
    nptr->p_frequency=0;
    nptr->pickup.x=nptr->pickup.y=0.0;
    if(ptr==NULL)
    {
        nptr->p_ID=0;
    }
    else
    {
        nptr->p_ID=ptr->p_ID+1;
    }
    strcpy(nptr->p_name,c);
    printf("NEW USER!\nEnter Mobile No.:\n");
    scanf("%s",nptr->p_mobile);
    nptr->next=ptr;
    *c_node=nptr;
    
    printf("NEW USER ADDED SUCCESSFULLY!\n");
}

// UPDATE BOOKING HISTORY
void Update_booking(Driver_Status_Node *d_ptr, Customer_Node *c_ptr, B_Hist_Node **h_node)
{
    B_Hist_Node *ptr;
    ptr=*h_node;
    while((ptr!=NULL)&&(ptr->d_id!=d_ptr->dr_id)&&(ptr->p_id!=c_ptr->p_ID))
    {
        ptr=ptr->next;
    }
    if(ptr!=NULL)
    {
        ptr->frequency++;
        ptr->distance+=sqrt(pow(c_ptr->pickup.x - c_ptr->drop.x , 2) + pow(c_ptr->pickup.y - c_ptr->drop.y , 2));
    }
    else
    {
        B_Hist_Node *nptr;
        nptr=(B_Hist_Node*)malloc(sizeof(B_Hist_Node));
        nptr->d_id=d_ptr->dr_id;
        nptr->p_id=c_ptr->p_ID;
        nptr->distance=sqrt(pow(c_ptr->pickup.x - c_ptr->drop.x , 2) + pow(c_ptr->pickup.y - c_ptr->drop.y , 2));
        nptr->frequency=1;
        strcpy(c_ptr->p_mobile,nptr->p_mob);
        // INSERTION AT START
        nptr->next=*h_node;
        *h_node=nptr;
        
    }
}

// UPDATE DRIVER LIST
Driver_Status_Node *Update_driver(Driver_Status_Node **d_node, float p_x, float p_y, int id, float d_x, float d_y)
{
    Driver_Status_Node *ptr;
    ptr=*d_node;
    if(ptr->dr_id!=id)
    {
        do
        {
            ptr=ptr->next;
        } while ((ptr!=*d_node)&&(ptr->dr_id!=id));

        ptr->d_position.x=d_x;
        ptr->d_position.y=d_y;
        ptr->earning+=sqrt(pow(p_x - d_x , 2) + pow(p_y - d_y , 2));
        
        
    }
    else
    {
        ptr->d_position.x=d_x;
        ptr->d_position.y=d_y;
        ptr->earning+=sqrt(pow(p_x - d_x , 2) + pow(p_y - d_y , 2));
    }
    return ptr;
}

// CALCULATE NEAREST CABS
int Calculate_nearest(Driver_Status_Node **d_node, float p_x, float p_y)
{
    float distance;
    int flag=0;
    Driver_Status_Node *ptr=*d_node;
    printf("DRIVER ID\tCAB TYPE\tDISTANCE\n");
    do
    {
        distance=sqrt(pow(p_x - ptr->d_position.x , 2)+pow(p_y - ptr->d_position.y , 2));
        if((ptr->sc==frie)&&(distance<=5.0))
        {
            printf("%d\t\t%d\t\t%f\n",ptr->dr_id,ptr->type,distance);
            flag=1;
        }
        ptr=ptr->next;
    }
    while(ptr!=*d_node);

    return flag;
}

// BOOKING A CAB
void book(Driver_Node **d_last, Driver_Status_Node **d_node, Customer_Node **c_node, B_Hist_Node **h_node)
{
    char c[10];
    Customer_Node *c_ptr;
    
    c_ptr=*c_node;
    printf("Enter passenger name:\n");
    scanf("%s",c);
    while((c_ptr!=NULL)&&(strcmp(c,c_ptr->p_name)!=0))
    {
        c_ptr=c_ptr->next;
    }

    // NEW CUSTOMER
    if(c_ptr==NULL)
    {
        add_passenger(c_node,c);
        c_ptr=*c_node;
    }

    // OLD CUSTOMER

    
    printf("Enter x and y coordinates of your location:\n");
    scanf("%f %f",&c_ptr->pickup.x,&c_ptr->pickup.y);
    printf("Enter your drop location:\n");
    scanf("%f %f",&c_ptr->drop.x,&c_ptr->drop.y);

    // FINDING NEAREST CABS
    int found=0;
    found=Calculate_nearest(d_node, c_ptr->pickup.x, c_ptr->pickup.y);
    if(found==0)
    {
        printf("NO CABS AVAILABLE NEARBY!\n");
        return 0;
    }
    // SELECTING ONE OF THE CABS
    int id;
    printf("Enter the driver ID to select cab:\n");
    scanf("%d",&id);

    // UPDATING PASSENGER FREQUENCY
    c_ptr->p_frequency++;
    
    // UPDATING THE DRIVER LIST
    Driver_Status_Node *d_ptr;
    d_ptr = Update_driver(d_node, c_ptr->pickup.x, c_ptr->pickup.y, id, c_ptr->drop.x, c_ptr->drop.y);

    // UPDATING BOOKING HISTORY LIST
    Update_booking(d_ptr, c_ptr, h_node);

    printf("CAB BOOKED SUCCESSFULLY!\nHAPPY RIDE!\n");
    float d;
    d=sqrt(pow(c_ptr->pickup.x - c_ptr->drop.x , 2) + (pow(c_ptr->pickup.y - c_ptr->drop.y , 2)));
    printf("YOUR TOTAL FARE IS : %f\n",d);
}

// DISPLAYING MOST FREQUENT DRIVER-PASSENGER PAIR
void frequent(B_Hist_Node **h_node, Customer_Node *c_node, Driver_Node *d_last)
{
    
    B_Hist_Node *ptr= *h_node;
    Customer_Node *c_ptr= c_node;
    Driver_Node *d_ptr= d_last;
    int max=ptr->frequency;
    ptr=ptr->next;
    while(ptr!=NULL)
    {
        if(ptr->frequency > max)
        {
            max=ptr->frequency;
        }
        ptr=ptr->next;
    }
    ptr=*h_node;
    printf("The Highest Frequency is %d.\nDRIVER\tPASSENGER\n",max);
    while(ptr!=NULL)
    {
        if(ptr->frequency==max)
        {
            d_ptr=d_last;
            while(d_ptr->d_ID!=ptr->d_id)
            {
                d_ptr=d_ptr->next;
            }
            printf("%s\t",d_ptr->d_name);

            c_ptr=c_node;
            while(c_ptr->p_ID!=ptr->p_id)
            {
                c_ptr=c_ptr->next;
            }
            printf("%s\n",c_ptr->p_name);
        }
        ptr=ptr->next;
    }
}


void FrontBackSplit_2(Driver_Status_Node* source, Driver_Status_Node** frontRef, Driver_Status_Node** backRef) 
{ 
    Driver_Status_Node* fast; 
    Driver_Status_Node* slow; 
    slow = source; 
    fast = source->next; 
  
    /* Advance 'fast' two nodes, and advance 'slow' one node */
    while (fast != NULL) { 
        fast = fast->next; 
        if (fast != NULL) { 
            slow = slow->next; 
            fast = fast->next; 
        } 
    } 
  
    /* 'slow' is before the midpoint in the list, so split it in two 
    at that point. */
    *frontRef = source; 
    *backRef = slow->next; 
    slow->next = NULL; 
} 

Driver_Status_Node* SortedMerge_2(Driver_Status_Node* a, Driver_Status_Node* b) 
{ 
    Driver_Status_Node* result = NULL; 
  
    /* Base cases */
    if (a == NULL) 
        return (b); 
    else if (b == NULL) 
        return (a); 
  
    /* Pick either a or b, and recur */
    if (a->earning >= b->earning) { 
        result = a; 
        result->next = SortedMerge_2(a->next, b); 
    } 
    else { 
        result = b; 
        result->next = SortedMerge_2(a, b->next); 
    } 
    return (result); 
} 

void MergeSort_2(Driver_Status_Node** d_node) 
{ 
    Driver_Status_Node* head = *d_node; 
    Driver_Status_Node* a; 
    Driver_Status_Node* b; 
  
    /* Base case -- length 0 or 1 */
    if ((head == NULL) || (head->next == NULL)) { 
        return; 
    } 
  
    /* Split head into 'a' and 'b' sublists */
    FrontBackSplit_2(head, &a, &b); 
  
    /* Recursively sort the sublists */
    MergeSort_2(&a); 
    MergeSort_2(&b); 
  
    /* answer = merge the two sorted lists together */
    *d_node = SortedMerge_2(a, b); 
} 

// SUCCESSFUL DRIVERS
void successful_drivers(Driver_Status_Node **d_node, Driver_Node *d_last)
{
    Driver_Status_Node *ptr;
    Driver_Node *d_ptr;

    // CIRCULAR LL -> SINGLY LL
    ptr=*d_node;
    *d_node=(*d_node)->next;
    ptr->next=NULL;
    ptr=*d_node;
    MergeSort_2(d_node);
    ptr=*d_node;

    printf("The 3 most successful drivers are:\nID\tD_NAME\tEARNING\n");

    int i=0;
    for(i=0;i<3;i++)
    {
        d_ptr=d_last;
        while(d_ptr->d_ID!=ptr->dr_id)
        {
            d_ptr=d_ptr->next;
        }
        printf("%d\t%s\t%f\n",d_ptr->d_ID,d_ptr->d_name,ptr->earning);
        ptr=ptr->next;
    }

    // SINGLY LL -> CIRCULAR LL
    ptr=*d_node;
    while(ptr->next!=NULL)
    {
        ptr=ptr->next;
    }
    ptr->next=*d_node;
    *d_node=ptr;

}

// MAIN FUNCTION
void main()
{
    Driver_Node *d_last=NULL,*dptr;
    Customer_Node *c_node=NULL,*cptr;
    B_Hist_Node *h_node=NULL,*hptr;
    Driver_Status_Node *d_node=NULL,*dsptr;
   
    FILE *fp;
    fp=fopen("abc.txt","r");
    char *str;
    int i=0;
    for(i=0;i<5;i++)
    {
        dptr=(Driver_Node*)malloc(sizeof(Driver_Node));
        dsptr=(Driver_Status_Node*)malloc(sizeof(Driver_Status_Node));
        fscanf(fp,"%d %s %d %f %f %d %d %f",&dptr->d_ID,dptr->d_name,&dsptr->dr_id,&dsptr->d_position.x,&dsptr->d_position.y,&dsptr->sc,&dsptr->type,&dsptr->earning);
        // INITIALISING DRIVER NODE LIST
        if(d_last==NULL)
        {
            d_last=dptr;
            d_last->next=d_last;
        }
        else
        {
            dptr->next=d_last->next;
            d_last->next=dptr;
            d_last=dptr;
        }
        // INITIALISING DRIVER STATUS NODE LIST
        if(d_node==NULL)
        {
            d_node=dsptr;
            d_node->next=d_node;
        }
        else
        {
            dsptr->next=d_node->next;
            d_node->next=dsptr;
            d_node=dsptr;
        }

    }

    for(i=0;i<5;i++)
    {
        cptr=(Customer_Node*)malloc(sizeof(Customer_Node));
        fscanf(fp,"%d %s %s %d %f %f %f %f",&cptr->p_ID,cptr->p_name,cptr->p_mobile,&cptr->p_frequency,&cptr->pickup.x,&cptr->pickup.y,&cptr->drop.x,&cptr->drop.y);
        // INITIALISING CUSTOMER NODE LIST
        cptr->next=c_node;
        c_node=cptr;
    }

    for(i=0;i<5;i++)
    {
        hptr=(B_Hist_Node*)malloc(sizeof(B_Hist_Node));
        fscanf(fp,"%d %d %s %d %f",&hptr->d_id,&hptr->p_id,hptr->p_mob,&hptr->frequency,&hptr->distance);
        // INITIALISING BOOKING HISTORY LIST
        hptr->next=h_node;
        h_node=hptr;
    }
    fclose(fp);

    int exit=0,input=0;

    while(exit==0)
    {
        printf("PRESS\n1.Add New Driver\n2.Book a Cab\n3.Display most frequent pair\n4.Display most successful drivers\n5.Exit\n");
        scanf("%d",&input);
        switch (input)
        {
        case 1: add_driver(&d_last,&d_node);
                break;
        
        case 2: book(&d_last,&d_node,&c_node,&h_node);
                break;

        case 3: frequent(&h_node, c_node, d_last);
                break;

        case 4: successful_drivers(&d_node, d_last);
                break;

        case 5: exit=1;
                break;

        default: printf("INVALID INPUT\n");
            
        }
    }

}