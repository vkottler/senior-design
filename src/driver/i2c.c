#include "i2c.h"
#include "stm32f3xx_ll_bus.h"
#include "board.h"
#include <stdio.h>

#define IS_LL_I2C_PERIPHERAL_MODE(__VALUE__)    (((__VALUE__) == LL_I2C_MODE_I2C)          || \
                                                 ((__VALUE__) == LL_I2C_MODE_SMBUS_HOST)   || \
                                                 ((__VALUE__) == LL_I2C_MODE_SMBUS_DEVICE) || \
                                                 ((__VALUE__) == LL_I2C_MODE_SMBUS_DEVICE_ARP))

#define IS_LL_I2C_ANALOG_FILTER(__VALUE__)      (((__VALUE__) == LL_I2C_ANALOGFILTER_ENABLE) || \
                                                 ((__VALUE__) == LL_I2C_ANALOGFILTER_DISABLE))

#define IS_LL_I2C_DIGITAL_FILTER(__VALUE__)     ((__VALUE__) <= 0x0000000FU)

#define IS_LL_I2C_OWN_ADDRESS1(__VALUE__)       ((__VALUE__) <= 0x000003FFU)

#define IS_LL_I2C_TYPE_ACKNOWLEDGE(__VALUE__)   (((__VALUE__) == LL_I2C_ACK) || \
                                                 ((__VALUE__) == LL_I2C_NACK))

#define IS_LL_I2C_OWN_ADDRSIZE(__VALUE__)       (((__VALUE__) == LL_I2C_OWNADDRESS1_7BIT) || \
                                                 ((__VALUE__) == LL_I2C_OWNADDRESS1_10BIT))

void i2c_write(uint8_t addr, uint8_t* data, uint8_t size, uint8_t stop)
{
    LL_I2C_SetSlaveAddr(I2C1, addr<<1);
    LL_I2C_SetTransferSize(I2C1, size);
    LL_I2C_SetTransferRequest(I2C1, LL_I2C_REQUEST_WRITE);
    LL_I2C_GenerateStartCondition(I2C1);

    while(!LL_I2C_IsActiveFlag_TC(I2C1)) {
        if(LL_I2C_IsActiveFlag_NACK(I2C1))
            return;
        if(LL_I2C_IsActiveFlag_TXIS(I2C1))
            LL_I2C_TransmitData8(I2C1, *data++);
    }

    if(stop) {
        LL_I2C_GenerateStopCondition(I2C1);
        while(!LL_I2C_IsActiveFlag_STOP(I2C1));
    }

}

void i2c_read(uint8_t addr, uint8_t *data, uint8_t size)
{
    LL_I2C_SetSlaveAddr(I2C1, addr<<1);
    LL_I2C_SetTransferSize(I2C1, size);
    LL_I2C_SetTransferRequest(I2C1, LL_I2C_REQUEST_READ);
    LL_I2C_GenerateStartCondition(I2C1);
    while(size)
        if(LL_I2C_IsActiveFlag_RXNE(I2C1)) {
            *data++ = LL_I2C_ReceiveData8(I2C1);
            size--;
        }

    LL_I2C_GenerateStopCondition(I2C1);
    while(!LL_I2C_IsActiveFlag_STOP(I2C1));

}

uint32_t i2c_scan(){

    LL_I2C_ClearFlag_NACK(I2C1);

    // SCAN I2C address
    for (int i = 0; i < 128; i ++)
    {
        LL_I2C_SetSlaveAddr(I2C1, i << 1);
        LL_I2C_GenerateStartCondition(I2C1);

        delay(20);
        if(!LL_I2C_IsActiveFlag_NACK(I2C1))
        {
          printf("Addr 0x%x\r\n",i);
        }
          LL_I2C_GenerateStopCondition(I2C1);
        LL_I2C_ClearFlag_NACK(I2C1);
    }
    return 0;
}


/**
  * @brief  De-initialize the I2C registers to their default reset values.
  * @param  I2Cx I2C Instance.
  * @retval An ErrorStatus enumeration value:
  *          - SUCCESS: I2C registers are de-initialized
  *          - ERROR: I2C registers are not de-initialized
  */
