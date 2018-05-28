# launchpad-tcp
**Transfer files from a CC3200 microcontroller to an EC2 server,
then to a second microcontroller.**

EC2 folders: use on an Ubuntu EC2. Need bash script to receive
             and send consecutively
LP1/2 folders: use on CC3200 launchpads

-LP1 requests TCP connection with EC2.
-Connection accepted. LP1 sends packets, EC2 receives.
-EC2 requests connection with LP2.
-Connection accepted. EC2 sends packets, LP2 receives.

