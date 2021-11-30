/********************************************************************
 * test_m8266wifi.c
 * .Description
 *     Source file of M8266WIFI testing application 
 * .Copyright(c) Anylinkin Technology 2015.5-
 *     IoT@anylinkin.com
 *     http://www.anylinkin.com
 *     http://anylinkin.taobao.com
 *  Author
 *     wzuo
 *  Date
 *  Version
 ********************************************************************/
 
#include "stdio.h"
#include "brd_cfg.h"
#include "M8266WIFIDrv.h"
#include "M8266HostIf.h"
#include "M8266WIFI_ops.h"
#include "led.h"

void M8266WIFI_Test(void)
{
	 u16 i;
	 u16 status = 0;
	 u8  link_no=0;

#ifdef USE_LED_AND_KEY_FOR_TEST	
	for(i=0; i<3; i++)   // Flash the Two LEDs 4 times in the Main Board to indicate we start test. not mandatory.
	{                    // Chinese: ��Ƭ�������ϵ�LED����˸4�Σ�������ʾ����ִ�е�����ķ�����ԡ���ģ��ͨ���޹أ��Ǳ��룬���ԡ���û�ж����USE_LED_AND_KEY_FOR_TEST���˴����ᱻ���������
      	 LED_set(0, 0); LED_set(1, 1);  M8266WIFI_Module_delay_ms(100);
		     LED_set(0, 1); LED_set(1, 0);  M8266WIFI_Module_delay_ms(100);
		     LED_set(0, 0); LED_set(1, 1);  M8266WIFI_Module_delay_ms(100);
		     LED_set(0, 1); LED_set(1, 0);  M8266WIFI_Module_delay_ms(100);
	 }
	 LED_set(0, 1); LED_set(1, 1);
#endif	 

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	 Macro Defitions SETUP  SOCKET CONNECTIONS  (Chinese: �����׽��ֵ�һЩ��)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	 
   /////////////
   ////	 Macro for Test Type(Chinese���������ͺ궨��)
	 #define TEST_M8266WIFI_TYPE    1	     //           1 = Repeative Sending, 2 = Repeative Reception, 3 = Echo  4 = multi-clients transimission test
	                                       // (Chinese: 1=ģ�����ⲻͣ�ط������� 2=ģ�鲻ͣ�ؽ������� 3= ģ�齫���յ������ݷ��͸����ͷ� 4=��ͻ��˲���) 

	 /////////////
   ////	 Macros for Socket Connection Type (Chinese���׽������͵ĺ궨��) 
	 #define TEST_CONNECTION_TYPE   1	    //           0=WIFI module as UDP, 1=WIFI module as TCP Client, 2=WIFI module as TCP Server
	                                      // (Chinese: 0=WIFIģ����UDP, 1=WIFIģ����TCP�ͻ���, 2=WIFIģ����TCP������
   /////////////
   ////	 Macros for Soket ip:port pairs  (Chinese���׽��ֵı��ض˿ں�Ŀ���ַĿ��˿ڵĺ궨��) 
   //local port	(Chinese���׽��ֵı��ض˿�)
#if (TEST_CONNECTION_TYPE==1)         //// if module as TCP Client (Chinese:���ģ����ΪTCP�ͻ���)
	 #define TEST_LOCAL_PORT  			0			//           local port=0 will generate a random local port each time fo connection. To avoid the rejection by TCP server due to repeative connection with the same ip:port
   	                                    // (Chinese: ��local port���ݵĲ���Ϊ0ʱ�����ض˿ڻ������������һ�����ģ�����ͻ��˷������ӷ�����ʱ�����á���Ϊ��������Ķ˿�ÿ�λ᲻һ�����Ӷ������������β���ͬ���ĵ�ַ�Ͷ˿�����ʱ���������ܾ���
#elif (TEST_CONNECTION_TYPE==0) || (TEST_CONNECTION_TYPE==2) //// if module as UDP or TCP Server (Chinese:���ģ����ΪUDP��TCP������)
   #define TEST_LOCAL_PORT  			4321  //           a local port should be specified //(Chinese:���ģ����ΪUDP��TCP������������Ҫָ�����׽��ֵı��ض˿�)
#else
#error WRONG TEST_CONNECTION_TYPE defined !
#endif                                  // (Chinese: ���ģ����ΪTCP��������UDP����ô����ָ�����ض˿�

   //local port	(Chinese���׽��ֵ�Ŀ���ַ��Ŀ��˿�)
#if (TEST_CONNECTION_TYPE==0)        //// if module as UDP (Chinese:���ģ����ΪUDP�������ָ��Ŀ���ַ�Ͷ˿ڣ�Ҳ���������䣬�ڷ�������ʱ�����û����)
   #define TEST_REMOTE_ADDR    		"192.168.4.2"
   #define TEST_REMOTE_PORT  	    1234
#elif (TEST_CONNECTION_TYPE==1)      //// if module as TCP Client (Chinese:���ģ����ΪTCP�ͻ��ˣ���Ȼ����ָ��Ŀ���ַ��Ŀ��˿ڣ���ģ����Ҫȥ���ӵ�TCP�������ĵ�ַ�Ͷ˿�)
   #define TEST_REMOTE_ADDR    	 	"192.168.4.2"  // "www.baidu.com"
   #define TEST_REMOTE_PORT  	    1234						// 80
#elif (TEST_CONNECTION_TYPE==2)     //// if module as TCP Server (Chinese:���ģ����ΪTCP������)
		#define TEST_REMOTE_ADDR      "1.1.1.1" // no need remote ip and port upon setup connection. below values just for filling and any value would be ok. 
    #define TEST_REMOTE_PORT  	  1234  	 //(Chinese: ����TCP������ʱ������Ҫָ��Ŀ���ַ�Ͷ˿ڣ����������ֻ��һ����ʽ��䣬�����д��
#else
#error WRONG TEST_CONNECTION_TYPE defined !
#endif			 


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	 Setup Connection and Config connection upon neccessary (Chinese: �����׽��֣��Լ���Ҫʱ���׽��ֵ�һЩ����)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////
//step 0: config tcp windows number (Chinese: ����0�������TCP���͵��׽��֣��������õ������ڲ�����
#if ( 0 && ((TEST_CONNECTION_TYPE==1)||(TEST_CONNECTION_TYPE==2)) ) //If you hope to change TCP Windows, please change '0' to '1' in the #if clause before setup the connection
																																		//(Chinese: �����ı��׽��ֵĴ����������Խ�#if����е�0�ĳ�1�����������Ҫ�ڴ����׽���֮ǰִ��)
// u8 M8266WIFI_SPI_Config_Tcp_Window_num(u8 link_no, u8 tcp_wnd_num, u16* status)
  if(M8266WIFI_SPI_Config_Tcp_Window_num(link_no, 4, &status)==0)
  {
		 while(1)
		 {
     #ifdef USE_LED_AND_KEY_FOR_TEST	 // led flash in 1Hz uppon errors
	      LED_set(0, 0); LED_set(1, 0); M8266WIFI_Module_delay_ms(100);
	      LED_set(0, 1); LED_set(1, 1); M8266WIFI_Module_delay_ms(100);
     #endif
	   }
	}
#endif

///////	
//step 1: setup connection (Chinese: ����1�������׽������ӣ�

  //  u8 M8266WIFI_SPI_Setup_Connection(u8 tcp_udp, u16 local_port, char remote_addr, u16 remote_port, u8 link_no, u8 timeout_in_s, u16* status);
	if(M8266WIFI_SPI_Setup_Connection(TEST_CONNECTION_TYPE, TEST_LOCAL_PORT, TEST_REMOTE_ADDR, TEST_REMOTE_PORT, link_no, 20, &status)==0)
	{		
		 while(1)
		 {
     #ifdef USE_LED_AND_KEY_FOR_TEST	 // led flash in 1Hz uppon errors (Chinese: ��������׽���ʧ�ܣ��������ѭ������˸�����ϵĵƣ�
	      LED_set(0, 0); LED_set(1, 0); M8266WIFI_Module_delay_ms(500);
	      LED_set(0, 1); LED_set(1, 1); M8266WIFI_Module_delay_ms(500);
     #endif
		 }
	}
	else  // else: setup connection successfully, we could config it (Chinese: �����׽��ֳɹ����Ϳ��������׽��֣�
	{
#if (0 && (TEST_CONNECTION_TYPE == 0) )  		//// If UDP, then the module could join a multicust group. If you hope to use multicust, Change first '0' to '1'  in the #if clause
		                                        //   (Chinese: �������׽�����UDP����ô�������ó��鲥ģʽ�������Ҫ���ó��鲥�����Խ�#if����еĵ�һ��'0'�ĳ�'1')
			//u8 M8266WIFI_SPI_Set_Multicuast_Group(u8 join_not_leave, char multicust_group_ip[15+1], u16* status)
		 if(M8266WIFI_SPI_Op_Multicuast_Group(0, "224.6.6.6", &status)==0)
     {
		   while(1)
		   {
       #ifdef USE_LED_AND_KEY_FOR_TEST   // led flash in 1Hz uppon errors (Chinese: ���ʧ�ܣ��������ѭ������˸�����ϵĵƣ�
				  LED_set(0, 0); LED_set(1, 0); M8266WIFI_Module_delay_ms(1000);
	        LED_set(0, 1); LED_set(1, 1); M8266WIFI_Module_delay_ms(1000);
       #endif				 
		   }
     }
     else

#elif (TEST_CONNECTION_TYPE == 2)          //// If TCP server, then tcp server auto disconnection timeout, and max clients allowed could be set	
		                                        //  (Chinese: ���TCP����������ô�����������TCP������(��ʱ����ͨ�Ŷ�)�Ͽ��ͻ��˵ĳ�ʱʱ��)
#if 1
		 //u8 M8266WIFI_SPI_Set_TcpServer_Auto_Discon_Timeout(u8 link_no, u16 timeout_in_s, u16* status)	
		 if( M8266WIFI_SPI_Set_TcpServer_Auto_Discon_Timeout(link_no, 120, &status) == 0)
     {
		   while(1)
		   {
       #ifdef USE_LED_AND_KEY_FOR_TEST	// led flash in 1Hz when error
	        LED_set(0, 0); LED_set(1, 0); M8266WIFI_Module_delay_ms(1000);
	        LED_set(0, 1); LED_set(1, 1); M8266WIFI_Module_delay_ms(1000);
       #endif		 
		   }
     }
#endif		 
#if 0
  		   //u8 M8266WIFI_SPI_Config_Max_Clients_Allowed_To_A_Tcp_Server(u8 server_link_no, u8 max_allowed, u16* status);
     else if( M8266WIFI_SPI_Config_Max_Clients_Allowed_To_A_Tcp_Server(link_no, 5, &status)==0)
		 {
		   while(1)
		   {
       #ifdef USE_LED_AND_KEY_FOR_TEST	// led flash in 1Hz when error
	        LED_set(0, 0); LED_set(1, 0); M8266WIFI_Module_delay_ms(1000);
	        LED_set(0, 1); LED_set(1, 1); M8266WIFI_Module_delay_ms(1000);
       #endif		 
		   }
		 }
#endif		 
		 else
#endif
		 //Setup Connection successfully (Chinese: �����׽������ӳɹ�)
     {
     #ifdef USE_LED_AND_KEY_FOR_TEST			 
		   // led 1 flash 4 times upon success 
		   LED_set(1, 0); M8266WIFI_Module_delay_ms(50); LED_set(1, 1); M8266WIFI_Module_delay_ms(50);
		   LED_set(1, 0); M8266WIFI_Module_delay_ms(50); LED_set(1, 1); M8266WIFI_Module_delay_ms(50);
		   LED_set(1, 0); M8266WIFI_Module_delay_ms(50); LED_set(1, 1); M8266WIFI_Module_delay_ms(50);
		   LED_set(1, 0); M8266WIFI_Module_delay_ms(50); LED_set(1, 1); M8266WIFI_Module_delay_ms(50);
     #endif			 
		 }
	}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	 Communication Test (Chinese: WIFI�׽��ֵ������շ�ͨ�Ų���)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if (TEST_M8266WIFI_TYPE==1) // Transmission Test: to test send data to remote constantly // (Chinese: ���Ͳ��ԣ����ϵ���Ŀ�귢������) 
{                            
	
#define TEST_SEND_DATA_SIZE   2920 //5840 //2048 // 1024   // If using large size of array, ensure system stack is large engough for the array variable�� Or stack over-bound leakage might bring about the mcu into "hardware fault exception"
	                                                  
   u8  snd_data[TEST_SEND_DATA_SIZE];                     // (Chinese: ���ʹ�ýϴ�����飬�ǵ�ȷ�����㹻���ϵͳ��ջ������������������. ���򣬵�Ƭ��������ܻ���Ϊ��ջ���Խ������롰hardware fault"ϵͳ�쳣) 
	 volatile u32 sent = 0;
   volatile u32 total_sent = 0, MBytes = 0; 
	 u16 batch;
	 
	 volatile u8 debug_point;

 	 for(i=0; i<TEST_SEND_DATA_SIZE; i++) snd_data[i]=i;  //Generate an array of data to transmit (Chinese: ����һ���������ݵ�������Ϊ�����͵�����) 
	 
   link_no=0;
	
 	 for(batch=0; ; batch++)
	 {
		  // below used to convenient reception end to differentiate packets when test and evaluate the reliability of transmission. Not necesary.
		  // (Chinese: �������仰���ڸ��췢�͵İ�������ǰ���������͵İ��᲻һ����������������Ŀɿ��Է���ʱ��������ն˷������ݡ��Ǳ��롣)
		  snd_data[0]= batch>>8;    							// (Chinese: first two bytes using bacth to differentiate the continous packets (Chinese: ����ʼ�����ֽ��趨Ϊ��ͬ����ţ��Ա����ֲ�ͬ�İ�) 
			snd_data[1]= batch&0xFF;                
		  snd_data[TEST_SEND_DATA_SIZE-2]=0xFF;		// last byte using customised to lable end of a packet for the sake of searching for data verification. Here FF FF as an example
      snd_data[TEST_SEND_DATA_SIZE-1]=0xFF;   // (Chinese: �������������ֽ��趨Ϊһ��������ֽڣ��Է�����ն��������Ľ�β����������õ��� FF FF����Ϊ�����ķ�������ʱ˳������ı������ܳ���FF FF����������������յ���FF FF��Ȼ�����β��Ҳ������������ģʽƥ�䡣

		  if(total_sent> 1024*1024)  // watch MBytes*1024*1024+total_sent, which is the count of data module sends, compared with the received count at the reception end, to determin the packet loss etc
			{                          // (Chinese: ��������һ��ʱ��󣬹۲���ʽ MBytes*1024*1024+total_sent ��ֵ���ͽ��ն˽��յ������ݸ������бȽϣ����Դ��Ժ���ģ��Ķ����ʡ�)
				 MBytes++;
				 total_sent -= 1024*1024;
			}

#if 1  // using M8266WIFI_SPI_Send_BlockData() to sent block data (Chinese: ����M8266WIFI_SPI_Send_BlockData() �����ʹ������
  {
		//u32 M8266WIFI_SPI_Send_BlockData(u8 Data[], u32 Data_len, u16 max_loops, u8 link_no, char* remote_ip, u16 remote_port, u16* status);
      sent = M8266WIFI_SPI_Send_BlockData(snd_data, (u32)TEST_SEND_DATA_SIZE, 5000, link_no, NULL, 0, &status); // !! Kind noted: For those platform compilation by TI/IAR, such MSP430, K60, TMS28335 etc, PLEASE add (u32) to mandatorily convert a const to u32, or, the parameter transmission will be 16-bit and bring about function calling failure
                                                                                                              // (Chinese) ������ЩTI�ṩ��ƽ̨������������MSP430, K60��TMS28335,�ȵȣ�ע�������Data_len������32λ�ģ����ԣ���ע�⣬����һ��������2^16�ĳ�ֵ�ĳ��Ȳ���ʱ��һ��Ҫ��ע��λu32������(u32)TEST_SEND_DATA_SIZE������ 2048UL ���򣬿��ܻ���ֲ������ݴ�λ�����Ρ�����������ֻ������TI��IAR��ĳЩ�����������¡�
		  total_sent += sent;
		  		
			if( (sent==TEST_SEND_DATA_SIZE) && ((status&0xFF)==0x00) ) //Send successfully 
			{
			}
			else if( (status&0xFF) == 0x1E)			       // 0x1E = too many errors ecountered during sending and can not fixed, or transsmission blocked heavily(Chinese: ���ͽ׶�����̫��Ĵ���������ˣ����Կ��ǼӴ�max_loops)
      {
				debug_point = 1;
				//add some process here (Chinese: �����ڴ˴���һЩ������������max_loops��ֵ)
			}
		  else if(  ((status&0xFF) == 0x14)			 // 0x14 = connection of link_no not present (Chinese: ���׽��ֲ�����)
             || ((status&0xFF) == 0x15) )    // 0x15 = connection of link_no closed(Chinese: ���׽����Ѿ��رջ�Ͽ�)			
      {
				 debug_point = 2;
				 //need to re-establish the socket connection (Chinese: ��Ҫ�ؽ������׽���)
			}
			else if( (status&0xFF) == 0x18 )        // 0x18 = TCP server in listening states and no tcp clients have connected. (Chinese: ���TCP��������û�пͻ�����������)
			{
				 debug_point = 3;
				 M8266HostIf_delay_us(99);
			}
			else
	    {
				 debug_point = 4;
	       M8266HostIf_delay_us(101);
	    }
	}			
#else // using M8266WIFI_SPI_Send_Data() to sent data packet by packet (Chinese: ����M8266WIFI_SPI_Send_Data() ��һ��һ�����ķ������ݣ�ʵ�ʼ�������M8266WIFI_SPI_Send_BlockData()�ĺ���ʵ�ִ���
	{
         u16 tcp_packet_size = 1024;
         u16 loops     = 0;
		     u16 max_loops = 5000;
			   u32 len       = TEST_SEND_DATA_SIZE; 
		     
         for(sent=0, loops=0; (sent<len)&&(loops<=max_loops); loops++)
         {		
				    sent += M8266WIFI_SPI_Send_Data(snd_data+sent, ((len-sent)>tcp_packet_size)?tcp_packet_size:(len-sent), link_no, &status);
            if(sent>=len)  break;
			      if((status&0xFF) == 0x00)
			      {
							 loops = 0;
			      }
		        else
			      {
				      if(   ((status&0xFF) == 0x14)      // 0x14 = connection of link_no not present (Chinese: ���׽��ֲ�����)
                 || ((status&0xFF) == 0x15) )    // 0x15 = connection of link_no closed(Chinese: ���׽����Ѿ��رջ�Ͽ�)
	            {
								 M8266HostIf_delay_us(99);
          			 //need to re-establish the socket connection (Chinese: ��Ҫ�ؽ������׽���)
	            }
							else if( (status&0xFF) == 0x18 )        // 0x18 = TCP server in listening states and no tcp clients have connected. (Chinese: ���TCP��������û�пͻ�����������)
			        {
				         M8266HostIf_delay_us(100);
			        }
	            else
	            {
	               M8266HostIf_delay_us(250);
	            }
			     }
         } // end of for(...
			
			   total_sent += sent;
		   }
#endif
			 
	 } // end of for(batch=0; ; batch++)
 }
	
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif (TEST_M8266WIFI_TYPE==2)  // to test constant reception from remote // (Chinese: �����Ͳ��ԣ����ϵؽ�������Զ�˽ڵ㷢�͵�����)
{
#define  RECV_DATA_MAX_SIZE  2048      // If using large size of array, ensure system stack is large engough for the array variable�� Or stack over-bound leakage might bring about the mcu into "hardware fault exception" 

   u8  RecvData[RECV_DATA_MAX_SIZE];    // (Chinese: ���ʹ�ýϴ�����飬�ǵ�ȷ�����㹻���ϵͳ��ջ������������������. ���򣬵�Ƭ��������ܻ���Ϊ��ջ���Խ������롰hardware fault"ϵͳ�쳣) 
   u16 received = 0;                   
	 u32 total_received = 0;
   u32 MBytes = 0;

	
	 for(i=0; i<RECV_DATA_MAX_SIZE; i++) RecvData[i]=0xFF-i; 	
   M8266WIFI_SPI_Send_Data(RecvData, 1024, link_no, &status);  // just send to check the ip address on TCP tester. not necessary 
																															 // (Chinese: ��Զ�˽ڵ㷢��һ�������Ա���Զ�˲鿴ģ���IP��ַ�Ͷ˿ڣ��Ǳ���) 
	 while(1)
	 {
			if(M8266WIFI_SPI_Has_DataReceived())
			{
				//u16 M8266WIFI_SPI_RecvData(u8 data[], u16 max_len, uint16_t max_wait_in_ms, u8* link_no, u16* status);
	      received = M8266WIFI_SPI_RecvData(RecvData, RECV_DATA_MAX_SIZE, 5*1000, &link_no, &status);

			  if(  (status&0xFF)!= 0 )  
			  {
				    if( (status&0xFF)==0x22 )      // 0x22 = Module buffer has no data received
				    {  
			         M8266HostIf_delay_us(250); 
					   //M8266WIFI_Module_delay_ms(1);
				    }
				    else if( (status&0xFF)==0x23 )   
				    { // the packet had not been received completed in the last call of M8266WIFI_SPI_RecvData()
							// and has continued to be received in this call of M8266WIFI_SPI_RecvData()
							// do some work here if necessary
							// (Chinese: �ϴε��ý��պ���M8266WIFI_SPI_RecvData()ʱ����δ�������ϴ��Ǹ�����������ε��ü�������֮ǰ�İ���������������һЩ���������罫һ�ν��ջ����������󳤶����޼Ӵ�
				    }
				    else if(  (status&0xFF)==0x24)   
				    { 
	            // the packet is large in size than max_len specified and received only the max_len // TCP?????????
							// normally caused by the burst transmision by the routers after some block. 
							// Suggest to stop the TCP transmission for some time
					    // do some work here if necessary
							// (Chinese: ģ�������յ������ڱ���ȡ��������ĳ��ȣ������������max_len������ָ���ĳ��ȡ�ͨ������ΪԶ���󷢷��ͻ�·����������ʱ�����˴����ճ�����µ���ģ��İ�������
							//           ����Զ��ʵ�ʷ��͵ľ���һ���������䳤�ȳ�����������ָ������󳤶����ޡ������ǰ�ߵ�ԭ�򣬽�����ͣԶ��TCPͨ��һ��ʱ�䡣����Ǻ��ߣ�����Ӵ�max_len����ֵ����
							//           �����κδ�����������ʱ����Ƭ������߽��յ��ĳ����ᱻ��ɶ��С����Ҫ�����ٴ��ƽ⡣
							//           ��Ҫʱ��������Ӧ�Ĵ���
				    }
						else
						{
					    // do some work here if necessary
							// (Chinese: �����쳣����Ҫʱ������һЩ����
						}
			  }				
#define  TEST_RECV_UNIT (1024*1024)
				 total_received += received;
				 if( total_received >= (TEST_RECV_UNIT) )
				 {
					   LED_set(0, MBytes&0x01);	
					   total_received = total_received%(TEST_RECV_UNIT);
				     MBytes++;
         }
			} // end of if(M8266WIFI_SPI_Has_DataReceived())				 
	 } // end of while(1)
 }	 

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif (TEST_M8266WIFI_TYPE==3)  // Echo test: to receive data from remote and then echo back to remote (Chinese: �շ����ԣ�ģ�齫���յ����������̷��ظ����ͷ�)
{
#define  RECV_DATA_MAX_SIZE  2048      // If using large size of array, ensure system stack is large engough for the array variable�� Or stack over-bound leakage might bring about the mcu into "hardware fault exception" 

   u8  RecvData[RECV_DATA_MAX_SIZE];   // (Chinese: ���ʹ�ýϴ�����飬�ǵ�ȷ�����㹻���ϵͳ��ջ������������������. ���򣬵�Ƭ��������ܻ���Ϊ��ջ���Խ������롰hardware fault"ϵͳ�쳣) 
   u16 received = 0;
	 u16 sent;
	
	 for(i=0; i<RECV_DATA_MAX_SIZE; i++) RecvData[i]=i; 

   link_no = 0;
   sent = M8266WIFI_SPI_Send_Data(RecvData, 1024, link_no, &status);

   while(1)
	 {
			if(M8266WIFI_SPI_Has_DataReceived()) //if received data (Chinese: ������յ�����)
			{
				//Receive the data (Chinese: ��Ƭ����������)
				//u16 M8266WIFI_SPI_RecvData(u8 data[], u16 max_len, uint16_t max_wait_in_ms, u8* link_no, u16* status);
	      received = M8266WIFI_SPI_RecvData(RecvData, RECV_DATA_MAX_SIZE, 5*1000, &link_no, &status);
				
				if(received!=0) //if received data length is not 0  (Chinese: �����Ƭ����ȷ���յ������ݣ������Ȳ�����0)
				{
					u16 tcp_packet_size = 1024;
          u16 loops     = 0;
		      u16 max_loops = 5000;
			    u32 len       = received; 
		     
          for(sent=0, loops=0; (sent<len)&&(loops<=max_loops); loops++)
          {		
				    sent += M8266WIFI_SPI_Send_Data(RecvData+sent, ((len-sent)>tcp_packet_size)?tcp_packet_size:(len-sent), link_no, &status);
            if(sent>=len)  break;
			      if((status&0xFF) == 0x00)
			      {
							 loops = 0;
			      }
		        else
			      {
				      if(   ((status&0xFF) == 0x14)      // 0x14 = connection of link_no not present (Chinese: ���׽��ֲ�����)
                 || ((status&0xFF) == 0x15) )    // 0x15 = connection of link_no closed(Chinese: ���׽����Ѿ��رջ�Ͽ�)
	            {
								 M8266HostIf_delay_us(99);
          			 //need to re-establish the socket connection (Chinese: ��Ҫ�ؽ������׽���)
	            }
							else if( (status&0xFF) == 0x18 )        // 0x18 = TCP server in listening states and no tcp clients have connected. (Chinese: ���TCP��������û�пͻ�����������)
			        {
				         M8266HostIf_delay_us(100);
			        }
	            else
	            {
	               M8266HostIf_delay_us(250);
	            }
			      }
          } // end of for(...
		    } // end of if(received!=0)
			}
		} // end of while(1)
}	 
 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif (TEST_M8266WIFI_TYPE==4)  //           multi-clients transimission test: TCP Server or UDP receive data from multilple sender and echo back the data to the coresponding source
																// (Chinese: ��ͻ����շ����ԣ�ģ����ΪTCP��������UDP���������Զ��Զ�˽ڵ�����ݣ��������յ����������̷��ͻظ���Ӧ�ķ��ͷ�)
#if ( (TEST_CONNECTION_TYPE!=0) && (TEST_CONNECTION_TYPE!=2) )
#error multiple clients test could be supported only when socket connection is UDP or TCP Server
#endif
{	
#define  RECV_DATA_MAX_SIZE  2048      // If using large size of array, ensure system stack is large engough for the array variable�� Or stack over-bound leakage might bring about the mcu into "hardware fault exception" 

   u8  RecvData[RECV_DATA_MAX_SIZE];   // (Chinese: ���ʹ�ýϴ�����飬�ǵ�ȷ�����㹻���ϵͳ��ջ������������������. ���򣬵�Ƭ��������ܻ���Ϊ��ջ���Խ������롰hardware fault"ϵͳ�쳣) 
   u16 received = 0;
	 u16 sent;
	 u8  remote_ip[4];
	 u16 remote_port;
	
	 for(i=0; i<RECV_DATA_MAX_SIZE; i++) RecvData[i]=i; 

   link_no = 0;

   while(1)
	 {
			if(M8266WIFI_SPI_Has_DataReceived()) //if received data (Chinese: ������յ�����)
			{
				//Receive the data using M8266WIFI_SPI_RecvData_ex() which could return the address and port of the sender
				//(Chinese: ��Ƭ��ʹ�ú���M8266WIFI_SPI_RecvData_ex()���������ݣ�����������Է��ط��ͷ��ĵ�ַ�Ͷ˿�)
		
        //u16 M8266WIFI_SPI_RecvData_ex(u8 Data[], u16 max_len, uint16_t max_wait_in_ms, u8* link_no, u8 remote_ip[4], u16* remote_port, u16* status)
				received = M8266WIFI_SPI_RecvData_ex(RecvData, RECV_DATA_MAX_SIZE, 5*1000, &link_no, remote_ip, &remote_port, &status);

				if(received!=0) //if received data length is not 0  (Chinese: �����Ƭ����ȷ���յ������ݣ������Ȳ�����0)
				{

					//format the address in hex to ascii(Chinese: ��16���Ƹ�ʽ��ip��ַת���ַ�����ʽ��ip��ַ����Ϊ����ĺ������ݵ����ַ������Ƶĵ�ַ����)
				  char dest_addr[15+1]={0};
				  sprintf(dest_addr, "%d.%d.%d.%d", remote_ip[0], remote_ip[1], remote_ip[2], remote_ip[3]);

					u16 tcp_packet_size = 1024;
          u16 loops     = 0;
		      u16 max_loops = 5000;
			    u32 len       = received; 
		     
          for(sent=0, loops=0; (sent<len)&&(loops<=max_loops); loops++)
          {		
#if ( TEST_CONNECTION_TYPE==0 )  //if UDP, (Chinese: UDPʵ�������Եȵ�UDP����������
						//If UDP, Send the data using M8266WIFI_SPI_Send_Udp_Data() which could send the data to the specified UDP peer
						//(Chinese: �����UDP����Ƭ��ʹ�ú���M8266WIFI_SPI_Send_Udp_Data()���������ݣ�����������Դ���Ŀ���ַ�Ͷ˿ڣ�Ҳ���ǿ���ָ�����͵��ĸ��Եȵ�UDP����ڵ�)
						  //u16 M8266WIFI_SPI_Send_Udp_Data(u8 Data[], u16 Data_len, u8 link_no, char* udp_dest_addr, u16 udp_dest_port, u16* status)
				    sent += M8266WIFI_SPI_Send_Udp_Data(RecvData+sent, ((len-sent)>tcp_packet_size)?tcp_packet_size:(len-sent), link_no, dest_addr, remote_port, &status);
            if(sent>=len)  break;
			      if((status&0xFF) == 0x00)
			      {
							 loops = 0;
			      }
		        else
			      {
				      if((status&0xFF) == 0x14)      // 0x14 = connection of link_no not present (Chinese: ���׽��ֲ�����)
	            {
								 M8266HostIf_delay_us(99);
          			 //need to re-establish the socket connection (Chinese: ��Ҫ�ؽ������׽���)
	            }
	            else
	            {
	               M8266HostIf_delay_us(250);
	            }
			      }            
#elif ( TEST_CONNECTION_TYPE==2 )
						//If tcp server, Send the data using M8266WIFI_SPI_Send_Data_to_TcpClient() which could send the data to the specified client
						//(Chinese: �����TCP����������Ƭ��ʹ�ú���M8266WIFI_SPI_Send_Data_to_TcpClient()���������ݣ�����������Դ���Ŀ���ַ�Ͷ˿ڣ�Ҳ���ǿ���ָ�����͵��ĸ��ͻ���)
						  //u16 M8266WIFI_SPI_Send_Data_to_TcpClient(u8 Data[], u16 Data_len, u8 server_link_no, char* tcp_client_dest_addr, u16 tcp_client_dest_port, u16* status)
				    sent += M8266WIFI_SPI_Send_Data_to_TcpClient(RecvData+sent, ((len-sent)>tcp_packet_size)?tcp_packet_size:(len-sent), link_no, dest_addr, remote_port, &status);
            if(sent>=len)  break;
			      if((status&0xFF) == 0x00)
			      {
							 loops = 0;
			      }
		        else
			      {
				      if(   ((status&0xFF) == 0x14)      // 0x14 = connection of link_no not present (Chinese: ���׽��ֲ�����)
                 || ((status&0xFF) == 0x15) )    // 0x15 = connection of link_no closed(Chinese: ���׽����Ѿ��رջ�Ͽ�)
	            {
								 M8266HostIf_delay_us(99);
          			 //need to re-establish the socket connection (Chinese: ��Ҫ�ؽ������׽���)
	            }
							else if( (status&0xFF) == 0x18 )        // 0x18 = TCP server in listening states and no tcp clients have connected. (Chinese: ���TCP��������û�пͻ�����������)
			        {
				         M8266HostIf_delay_us(100);
			        }
	            else
	            {
	               M8266HostIf_delay_us(250);
	            }
			      }
#endif						
          } // end of for(...
		    } // end of if(received!=0)
			}
		} // end of while(1)
}	 
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
#else
#error NOT Supported Test Type! should be 1~4!		
#endif

} // end of M8266WIFI_Test
