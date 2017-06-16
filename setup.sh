sudo echo "SUBSYSTEM=="usb", ATTR{idVendor}=="04b4", ATTR{idProduct}=="1005", MODE="0666", GROUP="plugdev"" > /etc/udev/rules.d/trakstar.rules

sudo udevadm control --reload-rules && udevadm trigger

