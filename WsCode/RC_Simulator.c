/*******************************************************************************
* This file implements the target IFI code when building for the simulator.
* This file also implements the socket code that receives OI data, sends
* Output data and receives Input data
*******************************************************************************/
#include "RC_Simulator.h"
#include "ws_includes.h"
#include "user_routines.h"
#include "ifi_aliases.h"
#include "ifi_default.h"
#include "ws_io.h"
#include "ws_autonomous.h"
#include "ws_serial2.h"
#include "stdio.h"
#include "eeprom.h"
#include "simulator_interface.h"

#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <unistd.h>    /* For sleep() */
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#define MS_IN_A_NANO_SEC (1000*1000)

/* Struct used to pass arguments from the main thread to extra helper threads */
struct args{
   int argc;
   char** argv;
};

/* Entry points for the helper threads */
static void uP_main(struct args* curr_args);
static void io_main(struct args* curr_args);

/* Function to send output data to registered output servers */
static void send_output_data(FrcIoPacket* io_pkt);
static int s_sender_sock;  /* Socket used for sending data from the RC simulator */

/* Data received from the OI socket and stored here for the main thread to access. 
 * The mutex is here to protect reading and writing of this data
 */
static pthread_mutex_t s_operator_intf_mutex;
static FrcOiData s_operator_intf_data;

/* Data received from the sensor socket and output data from the program is stored here. 
 * The mutex is here to protect reading and writing of this data
 */
static pthread_mutex_t s_sensor_mutex;
static FrcIoData s_sensor_data=
  /*ver,resv,resv,resv,resv,swA,maskA,swB,maskB,analog1,analog2,analog3,analog4, */
  { 0 , 0  ,   0,   0,  0  ,0  ,0xFF ,0  ,0xFF ,127    ,127    ,127    ,127    ,
  /*analog5,analog6,analog7,analog8,analog9,analog10,analog11,analog12,analog13, */
    127    ,127    ,127    ,127    ,127    ,127     ,127     ,127     ,127     ,
  /*analog14,analog15,analog16,analog_mask,reserved  */
    127     ,127     ,127     ,0xFFFF     ,0};

/* This array contains all the registered addresses that have requested to be notified
 * of changes in RC outputs (digital and pwm).
 */
#define MAX_REGISTERED_OUTPUT_SERVERS 10
static pthread_mutex_t     s_output_server_addr_mutex; 
static struct sockaddr_in  sp_output_server_address[MAX_REGISTERED_OUTPUT_SERVERS ];

/* Static array that looks like the EEPROM */
#define FRC_EEPROM_SIZE 1024
#define FRC_EEPROM_FILENAME "eeprom.dat"
static UINT8 sp_eeprom[FRC_EEPROM_SIZE];

/*
pthread_attr_t thread_attr;
int thread_policy;
struct sched_param thread_param;
pthread_attr_init(&thread_attr);
pthread_attr_setschedpolicy(&thread_attr, SCHED_RR);
thread_param.sched_priority = 50;
pthread_attr_setschedparam(&thread_attr, &thread_param);
pthread_attr_setinheritsched(&thread_attr, PTHREAD_EXPLICIT_SCHED);
pthread_create(&m_thread, &thread_attr, &CaptureThread, this);
*/

/* ifi_main is in main.c and is called main in a target build */

void ifi_main(void);


int main(int argc, char* argv[])
{
   pthread_t   uP_thread_id;
   pthread_t   io_thread_id;
   struct args curr_args;
   int         i;
   FILE*       p_eeprom_file;

   /* Put the current args in a struct so they can be passed to the new thread */ 
   curr_args.argc = argc;
   curr_args.argv = argv;

   if(pthread_mutex_init(&s_operator_intf_mutex, NULL) != 0)
   {
      printf("failed to init s_operator_intf_mutex\n");
   }
   
   if(pthread_mutex_init(&s_sensor_mutex, NULL) != 0)
   {
      printf("failed to init s_sensor_data_mutex\n");
   }
   
   /* Initialize the output server mutex and struct */ 
   if(pthread_mutex_init(&s_output_server_addr_mutex, NULL) != 0)
   {
      printf("failed to init s_output_server_addr_mutex\n");
   }

   for(i=0;i<MAX_REGISTERED_OUTPUT_SERVERS;i++)
   {
      /* Initialize the output server addresses to 0 to show they are empty */
      sp_output_server_address[i].sin_family      = AF_INET;
      sp_output_server_address[i].sin_addr.s_addr = 0;
      sp_output_server_address[i].sin_port        = 0;
   }

   /******************* EEPROM Code *******************/

   /* Zero out the EEPROM memory */
   memset(sp_eeprom,0,FRC_EEPROM_SIZE);

   p_eeprom_file = fopen(FRC_EEPROM_FILENAME, "r");
   if(p_eeprom_file == NULL)
   {
      printf("Eeprom file '%s' does not exist.  File will be created "
             "on first write.\n", FRC_EEPROM_FILENAME);
   }
   else
   {
      /* Read the data from the file into the static array */
      fread(sp_eeprom, FRC_EEPROM_SIZE, 1, p_eeprom_file);
   }

   /* close the file...if open */
   fclose(p_eeprom_file);
   /***************** End EEPROM Code *****************/
 
   /* Initialize all the digital inputs to 1 (the open state) */
   rc_dig_in01=1;
   rc_dig_in02=1;
   rc_dig_in03=1;
   rc_dig_in04=1;
   rc_dig_in05=1;
   rc_dig_in06=1;
   rc_dig_in07=1;
   rc_dig_in08=1;
   rc_dig_in09=1;
   rc_dig_in10=1;
   rc_dig_in11=1;
   rc_dig_in12=1;
   rc_dig_in13=1;
   rc_dig_in14=1;
   rc_dig_in15=1;
   rc_dig_in16=1;

   s_sender_sock = socket(AF_INET, SOCK_DGRAM, 0);
   if(s_sender_sock != -1)
   {
      printf("uP Server Socket created\n");
 
      /* We don't need to ever read from this socket so shutdown the read side of the socket */
      if(shutdown(s_sender_sock, SHUT_RD) == -1)
      {
         printf("uP failed to shutdown the READ side of sock\n");
      }
   }

   printf("Starting uP thread\n");
   pthread_create(&uP_thread_id, NULL, (void*)uP_main, (void*)&curr_args);
   pthread_create(&io_thread_id, NULL, (void*)io_main, (void*)&curr_args);

   /* Let the other threads initialize */
   sleep(2);

   ifi_main();

   waypoint_init();
   auto_main();

   return 0;
}

