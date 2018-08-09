# launchpad-tcp
**Transfer files from a CC3200 microcontroller to an EC2 server,<br/>
then to a second microcontroller.**

EC2 folders: use on an Ubuntu EC2. Need script to receive and send consecutively<br/>
LP1/2 folders: use on CC3200 launchpads<br/><br/>

**1.** LP1 requests TCP connection with EC2.<br/>
**2.** Connection accepted. LP1 sends packets, EC2 receives.<br/>
**3.** EC2 requests connection with LP2.<br/>
**4.** Connection accepted. EC2 sends packets, LP2 receives.<br/>

