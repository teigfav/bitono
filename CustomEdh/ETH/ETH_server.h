/*
 * ETH_server.h
 *
 *  Created on: Oct 12, 2023
 *      Author: giuli
 */

#ifndef INC_ETH_SERVER_H_
#define INC_ETH_SERVER_H_

void tcpserver_init(void);

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* DHCP process states */


/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#if LWIP_DHCP
void DHCP_Thread(void* argument);
#endif

#endif /* INC_ETH_SERVER_H_ */