void io_main(struct args* curr_args)
{
   while(1) 
   {
     printf("IO -> argc=%d, argv[0]=%s\n",curr_args->argc, curr_args->argv[0]);
     sleep(1);
   }
}


void uP_main(struct args* curr_args)
{
   int    sock;
   struct sockaddr_in  s_address;
   char   p_data[MAX_SOCKET_DATA_SIZE];
   int    pkt_size;
 
   /* Server Socket */
   s_address.sin_family  = AF_INET;          /* family = internet           */
   s_address.sin_port    = RC_SERVER_PORT;   /* Specify the uP Server port number */
   s_address.sin_addr.s_addr = INADDR_ANY;   /* Accept packets from any IP  */

   sock = socket(AF_INET, SOCK_DGRAM, 0);
   if(sock != -1)
   {
      printf("uP Server Socket created\n");
 
      /* We don't need to ever write to this socket so shutdown the write side of the socket */
      if(shutdown(sock, SHUT_WR) == -1)
      {
         printf("uP failed to shutdown the WRITE side of sock\n");
      }

      if(bind(sock, (struct sockaddr*)&s_address, sizeof(s_address)) != -1)
      {
         printf("uP Server Socket bind complete\n");

         while(1)
         {
            struct sockaddr_in  senders_addr;
            int                 addr_size;
            
            addr_size = sizeof(senders_addr);
            pkt_size = recvfrom(sock, (char*)p_data, MAX_SOCKET_DATA_SIZE, 0,
                                (struct sockaddr*)&senders_addr, &addr_size);
            if(pkt_size>4)  /* Size of version field in the struct */
            {
               FrcDataHeader* p_header;
               UINT32*        p32_data;

               p32_data = (UINT32*)p_data;
               printf("Pkt = 0x%08X, 0x%08X, 0x%08X, 0x%08X, 0x%08X, 0x%08X, 0x%08X, 0x%08X, 0x%08X, "
                      "0x%08X,0x%08X,0x%08X,0x%08X,0x%08X\n",
                      p32_data[0],p32_data[1],p32_data[2],p32_data[3],p32_data[4],
                      p32_data[5],p32_data[6],p32_data[7],p32_data[8],p32_data[9],
                      p32_data[10],p32_data[11],p32_data[12],p32_data[13]);

               p_header = (FrcDataHeader*)p_data;
               printf("uP Received packet with version %d\n", p_header->version);

               switch(p_header->packet_type)
               {
                  case OI_DATA_TYPE:   /* Data from the OI */
                  {
                     FrcOiData* p_oi_data;

                     /* To get to the PwmData struct use the offset_to_data field in the header */
                     p_oi_data = (FrcOiData*)&p_data[p_header->offset_to_data];

                     printf("uP received OI packet with version %d\n", p_oi_data->version);
                    
                     /* Since we are changing global data, grab the mutex */
                     if(pthread_mutex_lock(&s_operator_intf_mutex) != 0)
                     {
                        printf("OI data recv was unable to lock OI_mutex\n");
                     }
                     /* Copy the data received into the p_oi_data struct to be used by getdata */
                     s_operator_intf_data = *p_oi_data;

                     /* Set the global status flag to tell the main thread that there is new
                      * data from the uP available
                      */
                     statusflag.NEW_SPI_DATA = 1;
                     
                     /* Finished modifying the global s_sensor_data...release the hounds...uh, mutex */
                     pthread_mutex_unlock(&s_operator_intf_mutex);
                  }
                  break;

                  case OUT_SVR_DEREG_TYPE: /* Deregister a registered server */
                  case OUT_SVR_REG_TYPE:   /* This packet allows servers to register to be notified 
                                            * of uP output changes (digital out and PWM).
                                            */
                  {
                     int            i;
                     int            found_entry;

                     printf("uP received reg/dereg packet with version %d\n", p_header->version);
                  
                     /* Modifying the global sp_output_server...grab the mutex */
                     pthread_mutex_lock(&s_output_server_addr_mutex);

                     /* Search through the output_server_addr to find an open slot (addr=0) */
                     i = 0;
                     found_entry = FALSE;
                     do
                     {
                        /* If the packet is a registration pkt */
                        if(p_header->packet_type == OUT_SVR_REG_TYPE) 
                        {
                           /* If the addr is zero, then this slot is open, or if the 
                            * exact address is already registered.  This logic isn't perfect because
                            * it is possible that an open slot will be found before the matching slot.
                            */
                           if((sp_output_server_address[i].sin_addr.s_addr == 0) ||
                              ((sp_output_server_address[i].sin_addr.s_addr == senders_addr.sin_addr.s_addr) &&
                               (sp_output_server_address[i].sin_port == senders_addr.sin_port)) )
                           {
                              /* Register the address as an output server */
                              sp_output_server_address[i] = senders_addr;

                              printf("Registered output server 0x%08X:0x%04X in slot %d\n",
                                     sp_output_server_address[i].sin_addr.s_addr, 
                                     sp_output_server_address[i].sin_port, i); 

                              /* Set flag to get out of the loop */
                              found_entry = TRUE;
                           }
                        }
                        /* If the packet is a deregistration pkt */
                        else if(p_header->packet_type == OUT_SVR_DEREG_TYPE)
                        {
                           /* If the addr and port match, then dereg this entry */
                           if((sp_output_server_address[i].sin_addr.s_addr == senders_addr.sin_addr.s_addr) &&
                              (sp_output_server_address[i].sin_port == senders_addr.sin_port) )
                           {
                              /* Get the address of the server out of the packet */
                              sp_output_server_address[i].sin_addr.s_addr = 0; 
                              sp_output_server_address[i].sin_port = 0;

                              /* Register the address as an output server */
                              printf("Deregistered output server 0x%08X:0x%04X from slot %d\n",
                                     senders_addr.sin_addr.s_addr, 
                                     senders_addr.sin_port, i); 

                              /* Set flag to get out of the loop */
                              found_entry = TRUE;
                           }
                        }

                        i++;   /* Move to the next slot */
                     }while((i<MAX_REGISTERED_OUTPUT_SERVERS) && (found_entry == FALSE));

                     if(found_entry == FALSE) 
                     {
                        if(p_header->packet_type == OUT_SVR_REG_TYPE) 
                        {
                           printf("Unable to register output server 0x%08X:0x%04X due"
                                  " to not enough space\n",senders_addr.sin_addr.s_addr,senders_addr.sin_port);
                        }
                        else if(p_header->packet_type == OUT_SVR_DEREG_TYPE)
                        {
                           printf("Unable to find and deregister output server 0x%08X:0x%04X\n",
                                  senders_addr.sin_addr.s_addr,senders_addr.sin_port);
                        }
                     }
 
                     /* Finished modifying the global sp_output_server...release the mutex */
                     pthread_mutex_unlock(&s_output_server_addr_mutex);
                  }
                  break;

                  case IO_DATA_TYPE:  /* Input data for the IOs...like sensor input data */
                  {
                     FrcIoData* p_io_data;
                     UINT16     masked_analog;
                     UINT16     masked_digital;

                     /* To get to the PwmData struct use the offset_to_data field in the header */
                     p_io_data = (FrcIoData*)&p_data[p_header->offset_to_data];

                     printf("uP received sensor packet with version %d\n", p_io_data->version);
                  
                     /* Since we are changing global data, grab the mutex */
                     if(pthread_mutex_lock(&s_sensor_mutex) != 0)
                     {
                        printf("Sensor data recv was unable to lock sensor_mutex\n");
                     }

                     /* Copy the data received into the p_io_data struct to be used by getdata.
                      * Be sure to only copy the data that has a 1 set in the corresponding mask.
                      * The mask tells us which values in the packet are actually valid and should
                      * be put set to the RC.
                      *
                      * To only change the values that match the mask, we need to AND the
                      * the current data with the inverse of the mask and then OR in the
                      * data.
                      */

                     /* If the associated bit in the mask is set, use the analog value in the
                      * packet for the current RC sensor data.
                      */
                     masked_analog = p_io_data->rc_analog_mask;
                     if((masked_analog & RC_ANALOG_MASK1) == RC_ANALOG_MASK1)
                        s_sensor_data.rc_analog1 = p_io_data->rc_analog1;
                     if((masked_analog & RC_ANALOG_MASK2) == RC_ANALOG_MASK2)
                        s_sensor_data.rc_analog2 = p_io_data->rc_analog2;
                     if((masked_analog & RC_ANALOG_MASK3) == RC_ANALOG_MASK3)
                        s_sensor_data.rc_analog3 = p_io_data->rc_analog3;
                     if((masked_analog & RC_ANALOG_MASK4) == RC_ANALOG_MASK4)
                        s_sensor_data.rc_analog4 = p_io_data->rc_analog4;
                     if((masked_analog & RC_ANALOG_MASK5) == RC_ANALOG_MASK5)
                        s_sensor_data.rc_analog5 = p_io_data->rc_analog5;
                     if((masked_analog & RC_ANALOG_MASK6) == RC_ANALOG_MASK6)
                        s_sensor_data.rc_analog6 = p_io_data->rc_analog6;
                     if((masked_analog & RC_ANALOG_MASK7) == RC_ANALOG_MASK7)
                        s_sensor_data.rc_analog7 = p_io_data->rc_analog7;
                     if((masked_analog & RC_ANALOG_MASK8) == RC_ANALOG_MASK8)
                        s_sensor_data.rc_analog8 = p_io_data->rc_analog8;
                     if((masked_analog & RC_ANALOG_MASK9) == RC_ANALOG_MASK9)
                        s_sensor_data.rc_analog9 = p_io_data->rc_analog9;
                     if((masked_analog & RC_ANALOG_MASK10) == RC_ANALOG_MASK10)
                        s_sensor_data.rc_analog10 = p_io_data->rc_analog10;
                     if((masked_analog & RC_ANALOG_MASK11) == RC_ANALOG_MASK11)
                        s_sensor_data.rc_analog11 = p_io_data->rc_analog11;
                     if((masked_analog & RC_ANALOG_MASK12) == RC_ANALOG_MASK12)
                        s_sensor_data.rc_analog12 = p_io_data->rc_analog12;
                     if((masked_analog & RC_ANALOG_MASK13) == RC_ANALOG_MASK13)
                        s_sensor_data.rc_analog13 = p_io_data->rc_analog13;
                     if((masked_analog & RC_ANALOG_MASK14) == RC_ANALOG_MASK14)
                        s_sensor_data.rc_analog14 = p_io_data->rc_analog14;
                     if((masked_analog & RC_ANALOG_MASK15) == RC_ANALOG_MASK15)
                        s_sensor_data.rc_analog15 = p_io_data->rc_analog15;
                     if((masked_analog & RC_ANALOG_MASK16) == RC_ANALOG_MASK16)
                        s_sensor_data.rc_analog16 = p_io_data->rc_analog16;

                     /* Finished modifying the global s_sensor_data...release the hounds...uh, mutex */
                     pthread_mutex_unlock(&s_sensor_mutex);

                     /* The robot code reads the digital inputs directly from the input register 
                      * of the processor so this makes it difficult to simulate.  Since there is no
                      * one call that we can stub out, we must simulate the processor memory and then
                      * just change the memory.  Do that now for the digital in values.
                      */
                     masked_digital = p_io_data->rc_digital_mask;
    
                     /* Since rc_dig_inxx is a bit we must mask off the other bits in the packet and
                      * shift the appropriate bit down to bit0 prior to setting it.
                      * There must be a better way!
                      */

                     if((masked_digital & RC_DIGITAL_1) == RC_DIGITAL_1)
                        rc_dig_in01 = ((p_io_data->rc_digital & RC_DIGITAL_1) >> 0);
                     if((masked_digital & RC_DIGITAL_2) == RC_DIGITAL_2)
                        rc_dig_in02 = ((p_io_data->rc_digital & RC_DIGITAL_2) >> 1);
                     if((masked_digital & RC_DIGITAL_3) == RC_DIGITAL_3)
                        rc_dig_in03 = ((p_io_data->rc_digital & RC_DIGITAL_3) >> 2);
                     if((masked_digital & RC_DIGITAL_4) == RC_DIGITAL_4)
                        rc_dig_in04 = ((p_io_data->rc_digital & RC_DIGITAL_4) >> 3);
                     if((masked_digital & RC_DIGITAL_5) == RC_DIGITAL_5)
                        rc_dig_in05 = ((p_io_data->rc_digital & RC_DIGITAL_5) >> 4);
                     if((masked_digital & RC_DIGITAL_6) == RC_DIGITAL_6)
                        rc_dig_in06 = ((p_io_data->rc_digital & RC_DIGITAL_6) >> 5);
                     if((masked_digital & RC_DIGITAL_7) == RC_DIGITAL_7)
                        rc_dig_in07 = ((p_io_data->rc_digital & RC_DIGITAL_7) >> 6);
                     if((masked_digital & RC_DIGITAL_8) == RC_DIGITAL_8)
                        rc_dig_in08 = ((p_io_data->rc_digital & RC_DIGITAL_8) >> 7);
                     if((masked_digital & RC_DIGITAL_9) == RC_DIGITAL_9)
                        rc_dig_in09 = ((p_io_data->rc_digital & RC_DIGITAL_9) >> 8);
                     if((masked_digital & RC_DIGITAL_10) == RC_DIGITAL_10)
                        rc_dig_in10 = ((p_io_data->rc_digital & RC_DIGITAL_10) >> 9);
                     if((masked_digital & RC_DIGITAL_11) == RC_DIGITAL_11)
                        rc_dig_in11 = ((p_io_data->rc_digital & RC_DIGITAL_11) >> 10);
                     if((masked_digital & RC_DIGITAL_12) == RC_DIGITAL_12)
                        rc_dig_in12 = ((p_io_data->rc_digital & RC_DIGITAL_12) >> 11);
                     if((masked_digital & RC_DIGITAL_13) == RC_DIGITAL_13)
                        rc_dig_in13 = ((p_io_data->rc_digital & RC_DIGITAL_13) >> 12);
                     if((masked_digital & RC_DIGITAL_14) == RC_DIGITAL_14)
                        rc_dig_in14 = ((p_io_data->rc_digital & RC_DIGITAL_14) >> 13);
                     if((masked_digital & RC_DIGITAL_15) == RC_DIGITAL_15)
                        rc_dig_in15 = ((p_io_data->rc_digital & RC_DIGITAL_15) >> 14);
                     if((masked_digital & RC_DIGITAL_16) == RC_DIGITAL_16)
                        rc_dig_in16 = ((p_io_data->rc_digital & RC_DIGITAL_16) >> 15);

                  }
                  break;

                  default:
                  {
                     printf("uP Received invalid data type of %X\n", p_header->packet_type);
                  }
              }
            }
            else
            {
               printf("up received small packet with size %d\n", pkt_size);
            }
         }
      }

   }

while(1) 
   {
     printf("uP -> argc=%d, argv[0]=%s\n",curr_args->argc, curr_args->argv[0]);
     sleep(1);
   }


}

