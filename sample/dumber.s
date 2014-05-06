  loadd r31, 0x1000000 // max
  loadb r30, 0x1 // Increment
  loadb r29, 0x0 // cpt a
  loadb r28, 0x0 // cpt b
  loadb r0, 0xff // a
  loadb r1, 0xff // a
  loadw r2, $for_a
  loadw r3, $for_a_end
  loadw r4, $for_b
  loadw r5, $for_b_end
for_a:
  // for (i = 0; i < 0x1000000; i++)
  infd r29, r31, r3
  // for (j = 0; j < 0x1000000; j++)
for_b:
  infd r28, r31, r5
  equb r0, r1, r1, r3
  equb r0, r1, r1, r3
  equb r0, r1, r1, r3
  equb r0, r1, r1, r3
  addd r28, r30, r28
  jmpw r4
for_b_end:
  // for end
  addd r29, r30, r29
  jmpw r2
for_a_end:
  // for end
  intq r29
  intq r28
  intq r20 // End of execution
