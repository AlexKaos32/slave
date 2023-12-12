from pymodbus.server.asyncio import StartTcpServer
from pymodbus.datastore import ModbusSequentialDataBlock, ModbusSlaveContext, ModbusServerContext
from pymodbus.transaction import ModbusRtuFramer
from twisted.internet import asyncioreactor

asyncioreactor.install()

# Static values for holding registers, input registers, and coils
holding_register_values = [1] * 10
input_register_values = [1] * 10
coil_values = [1] * 10

# Create a Modbus data block for holding registers, input registers, and coils
store = ModbusSlaveContext(
    di=None,
    co=ModbusSequentialDataBlock(0, coil_values),
    hr=ModbusSequentialDataBlock(0, holding_register_values),
    ir=ModbusSequentialDataBlock(0, input_register_values)
)
context = ModbusServerContext(slaves=store, single=True)

# Start the Modbus TCP server
async def run_server():
    StartTcpServer(context, identity=None, address=("localhost", 5020))

if __name__ == "__main__":
    import asyncio
    loop = asyncio.get_event_loop()
    loop.run_until_complete(run_server())