/*****************  From ifi_library.c *******************************/
void Getdata(rx_data_ptr ptr)
{
  /* Since we are accessing global data, grab the mutex */
  if(pthread_mutex_lock(&s_operator_intf_mutex) != 0)
  {
     printf("Unable to lock OI_mutex\n");
  }

  ptr->packet_num = s_operator_intf_data.oi_packet_num;

  ptr->rc_mode_byte.allbits=0; /* First clear all the bits */
  ptr->rc_mode_byte.mode.user_display=0;    /* User display enabled is 1 */
  ptr->rc_mode_byte.mode.autonomous=0;      /* Autonomous enabled is 1 */
  ptr->rc_mode_byte.mode.disabled=0;        /* Competition disabled is 1 */

  ptr->oi_swA_byte.allbits=s_operator_intf_data.oi_sw_a;
  ptr->oi_swB_byte.allbits=s_operator_intf_data.oi_sw_b;

  ptr->rc_swA_byte.allbits=0;  /* First clear all the bits */
  ptr->rc_swB_byte.allbits=0;  /* First clear all the bits */

  ptr->oi_analog01=s_operator_intf_data.oi_p1_y;
  ptr->oi_analog02=s_operator_intf_data.oi_p2_y;
  ptr->oi_analog03=s_operator_intf_data.oi_p3_y;
  ptr->oi_analog04=s_operator_intf_data.oi_p4_y;
  ptr->oi_analog05=s_operator_intf_data.oi_p1_x;
  ptr->oi_analog06=s_operator_intf_data.oi_p2_x;
  ptr->oi_analog07=s_operator_intf_data.oi_p3_x;
  ptr->oi_analog08=s_operator_intf_data.oi_p4_x;
  ptr->oi_analog09=s_operator_intf_data.oi_p1_wheel;
  ptr->oi_analog10=s_operator_intf_data.oi_p2_wheel;
  ptr->oi_analog11=s_operator_intf_data.oi_p3_wheel;
  ptr->oi_analog12=s_operator_intf_data.oi_p4_wheel;
  ptr->oi_analog13=s_operator_intf_data.oi_p1_aux; 
  ptr->oi_analog14=s_operator_intf_data.oi_p2_aux;
  ptr->oi_analog15=s_operator_intf_data.oi_p3_aux;     
  ptr->oi_analog16=s_operator_intf_data.oi_p4_aux;    

  ptr->rc_main_batt=255;       /* Assume the batter volts are good */
  ptr->rc_backup_batt=255;

  ptr->reserve[0]=0;           /* Zero out the reserved bytes */
  ptr->reserve[1]=0;
  ptr->reserve[2]=0;
  ptr->reserve[3]=0;
  ptr->reserve[4]=0;
  ptr->reserve[5]=0;
  ptr->reserve[6]=0;
  ptr->reserve[7]=0;
  
  statusflag.NEW_SPI_DATA = 0;  /* This will indicate that there is no new uP data so future
                                 * loops won't call this function until it is cleared
                                 */
  
  /* Finished modifying the global s_sensor_data...release the hounds...uh, mutex */
  pthread_mutex_unlock(&s_operator_intf_mutex);
  
  printf("p3_x = %d, p3_y = %d\n", ptr->oi_analog07, ptr->oi_analog03);
  
}

