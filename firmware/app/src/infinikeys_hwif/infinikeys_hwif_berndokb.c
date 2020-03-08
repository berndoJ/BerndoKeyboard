// infinikeys_hwif_berndokb.c
// Copyright (c) 2020 Johannes Berndorfer (berndoJ)

#include "infinikeys_hwif/infinikeys_hwif_berndokb.h"

#include "sys_gpio.h"
#include "sys_usart.h"
#include "sys_pca9555.h"

#include "sys_usb.h"
#include "infinikeys_stm32cube_usb_hid.h"

#include <stdlib.h>
#include <stdio.h>

static IK_Status_t _ikhwif_berndokb_usb_init(void);
static void        _ikhwif_berndokb_usb_deinit(void);
static IK_Status_t _ikhwif_berndokb_usb_transmitusb(IK_Buffer_t buf, uint8_t ep_addr);
static IK_Status_t _ikhwif_berndokb_debug_printmsg(const char *msg, uint32_t len);
static IK_Status_t _ikhwif_berndokb_cfgmem_init(void);
static IK_Status_t _ikhwif_berndokb_cfgmem_deinit(void);
static IK_Status_t _ikhwif_berndokb_cfgmem_wbyte(uint32_t address, uint8_t data);
static IK_Status_t _ikhwif_berndokb_cfgmem_wbuf(uint32_t address, IK_Buffer_t data_buf);
static uint8_t     _ikhwif_berndokb_cfgmem_rbyte(uint32_t address);
static IK_Buffer_t _ikhwif_berndokb_cfgmem_rbuf(uint32_t address, uint32_t size);
static IK_Status_t _ikhwif_berndokb_kbm_pollmatrix(void);

/*--------------------------------------------------------------------------------------------------------------------*/

IK_USB_HW_Interface_t infinikeys_hwif_berndokb_usb =
{
    .Init = _ikhwif_berndokb_usb_init,
    .DeInit = _ikhwif_berndokb_usb_deinit,
    .TransmitUSB = _ikhwif_berndokb_usb_transmitusb
};

IK_DEBUG_HW_Interface_t infinikeys_hwif_berndokb_debug =
{
    .PrintMessage = _ikhwif_berndokb_debug_printmsg
};

IK_CFG_Memory_HW_Interface_t infinikeys_hwif_berndokb_cfgmem =
{
    .Init = _ikhwif_berndokb_cfgmem_init,
    .DeInit = _ikhwif_berndokb_cfgmem_deinit,
    .WriteByte = _ikhwif_berndokb_cfgmem_wbyte,
    .WriteBuffer = _ikhwif_berndokb_cfgmem_wbuf,
    .ReadByte = _ikhwif_berndokb_cfgmem_rbyte,
    .ReadBuffer = _ikhwif_berndokb_cfgmem_rbuf,
    .MemorySize = 0U
};

IK_KBMatrix_HW_Interface_t infinikeys_hwif_berndokb_kbmatrix =
{
    .PollMatrix = _ikhwif_berndokb_kbm_pollmatrix
};

/*--------------------------------------------------------------------------------------------------------------------*/

static IK_Status_t _ikhwif_berndokb_usb_init(void)
{
    return IK_OK;
}

static void _ikhwif_berndokb_usb_deinit(void)
{

}

static IK_Status_t _ikhwif_berndokb_usb_transmitusb(IK_Buffer_t buf, uint8_t ep_addr)
{
    if (ep_addr == 1)
    {
        IK_STM32CUBE_USBD_HID_SendReport(hw_usbd_handle, buf.DataPtr, buf.Size);

        #if (0)
        uint32_t i, c = 0;
        char *vbuf, *cbuf;

        vbuf = malloc(3 * 32 + 1);
        cbuf = malloc(4);

        IK_DEBUG_PrintMessage("USB Transmission to EP%i {\n", ep_addr);
        for (i = 0; i < buf.Size; i++)
        {
            sprintf(cbuf, "%02X ", buf.DataPtr[i]);
            vbuf[c++] = cbuf[0];
            vbuf[c++] = cbuf[1];
            vbuf[c++] = cbuf[2];

            if (c == 3 * 32)
            {
                vbuf[c] = '\0';
                IK_DEBUG_PrintMessage("%s\n", vbuf);
                c = 0;
            }
        }

        if (c != 0)
        {
            vbuf[c] = '\0';
            IK_DEBUG_PrintMessage("%s\n", vbuf);
        }

        IK_DEBUG_PrintMessage("}\n");

        free(cbuf);
        free(vbuf);
        #endif
    }

    return IK_OK;
}

