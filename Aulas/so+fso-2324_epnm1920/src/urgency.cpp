/**
 * @file
 *
 * \brief A hospital pediatric urgency with a Manchester triage system.
 */

#include  <stdio.h>
#include  <stdlib.h>
#include  <string.h>
#include  <libgen.h>
#include  <unistd.h>
#include  <sys/wait.h>
#include  <sys/types.h>
#include  <thread.h>
#include  <math.h>
#include  <stdint.h>
#include  <signal.h>
#include  <utils.h>
#include <iostream>
#include  "settings.h"
#include  "pfifo.h"

/* DO NOT CHANGE THE FOLLOWING VALUES, run program with option -h to set a different values */

static int npatients = 4;  ///< number of patients
static int nnurses = 1;    ///< number of triage nurses
static int ndoctors = 1;   ///< number of doctors

#define USAGE "Synopsis: %s [options]\n" \
   "\t----------+-------------------------------------------------------------\n" \
   "\t  Option  |          Description                                        \n" \
   "\t----------+-------------------------------------------------------------\n" \
   "\t -p num   | number of patients (dfl: %d, min: %d, max: %d)              \n" \
   "\t -n num   | number of nurses (dfl: %d, min: %d, max: %d)                \n" \
   "\t -d num   | number of doctors (dfl: %d, min: %d, max: %d)               \n" \
   "\t -h       | this help                                                   \n" \
   "\t----------+-------------------------------------------------------------\n", \
   basename(argv[0]), npatients, 1, MAX_PATIENTS, nnurses, 1, MAX_NURSES, ndoctors, 1, MAX_DOCTORS


// TODO point: changes may be required in these date structures

/**
 * \brief Patient data structure
 */
typedef struct
{
   char name[MAX_NAME+1];
   int done; // 0: waiting for consultation; 1: consultation finished
   // TODO point: if necessary, add new fields here
} Patient;

typedef struct
{
   int num_patients;
   int num_nurses;
   int num_doctors;
   Patient all_patients[MAX_PATIENTS];
   PriorityFIFO triage_queue;
   PriorityFIFO doctor_queue;
   // TODO point: if necessary, add new fields here
} HospitalData;

HospitalData * hd = NULL;

// TODO point: if necessary, add module variables here
static pthread_t* patient_thrs;
static pthread_t* nurses_thrs;
static pthread_t* doctor_thrs;
static pthread_mutex_t* patient_mutex;
static pthread_cond_t* patient_cond;

/**
 *  \brief verification tests:
 */
#define check_valid_patient(id) do { check_valid_patient_id(id); check_valid_name(hd->all_patients[id].name); } while(0)
#define check_valid_nurse(id) do { check_valid_nurse_id(id); } while(0)
#define check_valid_doctor(id) do { check_valid_doctor_id(id); } while(0)

int random_manchester_triage_priority();
void new_patient(Patient* patient); // initializes a new patient
void random_wait();


/* ************************************************* */

// TODO point: changes may be required to this function
void init_simulation(uint32_t np,uint32_t nn,uint32_t nd)
{
   printf("Initializing simulation\n");
   hd = (HospitalData*)mem_alloc(sizeof(HospitalData)); // mem_alloc is a malloc with NULL pointer verification
   memset(hd, 0, sizeof(HospitalData));
   hd->num_patients = np;
   hd->num_nurses=nn;
   hd->num_doctors=nd;

   patient_thrs = new pthread_t[np];
   nurses_thrs = new pthread_t[nn];
   doctor_thrs = new pthread_t[nd];
   patient_mutex = new pthread_mutex_t[np];
   patient_cond = new pthread_cond_t[np];

   // Initialize patient mutex and condition variable
   for (uint32_t i = 0; i < np; i++) {
      mutex_init(&patient_mutex[i], NULL);
      cond_init(&patient_cond[i], NULL);
   }


   init_pfifo(&hd->triage_queue);
   init_pfifo(&hd->doctor_queue);

}