void Putdata(tx_data_ptr ptr)
{
   FrcIoPacket io_pkt;

   io_pkt.header.version = FRC_DATA_HEADER_VER_0;
   io_pkt.header.packet_type = IO_DATA_TYPE;
   io_pkt.header.offset_to_data = (UINT32)((int)&(io_pkt.data) - (int)&io_pkt);

   io_pkt.data.version = FRC_IO_DATA_HEADER_VER_0;
   io_pkt.data.reserved1 = 0;
   io_pkt.data.reserved2 = 0;
   io_pkt.data.reserved3 = 0;
   io_pkt.data.reserved4 = 0;
   
   io_pkt.data.rc_digital_mask = 0;     /* No valid switch data in this packet */
   io_pkt.data.rc_analog_mask = 0;      /* No valid analog data in this packet */
  
   io_pkt.data.rc_pwm1 = ptr->rc_pwm01;   /* Fill in the pwms and mark them as valid */
   io_pkt.data.rc_pwm2 = ptr->rc_pwm02;
   io_pkt.data.rc_pwm3 = ptr->rc_pwm03;
   io_pkt.data.rc_pwm4 = ptr->rc_pwm04;
   io_pkt.data.rc_pwm5 = ptr->rc_pwm05;
   io_pkt.data.rc_pwm6 = ptr->rc_pwm06;
   io_pkt.data.rc_pwm7 = ptr->rc_pwm07;
   io_pkt.data.rc_pwm8 = ptr->rc_pwm08;
   io_pkt.data.rc_pwm9 = ptr->rc_pwm09;
   io_pkt.data.rc_pwm10= ptr->rc_pwm10;
   io_pkt.data.rc_pwm11= ptr->rc_pwm11;
   io_pkt.data.rc_pwm12= ptr->rc_pwm12;
   io_pkt.data.rc_pwm13= ptr->rc_pwm13;
   io_pkt.data.rc_pwm14= ptr->rc_pwm14;
   io_pkt.data.rc_pwm15= ptr->rc_pwm15;
   io_pkt.data.rc_pwm16= ptr->rc_pwm16;
   io_pkt.data.rc_pwm_mask = 0xFFFF; /* Mark all PWMs as valid */

   io_pkt.data.rc_LED_byte1 = ptr->LED_byte1.data;
   io_pkt.data.rc_LED_byte2 = ptr->LED_byte2.data;
   io_pkt.data.rc_user_byte1 = ptr->user_byte1.allbits;
   io_pkt.data.rc_user_byte2 = ptr->user_byte2.allbits;
   io_pkt.data.rc_user_byte3 = ptr->user_byte3;
   io_pkt.data.rc_user_byte4 = ptr->user_byte4;
   io_pkt.data.rc_user_byte5 = ptr->user_byte5;
   io_pkt.data.rc_user_byte6 = ptr->user_byte6;
   io_pkt.data.rc_user_data_mask = 0xFF;  /* Mark all user data as valid */
   
   io_pkt.data.rc_relays_mask = 0; /*  No valid relay info in this packet */

   /* Send the data to the output servers */
   send_output_data(&io_pkt);
  
   /* Handle data sent to the master uP. */
   printf("Putdata sorta stubbed out \n");
}

