/* 
*   BSD 2-Clause License
*   Copyright (c) 2022, LiteEMF
*   All rights reserved.
*   This software component is licensed by LiteEMF under BSD 2-Clause license,
*   the "License"; You may not use this file except in compliance with the
*   License. You may obtain a copy of the License at:
*       opensource.org/licenses/BSD-2-Clause
* 
*/

/************************************************************************************************************
**	Description:	
************************************************************************************************************/
#include "hw_config.h"
#if API_USBD_BIT_ENABLE
#include  "api/usb/usb_typedef.h"
#include "api/usb/device/usbd.h"


#define __LINUX_USB_CH9_H
#include "irq.h"
#include "gpio.h"
#include "jiffies.h"
#include "usb/usb_phy.h"

#include "api/api_log.h"

/******************************************************************************************************
** Defined
*******************************************************************************************************/

/******************************************************************************************************
**	public Parameters
*******************************************************************************************************/


/******************************************************************************************************
**	static Parameters
*******************************************************************************************************/
mem_buf_t usbd_mem_buf;
uint8_t usbd_ep_buf[0x200] __attribute__((aligned(4))) SEC(.usb_h_dma); 
static uint8_t ep0_dma_buffer[USBD_ENDP0_MTU + 4] __attribute__((aligned(4))) SEC(.usb_h_dma);
static uint8_t* m_ep_pbuffer[USBD_ENDP_NUM][2];

volatile uint8_t setup_phase = 1;

/*****************************************************************************************************
**	static Function
******************************************************************************************************/

void usb_isr(const usb_dev id)
{
    u32 intr_usb, intr_usbe;
    u32 intr_tx, intr_txe;
    u32 intr_rx, intr_rxe;

    __asm__ volatile("ssync");
    usb_read_intr(id, &intr_usb, &intr_tx, &intr_rx);
    usb_read_intre(id, &intr_usbe, &intr_txe, &intr_rxe);

    intr_usb &= intr_usbe;
    intr_tx &= intr_txe;
    intr_rx &= intr_rxe;

    logd("t=%x %x r=%x %x\n",intr_tx,intr_txe,intr_rx,intr_rxe);

    if (intr_usb & INTRUSB_SUSPEND) {
        logd("usb suspend\n");
        usb_sie_close(id);
        usbd_suspend_event(id);
    }
    if (intr_usb & INTRUSB_RESET_BABBLE) {
        logd("usb reset\n");
        usbd_reset_event(id);
    }
    if (intr_usb & INTRUSB_RESUME) {
        logd("usb resume\n");
        usbd_resume_event(id);
    }

    if (intr_tx & BIT(0)) {             // endp0 in out setup event 
        uint32_t reg = usb_read_csr0(id);
        logd("reg=%x %d\n",reg,setup_phase);

        if (reg & CSR0P_SentStall) {
            usb_write_csr0(id, 0);
            logd("CSR0P_SentStall\n");
            return;
        }
        if (reg & CSR0P_SetupEnd) {
            usb_write_csr0(id, CSR0P_ClrSetupEnd);
            logd("CSR0P_SetupEnd\n");
            setup_phase = 1;
        }

        if (reg & CSR0P_RxPktRdy) {     //setup out 
            if(setup_phase){ 
                usb_control_request_t req;
                usb_read_ep0(id, (u8*)&req, sizeof(usb_control_request_t));
                logd("req:");dumpd(&req, 8);

                usb_ep0_ClrRxPktRdy(id);
                usbd_setup_event(id, &req , sizeof(usb_control_request_t));
                
                if((USB_DIR_OUT == req.bmRequestType.bits.direction) && req.wLength){ //设置ack 继续接收OUT数据
                    setup_phase = 0;
                }
            }else{                      //out
                usbd_endp_out_event(id, 0, usb_read_rxcount(id, 0));
            }
        }else{                          //in
            if (reg & CSR0P_TxPktRdy) {
                loge("ep0 TXCSRP_TxPktRdy busy\n");
                return;
            } else {
                usbd_endp_in_event(id, 0x80);
            }
        }
    }else{
        for (int i = 1; i < USBD_ENDP_NUM; i++) {
            if (intr_tx & BIT(i)) {
                usbd_endp_in_event(id, USB_DIR_IN_MASK | i);
            }
        }

        for (int i = 1; i < USBD_ENDP_NUM; i++) {
            if (intr_rx & BIT(i)) {
                usbd_endp_out_event(id, i, usb_read_rxcount(id, i));
            }
        }
    }
    __asm__ volatile("csync");
}



