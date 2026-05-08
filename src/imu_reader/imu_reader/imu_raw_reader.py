import time
import rclpy
from rclpy.node import Node
from smbus2 import SMBus

BMI160_ADDR = 0x69

BMI160_CHIP_ID_REG = 0x00
BMI160_CMD_REG = 0x7E

BMI160_GYRO_X_LSB = 0x0C
BMI160_ACCEL_X_LSB = 0x12

BMI160_ACC_RANGE_REG = 0x41
BMI160_GYRO_RANGE_REG = 0x43

class ImuRawReader(Node):
    def __init__(self):
        super().__init__('imu_raw_reader')

        self.bus = SMBus(1)

        self.init_bmi160()

        self.timer = self.create_timer(0.1, self.timer_callback)

        self.get_logger().info('BMI160 raw reader started')

    def init_bmi160(self):
        chip_id = self.bus.read_byte_data(BMI160_ADDR, BMI160_CHIP_ID_REG)
        self.get_logger().info(f'BMI160 chip id: {hex(chip_id)}')

        if chip_id != 0xD1:
            self.get_logger().warn('Unexpected chip id. Check I2C address or wiring.')

        # Accelerometer normal mode
        self.bus.write_byte_data(BMI160_ADDR, BMI160_CMD_REG, 0x11)
        time.sleep(0.1)

        # Gyroscope normal mode
        self.bus.write_byte_data(BMI160_ADDR, BMI160_CMD_REG, 0x15)
        time.sleep(0.1)

        # Accelerometer range: +/- 2g
        self.bus.write_byte_data(BMI160_ADDR, BMI160_ACC_RANGE_REG, 0x03)
        time.sleep(0.05)

        # Gyroscope range: +/- 2000 deg/s
        self.bus.write_byte_data(BMI160_ADDR, BMI160_GYRO_RANGE_REG, 0x00)
        time.sleep(0.05)

    def timer_callback(self):
        accel_x, accel_y, accel_z = self.read_accelerometer()
        gyro_x, gyro_y, gyro_z = self.read_gyroscope()

        self.get_logger().info(
            f'ACC [g] x={accel_x:.3f}, y={accel_y:.3f}, z={accel_z:.3f} | '
            f'GYRO [deg/s] x={gyro_x:.3f}, y={gyro_y:.3f}, z={gyro_z:.3f}'
        )

    def read_accelerometer(self):
        data = self.bus.read_i2c_block_data(BMI160_ADDR, BMI160_ACCEL_X_LSB, 6)

        raw_x = self.bytes_to_int16(data[1], data[0])
        raw_y = self.bytes_to_int16(data[3], data[2])
        raw_z = self.bytes_to_int16(data[5], data[4])

        # Przy zakresie +/- 2g: 16384 LSB = 1g
        accel_x = raw_x / 16384.0
        accel_y = raw_y / 16384.0
        accel_z = raw_z / 16384.0

        return accel_x, accel_y, accel_z

    def read_gyroscope(self):
        data = self.bus.read_i2c_block_data(BMI160_ADDR, BMI160_GYRO_X_LSB, 6)

        raw_x = self.bytes_to_int16(data[1], data[0])
        raw_y = self.bytes_to_int16(data[3], data[2])
        raw_z = self.bytes_to_int16(data[5], data[4])

        # Przy zakresie +/- 2000 deg/s: 16.4 LSB = 1 deg/s
        gyro_x = raw_x / 16.4
        gyro_y = raw_y / 16.4
        gyro_z = raw_z / 16.4

        return gyro_x, gyro_y, gyro_z

    def bytes_to_int16(self, msb, lsb):
        value = (msb << 8) | lsb

        if value & 0x8000:
            value -= 65536

        return value

    def destroy_node(self):
        self.bus.close()
        super().destroy_node()


def main(args=None):
    rclpy.init(args=args)

    node = ImuRawReader()

    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass

    node.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()