void Setup_PWM_Output_Type(int pwmSpec1,int pwmSpec2,int pwmSpec3,int pwmSpec4)
{
  printf("Setup_PWM_Output_Type stubbed out \n"); 
}

void User_Proc_Is_Ready (void)
{
  printf("User_Proc_Is_Ready stubbed out \n");
   
}

/* Use this function to initialized anything that is managed by IFI or we want to
 * simulate
 */
void IFI_Initialization (void)
{
  printf("IFI_Initialization SORTA stubbed out \n");

  /**** rx_data is a global struct that lives in ... and holds the data received from the master uP */
  /*rx_data->packet_num=0;*/

  statusflag.FIRST_TIME = 1;  /* As far as I can tell this is only used in PacketNum_Check debug code
                               * by ISI.  Just leave set it to 1 to say it is the FIRST TIME.
                               */
  statusflag.NEW_SPI_DATA = 1; /* This way the first main loop will see there is uP data to be received */

}

/*****************  From ws_serial2.c *******************************/

/*******************************************************************************
* FUNCTION NAME: Open2USART
* PURPOSE:       Opens the serial port 2 for communication out the TTL port
* ARGUMENTS:     none
* RETURNS:       void
*******************************************************************************/
void Open2USART(UINT8 mode, UINT8 speed)
{
  TXINTF2 = 1; /* Keeps Wait4TxEmpty2 from getting hung up */
}

