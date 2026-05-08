import time
import rclpy
from rclpy.node import Node
from std_msgs.msg import String

try:
    from smbus2 import SMBus
except ImportError:
    from smbus import SMBus


BMI160_ADDR = 0x69

BMI160_CHIP_ID_REG = 0x00
BMI160_CMD_REG = 0x7E
BMI160_ACCEL_X_LSB = 0x12
BMI160_ACC_RANGE_REG = 0x41


class ImuDirectionPublisher(Node):
    def __init__(self):
        super().__init__('imu_direction_publisher')

        self.publisher_ = self.create_publisher(String, '/snake/direction', 10)

        self.bus = SMBus(1)

        self.threshold = 0.35
        self.neutral_threshold = 0.15
        self.waiting_for_neutral = False

        self.init_bmi160()

        self.timer = self.create_timer(0.1, self.timer_callback)

        self.get_logger().info('IMU direction publisher started')

    def init_bmi160(self):
        chip_id = self.bus.read_byte_data(BMI160_ADDR, BMI160_CHIP_ID_REG)
        self.get_logger().info(f'BMI160 chip id: {hex(chip_id)}')

        if chip_id != 0xD1:
            self.get_logger().warn('Unexpected chip id. Check I2C address or wiring.')

        self.bus.write_byte_data(BMI160_ADDR, BMI160_CMD_REG, 0x11)
        time.sleep(0.1)

        self.bus.write_byte_data(BMI160_ADDR, BMI160_ACC_RANGE_REG, 0x03)
        time.sleep(0.05)

    def timer_callback(self):
        accel_x, accel_y, accel_z = self.read_accelerometer()

        direction = self.calculate_direction(accel_x, accel_y)

        if direction is None:
            return

        msg = String()
        msg.data = direction
        self.publisher_.publish(msg)

        self.get_logger().info(
            f'ACC x={accel_x:.2f}, y={accel_y:.2f}, z={accel_z:.2f} -> {direction}'
        )

    def read_accelerometer(self):
        data = self.bus.read_i2c_block_data(BMI160_ADDR, BMI160_ACCEL_X_LSB, 6)

        raw_x = self.bytes_to_int16(data[1], data[0])
        raw_y = self.bytes_to_int16(data[3], data[2])
        raw_z = self.bytes_to_int16(data[5], data[4])

        accel_x = raw_x / 16384.0
        accel_y = raw_y / 16384.0
        accel_z = raw_z / 16384.0

        return accel_x, accel_y, accel_z

    def bytes_to_int16(self, msb, lsb):
        value = (msb << 8) | lsb

        if value & 0x8000:
            value -= 65536

        return value

    def calculate_direction(self, x, y): 
        if abs(x) < self.threshold and abs(y) < self.threshold: 
            return None 
        if abs(x) > abs(y): 
            if x > self.threshold: 
                return 'DOWN' 
            return 'UP' 
        if y > self.threshold: 
            return 'LEFT' 
        return 'RIGHT'

    def destroy_node(self):
        self.bus.close()
        super().destroy_node()


def main(args=None):
    rclpy.init(args=args)

    node = ImuDirectionPublisher()

    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass

    node.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()