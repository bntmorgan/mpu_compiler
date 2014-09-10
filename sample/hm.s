start:
  loadd r1_1, $0x12345678
  loadd r1_0, $0x87654321
  loadd r2, $0x00000001
  loadd r2_1, $0x00000000
  // intq r1
  mloadq r1, r2
  intq r1
end:
  intq r0
