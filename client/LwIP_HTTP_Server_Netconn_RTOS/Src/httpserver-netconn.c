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

#define WEBSERVER_THREAD_PRIO    osPriorityAboveNormal

char text[] = "switch";

static void http_server_netconn_thread(void *arg)
{
	struct netconn *conn;
	struct netbuf *inbuf;
	err_t err, recv_err;
	ip_addr_t ipaddr;
	char* buf;
	u16_t buflen;

	IP_ADDR4(&ipaddr,192,168,1,70);
	/* Create a new TCP connection handle */
	conn = netconn_new(NETCONN_TCP);

	if (conn!= NULL)
	{
		err = netconn_connect(conn, &ipaddr, 8080);

		if (err == ERR_OK)
		{
			while(1)
			{
				netconn_write_partly (conn, text, sizeof(text), NETCONN_COPY, NULL);

				recv_err = netconn_recv(conn, &inbuf);
				if (recv_err == ERR_OK)
				{
					netbuf_data(inbuf, (void**)&buf, &buflen);
					LCD_UsrLog ("Response from server: %s\n", buf);
			    }
				netbuf_delete(inbuf);
				HAL_Delay(5000);
			  }
		  }
	  }
	  netconn_close(conn);
}

void http_server_netconn_init()
{
  sys_thread_new("HTTP", http_server_netconn_thread, NULL, DEFAULT_THREAD_STACKSIZE, WEBSERVER_THREAD_PRIO);
}
