.entry lol
mov/0 #1,lol
mov/0 Hallo, lol
lol:.data -20
.extern Hallo
a:.string "hmz"
kicks:mov/0 a{lol}, r2
