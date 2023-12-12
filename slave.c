/*
 * slave.c - a MODBUS slave
 */

#include <stdio.h>
#ifndef _MSC_VER
#include <unistd.h>
#endif
#include <stdlib.h>
#include <errno.h>

#include <modbus.h>

int main(void)
{
  int i;
  int s = -1;
  modbus_t *ctx;
  modbus_mapping_t *mb_mapping;
  
  ctx = modbus_new_tcp("127.0.0.1", 1502);
  //    modbus_set_debug(ctx, TRUE); 
  
  mb_mapping = modbus_mapping_new(10, 10, 10, 10);  // Specify the size for each type of register or coil
  if (mb_mapping == NULL) {
    fprintf(stderr, "Failed to allocate the mapping: %s\n",
	    modbus_strerror(errno));
    modbus_free(ctx);
    return -1;
  }

  // Assign static values of "1" to all 10 input registers, holding registers, coils, and input coils
  for (i = 0; i < 10; i++) {
    mb_mapping->tab_input_registers[i] = 1;
    mb_mapping->tab_registers[i] = 1;
    mb_mapping->tab_input_bits[i] = 1;
    mb_mapping->tab_bits[i] = 1;
  }
  
  s = modbus_tcp_listen(ctx, 1);
  modbus_tcp_accept(ctx, &s);
  
  for (;;) {
    uint8_t query[MODBUS_TCP_MAX_ADU_LENGTH];
    int rc;
    
    rc = modbus_receive(ctx, query);
    printf("SLAVE: regs[] =\t");
    for(i = 0; i < 10; i++) {
      printf("%d ", mb_mapping->tab_registers[i]);
    }
    printf("\n");
    
    if (rc > 0) {
      /* rc is the query size */
      modbus_reply(ctx, query, rc, mb_mapping);
    } else if (rc == -1) {
      /* Connection closed by the client or error */
      break;
    }
  }
  
  printf("Quit the loop: %s\n", modbus_strerror(errno));
  
  if (s != -1) {
    close(s);
  }
  modbus_mapping_free(mb_mapping);
  modbus_close(ctx);
  modbus_free(ctx);
  
  return 0;
}