/*******************************************************************************
* FUNCTION NAME: Open2USART
* PURPOSE:       Write a byte out to serial port 2
* CALLED FROM:   ws_serial2.c
* ARGUMENTS:     none
* RETURNS:       void
*******************************************************************************/
void write_serial2(UINT8 data)
{
   printf("[2]%c\n", data);
}

/*******************************************************************************
* FUNCTION NAME: recv_from_serial2
* PURPOSE:       Get a byte from serial port 2
* CALLED FROM:   ws_serial2.c
* ARGUMENTS:     none
* RETURNS:       void
*******************************************************************************/
UINT8 recv_from_serial2(void)
{
   printf("[RX2]\n");
   return('a');
}

/*****************  From ifi_utilities.c *******************************/

/*******************************************************************************
* FUNCTION NAME: Initialize_Serial_Comms
* PURPOSE:       Opens the serial port 1 for communicating with your PC at
*                115k baud, 8 bits, no parity, one stop bit, and no flow control.
* CALLED FROM:   user_routines.c
* ARGUMENTS:     none
* RETURNS:       void
*******************************************************************************/
void Initialize_Serial_Comms (void)
{
}

/*******************************************************************************
* FUNCTION NAME: Set_Number_of_Analog_Channels
* PURPOSE:       Sets the variable used in Get_Analog_Value routine (below)
*                to the number of analog channels desired by the user.
* CALLED FROM:   user_routines.c initialization, typically
* ARGUMENTS:     
*      Argument            Type    IO   Description
*     -----------          -----   --   -----------
*     number_of_channels   alias   I    choose alias from ifi_aliases.h
* RETURNS:       void
*******************************************************************************/
void Set_Number_of_Analog_Channels (unsigned char number_of_channels)
{
}