SET_INTERRUPT
void usb0_g_isr()
{
    usb_isr(0);
}

SET_INTERRUPT
void usb0_sof_isr()
{
    const usb_dev id = 0;
    usb_sof_clr_pnd(id);
    static u32 sof_count = 0;
    if ((sof_count++ % 1000) == 0) {
        logd("sof 1s isr frame:%d", usb_read_sofframe(id));
    }
}


void usb_g_isr_reg(const usb_dev id, u8 priority, u8 cpu_id)
{
    request_irq(IRQ_USB_CTRL_IDX, priority, usb0_g_isr, cpu_id);
}
void usb_sof_isr_reg(const usb_dev id, u8 priority, u8 cpu_id)
{
    request_irq(IRQ_USB_SOF_IDX, priority, usb0_sof_isr, cpu_id);
}



/*****************************************************************************************************
**  Function
******************************************************************************************************/

/*******************************************************************
** Parameters:		
** Returns:	
** Description:		
*******************************************************************/
__attribute__((always_inline_when_const_args))
uint8_t *hal_usbd_get_endp_buffer(uint8_t id, uint8_t ep)
{
    uint8_t *ep_buffer = NULL;
    uint8_t ep_addr = ep & 0x0f;

    if(0 == ep_addr){
        ep_buffer = ep0_dma_buffer;
    }else{
        if(USB_DIR_IN_MASK & ep){
            ep_buffer = m_ep_pbuffer[ep_addr][1];
        }else{
            ep_buffer = m_ep_pbuffer[ep_addr][0];
        }
    }
    return ep_buffer;
}

error_t hal_usbd_endp_dma_init(uint8_t id)
{
    memset(m_ep_pbuffer, 0, sizeof(m_ep_pbuffer));
    m_ep_pbuffer[0][0] = ep0_dma_buffer;
    m_ep_pbuffer[0][1] = ep0_dma_buffer;
    mem_buf_init(&usbd_mem_buf, usbd_ep_buf, sizeof(usbd_ep_buf), 8);

	return ERROR_SUCCESS;
}

error_t hal_usbd_endp_open(uint8_t id, usb_endp_t *pendp)
{
    uint8_t ep_addr = pendp->addr;
    if(0 == ep_addr) return ERROR_FAILE;

    if(USB_DIR_IN == pendp->dir){
		switch(pendp->type){
        case USB_ENDP_TYPE_ISOCH:
            m_ep_pbuffer[ep_addr][1] = mem_buf_alloc(&usbd_mem_buf, pendp->mtu);
            usb_g_ep_config(id, ep_addr | USB_DIR_IN, USB_ENDP_TYPE_ISOCH, 1, m_ep_pbuffer[ep_addr][1], pendp->mtu);
            usb_enable_ep(id, ep_addr);
            break;
        case USB_ENDP_TYPE_BULK:
            m_ep_pbuffer[ep_addr][1] = mem_buf_alloc(&usbd_mem_buf, pendp->mtu);
            usb_g_ep_config(id, ep_addr | USB_DIR_IN, USB_ENDP_TYPE_BULK, 0, m_ep_pbuffer[ep_addr][1], pendp->mtu);
            usb_enable_ep(id, ep_addr);
            break;
        case USB_ENDP_TYPE_INTER:
            m_ep_pbuffer[ep_addr][1] = mem_buf_alloc(&usbd_mem_buf, pendp->mtu);
            usb_g_ep_config(id, ep_addr | USB_DIR_IN, USB_ENDP_TYPE_INTER, 0, m_ep_pbuffer[ep_addr][1], pendp->mtu);
            usb_enable_ep(id, ep_addr);
            break;
		}
	}else{     //out
		switch(pendp->type){
        case USB_ENDP_TYPE_ISOCH:
            m_ep_pbuffer[ep_addr][0] = mem_buf_alloc(&usbd_mem_buf, pendp->mtu);
            usb_g_ep_config(id, ep_addr , USB_ENDP_TYPE_ISOCH, 1, m_ep_pbuffer[ep_addr][0], pendp->mtu);
            usb_enable_ep(id, ep_addr);
            break;
        case USB_ENDP_TYPE_BULK: //BULK out 如果和BULK in 不同时使用(比如U盘协议),可以优化共用BULK in buf
            m_ep_pbuffer[ep_addr][0] = mem_buf_alloc(&usbd_mem_buf, pendp->mtu);
            usb_g_ep_config(id, ep_addr, USB_ENDP_TYPE_BULK, 1, m_ep_pbuffer[ep_addr][0], pendp->mtu);
            usb_enable_ep(id, ep_addr);
            break;
        case USB_ENDP_TYPE_INTER:
            m_ep_pbuffer[ep_addr][0] = mem_buf_alloc(&usbd_mem_buf, pendp->mtu);
            usb_g_ep_config(id, ep_addr, USB_ENDP_TYPE_INTER, 1, m_ep_pbuffer[ep_addr][0], pendp->mtu);
            usb_enable_ep(id, ep_addr);
            break;
		}
	}

	return ERROR_SUCCESS;
}
error_t hal_usbd_endp_close(uint8_t id, uint8_t ep)
{
    usb_disable_ep(id, ep);
	return ERROR_SUCCESS;
}
error_t hal_usbd_endp_ack(uint8_t id, uint8_t ep, uint16_t len)
{
    uint32_t reg = usb_read_csr0(id);
    logd("ack reg=%x\n",reg);

	switch (ep) {
    case 0x80:
        if(len < USBD_ENDP0_MTU){
            usb_ep0_TxPktEnd(id);                       //结束TX开始RX
        }else{
            usb_write_csr0(id, CSR0P_TxPktRdy);
        }
        reg = usb_read_csr0(id);    logd("ack reg=%x\n",reg);
        break;
    case 0x00:

        usb_ep0_RxPktEnd(id);                       //结束RX开始TX
     reg = usb_read_csr0(id);    logd("ack reg=%x\n",reg);
        usb_ep0_ClrRxPktRdy(id);
     reg = usb_read_csr0(id);    logd("ack reg=%x\n",reg);
        break;
    default:
        break;
    }
	return ERROR_SUCCESS;

	usb_write_csr0(id, CSR0P_TxPktRdy);
}
error_t hal_usbd_endp_nak(uint8_t id, uint8_t ep)
{
	return ERROR_SUCCESS;
}

