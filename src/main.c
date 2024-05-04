
#include "main.h"
#include <stdio.h>
#include <stdarg.h>
#include "disk.h"
#include "fat_filelib.h"
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>


static void MX_GPIO_Init(void);
void SystemClock_Config(void);





 
 static int test(lua_State *L)
{
  printf("test\n");
  return 1;
}

static const luaL_Reg mylib[] =
{
  {"test",test},
  {NULL,NULL}
};

const char LUA_SCRIPT_GLOBAL[] ="  \
while 1 do \
 test() \
 print(\"Hello,I am lua!\\n--this is newline\\r\\n\") \
end";



 




int main(void) {
  HAL_Init();
  SystemClock_Config();

  MX_GPIO_Init();
  
  GPIO_InitTypeDef BoardLEDs;
  BoardLEDs.Mode = GPIO_MODE_OUTPUT_PP;
  BoardLEDs.Pin = GPIO_PIN_13;
  HAL_GPIO_Init(GPIOC, &BoardLEDs);
  
  GPIO_InitTypeDef GPIO_InitStruct;
  GPIO_InitStruct.Pin = GPIO_PIN_0;      // Pin GPIO A0
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT; // Modo de entrada
  GPIO_InitStruct.Pull = GPIO_PULLUP;     // Activar resistencia pull-up (opcional)
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW; // Velocidad baja (opcional)
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  

  if(!HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)){
      MX_USB_DEVICE_Init();
      while(1);
  }   


    init_cdc();

    uint32 size;
    again:
    size = sd_init();
    fl_init();
    if (fl_attach_media(sd_readsector, sd_writesector) != FAT_INIT_OK){
        //kprintf("ERROR: Failed to init file system\n");
        if (fl_format(size, "")){
            printf("format ok\n");
            goto again;
        }
      return -1;
    }
    
    do{
        HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_13);
        HAL_Delay(50);
    }
    while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0));
    fl_listdirectory("/");

   
  int status, result;
  lua_State *L = luaL_newstate();  /* create state */
  if (L == NULL) {
    printf( "cannot create state: not enough memory");
    return EXIT_FAILURE;
  }
  
  luaL_openlibs(L);//luaopen_base(L);
  luaL_setfuncs(L,mylib, 0);
  int ret= luaL_dostring(L, LUA_SCRIPT_GLOBAL);
  if (ret != 0)
    return 0;





  lua_close(L);


  while(1);
while (1)
  {
     
     //if(__usb_available()){
       //  printf("ok usb\n");
      //}
 
    HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_13);
    //printf("ok usb %d\n",size);
    HAL_Delay(100);

    
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
return 0;
}




static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pin : PB15 */
  GPIO_InitStruct.Pin = GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}
 
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};


//  __HAL_RCC_FLASH_CLK_ENABLE();



  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 384;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 8;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
  /*PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_I2S;
  PeriphClkInitStruct.PLLI2S.PLLI2SN = 271;
  PeriphClkInitStruct.PLLI2S.PLLI2SM = 25;
  PeriphClkInitStruct.PLLI2S.PLLI2SR = 6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }*/
}
 
 
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}