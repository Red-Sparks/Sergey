/**
  ******************************************************************************
  * @file    LwIP/LwIP_HTTP_Server_Netconn_RTOS/Src/httpser-netconn.c 
  * @author  MCD Application Team
  * @brief   Basic http server implementation using LwIP netconn API  
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "lwip/opt.h"
#include "lwip/arch.h"
#include "lwip/api.h"
#include "lwip/apps/fs.h"
#include "string.h"
#include <stdio.h>
#include "httpserver-netconn.h"
#include "cmsis_os.h"
#include "stm32746g_discovery.h"
#include "lcd_log.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define WEBSERVER_THREAD_PRIO    osPriorityAboveNormal

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
int led_status = 0;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief serve tcp connection  
  * @param conn: pointer on connection structure 
  * @retval None
  */
static void http_server_serve(struct netconn *conn) 
{
  struct netbuf *inbuf;
  err_t recv_err;
  char* buf;
  u16_t buflen;
  char resp1[] = "Led is on";
  char resp2[] = "Led is off";
  size_t bytes_written = 0;
  static ip_addr_t ipaddr;
  static u16_t port;
  uint8_t lastIP;

  netconn_getaddr(conn, &ipaddr, &port, 0);
  lastIP = (uint8_t)(ipaddr.addr >> 24);
  LCD_UsrLog ("Connected with %d\n", lastIP);
  if (lastIP == 71) {
	  while (1) {
		  recv_err = netconn_recv(conn, &inbuf);

		  if (recv_err == ERR_OK) {

			  if (netconn_err(conn) == ERR_OK) {

				  netbuf_data(inbuf, (void**)&buf, &buflen);

				  if ((buflen >= 6) && (strncmp(buf, "switch", 6) == 0)) {
					  if (led_status == 0) {
						  led_status = 1;
						  BSP_LED_On(LED1);
						  LCD_UsrLog ("Led is on.\n");
						  netconn_write_partly (conn, resp1, sizeof(resp1), NETCONN_COPY, &bytes_written);
					  } else {
						  led_status = 0;
						  BSP_LED_Off(LED1);
						  LCD_UsrLog ("Led is off.\n");
						  netconn_write_partly (conn, resp2, sizeof(resp2), NETCONN_COPY, &bytes_written);
					  }
				  }
		      }
		  }
		  netbuf_delete(inbuf);
	  }
  }
//  if (recv_err == ERR_OK)
//  {
//    if (netconn_err(conn) == ERR_OK)
//    {
//      netbuf_data(inbuf, (void**)&buf, &buflen);
//
//      if ((buflen >= 6) && (strncmp(buf, "switch", 6) == 0)) {
//    	  if (led_status == 0) {
//    		  led_status = 1;
//    		  BSP_LED_On(LED1);
//    		  LCD_UsrLog ("Led is on.\n");
//    		  netconn_write_partly (conn, resp1, sizeof(resp1), NETCONN_COPY, &bytes_written);
//    	  } else {
//    		  led_status = 0;
//    		  BSP_LED_Off(LED1);
//    		  LCD_UsrLog ("Led is off.\n");
//    		  netconn_write_partly (conn, resp2, sizeof(resp2), NETCONN_COPY, &bytes_written);
//    	  }
//      }
//    }
//  }
  netconn_close(conn);
//
//  netbuf_delete(inbuf);
}


/**
  * @brief  http server thread 
  * @param arg: pointer on argument(not used here) 
  * @retval None
  */
static void http_server_netconn_thread(void *arg)
{ 
  struct netconn *conn, *newconn;
  err_t err, accept_err;
  
  /* Create a new TCP connection handle */
  conn = netconn_new(NETCONN_TCP);
  
  if (conn!= NULL)
  {
    /* Bind to port 8080 (HTTP) with default IP address */
    err = netconn_bind(conn, NULL, 8080);
    
    if (err == ERR_OK)
    {
      /* Put the connection into LISTEN state */
      netconn_listen(conn);
  
      while(1) 
      {
        /* accept any incoming connection */
        accept_err = netconn_accept(conn, &newconn);
        if(accept_err == ERR_OK)
        {
          /* serve connection */
          http_server_serve(newconn);

          /* delete connection */
          netconn_delete(newconn);
        }
      }
    }
  }
}

/**
  * @brief  Initialize the HTTP server (start its thread) 
  * @param  none
  * @retval None
  */
void http_server_netconn_init()
{
  sys_thread_new("HTTP", http_server_netconn_thread, NULL, DEFAULT_THREAD_STACKSIZE, WEBSERVER_THREAD_PRIO);
}