/*******************************************************************************
* FUNCTION NAME: Get_Analog_Value
* PURPOSE:       Reads the analog voltage on an A/D port and returns the
*                10-bit value read stored in an unsigned int.
* CALLED FROM:   user_routines.c, typically
* ARGUMENTS:     
*      Argument         Type        IO   Description
*     -----------   -------------   --   -----------
*     ADC_channel       alias       I    alias found in ifi_aliases.h
* RETURNS:       unsigned int
*******************************************************************************/
unsigned int Get_Analog_Value (unsigned char ADC_channel)
{
  unsigned int result = 0;

  /* Since we are accessing global data, grab the mutex */
  if(pthread_mutex_lock(&s_sensor_mutex) != 0)
  {
     printf("GAV was unable to lock sensor_mutex\n");
  }

  switch(ADC_channel)
  {
    case ADC_CH0: 
    {
      result = s_sensor_data.rc_analog1;     
    }
    break;
    
    case ADC_CH1: 
    {
      result = s_sensor_data.rc_analog2;     
    }
    break;
    
    case ADC_CH2:  
    {
      result = s_sensor_data.rc_analog3;     
    }
    break;
    
    case ADC_CH3:  
    {
      result = s_sensor_data.rc_analog4;     
    }
    break;
    
    case ADC_CH4:  
    {
      result = s_sensor_data.rc_analog5;     
    }
    break;

    case ADC_CH5:  
    {
      result = s_sensor_data.rc_analog6;     
    }
    break;

    case ADC_CH6:  
    {
      result = s_sensor_data.rc_analog7;     
    }
    break;

    case ADC_CH7:  
    {
      result = s_sensor_data.rc_analog8;     
    }
    break;

    case ADC_CH8:  
    {
      result = s_sensor_data.rc_analog9;     
    }
    break;

    case ADC_CH9:  
    {
      result = s_sensor_data.rc_analog10;     
    }
    break;

    case ADC_CH10:  
    {
      result = s_sensor_data.rc_analog11;     
    }
    break;

    case ADC_CH11:  
    {
      result = s_sensor_data.rc_analog12;     
    }
    break;

    case ADC_CH12:  
    {
      result = s_sensor_data.rc_analog13;     
    }
    break;

    case ADC_CH13: 
    {
      result = s_sensor_data.rc_analog14;     
    }
    break;

    case ADC_CH14: 
    {
      result = s_sensor_data.rc_analog15;     
    }
    break;

    case ADC_CH15: 
    {
      result = s_sensor_data.rc_analog16;     
    }
    break;

    default:
    {
      printf("Invalid channel value %d/n", ADC_channel);
    }
  }
  
  /* Finished modifying the global s_sensor_data...release the hounds...uh, mutex */
  pthread_mutex_unlock(&s_sensor_mutex);

  return result;
}

/****************************** From Util_lib.asm ************************************************/
#ifdef _FRC_BOARD
/* located in ifi_library.lib */
void Generate_Pwms(unsigned char pwm_13,unsigned char pwm_14,
                   unsigned char pwm_15,unsigned char pwm_16)
#else
/* located in ifi_library.lib */
void Generate_Pwms(unsigned char pwm_1,unsigned char pwm_2,
                   unsigned char pwm_3,unsigned char pwm_4,
                   unsigned char pwm_5,unsigned char pwm_6,
                   unsigned char pwm_7,unsigned char pwm_8)
#endif
{
   FrcIoPacket io_pkt;

   io_pkt.header.version = FRC_DATA_HEADER_VER_0;
   io_pkt.header.packet_type = IO_DATA_TYPE;
   io_pkt.header.offset_to_data = (UINT32)((int)&(io_pkt.data) - (int)&io_pkt);

   io_pkt.data.version = FRC_IO_DATA_HEADER_VER_0;
   io_pkt.data.reserved1 = 0;
   io_pkt.data.reserved2 = 0;
   io_pkt.data.reserved3 = 0;
   io_pkt.data.reserved4 = 0;
   
   io_pkt.data.rc_digital_mask = 0;     /* No valid switch data in this packet */
   io_pkt.data.rc_analog_mask = 0;      /* No valid analog data in this packet */
  
   io_pkt.data.rc_pwm13 = pwm_13;   /* Fill in pwm13-pwm16 and mark them as valid */
   io_pkt.data.rc_pwm14 = pwm_14;
   io_pkt.data.rc_pwm15 = pwm_15;
   io_pkt.data.rc_pwm16 = pwm_16;
   io_pkt.data.rc_pwm_mask = RC_PWM_MASK13 | RC_PWM_MASK14 | RC_PWM_MASK15 | RC_PWM_MASK16;
 
   io_pkt.data.rc_user_data_mask = 0;  /* No valid user data in this packet */
   io_pkt.data.rc_relays_mask = 0;  /* No valid relay data in this packet */

   /* Send the data to the output servers */
   send_output_data(&io_pkt);
   
   /*
   printf("GENERATE_PWMS:pwm13=%03d, pwm14=%03d, pwm15=%03d, pwm16=%03d\n",
                         pwm_13,     pwm_14,     pwm_15,     pwm_16);
   */

}


/****************************** From eeprom.c ****************************/

/* Not needed for simulator...writes buffer to EEPROM */
void processEEQueue(void)
{

}

/* Not needed for simulator...checks if write buffer is empty */
UINT8 checkEEQueue(void)
{
  return (EE_EMPTY);
}

/*******************************************************************************
* FUNCTION NAME: writeEE
* PURPOSE:       Write value to EEPROM
* CALLED FROM:   
* ARGUMENTS:     address - address of value to write
*                data - data to store in EEPROM
* RETURNS:       none
*******************************************************************************/
void writeEE(unsigned short address, unsigned char data)
{
   FILE*   p_eeprom_file;

   /* First update the static struct */
   sp_eeprom[address] = data;

   /* Now write the data to file */
   p_eeprom_file = fopen(FRC_EEPROM_FILENAME, "w+");
   if(p_eeprom_file == NULL)
   {
      printf("Unable to create eeprom file\n");
   }
   else
   {
      /* Write the data to the newly created file */
      fwrite(sp_eeprom, FRC_EEPROM_SIZE, 1, p_eeprom_file);

   }

   fclose(p_eeprom_file);

}

/*******************************************************************************
* FUNCTION NAME: readEE
* PURPOSE:       Read value from EEPROM
* CALLED FROM:   
* ARGUMENTS:     address - address of value to read
* RETURNS:       value at address
*******************************************************************************/
unsigned char readEE(unsigned short address)
{
   /* Return the data from the static EEPROM struct */
   return(sp_eeprom[address]);
}


