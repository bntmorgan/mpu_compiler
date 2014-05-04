loadw r31, 0x1000 // max
loadb r30, 0x1 // Increment
loadb r29, 0x0 // cpt
loadb r0, 0xff // a
loadb r1, 0xff // a
loadw r2, 0x0018 // For start
loadw r3, 0x0027 // For end
// for (i = 0; i < 4096; i++)
infw r31, r30, r3
equb r0, r1, r1, r3
addw r29, r30, r29
jmpw r2
// for end
intq r0
intq r20 // End of execution