/* ************************************************* */

// TODO point: changes may be required to this function
void term_simulation(int np) {
   // DO NOT WAIT THE TERMINATION OF ACTIVE ENTITIES IN THIS FUNCTION!
   // This function is just to release the allocated resources

   printf("Releasing resources\n");
   term_pfifo(&hd->doctor_queue);
   term_pfifo(&hd->triage_queue);
   free(hd);
   hd = NULL;


   for (uint32_t i = 0; i < npatients; i++) {
      mutex_destroy(&patient_mutex[i]);
      cond_destroy(&patient_cond[i]);
   }
   //cond_destroy(&patient_mutex->patient);
   //mutex_destroy(&patient_cond);
}

/* ************************************************* */

// TODO point: changes may be required to this function
void* nurse_iteration(void* arg) // return value can be used to request termination
{
   while(1){
      printf("\e[34;01mNurse %d: get next patient\e[0m\n", arg);
      int patient = retrieve_pfifo(&hd->triage_queue);
      check_valid_patient(patient);
      // TODO point: PUT YOUR NURSE TERMINATION CODE HERE:
      printf("\e[34;01mNurse %d: evaluate patient %d priority\e[0m\n", arg, patient);
      int priority = random_manchester_triage_priority();
      printf("\e[34;01mNurse %d: add patient %d with priority %d to doctor queue\e[0m\n", arg, patient, priority);
      insert_pfifo(&hd->doctor_queue, patient, priority);
   }

   return 0;
}



/* ************************************************* */

// TODO point: changes may be required to this function
void* doctor_iteration(void* arg) // return value can be used to request termination
{
   while(1){
      printf("\e[32;01mDoctor %d: get next patient\e[0m\n", arg);
      int patient = retrieve_pfifo(&hd->doctor_queue);
      check_valid_patient(patient);
      // TODO point: PUT YOUR DOCTOR TERMINATION CODE HERE:
      printf("\e[32;01mDoctor %d: treat patient %d\e[0m\n", arg, patient);
      random_wait();
      printf("\e[32;01mDoctor %d: patient %d treated\e[0m\n", arg, patient);
      // TODO point: PUT YOUR PATIENT CONSULTATION FINISHED NOTIFICATION CODE HERE:
      mutex_lock(&patient_mutex[patient]);
      hd->all_patients[patient].done = 1;
      cond_broadcast(&patient_cond[patient]);
      mutex_unlock(&patient_mutex[patient]);
   }
   return 0;
}


/* ************************************************* */

void patient_goto_urgency(int id)
{
   new_patient(&hd->all_patients[id]);
   check_valid_name(hd->all_patients[id].name);
   printf("\e[30;01mPatient %s (number %d): get to hospital\e[0m\n", hd->all_patients[id].name, id);
   insert_pfifo(&hd->triage_queue, id, 1); // all elements in triage queue with the same priority!
}

// TODO point: changes may be required to this function
void patient_wait_end_of_consultation(int id)
{
  mutex_lock(&patient_mutex[id]);           // Exclusive access for the patient

   while(hd->all_patients[id].done == 0) {
      cond_wait(&patient_cond[id], &patient_mutex[id]);
   }

   check_valid_name(hd->all_patients[id].name);
   printf("\e[30;01mPatient %s (number %u): health problems treated\e[0m\n", hd->all_patients[id].name, id);

   mutex_unlock(&patient_mutex[id]);         // Release exclusive access for the patient
}


// TODO point: changes are required to this function
void* patient_life(void* arg)
{
   int id=*(int *)arg;
   free(arg);// libertar pacientes

   patient_goto_urgency(id);
   //nurse_iteration(0);  // TODO point: to be commented/deleted in concurrent version
   //doctor_iteration(0); // TODO point: to be commented/deleted in concurrent version
   patient_wait_end_of_consultation(id);
   memset(&(hd->all_patients[id]), 0, sizeof(Patient)); // patient finished

   return NULL;
}