uint32_t LL_I2C_DeInit(I2C_TypeDef *I2Cx)
{
  ErrorStatus status = SUCCESS;

  /* Check the I2C Instance I2Cx */

  if (I2Cx == I2C1)
  {
    /* Force reset of I2C clock */
    LL_APB1_GRP1_ForceReset(LL_APB1_GRP1_PERIPH_I2C1);

    /* Release reset of I2C clock */
    LL_APB1_GRP1_ReleaseReset(LL_APB1_GRP1_PERIPH_I2C1);
  }
#if defined(I2C2)
  else if (I2Cx == I2C2)
  {
    /* Force reset of I2C clock */
    LL_APB1_GRP1_ForceReset(LL_APB1_GRP1_PERIPH_I2C2);

    /* Release reset of I2C clock */
    LL_APB1_GRP1_ReleaseReset(LL_APB1_GRP1_PERIPH_I2C2);

  }
#endif
#if defined(I2C3)
  else if (I2Cx == I2C3)
  {
    /* Force reset of I2C clock */
    LL_APB1_GRP1_ForceReset(LL_APB1_GRP1_PERIPH_I2C3);

    /* Release reset of I2C clock */
    LL_APB1_GRP1_ReleaseReset(LL_APB1_GRP1_PERIPH_I2C3);
  }
#endif
  else
  {
    status = ERROR;
  }

  return status;
}

/**
  * @brief  Initialize the I2C registers according to the specified parameters in I2C_InitStruct.
  * @param  I2Cx I2C Instance.
  * @param  I2C_InitStruct pointer to a @ref LL_I2C_InitTypeDef structure.
  * @retval An ErrorStatus enumeration value:
  *          - SUCCESS: I2C registers are initialized
  *          - ERROR: Not applicable
  */
uint32_t LL_I2C_Init(I2C_TypeDef *I2Cx, LL_I2C_InitTypeDef *I2C_InitStruct)
{
  /* Disable the selected I2Cx Peripheral */
  LL_I2C_Disable(I2Cx);

  /*---------------------------- I2Cx CR1 Configuration ------------------------
   * Configure the analog and digital noise filters with parameters :
   * - AnalogFilter: I2C_CR1_ANFOFF bit
   * - DigitalFilter: I2C_CR1_DNF[3:0] bits
   */
  LL_I2C_ConfigFilters(I2Cx, I2C_InitStruct->AnalogFilter, I2C_InitStruct->DigitalFilter);

  /*---------------------------- I2Cx TIMINGR Configuration --------------------
   * Configure the SDA setup, hold time and the SCL high, low period with parameter :
   * - Timing: I2C_TIMINGR_PRESC[3:0], I2C_TIMINGR_SCLDEL[3:0], I2C_TIMINGR_SDADEL[3:0],
   *           I2C_TIMINGR_SCLH[7:0] and I2C_TIMINGR_SCLL[7:0] bits
   */
  LL_I2C_SetTiming(I2Cx, I2C_InitStruct->Timing);

  /* Enable the selected I2Cx Peripheral */
  LL_I2C_Enable(I2Cx);

  /*---------------------------- I2Cx OAR1 Configuration -----------------------
   * Disable, Configure and Enable I2Cx device own address 1 with parameters :
   * - OwnAddress1:  I2C_OAR1_OA1[9:0] bits
   * - OwnAddrSize:  I2C_OAR1_OA1MODE bit
   */
  LL_I2C_DisableOwnAddress1(I2Cx);
  LL_I2C_SetOwnAddress1(I2Cx, I2C_InitStruct->OwnAddress1, I2C_InitStruct->OwnAddrSize);

  /* OwnAdress1 == 0 is reserved for General Call address */
  if (I2C_InitStruct->OwnAddress1 != 0U)
  {
    LL_I2C_EnableOwnAddress1(I2Cx);
  }

  /*---------------------------- I2Cx MODE Configuration -----------------------
  * Configure I2Cx peripheral mode with parameter :
   * - PeripheralMode: I2C_CR1_SMBDEN and I2C_CR1_SMBHEN bits
   */
  LL_I2C_SetMode(I2Cx, I2C_InitStruct->PeripheralMode);

  /*---------------------------- I2Cx CR2 Configuration ------------------------
   * Configure the ACKnowledge or Non ACKnowledge condition
   * after the address receive match code or next received byte with parameter :
   * - TypeAcknowledge: I2C_CR2_NACK bit
   */
  LL_I2C_AcknowledgeNextData(I2Cx, I2C_InitStruct->TypeAcknowledge);

  return SUCCESS;
}

/**
  * @brief  Set each @ref LL_I2C_InitTypeDef field to default value.
  * @param  I2C_InitStruct Pointer to a @ref LL_I2C_InitTypeDef structure.
  * @retval None
  */
void LL_I2C_StructInit(LL_I2C_InitTypeDef *I2C_InitStruct)
{
  /* Set I2C_InitStruct fields to default values */
  I2C_InitStruct->PeripheralMode  = LL_I2C_MODE_I2C;
  I2C_InitStruct->Timing          = 0U;
  I2C_InitStruct->AnalogFilter    = LL_I2C_ANALOGFILTER_ENABLE;
  I2C_InitStruct->DigitalFilter   = 0U;
  I2C_InitStruct->OwnAddress1     = 0U;
  I2C_InitStruct->TypeAcknowledge = LL_I2C_NACK;
  I2C_InitStruct->OwnAddrSize     = LL_I2C_OWNADDRESS1_7BIT;
}
