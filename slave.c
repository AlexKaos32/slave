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
  int s = -1;
  modbus_t *ctx;
  modbus_mapping_t *mb_mapping;
  
  ctx = modbus_new_tcp("127.0.0.1", 1502);
  //    modbus_set_debug(ctx, TRUE); 
  
  mb_mapping = modbus_mapping_new(0, 0, 500, 500);  // Adjust the sizes as needed
  if (mb_mapping == NULL) {
    fprintf(stderr, "Failed to allocate the mapping: %s\n",
	    modbus_strerror(errno));
    modbus_free(ctx);
    return -1;
  }

  // Assign static values to holding registers and coils at specific addresses
  mb_mapping->tab_registers[170] = 1;  // Holding register at address 28171
  mb_mapping->tab_bits[153] = 101;     // Coil at address 27154
  
  s = modbus_tcp_listen(ctx, 1);
  modbus_tcp_accept(ctx, &s);
  
  for (;;) {
    uint8_t query[MODBUS_TCP_MAX_ADU_LENGTH];
    int rc;
    
    rc = modbus_receive(ctx, query);
    printf("SLAVE: Holding Register at address 28171 = %d\n", mb_mapping->tab_registers[170]);
    printf("SLAVE: Coil at address 27154 = %d\n", mb_mapping->tab_bits[153]);
    
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
