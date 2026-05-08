#include <iostream>
#include <memory>
#include <thread>

#include "snake/Game.hpp"

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

class SnakeDirectionNode : public rclcpp::Node
{
private:
    Game &game_;
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr directionSubscriber_;

public:
    explicit SnakeDirectionNode(Game &game)
        : Node("snake_direction_node"), game_(game)
    {
        directionSubscriber_ = this->create_subscription<std_msgs::msg::String>(
            "/snake/direction",
            10,
            std::bind(&SnakeDirectionNode::directionCallback, this, std::placeholders::_1));

        RCLCPP_INFO(this->get_logger(), "Snake direction node started");
    }

private:
    void directionCallback(const std_msgs::msg::String::SharedPtr msg)
    {
        
        if (msg->data == "UP")
        {
            game_.setDirection(Direction::Up);
        }
        else if (msg->data == "DOWN")
        {
            game_.setDirection(Direction::Down);
        }
        else if (msg->data == "LEFT")
        {
            game_.setDirection(Direction::Left);
        }
        else if (msg->data == "RIGHT")
        {
            game_.setDirection(Direction::Right);
        }
        else
        {
            //RCLCPP_INFO(this->get_logger(), "Unknown direction: %s", msg->data.c_str());
        }

        //RCLCPP_INFO(this->get_logger(), "Direction received: %s", msg->data.c_str());
    }
};

int main(int argc, char const *argv[])
{
    rclcpp::init(argc, argv);
    Game game;

    auto node = std::make_shared<SnakeDirectionNode>(game);

    std::thread rosThread([&node]()
                          { rclcpp::spin(node); });

    game.run();
    rclcpp::shutdown();

    if (rosThread.joinable())
    {
        rosThread.join();
    }

    return 0;
}