/* ************************************************* */

// TODO point: if necessary, add extra functions here:

/* ************************************************* */

int main(int argc, char *argv[])
{
   /* command line processing */
   int option;
   while ((option = getopt(argc, argv, "p:n:d:h")) != -1)
   {
      switch (option)
      {
         case 'p':
            npatients = atoi(optarg);
            if (npatients < 1 || npatients > MAX_PATIENTS)
            {
               fprintf(stderr, "Invalid number of patients!\n");
               return EXIT_FAILURE;
            }
            break;
         case 'n':
            nnurses = atoi(optarg);
            if (nnurses < 1 || nnurses > MAX_NURSES)
            {
               fprintf(stderr, "Invalid number of nurses!\n");
               return EXIT_FAILURE;
            }
            break;
         case 'd':
            ndoctors = atoi(optarg);
            if (ndoctors < 1 || nnurses > MAX_DOCTORS)
            {
               fprintf(stderr, "Invalid number of doctors!\n");
               return EXIT_FAILURE;
            }
            break;
         case 'h':
            printf(USAGE);
            return EXIT_SUCCESS;
         default:
            fprintf(stderr, "Non valid option!\n");
            fprintf(stderr, USAGE);
            return EXIT_FAILURE;
      }
   }

   /* start random generator */
   srand(getpid());

   /* init simulation */
   init_simulation(npatients,nnurses,ndoctors);


 
   // TODO point: REPLACE THE FOLLOWING DUMMY CODE WITH code to launch
   // active entities and code to properly terminate the simulation.
   /* dummy code to show a very simple sequential behavior */
    // Create nurses thread
   for (uint32_t i = 0; i < nnurses; i++) {
      pthread_create(&nurses_thrs[i], NULL, nurse_iteration, NULL);
   }

   // Create doctors thread
   for (uint32_t i = 0; i < nnurses; i++) {
      pthread_create(&doctor_thrs[i], NULL, doctor_iteration, NULL);
   }

   // Create patients thread
   for (uint32_t i = 0; i < npatients; i++) {
      int *id = (int *)malloc(sizeof(int));
      *id = i;
      pthread_create(&patient_thrs[i], NULL, patient_life, id);
   }

   // Create patients thread
   for (uint32_t i = 0; i < npatients; i++) {
      pthread_join(patient_thrs[i], NULL);
   }

   // Gracefully end simulation
   for (uint32_t i = 0; i < nnurses; i++) {
      pthread_detach(nurses_thrs[i]);
   }
   for (uint32_t i = 0; i < ndoctors; i++) {
      pthread_detach(doctor_thrs[i]);
   }
   
   free(hd);

   return EXIT_SUCCESS;
}

/* IGNORE THE FOLLOWING CODE */

int random_manchester_triage_priority()
{
   int result;
   int perc = (int)(100*(double)rand()/((double)RAND_MAX)); // in [0;100]
   if (perc < 10)
      result = RED;
   else if (perc < 30)
      result = ORANGE;
   else if (perc < 50)
      result = YELLOW;
   else if (perc < 75)
      result = GREEN;
   else
      result = BLUE;
   return result;
}

static char **names = (char *[]) {"Ana", "Miguel", "Luis", "Joao", "Artur", "Maria", "Luisa", "Mario", "Augusto", "Antonio", "Jose", "Alice", "Almerindo", "Gustavo", "Paulo", "Paula", NULL};

char* random_name()
{
   static int names_len = 0;
   if (names_len == 0)
   {
      for(names_len = 0; names[names_len] != NULL; names_len++)
         ;
   }

   return names[(int)(names_len*(double)rand()/((double)RAND_MAX+1))];
}

void new_patient(Patient* patient)
{
   strcpy(patient->name, random_name());
   patient->done = 0;
}

void random_wait()
{
   usleep((useconds_t)(MAX_WAIT*(double)rand()/(double)RAND_MAX));
}

