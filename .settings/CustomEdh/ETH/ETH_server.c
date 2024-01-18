/*
 * ETH_server.c
 *
 *  Created on: Oct 12, 2023
 *      Author: giuli
 */
#include "lwipopts.h"
#include "cmsis_os.h"
#include "lwip/opt.h"
#include "stdio.h"
#include "lwip/api.h"
#include "lwip/sys.h"
#include "string.h"
#include "embedded_cli.h"
static void tcp_thread(void *arg);

struct netconn *conn, *newconn;
struct netbuf *buf;
static ip_addr_t *addr;
static unsigned short port;
char msg[100];
char smsg[200];

extern EmbeddedCli *cli;
osThreadId_t tcp_thread_TaskHandle;

void tcpserver_init(void)
{
	const osThreadAttr_t tcp_thread_attributes = {
	  .name = "tcp_thread",
	  .stack_size = DEFAULT_THREAD_STACKSIZE,
	  .priority = (osPriority_t) osPriorityNormal,
	};
	tcp_thread_TaskHandle=osThreadNew(tcp_thread, NULL, &tcp_thread_attributes);
}

/**** Send RESPONSE every time the client sends some data ******/
static void tcp_thread(void *arg)
{
	err_t err, accept_err, recv_error;
	/* Create a new connection identifier. */
	conn = netconn_new(NETCONN_TCP);
	if (conn!=NULL)
	{
		/* Bind connection to the port number 7. */
		err = netconn_bind(conn, IP_ADDR_ANY, 5000); /*Next we will bind the Connection to the Local IP (configured in cube) and local port (7 in this case). These will act as the IP and Port of the server*/
		if (err == ERR_OK)
		{
			/* Tell connection to go into listening mode. */
			netconn_listen(conn);
			while(1)
			{
				/* Grab new connection. */
				accept_err = netconn_accept(conn, &newconn);
				/*Note that while accepting the connection, we save the connection parameter in a new netconn identifier, i.e. newconn*/
				/* Process the new connection. */
				if (accept_err == ERR_OK)
				{
					/*At this point, the Client is connected to the Server.
					  The server can only respond to the requests sent by the client,
					  so it will wait for some data to be received by the client.
					  netconn_recv function will wait for the client to send some data to the server*/
					while (netconn_recv(newconn, &buf) == ERR_OK)
					{
						/* Extrct the address and port in case they are required */
						addr = netbuf_fromaddr(buf);  // get the address of the client
						port = netbuf_fromport(buf);  // get the Port of the client

						do
						{
							strncpy(msg,buf->p->payload,buf->p->len);
							int len=sprintf(smsg,"%s\n",msg);
							//int len=sprintf(smsg,"\"%s\" was sent by the Server\n",msg);
							char *psmsg=&smsg[0];
							for(int i=0;i<len;i++)
							{
								embeddedCliReceiveChar(cli, *psmsg);
								psmsg++;
								osDelay(1); //serve altrimenti il task della CLI non gli sta dietro
							}
							//netconn_write(newconn, smsg, len, NETCONN_COPY);
							memset(msg,'\0' ,100);
						}
						while(netbuf_next(buf)>0);
						netbuf_delete(buf);
					}
					netconn_close(newconn);
					netconn_delete(newconn);
				}

			}
		}
		else
		{
			netconn_delete(conn);
		}
	}
}
