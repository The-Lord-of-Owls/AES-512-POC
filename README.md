# AES-512-POC
An AES-512 Proof of concept written in C++. Because nobody else would make one.

Yes, AES-256 is secure enough for most things.
Do I care to keep being told this?
No, I do not. I expect to be told how, not "you don't need this" or "Why do you need this". Its because I want it.
And now every time somebody says those same responses, show them this and make them eat their words.

I've also added a version of it to work with the AES-ni instruction set on intel CPUs for hardware acceleration

# Important Notice

AES-ni does have a few caveats.

If you are running on a VPS/VM/EC2 Instance or other form of Virtual Machine by any other name, you will see no benefit.
AES-ni can behave strangely on AMD CPUs, some report that it works well and some report otherwise.
I do not have time to check across every CPU that exists. If AES-ni doesn't work as it should, use the regular version of AES found here.

# Planned
All modes will eventually be supported for AES and AES-ni
ECB MODE: AES and AES-ni complete
CBC MODE: Pending
CFB Mode: Pending
OFB Mode: Pending
CTR Mode: Pending
GCM Mode: Pending
CCM Mode: Penidng
XTS Mode: Pending

# Was it tested?
Nope lol

# Specifications

Key Size: 512 bits

Number of Rounds: 16
