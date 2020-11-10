# Bachelorarbeit ESP-Now as light protocol

## TODO

Broadcasting just works fine. But I need a good solution, if I will transmit more than one universer at once.

Now I will try to send Messages as Slave and receive as Master. So the Slave tells the Master how many bytes he needs (via unicast) and the Master talks back his position.

After this I have to check, with a lot of nodes to transmit two universes. There has to be a checksum in the beginning.
Maybe its more easy to send a part of the Mac of each fixture, and the number of used channels. There is to calculate the lost traffic.
Optional: In the ideal case, the master splits all Broadcasts in same sized packages.

## Roadmap

- [ ] slave-master  : need offset, broadcastID; have lenght
- [ ] master-slave  : send offset, broadcastID
- [ ] master-broad  : send broadcast
- [ ] slave         : write extracted input to serial   
