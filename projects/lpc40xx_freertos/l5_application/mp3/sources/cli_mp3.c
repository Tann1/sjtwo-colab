#include "cli_handlers.h"
#include <stdio.h>

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

#include "uart_printf.h"

extern QueueHandle_t songname_q;

app_cli_status_e cli__mp3_play(app_cli__argument_t argument, sl_string_s user_input_minus_command_name,
                               app_cli__print_string_function cli_output) {
  // user_input_minus_command_name is actually a 'char *' pointer type
  // We tell the Queue to copy 32 bytes of songname from this location
  xQueueSend(songname_q, sl_string__c_str(user_input_minus_command_name), portMAX_DELAY);
  return APP_CLI_STATUS__SUCCESS;
}