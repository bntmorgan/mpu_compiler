loadd r31, 0x1000000 // max
loadb r30, 0x1 // Increment
loadb r29, 0x0 // cpt a
loadb r28, 0x0 // cpt b
loadb r0, 0xff // a
loadb r1, 0xff // a
loadw r2, 0x0025 // For a start
loadw r3, 0x004d // For a end
loadw r4, 0x0029 // For a start
loadw r5, 0x0047 // For a end
// for (i = 0; i < 0x1000000; i++)
infd r29, r31, r3
// for (j = 0; j < 0x1000000; j++)
infd r28, r31, r5
equb r0, r1, r1, r3
equb r0, r1, r1, r3
equb r0, r1, r1, r3
equb r0, r1, r1, r3
addd r28, r30, r28
jmpw r4
// for end
addd r29, r30, r29
jmpw r2
// for end
intq r29
intq r28
intq r20 // End of execution

// loadw r31, 0x10 // max
// loadb r30, 0x1 // Increment
// loadb r29, 0x0 // cpt a
// loadb r28, 0x0 // cpt b
// loadb r0, 0xff // a
// loadb r1, 0xff // a
// loadw r2, 0x0023 // For a start
// loadw r3, 0x003c // For a end
// loadw r4, 0x0027 // For b start
// loadw r5, 0x0036 // For b end
// // for (i = 0; i < 4096; i++)
// infw r29, r31, r3
//   // for (i = 0; i < 4096; i++)
//   infw r28, r31, r3
//     equb r0, r1, r1, r3
//     addw r28, r30, r28
//   jmpw r4
//   // for end
//   addw r29, r30, r29
// jmpw r2
// // for end
// intq r0
// intq r20 // End of execution
