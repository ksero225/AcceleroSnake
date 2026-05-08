#include "std_msgs/msg/string.hpp"
#include "rclcpp/rclcpp.hpp"

#include <string>

class DirectionSubscriber : public rclcpp::Node
{
private:
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr direction_subscriber_;

public:
    DirectionSubscriber() : Node("direction_node")
    {
        direction_subscriber_ = this->create_subscription<std_msgs::msg::String>(
            "/snake/direction", 10,
            std::bind(&DirectionSubscriber::directionCallback, this, std::placeholders::_1));

        RCLCPP_INFO(this->get_logger(), "DirectionSubscriber started");
    }

private:
    void directionCallback(const std_msgs::msg::String::SharedPtr msg)
    {
        RCLCPP_INFO(this->get_logger(), "Direction received: %s ", msg->data.c_str());
    }
};

int main(int argc, char const *argv[])
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<DirectionSubscriber>();
    rclcpp::spin(node);

    rclcpp::shutdown();
    return 0;
}
