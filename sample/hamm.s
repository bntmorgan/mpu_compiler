hamm:
  loadd r31, $0x0101
  loadd r30, $0x1010
  hammd r29, r30, r31
  intq r29
  intq r0
hamm_end:
