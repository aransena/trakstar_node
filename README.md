# trakstar_node
ROS Package for Ascension Trakstar

First, set up the TrakStar for use on linux systems (this is separate to ROS). Follow the installation steps as described here [ I recommend you just clone the repo to your home directory '~/'], but follow the instructions below when you get to the section "*libusb configuration": 
https://github.com/ChristophJud/ATC3DGTracker

Device permissions:
Create a file in /etc/udev/rules.d/ called trakstar.rules

``
sudo gedit /etc/udev/rules.d/trakstar.rules
``

and then put this in there:

``
SUBSYSTEM=="usb", ATTR{idVendor}=="04b4", ATTR{idProduct}=="1005", MODE="0666", GROUP="plugdev" 
``

Save and exit the text editor, and then enter this in the terminal:

``
sudo udevadm control --reload-rules && udevadm trigger
``

Then unplug the TrakStar, power it off and then on again, plug it in, and run atcTest - you should be able to get a stream of data showing in the console from the TrakStar.

# Running the node
After setup to run the node, use

``
rosrun trakstar_node trakstar
``

to launch the node, where each sensor connected will be published on it's own topic (msg type geometry_msg::PoseStamped)

``
/trakstar/trakstar_0
/trakstar/trakstar_1
/trakstar/trakstar_2
``

or alternatively, use

``
rosrun trakstar_node trakstar a
``

to launch the node where each sensor connected will be published in an array (msg type geometry_msg::PoseArray)

``
/trakstar/trakstar_data
``
