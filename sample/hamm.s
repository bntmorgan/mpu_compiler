/**
 * Calcul de la distance de hamming de 2 entrées consécutives d'une page et test
 * si <= 9
 */
  maskw r0_1, r1_2, r2_7, r4_0
  addd r31_1, r27_0, r29_1
hamm:
  // Constants
  loadd r31, $0x9 // hamm < 9
  loadd r30, $0xff8 // 512 entries
  // loadd r30, $0x28 // 512 entries
  loadd r29, $0x8 // Increment
  loadd r28, $0xffffffff // Hamming mask
  loadd r28_1, $0xffffffff // Hamming mask
  // Global variables
  loadd r27, $0x0 // i
  // Jumps
  loadd r20, $loop // lood start
  loadd r21, $endloop // lood end
  // Return codes
  loadd r10, $0x1 // ok 
  loadd r11, $0x2 // ko 
  // locals
  loadd r1, $0x0 // entry i
  loadd r2, $0x0 // entry i + 1
  loadd r3, $0x0 // computed hamm
// for (i = 0; i < 0x1000 - 8; i = i + 8)
loop:
  infd r27, r30, r21 // si not (i < 0xff8) => fin
  mloadq r1, r27 // r1 = entry i
  addd r27, r27, r29 // i = i + 8
  mloadq r2, r27 // r1 = entry i
  hammq r3, r2, r1, r28 // r3 = hamm (entry i, entry i + 1)
  hammq r3, r2, r1, r28 // r3 = hamm (entry i, entry i + 1)
  hammq r3, r2, r1, r28 // r3 = hamm (entry i, entry i + 1)
  // if (hamm <= 9)
  infd r31, r3, r20 // pas d'erreur on revient au debut
  intq r11 // KO
  intq r0 // end
endloop:
  intq r10 // OK
  intq r0 // end
hamm_end:
