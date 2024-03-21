/*
 * ETH_DHCP.h
 *
 *  Created on: 18 mar 2024
 *      Author: giuli
 */

#ifndef ETH_ETH_DHCP_H_
#define ETH_ETH_DHCP_H_


#define DHCP_OFF                   (uint8_t) 0
#define DHCP_START                 (uint8_t) 1
#define DHCP_WAIT_ADDRESS          (uint8_t) 2
#define DHCP_ADDRESS_ASSIGNED      (uint8_t) 3
#define DHCP_TIMEOUT               (uint8_t) 4
#define DHCP_LINK_DOWN             (uint8_t) 5

/*Static IP ADDRESS*/
#define IP_ADDR0   192
#define IP_ADDR1   168
#define IP_ADDR2   0
#define IP_ADDR3   10

/*NETMASK*/
#define NETMASK_ADDR0   255
#define NETMASK_ADDR1   255
#define NETMASK_ADDR2   255
#define NETMASK_ADDR3   0

/*Gateway Address*/
#define GW_ADDR0   192
#define GW_ADDR1   168
#define GW_ADDR2   0
#define GW_ADDR3   1

void DHCP_init(void);
#if LWIP_DHCP
void DHCP_Thread(void* argument);
#endif

#endif /* ETH_ETH_DHCP_H_ */
