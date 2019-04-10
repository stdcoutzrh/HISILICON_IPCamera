To disable the mute circuit about inner_codec, please configure the pin of GPIO as follows:

1.demo board
himm 0x200f0078 0x0
himm 0x201A0400 0xF
himm 0x201A03FC 0xF


2.socket board
himm 0x200f0078 0x0
himm 0x20140400 0xF
himm 0x201403FC 0xF