/****************************** Helper Functions ************************/
void send_output_data(FrcIoPacket* io_pkt)
{
   int i;

   /* Lock the list of output servers while we access it */ 
   pthread_mutex_lock(&s_output_server_addr_mutex);

   for(i=0;i<MAX_REGISTERED_OUTPUT_SERVERS;i++)
   {
      if(sp_output_server_address[i].sin_addr.s_addr != 0 &&
         sp_output_server_address[i].sin_port != 0  )
      {
         int ret_val;

         /* Send the current IO struct to the registered output server */ 
         ret_val = sendto(s_sender_sock, (char*)io_pkt, sizeof(*io_pkt), 0,
                          (struct sockaddr*)&(sp_output_server_address[i]), 
                          sizeof(sp_output_server_address[i]));
         printf("Sent output to 0x%08X:0x%04X, ret_val=%d[%d]\n",sp_output_server_address[i].sin_addr.s_addr,
                sp_output_server_address[i].sin_port, ret_val, errno);
      }
   }
   
   /* Unlock the list of output servers */ 
   pthread_mutex_unlock(&s_output_server_addr_mutex);

   return;
}
  
/* This function is called at the end of the main loop of the robot program.  It gives us a chance to
 * send out the values of the digital outputs and also allows us to take a quick nap so we don't run
 * run the processor full tilt.
 */
void simulator_end_main_loop(void)
{
   FrcIoPacket io_pkt;

   /* At the end of each loop we have a great chance to send out the current status of the digital
    * outputs (not yet supported) and the relay output values.
    * NOTE: This doesn't provide true simulation since it is possible that the digital outs and the
    *       relays changed multiple times throughout the main loop...oh, well.
    */
   io_pkt.header.version = FRC_DATA_HEADER_VER_0;
   io_pkt.header.packet_type = IO_DATA_TYPE;
   io_pkt.header.offset_to_data = (UINT32)((int)&(io_pkt.data) - (int)&io_pkt);

   io_pkt.data.version = FRC_IO_DATA_HEADER_VER_0;
   io_pkt.data.reserved1 = 0;
   io_pkt.data.reserved2 = 0;
   io_pkt.data.reserved3 = 0;
   io_pkt.data.reserved4 = 0;
   
   io_pkt.data.rc_digital_mask = 0;     /* No valid switch data in this packet */
   io_pkt.data.rc_analog_mask = 0;      /* No valid analog data in this packet */
   io_pkt.data.rc_pwm_mask = 0;         /* No valid pwm data in this packet */
   io_pkt.data.rc_user_data_mask = 0;  /* No valid user data in this packet */

   /*
   printf("mask is %d bytes into data\n", (int)&(io_pkt.data.rc_relays_mask)-(int)&(io_pkt.data.version));
   */
   io_pkt.data.rc_relays_mask = 0xFFFF;  /* All the relays are valid in this packet */
   io_pkt.data.rc_relays = 0;            /* Init to all 0 and or in the 1s */
   if(relay1_fwd == 1) io_pkt.data.rc_relays |= RC_RELAY1_FWD;
   if(relay2_fwd == 1) io_pkt.data.rc_relays |= RC_RELAY2_FWD;
   if(relay3_fwd == 1) io_pkt.data.rc_relays |= RC_RELAY3_FWD;
   if(relay4_fwd == 1) io_pkt.data.rc_relays |= RC_RELAY4_FWD;
   if(relay5_fwd == 1) io_pkt.data.rc_relays |= RC_RELAY5_FWD;
   if(relay6_fwd == 1) io_pkt.data.rc_relays |= RC_RELAY6_FWD;
   if(relay7_fwd == 1) io_pkt.data.rc_relays |= RC_RELAY7_FWD;
   if(relay8_fwd == 1) io_pkt.data.rc_relays |= RC_RELAY8_FWD;

   if(relay1_rev == 1) io_pkt.data.rc_relays |= RC_RELAY1_REV;
   if(relay2_rev == 1) io_pkt.data.rc_relays |= RC_RELAY2_REV;
   if(relay3_rev == 1) io_pkt.data.rc_relays |= RC_RELAY3_REV;
   if(relay4_rev == 1) io_pkt.data.rc_relays |= RC_RELAY4_REV;
   if(relay5_rev == 1) io_pkt.data.rc_relays |= RC_RELAY5_REV;
   if(relay6_rev == 1) io_pkt.data.rc_relays |= RC_RELAY6_REV;
   if(relay7_rev == 1) io_pkt.data.rc_relays |= RC_RELAY7_REV;
   if(relay8_rev == 1) io_pkt.data.rc_relays |= RC_RELAY8_REV;
   /*
   printf("relays=0x%X, mask=0x%X\n",io_pkt.data.rc_relays, io_pkt.data.rc_relays_mask );
   */

   /* Send the data to the output servers */
   send_output_data(&io_pkt);

   /* To avoid running the processor at full, put in a little sleep at each loop */
   struct timespec  sleep_time;
   sleep_time.tv_sec=0;
   sleep_time.tv_nsec=MS_IN_A_NANO_SEC*2; /* A quick 2ms nap */
   /* For a simulator build, let the processor rest for a bit or my battery will be drained in a sec */
   nanosleep(&sleep_time,NULL);
}
