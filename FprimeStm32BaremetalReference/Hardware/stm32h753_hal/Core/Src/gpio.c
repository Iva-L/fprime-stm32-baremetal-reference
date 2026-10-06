/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "gpio.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins
     PI6   ------> FMC_D28
     PI5   ------> FMC_NBL3
     PI4   ------> FMC_NBL2
     PB5   ------> USB_OTG_HS_ULPI_D7
     PK5   ------> LTDC_B6
     PG10   ------> FMC_NE3
     PG9   ------> QUADSPI_BK2_IO2
     PD5   ------> FMC_NWE
     PD4   ------> FMC_NOE
     PC10   ------> SDMMC1_D2
     PA15 (JTDI)   ------> DEBUG_JTDI
     PI1   ------> FMC_D25
     PI0   ------> FMC_D24
     PI7   ------> FMC_D29
     PE1   ------> FMC_NBL1
     PB4 (NJTRST)   ------> DEBUG_JTRST
     PK4   ------> LTDC_B5
     PG11   ------> ETH_TX_EN
     PJ15   ------> LTDC_B3
     PD6   ------> FMC_NWAIT
     PC11   ------> SDMMC1_D3
     PA14 (JTCK/SWCLK)   ------> DEBUG_JTCK-SWCLK
     PI2   ------> FMC_D26
     PH15   ------> FMC_D23
     PH14   ------> FMC_D22
     PC15-OSC32_OUT (OSC32_OUT)   ------> RCC_OSC32_OUT
     PC14-OSC32_IN (OSC32_IN)   ------> RCC_OSC32_IN
     PE2   ------> SAI4_CK1
     PE0   ------> FMC_NBL0
     PB3 (JTDO/TRACESWO)   ------> DEBUG_JTDO-SWO
     PK6   ------> LTDC_B7
     PK3   ------> LTDC_B4
     PG12   ------> ETH_TXD1
     PD7   ------> FMC_NE1
     PC12   ------> SDMMC1_CK
     PI3   ------> FMC_D27
     PA13 (JTMS/SWDIO)   ------> DEBUG_JTMS-SWDIO
     PE5   ------> SAI1_SCK_A
     PE4   ------> SAI1_FS_A
     PE3   ------> SAI1_SD_B
     PB9   ------> SDMMC1_CDIR
     PB8   ------> SDMMC1_CKIN
     PG15   ------> FMC_SDNCAS
     PK7   ------> LTDC_DE
     PG14   ------> QUADSPI_BK2_IO3
     PG13   ------> ETH_TXD0
     PJ14   ------> LTDC_B2
     PJ12   ------> LTDC_B0
     PD2   ------> SDMMC1_CMD
     PD0   ------> FMC_D2_DA2
     PA10   ------> USB_OTG_FS_ID
     PA9   ------> USB_OTG_FS_VBUS
     PH13   ------> FMC_D21
     PI9   ------> FMC_D30
     PC13   ------> RTC_TAMP1
     PE6   ------> SAI1_SD_A
     PJ13   ------> LTDC_B1
     PD1   ------> FMC_D3_DA3
     PC8   ------> SDMMC1_D0
     PC9   ------> SDMMC1_D1
     PA8   ------> RCC_MCO_1
     PA12   ------> USB_OTG_FS_DP
     PA11   ------> USB_OTG_FS_DM
     PI10   ------> FMC_D31
     PI11   ------> USB_OTG_HS_ULPI_DIR
     PC7   ------> SDMMC1_D123DIR
     PC6   ------> SDMMC1_D0DIR
     PG8   ------> FMC_SDCLK
     PG7   ------> SAI1_MCLK_A
     PF2   ------> FMC_A2
     PF1   ------> FMC_A1
     PF0   ------> FMC_A0
     PG5   ------> FMC_A15_BA1
     PG6   ------> QUADSPI_BK1_NCS
     PI12   ------> LTDC_HSYNC
     PI13   ------> LTDC_VSYNC
     PI14   ------> LTDC_CLK
     PF3   ------> FMC_A3
     PG4   ------> FMC_A14_BA0
     PG3   ------> FMC_A13
     PG2   ------> FMC_A12
     PK2   ------> LTDC_G7
     PH1-OSC_OUT (PH1)   ------> RCC_OSC_OUT
     PH0-OSC_IN (PH0)   ------> RCC_OSC_IN
     PF5   ------> FMC_A5
     PF4   ------> FMC_A4
     PK0   ------> LTDC_G5
     PK1   ------> LTDC_G6
     PF6   ------> QUADSPI_BK1_IO3
     PF7   ------> QUADSPI_BK1_IO2
     PF8   ------> QUADSPI_BK1_IO0
     PJ11   ------> LTDC_G4
     PC0   ------> USB_OTG_HS_ULPI_STP
     PF9   ------> QUADSPI_BK1_IO1
     PJ10   ------> LTDC_G3
     PC1   ------> ETH_MDC
     PC2   ------> S_CKOUTDFSDM1
     PC3   ------> S_DATAIN1DFSDM1
     PJ9   ------> LTDC_G2
     PH2   ------> QUADSPI_BK2_IO0
     PA2   ------> ETH_MDIO
     PA1   ------> ETH_REF_CLK
     PJ0   ------> LTDC_R1
     PE10   ------> FMC_D7_DA7
     PJ8   ------> LTDC_G1
     PJ7   ------> LTDC_G0
     PJ6   ------> LTDC_R7
     PH3   ------> QUADSPI_BK2_IO1
     PH4   ------> USB_OTG_HS_ULPI_NXT
     PH5   ------> FMC_SDNWE
     PI15   ------> LTDC_R0
     PJ1   ------> LTDC_R2
     PF13   ------> FMC_A7
     PF14   ------> FMC_A8
     PE9   ------> FMC_D6_DA6
     PE11   ------> FMC_D8_DA8
     PB10   ------> USB_OTG_HS_ULPI_D3
     PB11   ------> USB_OTG_HS_ULPI_D4
     PH10   ------> FMC_D18
     PH11   ------> FMC_D19
     PD15   ------> FMC_D1_DA1
     PD14   ------> FMC_D0_DA0
     PA7   ------> ETH_CRS_DV
     PB2   ------> QUADSPI_CLK
     PF12   ------> FMC_A6
     PF15   ------> FMC_A9
     PE12   ------> FMC_D9_DA9
     PE15   ------> FMC_D12_DA12
     PJ5   ------> LTDC_R6
     PH9   ------> FMC_D17
     PH12   ------> FMC_D20
     PD11   ------> FMC_A16_CLE
     PD12   ------> FMC_A17_ALE
     PD13   ------> FMC_A18
     PA0_C   ------> ADCx_INN1
     PA5   ------> USB_OTG_HS_ULPI_CK
     PC4   ------> ETH_RXD0
     PB1   ------> USB_OTG_HS_ULPI_D2
     PJ2   ------> LTDC_R3
     PF11   ------> FMC_SDNRAS
     PG0   ------> FMC_A10
     PE8   ------> FMC_D5_DA5
     PE13   ------> FMC_D10_DA10
     PH6   ------> FMC_SDNE1
     PH8   ------> FMC_D16
     PB12   ------> USB_OTG_HS_ULPI_D5
     PD10   ------> FMC_D15_DA15
     PD9   ------> FMC_D14_DA14
     PA3   ------> USB_OTG_HS_ULPI_D0
     PC5   ------> ETH_RXD1
     PB0   ------> USB_OTG_HS_ULPI_D1
     PJ3   ------> LTDC_R4
     PJ4   ------> LTDC_R5
     PG1   ------> FMC_A11
     PE7   ------> FMC_D4_DA4
     PE14   ------> FMC_D11_DA11
     PH7   ------> FMC_SDCKE1
     PB13   ------> USB_OTG_HS_ULPI_D6
     PD8   ------> FMC_D13_DA13
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOI_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOK_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOJ_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(FDCAN1_STBY_GPIO_Port, FDCAN1_STBY_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED1_RGB_GPIO_Port, LED1_RGB_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LCD_BL_CTRL_Pin|LED3_RGB_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : D28_Pin FMC_NBL3_Pin FMC_NBL2_Pin D25_Pin
                           D24_Pin D29_Pin D26_Pin D27__IS42S32800G_DQ27_Pin
                           D30_Pin D31_Pin */
  GPIO_InitStruct.Pin = D28_Pin|FMC_NBL3_Pin|FMC_NBL2_Pin|D25_Pin
                          |D24_Pin|D29_Pin|D26_Pin|D27__IS42S32800G_DQ27_Pin
                          |D30_Pin|D31_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF12_FMC;
  HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);

  /*Configure GPIO pins : ULPI_D7_Pin ULPI_D3_Pin ULPI_D4_Pin ULPI_D2_Pin
                           ULPI_D5_Pin ULPI_D1_Pin ULPI_D6_Pin */
  GPIO_InitStruct.Pin = ULPI_D7_Pin|ULPI_D3_Pin|ULPI_D4_Pin|ULPI_D2_Pin
                          |ULPI_D5_Pin|ULPI_D1_Pin|ULPI_D6_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF10_OTG2_HS;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : LCD_B6_Pin LCD_B5_Pin LCD_B7_Pin LCD_B4_Pin
                           LCD_DE_Pin LCD_G7_Pin LCD_G5_Pin LCD_G6_Pin */
  GPIO_InitStruct.Pin = LCD_B6_Pin|LCD_B5_Pin|LCD_B7_Pin|LCD_B4_Pin
                          |LCD_DE_Pin|LCD_G7_Pin|LCD_G5_Pin|LCD_G6_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
  HAL_GPIO_Init(GPIOK, &GPIO_InitStruct);

  /*Configure GPIO pins : FMC_NE3_Pin SDNCAS_Pin SDCLK_Pin PG5
                           PG4 A13_Pin A12_Pin A10_Pin
                           A11_Pin */
  GPIO_InitStruct.Pin = FMC_NE3_Pin|SDNCAS_Pin|SDCLK_Pin|GPIO_PIN_5
                          |GPIO_PIN_4|A13_Pin|A12_Pin|A10_Pin
                          |A11_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF12_FMC;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  /*Configure GPIO pins : QSPI_BK2_IO2_Pin QSPI_BK2_IO3_Pin */
  GPIO_InitStruct.Pin = QSPI_BK2_IO2_Pin|QSPI_BK2_IO3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF9_QUADSPI;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  /*Configure GPIO pins : FMC_NWE_Pin FMC_NOE_Pin FMC_NWAIT_Pin FMC_NE1_Pin
                           D2_Pin D3_Pin D1_Pin D0_Pin
                           A16_Pin A17_Pin A18_Pin D15_Pin
                           D14_Pin D13_Pin */
  GPIO_InitStruct.Pin = FMC_NWE_Pin|FMC_NOE_Pin|FMC_NWAIT_Pin|FMC_NE1_Pin
                          |D2_Pin|D3_Pin|D1_Pin|D0_Pin
                          |A16_Pin|A17_Pin|A18_Pin|D15_Pin
                          |D14_Pin|D13_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF12_FMC;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : SDIO1_D2_Pin SDIO1_D3_Pin SDIO1_CLK_Pin SDIO1_D0_Pin
                           SDIO1_D1_Pin */
  GPIO_InitStruct.Pin = SDIO1_D2_Pin|SDIO1_D3_Pin|SDIO1_CLK_Pin|SDIO1_D0_Pin
                          |SDIO1_D1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF12_SDIO1;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : FMC_NBL1_Pin FMC_NBL0_Pin D7_Pin D6_Pin
                           D8_Pin D9_Pin D12_Pin D5_Pin
                           D10_Pin D4_Pin D11_Pin */
  GPIO_InitStruct.Pin = FMC_NBL1_Pin|FMC_NBL0_Pin|D7_Pin|D6_Pin
                          |D8_Pin|D9_Pin|D12_Pin|D5_Pin
                          |D10_Pin|D4_Pin|D11_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF12_FMC;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : RMII_TX_EN_Pin RMII_TXD1_Pin RMII_TXD0_Pin */
  GPIO_InitStruct.Pin = RMII_TX_EN_Pin|RMII_TXD1_Pin|RMII_TXD0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  /*Configure GPIO pins : LCD_B3_Pin LCD_B2_Pin LCD_B0_Pin LCD_B1_Pin
                           LCD_G4_Pin LCd_G3_Pin LCD_G2_Pin LCD_R1_Pin
                           LCD_G1_Pin LCD_G0_Pin LCD_R7_Pin LCD_R2_Pin
                           LCD_R6_Pin LCD_R3_Pin LCD_R4_Pin LCD_R5_Pin */
  GPIO_InitStruct.Pin = LCD_B3_Pin|LCD_B2_Pin|LCD_B0_Pin|LCD_B1_Pin
                          |LCD_G4_Pin|LCd_G3_Pin|LCD_G2_Pin|LCD_R1_Pin
                          |LCD_G1_Pin|LCD_G0_Pin|LCD_R7_Pin|LCD_R2_Pin
                          |LCD_R6_Pin|LCD_R3_Pin|LCD_R4_Pin|LCD_R5_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
  HAL_GPIO_Init(GPIOJ, &GPIO_InitStruct);

  /*Configure GPIO pin : FDCAN1_STBY_Pin */
  GPIO_InitStruct.Pin = FDCAN1_STBY_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(FDCAN1_STBY_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : D23_Pin D22_Pin D21_Pin SDNWE_Pin
                           D18_Pin D19_Pin D17_Pin D20_Pin
                           SDNE1_Pin D16_Pin SDCKE1_Pin */
  GPIO_InitStruct.Pin = D23_Pin|D22_Pin|D21_Pin|SDNWE_Pin
                          |D18_Pin|D19_Pin|D17_Pin|D20_Pin
                          |SDNE1_Pin|D16_Pin|SDCKE1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF12_FMC;
  HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

  /*Configure GPIO pin : PDM1_CLK_Pin */
  GPIO_InitStruct.Pin = PDM1_CLK_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF10_SAI4;
  HAL_GPIO_Init(PDM1_CLK_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : SAI1_SCKA_Pin SAI1_FSA_Pin SAI1_SDB_Pin SAI1_SDA_Pin */
  GPIO_InitStruct.Pin = SAI1_SCKA_Pin|SAI1_FSA_Pin|SAI1_SDB_Pin|SAI1_SDA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF6_SAI1;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : SDIO1_CDIR_Pin SDIO1_CKIN_Pin */
  GPIO_InitStruct.Pin = SDIO1_CDIR_Pin|SDIO1_CKIN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF7_SDIO1;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : SDIO1_CMD_Pin */
  GPIO_InitStruct.Pin = SDIO1_CMD_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF12_SDIO1;
  HAL_GPIO_Init(SDIO1_CMD_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : USB_FS1_ID_Pin USB_FS1_DP_Pin USB_FS1_DM_Pin */
  GPIO_InitStruct.Pin = USB_FS1_ID_Pin|USB_FS1_DP_Pin|USB_FS1_DM_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF10_OTG1_FS;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : MFX_IRQOUT_Pin */
  GPIO_InitStruct.Pin = MFX_IRQOUT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(MFX_IRQOUT_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : MCO_Pin */
  GPIO_InitStruct.Pin = MCO_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF0_MCO;
  HAL_GPIO_Init(MCO_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : ULPI_DIR_Pin */
  GPIO_InitStruct.Pin = ULPI_DIR_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF10_OTG2_HS;
  HAL_GPIO_Init(ULPI_DIR_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : SDIO1_D123DIR_Pin SDIO1_D0DIR_Pin */
  GPIO_InitStruct.Pin = SDIO1_D123DIR_Pin|SDIO1_D0DIR_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF8_SDIO1;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : SAI1_MCLKA_Pin */
  GPIO_InitStruct.Pin = SAI1_MCLKA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF6_SAI1;
  HAL_GPIO_Init(SAI1_MCLKA_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : A2_Pin A1_Pin A0_Pin A3_Pin
                           A5_Pin A4_Pin A7_Pin A8_Pin
                           A6_Pin A9_Pin SNDRAS_Pin */
  GPIO_InitStruct.Pin = A2_Pin|A1_Pin|A0_Pin|A3_Pin
                          |A5_Pin|A4_Pin|A7_Pin|A8_Pin
                          |A6_Pin|A9_Pin|SNDRAS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF12_FMC;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /*Configure GPIO pin : QSPI_BK1_NCS_Pin */
  GPIO_InitStruct.Pin = QSPI_BK1_NCS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF10_QUADSPI;
  HAL_GPIO_Init(QSPI_BK1_NCS_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LCD_HSYNC_Pin LCD_VSYNC_Pin LCD_CLK_Pin LCD_R0_Pin */
  GPIO_InitStruct.Pin = LCD_HSYNC_Pin|LCD_VSYNC_Pin|LCD_CLK_Pin|LCD_R0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
  HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);

  /*Configure GPIO pins : QSPI_BK1_IO3_Pin QSPI_BK1_IO2_Pin */
  GPIO_InitStruct.Pin = QSPI_BK1_IO3_Pin|QSPI_BK1_IO2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF9_QUADSPI;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /*Configure GPIO pins : QSPI_BK1_IO0_Pin QSPI_BK1_IO1_Pin */
  GPIO_InitStruct.Pin = QSPI_BK1_IO0_Pin|QSPI_BK1_IO1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF10_QUADSPI;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /*Configure GPIO pin : ULPI_STP_Pin */
  GPIO_InitStruct.Pin = ULPI_STP_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF10_OTG2_HS;
  HAL_GPIO_Init(ULPI_STP_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LED1_RGB_Pin */
  GPIO_InitStruct.Pin = LED1_RGB_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED1_RGB_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : RMII_MDC_Pin RMII_RXD0_Pin RMII_RXD1_Pin */
  GPIO_InitStruct.Pin = RMII_MDC_Pin|RMII_RXD0_Pin|RMII_RXD1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : DFSDM_CLK_Pin */
  GPIO_InitStruct.Pin = DFSDM_CLK_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF6_DFSDM1;
  HAL_GPIO_Init(DFSDM_CLK_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : DFSM_DAT1_Pin */
  GPIO_InitStruct.Pin = DFSM_DAT1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF3_DFSDM1;
  HAL_GPIO_Init(DFSM_DAT1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : QSPI_BK2_IO0_Pin QSPI_BK2_IO1_Pin */
  GPIO_InitStruct.Pin = QSPI_BK2_IO0_Pin|QSPI_BK2_IO1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF9_QUADSPI;
  HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

  /*Configure GPIO pins : ETH_MDIO_Pin RMII_REF_CLK_Pin RMII_CRS_DV_Pin */
  GPIO_InitStruct.Pin = ETH_MDIO_Pin|RMII_REF_CLK_Pin|RMII_CRS_DV_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : ULPI_NXT_Pin */
  GPIO_InitStruct.Pin = ULPI_NXT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF10_OTG2_HS;
  HAL_GPIO_Init(ULPI_NXT_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LCD_BL_CTRL_Pin LED3_RGB_Pin */
  GPIO_InitStruct.Pin = LCD_BL_CTRL_Pin|LED3_RGB_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : QSPI_CLK_Pin */
  GPIO_InitStruct.Pin = QSPI_CLK_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF9_QUADSPI;
  HAL_GPIO_Init(QSPI_CLK_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : ULPI_CK_Pin ULPI_D0_Pin */
  GPIO_InitStruct.Pin = ULPI_CK_Pin|ULPI_D0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF10_OTG2_HS;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*AnalogSwitch Config */
  HAL_SYSCFG_AnalogSwitchConfig(SYSCFG_SWITCH_PA0, SYSCFG_SWITCH_PA0_OPEN);

}

/* USER CODE BEGIN 2 */

/* USER CODE END 2 */