error_t hal_usbd_clear_endp_stall(uint8_t id, uint8_t ep)
{
    if(0 == (ep & 0x0f)){
        usb_write_csr0(id, 0);
    }else if( USB_DIR_IN_MASK & ep){
        usb_write_txcsr(id, ep, TXCSRP_ClrDataTog);
    }else{
        usb_write_rxcsr(id, ep, RXCSRP_ClrDataTog);
    }
	return ERROR_SUCCESS;
}
error_t hal_usbd_endp_stall(uint8_t id, uint8_t ep)
{
	if(0 == (ep & 0x0f)) {
		usb_ep0_Set_Stall(id);
	}else{
	    if (ep & 0x80){
			usb_write_txcsr(id, ep&0x7f, TXCSRP_SendStall);
		}else{
			usb_write_rxcsr(id, ep, RXCSRP_SendStall);
		}
    }

	return ERROR_SUCCESS;
}


	


/*******************************************************************
** Parameters:
** Returns:
** Description: 注意: 端点0 发送需要处理usbd_req_t
*******************************************************************/
error_t hal_usbd_in(uint8_t id, uint8_t ep, uint8_t* buf,uint16_t len)
{
	error_t err = ERROR_FAILE;
    uint8_t ep_addr = ep & 0x7f;
    uint16_t send_len;
    uint8_t* endp_buf = hal_usbd_get_endp_buffer(id, USB_DIR_IN_MASK | ep);

    if(0 == ep_addr){
        usbd_req_t *preq = usbd_get_req(id);

        if(preq->setup_index <= preq->setup_len){
            send_len = preq->setup_len - preq->setup_index;
            send_len = (send_len >= USBD_ENDP0_MTU) ? USBD_ENDP0_MTU : send_len; //本次传输长度
			usb_write_ep0(id, (void*)(preq->setup_buf+preq->setup_index), send_len);
        logd("ep%x,in%d:",ep,send_len);dumpd((void*)(preq->setup_buf+preq->setup_index), send_len);
            preq->setup_index += send_len;

            err = hal_usbd_endp_ack(id, ep, send_len);
            if(preq->setup_index == preq->setup_len){
                if(USBD_ENDP0_MTU != send_len){             //判断发送最后一包数据
                    usbd_free_setup_buffer(preq);           //发送完成释放内存
                    //hal_usbd_endp_ack(id, 0x00, 0);         
                }
            }
        }
    }else{
        usbd_class_t *pclass = usbd_class_find_by_ep(id, ep);
		if(NULL != pclass){
			uint16_t tx_len;
			if(USB_ENDP_TYPE_ISOCH == pclass->endpin.type ){
				tx_len = usb_g_iso_write(id, ep_addr, buf, len);
			}else if(USB_ENDP_TYPE_BULK == pclass->endpin.type ){
				tx_len = usb_g_bulk_write(id, ep_addr, buf, len);
			}else{
				tx_len = usb_g_intr_write(id, ep_addr, buf, len);	// 无法忽略当前发送,如果一个间隔发送多个数据,会导致当前任务延时卡住,必须保证interval 大于发送间隔
			}
			if(tx_len == len) err = ERROR_SUCCESS;
		}
    }

    return err;

}
error_t hal_usbd_out(uint8_t id, uint8_t ep, uint8_t* buf, uint16_t* plen)
{
	uint16_t rx_len = 0;
	ep &= 0x7f;

	if(0 == ep){
    	usb_read_ep0(id, buf, *plen);
        hal_usbd_endp_ack(id, 0, 0);
	}else{
		usbd_class_t *pclass = usbd_class_find_by_ep(id, ep);
		if(NULL != pclass){
			if(USB_ENDP_TYPE_ISOCH == pclass->endpout.type ){
				*plen = usb_g_iso_read(id, ep, buf, *plen, 0);
			}else if(USB_ENDP_TYPE_BULK == pclass->endpout.type ){
				*plen = usb_g_bulk_read(id, ep, buf, *plen, 0);
			}else{
				*plen = usb_g_intr_read(id, ep, buf, *plen, 0);
			}
		}
	}

	return ERROR_SUCCESS;
}
error_t hal_usbd_reset(uint8_t id)
{
	hal_usbd_deinit(id);

	os_time_dly(1);

	hal_usbd_init(id);

	return ERROR_SUCCESS;
}
error_t hal_usbd_set_address(uint8_t id,uint8_t address)
{
	usb_write_faddr(id, address);
	return ERROR_SUCCESS;
}

