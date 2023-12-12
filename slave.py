from pymodbus.server.sync import StartTcpServer
from pymodbus.datastore import ModbusSequentialDataBlock, ModbusSparseDataBlock, ModbusServerContext

# Static values for holding registers, input registers, coils, and input coils
holding_register_values = [1] * 10
input_register_values = [1] * 10
coil_values = [1] * 10
input_coil_values = [1] * 10

# Create a Modbus data block for holding registers, input registers, coils, and input coils
store = ModbusServerContext(
    di=ModbusSparseDataBlock({}),
    co=ModbusSequentialDataBlock(0, coil_values),
    hr=ModbusSequentialDataBlock(0, holding_register_values),
    ir=ModbusSequentialDataBlock(0, input_register_values),
    ao=ModbusSparseDataBlock({}),
    bo=ModbusSparseDataBlock({}),
    bi=ModbusSequentialDataBlock(0, input_coil_values)
)

# Start the Modbus TCP server
StartTcpServer(store, address=("localhost", 5020))