static IK_Status_t _ikhwif_berndokb_debug_printmsg(const char *msg, uint32_t len)
{
    if (hw_uart1_init_status != SYS_INIT_STATUS_INITIALISED)
        return IK_ERROR;
    
    HAL_UART_Transmit(hw_uart1_handle, (uint8_t *) msg, (uint16_t) len, 10);

    return IK_OK;
}

static IK_Status_t _ikhwif_berndokb_cfgmem_init(void)
{
    return IK_OK;
}

static IK_Status_t _ikhwif_berndokb_cfgmem_deinit(void)
{
    return IK_OK;
}

static IK_Status_t _ikhwif_berndokb_cfgmem_wbyte(uint32_t address, uint8_t data)
{
    return IK_OK;
}

static IK_Status_t _ikhwif_berndokb_cfgmem_wbuf(uint32_t address, IK_Buffer_t data_buf)
{
    return IK_OK;
}

static uint8_t _ikhwif_berndokb_cfgmem_rbyte(uint32_t address)
{
    return 0x00;
}

static IK_Buffer_t _ikhwif_berndokb_cfgmem_rbuf(uint32_t address, uint32_t size)
{
    return (IK_Buffer_t) { NULL, 0 };
}

static IK_Status_t _ikhwif_berndokb_kbm_pollmatrix(void)
{
    uint8_t i;
    uint32_t poll_lines;

    if (hw_portex0_init_status != SYS_INIT_STATUS_INITIALISED || hw_portex1_init_status != SYS_INIT_STATUS_INITIALISED)
        return IK_ERROR;

    for (i = 0; i < 22; i++)
    {
        // Get the active poll line.
        poll_lines = 1 << i;

        // Update the poll line port expanders.
        PCA9555_WritePort(hw_portex0_handle, 0, (uint8_t)(poll_lines & 0x000000ff));
        PCA9555_WritePort(hw_portex0_handle, 1, (uint8_t)((poll_lines & 0x0000ff00) >> 8));
        PCA9555_WritePort(hw_portex1_handle, 0, (uint8_t)((poll_lines & 0x00ff0000) >> 16));
        PCA9555_WritePort(hw_portex1_handle, 1, (uint8_t)((poll_lines & 0xff000000) >> 24));

        // Report the sense line states.
        IK_SWACQ_PollStatusReportCallback((IK_KeyState_t) { 1   + i, HAL_GPIO_ReadPin(GPIO_KBM_DATA_0_PORT, GPIO_KBM_DATA_0_PIN) == GPIO_PIN_SET});
        IK_SWACQ_PollStatusReportCallback((IK_KeyState_t) { 23  + i, HAL_GPIO_ReadPin(GPIO_KBM_DATA_1_PORT, GPIO_KBM_DATA_1_PIN) == GPIO_PIN_SET});
        IK_SWACQ_PollStatusReportCallback((IK_KeyState_t) { 45  + i, HAL_GPIO_ReadPin(GPIO_KBM_DATA_2_PORT, GPIO_KBM_DATA_2_PIN) == GPIO_PIN_SET});
        IK_SWACQ_PollStatusReportCallback((IK_KeyState_t) { 67  + i, HAL_GPIO_ReadPin(GPIO_KBM_DATA_3_PORT, GPIO_KBM_DATA_3_PIN) == GPIO_PIN_SET});
        IK_SWACQ_PollStatusReportCallback((IK_KeyState_t) { 89  + i, HAL_GPIO_ReadPin(GPIO_KBM_DATA_4_PORT, GPIO_KBM_DATA_4_PIN) == GPIO_PIN_SET});
        IK_SWACQ_PollStatusReportCallback((IK_KeyState_t) { 111 + i, HAL_GPIO_ReadPin(GPIO_KBM_DATA_5_PORT, GPIO_KBM_DATA_5_PIN) == GPIO_PIN_SET});
        IK_SWACQ_PollStatusReportCallback((IK_KeyState_t) { 133 + i, HAL_GPIO_ReadPin(GPIO_KBM_DATA_6_PORT, GPIO_KBM_DATA_6_PIN) == GPIO_PIN_SET});
    }

    return IK_OK;
}