static struct usb_ep_addr_t usb_ep_addr  SEC(.usb_config_var);

error_t hal_usbd_init(uint8_t id)
{
    memset(&usb_ep_addr, 0, sizeof(usb_ep_addr));
    memset(&usbd_mem_buf, 0, sizeof(usbd_mem_buf));
    memset(m_ep_pbuffer, 0, sizeof(m_ep_pbuffer));
    m_ep_pbuffer[0][0] = ep0_dma_buffer;
    m_ep_pbuffer[0][1] = ep0_dma_buffer;

    usb_var_init(id, &usb_ep_addr);

    usb_g_sie_init(id);
    usb_slave_init(id);
    usb_set_dma_raddr(id, 0, ep0_dma_buffer);

    usb_write_intr_usbe(id, INTRUSB_RESET_BABBLE | INTRUSB_SUSPEND);
    usb_clr_intr_txe(id, -1);
    usb_clr_intr_rxe(id, -1);
    usb_set_intr_txe(id, 0);
    usb_set_intr_rxe(id, 0);
    usb_g_isr_reg(id, IRQ_USB_IP, 0);
    /* usb_sof_isr_reg(id,3,0); */
    /* usb_sofie_enable(id); */

	return ERROR_SUCCESS;
}
error_t hal_usbd_deinit(uint8_t id)
{
	usb_sie_close(id);

	gpio_set_direction(IO_PORT_DM + 2 * id, 1);
	gpio_set_pull_up(IO_PORT_DM + 2 * id, 0);
	gpio_set_pull_down(IO_PORT_DM + 2 * id, 0);
	gpio_set_die(IO_PORT_DM + 2 * id, 0);

	gpio_set_direction(IO_PORT_DP + 2 * id, 1);
	gpio_set_pull_up(IO_PORT_DP + 2 * id, 0);
	gpio_set_pull_down(IO_PORT_DP + 2 * id, 0);
	gpio_set_die(IO_PORT_DP + 2 * id, 0);

	os_time_dly(20);

	gpio_set_die(IO_PORT_DM + 2 * id, 1);
	gpio_set_die(IO_PORT_DP + 2 * id, 1);
	usb_g_hold(id);

	return ERROR_SUCCESS;
}

#endif



